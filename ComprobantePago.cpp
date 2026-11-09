// ===============================================================
// ComprobantePago.cpp
// Implemento los datos y operaciones del comprobante. La diferencia clave es que bruto suma ingresos, y neto resta las deducciones.
// ===============================================================
#include "ComprobantePago.h"
#include "Empleado.h"
#include "Departamento.h"
// Guardo punteros al empleado y al departamento sin crearlos ni destruirlos aqui; esa relacion es agregacion.
ComprobantePago::ComprobantePago(int n, Empleado* e, Departamento* d, const std::string& p,
                                 double b, double x, double bo, double de, bool pa)
    : numero(n), empleadoId(e->getId()), departamentoId(d->getId()), empleado(e),
      departamento(d), periodo(p), base(b), extras(x), bonos(bo), deducciones(de), pagado(pa) {}
int ComprobantePago::getNumero() const { return numero; }
int ComprobantePago::getEmpleadoId() const { return empleadoId; }
int ComprobantePago::getDepartamentoId() const { return departamentoId; }
const std::string& ComprobantePago::getPeriodo() const { return periodo; }
double ComprobantePago::getBase() const { return base; }
double ComprobantePago::getExtras() const { return extras; }
double ComprobantePago::getBonos() const { return bonos; }
double ComprobantePago::getDeducciones() const { return deducciones; }
// Sumo salario base, dinero de horas extra y bonificaciones.
double ComprobantePago::bruto() const { return base + extras + bonos; }
// Resto las deducciones al salario bruto para obtener el monto final.
double ComprobantePago::neto() const { return bruto() - deducciones; }
bool ComprobantePago::estaPagado() const { return pagado; }
// Solo cambio la bandera pagado; despues Sistema solicita guardar para que persista.
void ComprobantePago::marcarPagado() { pagado = true; }
const Empleado* ComprobantePago::getEmpleado() const { return empleado; }
const Departamento* ComprobantePago::getDepartamento() const { return departamento; }
