// ===============================================================
// Empleado.h
// En este archivo declaro la clase padre Empleado y sus tres clases hijas. Hice Empleado abstracta con funciones virtuales puras para que cada tipo calcule sus propias bonificaciones.
// ===============================================================
#ifndef EMPLEADO_H
#define EMPLEADO_H
#include <string>

// Esta clase es abstracta: nunca creo un Empleado sin indicar su tipo.
class Empleado {
protected:
    int id;
    std::string nombre;
    int departamentoId;
    double salarioBase;
    double horasExtra;
    int turnosNocturnos;
    int guardias;
    std::string detalle;
public:
    Empleado(int id, const std::string& nombre, int departamentoId,
             double salarioBase, double horasExtra, int turnosNocturnos,
             int guardias, const std::string& detalle);
    virtual ~Empleado() {}
    int getId() const;
    const std::string& getNombre() const;
    int getDepartamentoId() const;
    double getSalarioBase() const;
    double getHorasExtra() const;
    int getTurnosNocturnos() const;
    int getGuardias() const;
    const std::string& getDetalle() const;
    void cambiarJornada(double extras, int nocturnos, int guardiasNuevas);
    // Cada tipo devuelve su bonificacion usando polimorfismo.
    // El = 0 significa que obligo a cada hijo a implementar el metodo.
    // Asi calculo bonificaciones sin preguntar el tipo con if o switch.
    virtual double bonificaciones() const = 0;
    virtual std::string tipo() const = 0;
};

class Medico : public Empleado {
public:
    Medico(int id, const std::string& n, int d, double s, double h, int t, int g, const std::string& especialidad);
    double bonificaciones() const;
    std::string tipo() const;
};
class Enfermero : public Empleado {
public:
    Enfermero(int id, const std::string& n, int d, double s, double h, int t, int g, const std::string& area);
    double bonificaciones() const;
    std::string tipo() const;
};
class Administrativo : public Empleado {
public:
    Administrativo(int id, const std::string& n, int d, double s, double h, int t, int g, const std::string& cargo);
    double bonificaciones() const;
    std::string tipo() const;
};
#endif
