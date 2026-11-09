// ===============================================================
// Sistema.cpp
// Aqui escribi la logica principal del hospital: validar registros, recorrer listas, calcular salarios y procesar pagos. El repositorio guarda los cambios.
// ===============================================================
#include "Sistema.h"
#include <cmath>
// Recibo el repositorio como referencia. Asi puedo cambiar la forma de guardar sin reescribir la logica del hospital.
Sistema::Sistema(IRepositorio& repo) : repositorio(repo) {}
// Como cree empleados, departamentos y comprobantes con new, debo borrar esos objetos con delete antes de destruir las listas de nodos.
Sistema::~Sistema() {
    // Destruyo primero los comprobantes que apuntan a empleados y departamentos.
    for (Nodo<ComprobantePago*>* n = comprobantes.inicio(); n; n = n->siguiente) delete n->dato;
    for (Nodo<Empleado*>* n = empleados.inicio(); n; n = n->siguiente) delete n->dato;
    for (Nodo<Departamento*>* n = departamentos.inicio(); n; n = n->siguiente) delete n->dato;
    // Las tres listas borran sus nodos con su propio destructor.
}
// Delego la lectura inicial al repositorio en vez de leer archivos directamente aqui.
bool Sistema::cargar() { return repositorio.cargar(*this); }
// Delego el guardado al repositorio para mantener separadas las responsabilidades.
bool Sistema::guardar() const { return repositorio.guardar(*this); }
// Camino nodo por nodo hasta encontrar el ID solicitado. Si no aparece, devuelvo NULL.
Departamento* Sistema::buscarDepartamento(int id) const {
    for (Nodo<Departamento*>* n = departamentos.inicio(); n; n = n->siguiente)
        if (n->dato->getId() == id) return n->dato;
    return NULL;
}
// Busco un empleado por ID en la lista; asi detecto duplicados y encuentro a quien calcularle el salario.
Empleado* Sistema::buscarEmpleado(int id) const {
    for (Nodo<Empleado*>* n = empleados.inicio(); n; n = n->siguiente)
        if (n->dato->getId() == id) return n->dato;
    return NULL;
}
// Busco un comprobante por su numero para consultarlo o marcarlo como pagado.
ComprobantePago* Sistema::buscarComprobante(int numero) const {
    for (Nodo<ComprobantePago*>* n = comprobantes.inicio(); n; n = n->siguiente)
        if (n->dato->getNumero() == numero) return n->dato;
    return NULL;
}
const Lista<Departamento*>& Sistema::getDepartamentos() const { return departamentos; }
const Lista<Empleado*>& Sistema::getEmpleados() const { return empleados; }
const Lista<ComprobantePago*>& Sistema::getComprobantes() const { return comprobantes; }
// Verifico ID positivo, nombre no vacio y que no exista otro departamento con el mismo ID.
bool Sistema::nuevoDepartamento(int id, const std::string& nombre, bool persistir) {
    if (id <= 0 || nombre.empty() || buscarDepartamento(id)) return false;
    departamentos.agregar(new Departamento(id, nombre));
    return !persistir || guardar();
}
// Busco el departamento y uso su metodo agregarPuesto(), porque cada departamento administra sus propios puestos.
bool Sistema::nuevoPuesto(int deptoId, int id, const std::string& nombre, bool persistir) {
    Departamento* d = buscarDepartamento(deptoId);
    if (!d || id <= 0 || nombre.empty() || !d->agregarPuesto(id, nombre)) return false;
    return !persistir || guardar();
}
// Antes de aceptar el puntero, valido identificacion, salario, jornada y departamento. Si entra a la lista, Sistema se vuelve responsable de liberarlo.
bool Sistema::nuevoEmpleado(Empleado* e, bool persistir) {
    // Solo acepto el puntero si los datos son validos; si no, quien llamo debe liberarlo.
    if (!e || e->getId() <= 0 || e->getNombre().empty() || e->getSalarioBase() < 0 ||
        !std::isfinite(e->getSalarioBase()) || e->getHorasExtra() < 0 ||
        !std::isfinite(e->getHorasExtra()) || e->getTurnosNocturnos() < 0 ||
        e->getGuardias() < 0 || !buscarDepartamento(e->getDepartamentoId()) || buscarEmpleado(e->getId()))
        return false;
    empleados.agregar(e); // Desde aqui Sistema se encarga de destruir el empleado.
    return !persistir || guardar();
}
// Localizo al empleado, reviso cantidades no negativas y guardo la jornada modificada.
bool Sistema::actualizarJornada(int id, double h, int t, int g) {
    Empleado* e = buscarEmpleado(id);
    if (!e || !std::isfinite(h) || h < 0 || t < 0 || g < 0) return false;
    e->cambiarJornada(h, t, g);
    return guardar();
}
// Esta es mi funcion principal de calculo: valido datos, evito duplicados, sumo ingresos, descuento un porcentaje y creo el comprobante.
int Sistema::calcularPlanilla(int id, const std::string& periodo, double porcentaje) {
    Empleado* e = buscarEmpleado(id);
    if (!e || periodo.empty() || !std::isfinite(porcentaje) || porcentaje < 0 || porcentaje > 100) return -1;
    // Evito comprobantes duplicados del mismo empleado para el mismo periodo.
    for (Nodo<ComprobantePago*>* n = comprobantes.inicio(); n; n = n->siguiente)
        if (n->dato->getEmpleadoId() == id && n->dato->getPeriodo() == periodo) return -2;
    Departamento* d = buscarDepartamento(e->getDepartamentoId());
    if (!d) return -1;
    int numero = 1;
    for (Nodo<ComprobantePago*>* n = comprobantes.inicio(); n; n = n->siguiente)
        if (n->dato->getNumero() >= numero) numero = n->dato->getNumero() + 1;
    // Supongo 240 horas al mes y recargo 1.5x; ambos son parametros didacticos.
    // Primero convierto el salario mensual a valor por hora dividiendo entre 240.
    // Multiplico por 1.5 para el recargo didactico y por las horas extras.
    double extras = (e->getSalarioBase() / 240.0) * 1.5 * e->getHorasExtra();
    double bonos = e->bonificaciones(); // Aqui ocurre el polimorfismo.
    double bruto = e->getSalarioBase() + extras + bonos;
    // El porcentaje se pasa a fraccion dividiendo entre 100.
    // Luego obtengo cuanto dinero se rebaja del ingreso bruto.
    double descuentos = bruto * porcentaje / 100.0;
    comprobantes.agregar(new ComprobantePago(numero, e, d, periodo, e->getSalarioBase(),
                                            extras, bonos, descuentos));
    return guardar() ? numero : -3;
}
// Solo marco como pagado un comprobante existente que todavia esta pendiente.
bool Sistema::procesarPago(int numero) {
    ComprobantePago* c = buscarComprobante(numero);
    if (!c || c->estaPagado()) return false;
    c->marcarPagado();
    return guardar();
}
// Reconstruyo un comprobante desde el archivo y compruebo que sus referencias apunten a un empleado y departamento validos.
bool Sistema::agregarComprobanteCargado(int numero, int empId, int depId, const std::string& periodo,
                                        double base, double extras, double bonos, double descuentos, bool pagado) {
    Empleado* e = buscarEmpleado(empId);
    Departamento* d = buscarDepartamento(depId);
    if (!e || !d || e->getDepartamentoId() != depId || numero <= 0 || periodo.empty() || buscarComprobante(numero) ||
        !std::isfinite(base) || !std::isfinite(extras) || !std::isfinite(bonos) || !std::isfinite(descuentos) ||
        base < 0 || extras < 0 || bonos < 0 || descuentos < 0) return false;
    comprobantes.agregar(new ComprobantePago(numero, e, d, periodo, base, extras, bonos, descuentos, pagado));
    return true;
}
