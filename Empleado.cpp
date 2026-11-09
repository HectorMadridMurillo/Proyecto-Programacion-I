// ===============================================================
// Empleado.cpp
// Aqui implemento los constructores, los metodos de acceso y las bonificaciones. Los montos de las bonificaciones son ejemplos configurados para el proyecto, no tarifas oficiales.
// ===============================================================
#include "Empleado.h"

// Inicializo todos los atributos compartidos en el constructor del padre para no repetir esa asignacion en cada clase hija.
Empleado::Empleado(int i, const std::string& n, int d, double s, double h, int t, int g, const std::string& det)
    : id(i), nombre(n), departamentoId(d), salarioBase(s), horasExtra(h),
      turnosNocturnos(t), guardias(g), detalle(det) {}
int Empleado::getId() const { return id; }
const std::string& Empleado::getNombre() const { return nombre; }
int Empleado::getDepartamentoId() const { return departamentoId; }
double Empleado::getSalarioBase() const { return salarioBase; }
double Empleado::getHorasExtra() const { return horasExtra; }
int Empleado::getTurnosNocturnos() const { return turnosNocturnos; }
int Empleado::getGuardias() const { return guardias; }
const std::string& Empleado::getDetalle() const { return detalle; }
// Actualizo los valores de jornada del empleado que ya esta registrado.
void Empleado::cambiarJornada(double h, int t, int g) {
    horasExtra = h; turnosNocturnos = t; guardias = g;
}

// Envio al constructor padre los datos compartidos; el ultimo campo representa la especialidad.
Medico::Medico(int i, const std::string& n, int d, double s, double h, int t, int g, const std::string& esp)
    : Empleado(i, n, d, s, h, t, g, esp) {}
// Calculo un total multiplicando las noches y guardias por sus montos de ejemplo. Cada clase hija define sus valores.
double Medico::bonificaciones() const {
    // Uso montos de ejemplo: el enunciado no fija tarifas por guardia o noche.
    return turnosNocturnos * 8000.0 + guardias * 12000.0;
}
// Devuelvo un texto que identifica la clase hija, necesario tambien para guardar y reconstruir los registros.
std::string Medico::tipo() const { return "MEDICO"; }

// Reutilizo el constructor de Empleado; el dato particular de esta clase es el area.
Enfermero::Enfermero(int i, const std::string& n, int d, double s, double h, int t, int g, const std::string& area)
    : Empleado(i, n, d, s, h, t, g, area) {}
// Calculo un total multiplicando las noches y guardias por sus montos de ejemplo. Cada clase hija define sus valores.
double Enfermero::bonificaciones() const {
    return turnosNocturnos * 7000.0 + guardias * 9000.0;
}
// Devuelvo un texto que identifica la clase hija, necesario tambien para guardar y reconstruir los registros.
std::string Enfermero::tipo() const { return "ENFERMERO"; }

// Reutilizo el constructor de Empleado; el campo detalle representa el cargo administrativo.
Administrativo::Administrativo(int i, const std::string& n, int d, double s, double h, int t, int g, const std::string& cargo)
    : Empleado(i, n, d, s, h, t, g, cargo) {}
// Calculo un total multiplicando las noches y guardias por sus montos de ejemplo. Cada clase hija define sus valores.
double Administrativo::bonificaciones() const {
    return turnosNocturnos * 5000.0 + guardias * 0.0;
}
// Devuelvo un texto que identifica la clase hija, necesario tambien para guardar y reconstruir los registros.
std::string Administrativo::tipo() const { return "ADMINISTRATIVO"; }
