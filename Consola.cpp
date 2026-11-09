// ===============================================================
// Consola.cpp
// Aqui manejo la entrada del usuario y el menu. Uso getline para recibir lineas completas y despues las valido antes de enviarlas al Sistema.
// ===============================================================
#include "Consola.h"
#include "Sistema.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cmath>

namespace {
// Leo una linea completa y no acepto el simbolo | porque uso ese caracter para separar campos en los archivos.
std::string texto(const std::string& pregunta) {
    std::string respuesta;
    for (;;) {
        std::cout << pregunta;
        if (!std::getline(std::cin, respuesta)) return "";
        if (!respuesta.empty() && respuesta.find('|') == std::string::npos) return respuesta;
        std::cout << "Ingrese texto no vacio y sin el caracter |.\n";
    }
}
// Repito la pregunta hasta recibir un entero dentro del rango permitido. Reviso que no queden caracteres sobrantes.
int numero(const std::string& pregunta, int minimo, int maximo) {
    for (;;) {
        std::string linea;
        std::cout << pregunta;
        if (!std::getline(std::cin, linea)) return minimo - 1;
        std::stringstream ss(linea);
        int n;
        char sobrante;
        if (ss >> n && !(ss >> sobrante) && n >= minimo && n <= maximo) return n;
        std::cout << "Numero invalido. Debe estar entre " << minimo << " y " << maximo << ".\n";
    }
}
// Esta validacion es parecida a numero(), pero acepta decimales y rechaza valores no finitos.
double real(const std::string& pregunta, double minimo, double maximo) {
    for (;;) {
        std::string linea;
        std::cout << pregunta;
        if (!std::getline(std::cin, linea)) return -1;
        std::stringstream ss(linea);
        double n;
        char sobrante;
        if (ss >> n && !(ss >> sobrante) && std::isfinite(n) && n >= minimo && n <= maximo) return n;
        std::cout << "Valor invalido.\n";
    }
}
// Centralizo aqui el mensaje de exito o error para no repetir el mismo cout en cada opcion.
void aviso(bool bien) { std::cout << (bien ? "Operacion realizada.\n" : "No se pudo realizar. Revise los datos o los archivos.\n"); }
}
Consola::Consola(Sistema& s) : sistema(s) {}
// Primero pido el ID y el nombre; Sistema revisa los duplicados y luego guarda el cambio.
void Consola::crearDepartamento() {
    int id = numero("ID del departamento: ", 1, 999999);
    if (id < 1) return;
    aviso(sistema.nuevoDepartamento(id, texto("Nombre: ")));
}
// Antes de agregar un puesto, verifico que el departamento al que pertenece si exista.
void Consola::crearPuesto() {
    int d = numero("ID del departamento: ", 1, 999999);
    if (d < 1 || !sistema.buscarDepartamento(d)) { std::cout << "Departamento inexistente.\n"; return; }
    int p = numero("ID del puesto: ", 1, 999999);
    if (p < 1) return;
    aviso(sistema.nuevoPuesto(d, p, texto("Nombre del puesto: ")));
}
// Pido los datos comunes y segun el tipo elegido creo un Medico, Enfermero o Administrativo usando new.
void Consola::crearEmpleado() {
    std::cout << "1. Medico | 2. Enfermero | 3. Administrativo\n";
    int tipo = numero("Tipo: ", 1, 3);
    if (tipo < 1) return;
    int id = numero("ID del empleado: ", 1, 999999);
    if (id < 1) return;
    if (sistema.buscarEmpleado(id)) { std::cout << "Ese ID ya existe.\n"; return; }
    std::string nombre = texto("Nombre: ");
    if (nombre.empty()) return;
    int dep = numero("ID del departamento: ", 1, 999999);
    if (!sistema.buscarDepartamento(dep)) { std::cout << "Departamento inexistente.\n"; return; }
    double salario = real("Salario base mensual (CRC): ", 0, 1000000000);
    double horas = real("Horas extra del mes: ", 0, 744);
    int noches = numero("Turnos nocturnos: ", 0, 1000);
    int guardias = numero("Guardias: ", 0, 1000);
    std::string detalle = texto("Especialidad, area o cargo: ");
    if (salario < 0 || horas < 0 || noches < 0 || guardias < 0 || detalle.empty()) return;
    // Uso un puntero de la clase padre; puede apuntar a cualquiera
    // de las tres clases hijas sin cambiar el codigo que lo recibe.
    Empleado* e = NULL;
    if (tipo == 1) e = new Medico(id, nombre, dep, salario, horas, noches, guardias, detalle);
    if (tipo == 2) e = new Enfermero(id, nombre, dep, salario, horas, noches, guardias, detalle);
    if (tipo == 3) e = new Administrativo(id, nombre, dep, salario, horas, noches, guardias, detalle);
    // Si no se agrega el objeto, libero la memoria porque Sistema no lo recibio.
    if (!sistema.nuevoEmpleado(e)) {
        // Si fallo el disco pero ya quedo en la lista, no borro ese objeto.
        if (sistema.buscarEmpleado(id) != e) delete e;
        aviso(false);
    }
    else aviso(true);
}
// Recorro mi lista de departamentos, y dentro de cada departamento recorro su lista de puestos.
void Consola::mostrarDepartamentos() const {
    if (sistema.getDepartamentos().vacia()) std::cout << "No hay departamentos.\n";
    for (Nodo<Departamento*>* n = sistema.getDepartamentos().inicio(); n; n = n->siguiente) {
        const Departamento* d = n->dato;
        std::cout << "Departamento " << d->getId() << ": " << d->getNombre() << '\n';
        for (Nodo<Puesto>* p = d->getPuestos().inicio(); p; p = p->siguiente)
            std::cout << "   Puesto " << p->dato.getId() << ": " << p->dato.getNombre() << '\n';
    }
}
// Uso el mismo puntero a Empleado para mostrar cualquier clase hija; esa es una ventaja del polimorfismo.
void Consola::mostrarEmpleados() const {
    if (sistema.getEmpleados().vacia()) std::cout << "No hay empleados.\n";
    for (Nodo<Empleado*>* n = sistema.getEmpleados().inicio(); n; n = n->siguiente) {
        const Empleado* e = n->dato;
        std::cout << "ID: " << e->getId() << " | " << e->getNombre() << " | " << e->tipo()
                  << " | Departamento: " << e->getDepartamentoId() << " | Base: CRC "
                  << e->getSalarioBase() << " | Extra: " << e->getHorasExtra()
                  << " | Noches: " << e->getTurnosNocturnos() << " | Guardias: "
                  << e->getGuardias() << " | Detalle: " << e->getDetalle() << '\n';
    }
}
// Pido horas extra, noches y guardias nuevas y dejo que Sistema valide y guarde la actualizacion.
void Consola::cambiarJornada() {
    int id = numero("ID del empleado: ", 1, 999999);
    if (!sistema.buscarEmpleado(id)) { std::cout << "No existe.\n"; return; }
    double h = real("Nuevas horas extra: ", 0, 744);
    int t = numero("Nuevos turnos nocturnos: ", 0, 1000);
    int g = numero("Nuevas guardias: ", 0, 1000);
    aviso(sistema.actualizarJornada(id, h, t, g));
}
// Pido empleado, periodo y porcentaje de deduccion; Sistema hace las operaciones y yo solo muestro el resultado.
void Consola::generarComprobante() {
    int id = numero("ID del empleado: ", 1, 999999);
    if (!sistema.buscarEmpleado(id)) { std::cout << "No existe.\n"; return; }
    std::string periodo = texto("Periodo (ejemplo: 2026-11): ");
    if (periodo.empty()) return;
    double deduccion = real("Porcentaje total de deducciones configurado (0 a 100): ", 0, 100);
    if (deduccion < 0) return;
    int resultado = sistema.calcularPlanilla(id, periodo, deduccion);
    if (resultado > 0) {
        const ComprobantePago* c = sistema.buscarComprobante(resultado);
        std::cout << "Comprobante #" << resultado << " | Bruto: CRC " << c->bruto()
                  << " | Deducciones: CRC " << c->getDeducciones()
                  << " | Neto: CRC " << c->neto() << " | PENDIENTE\n";
    } else if (resultado == -2) std::cout << "Ya existe comprobante de ese empleado para ese periodo.\n";
    else std::cout << "Error al calcular o guardar el comprobante.\n";
}
// Recorro los comprobantes para mostrar el detalle y si cada pago esta pendiente o pagado.
void Consola::mostrarComprobantes() const {
    if (sistema.getComprobantes().vacia()) std::cout << "No hay comprobantes.\n";
    for (Nodo<ComprobantePago*>* n = sistema.getComprobantes().inicio(); n; n = n->siguiente) {
        const ComprobantePago* c = n->dato;
        std::cout << "#" << c->getNumero() << " | " << c->getPeriodo()
                  << " | Empleado: " << c->getEmpleado()->getNombre()
                  << " | Departamento: " << c->getDepartamento()->getNombre()
                  << " | Base: " << c->getBase() << " | Extras: " << c->getExtras()
                  << " | Bonos: " << c->getBonos() << " | Deducciones: " << c->getDeducciones()
                  << " | Neto: CRC " << c->neto() << " | "
                  << (c->estaPagado() ? "PAGADO" : "PENDIENTE") << '\n';
    }
}
// Pido el numero del comprobante; Sistema evita pagarlo dos veces y registra el nuevo estado.
void Consola::realizarPago() {
    int n = numero("Numero de comprobante: ", 1, 999999);
    aviso(sistema.procesarPago(n));
}
// Mantengo abierto el menu mientras no elijan cero. El switch llama a una funcion diferente segun la opcion.
void Consola::ejecutar() {
    // Solo esta clase utiliza cin y cout; las entidades no conocen la consola.
    std::cout << std::fixed << std::setprecision(2);
    int opcion = -1;
    while (opcion != 0 && std::cin.good()) {
        std::cout << "\n===== PLANILLA HOSPITALARIA =====\n"
                  << "1. Registrar departamento\n2. Registrar puesto\n3. Registrar empleado\n"
                  << "4. Ver departamentos\n5. Ver empleados\n6. Actualizar jornada\n"
                  << "7. Calcular planilla\n8. Ver comprobantes\n9. Procesar pago\n0. Salir\n";
        opcion = numero("Opcion: ", 0, 9);
        // Cada case representa una opcion del menu. Uso break para que
        // el programa no continue ejecutando la siguiente opcion.
        switch (opcion) {
            case 1: crearDepartamento(); break;
            case 2: crearPuesto(); break;
            case 3: crearEmpleado(); break;
            case 4: mostrarDepartamentos(); break;
            case 5: mostrarEmpleados(); break;
            case 6: cambiarJornada(); break;
            case 7: generarComprobante(); break;
            case 8: mostrarComprobantes(); break;
            case 9: realizarPago(); break;
            case 0: std::cout << "Hasta luego.\n"; break;
        }
    }
}
