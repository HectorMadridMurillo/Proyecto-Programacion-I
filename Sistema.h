// ===============================================================
// Sistema.h
// Esta clase coordina empleados, departamentos y comprobantes. Mantengo aqui la validacion y el calculo salarial, en vez de hacerlo en el menu.
// ===============================================================
#ifndef SISTEMA_H
#define SISTEMA_H
#include "Lista.h"
#include "Empleado.h"
#include "Departamento.h"
#include "ComprobantePago.h"
#include "Repositorio.h"
#include <string>

class Sistema {
private:
    // En estas listas guardo punteros a objetos creados dinamicamente.
    // La clase Sistema se responsabiliza de destruir los objetos.
    Lista<Empleado*> empleados;
    Lista<Departamento*> departamentos;
    Lista<ComprobantePago*> comprobantes;
    IRepositorio& repositorio; // Inyecto la dependencia en el constructor.
    Sistema(const Sistema&);
    Sistema& operator=(const Sistema&);
public:
    explicit Sistema(IRepositorio& repositorio);
    ~Sistema();
    bool cargar();
    bool guardar() const;
    Departamento* buscarDepartamento(int id) const;
    Empleado* buscarEmpleado(int id) const;
    ComprobantePago* buscarComprobante(int numero) const;
    const Lista<Departamento*>& getDepartamentos() const;
    const Lista<Empleado*>& getEmpleados() const;
    const Lista<ComprobantePago*>& getComprobantes() const;

    // persistir=false lo uso solamente al cargar datos existentes desde un archivo.
    bool nuevoDepartamento(int id, const std::string& nombre, bool persistir = true);
    bool nuevoPuesto(int deptoId, int puestoId, const std::string& nombre, bool persistir = true);
    bool nuevoEmpleado(Empleado* empleado, bool persistir = true);
    bool actualizarJornada(int id, double extras, int nocturnos, int guardias);
    // La funcion central no pregunta si el empleado es medico o enfermero.
    int calcularPlanilla(int empleadoId, const std::string& periodo, double porcentajeDeduccion);
    bool procesarPago(int numero);
    bool agregarComprobanteCargado(int numero, int empleadoId, int deptoId,
                                  const std::string& periodo, double base, double extras,
                                  double bonos, double deducciones, bool pagado);
};
#endif
