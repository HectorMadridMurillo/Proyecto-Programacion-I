// ===============================================================
// ComprobantePago.h
// Un comprobante representa una planilla calculada para cierto empleado y periodo. Guardo referencias a empleado y departamento, pero el comprobante no es dueno de esos objetos.
// ===============================================================
#ifndef COMPROBANTE_PAGO_H
#define COMPROBANTE_PAGO_H
#include <string>
class Empleado;
class Departamento;

class ComprobantePago {
private:
    int numero;
    int empleadoId;
    int departamentoId;
    Empleado* empleado; // Agregacion: solo lo referencia, no lo destruye.
    Departamento* departamento;
    std::string periodo;
    double base;
    double extras;
    double bonos;
    double deducciones;
    bool pagado;
public:
    ComprobantePago(int numero, Empleado* empleado, Departamento* departamento,
                    const std::string& periodo, double base, double extras,
                    double bonos, double deducciones, bool pagado = false);
    int getNumero() const;
    int getEmpleadoId() const;
    int getDepartamentoId() const;
    const std::string& getPeriodo() const;
    double getBase() const;
    double getExtras() const;
    double getBonos() const;
    double getDeducciones() const;
    double bruto() const;
    double neto() const;
    bool estaPagado() const;
    void marcarPagado();
    const Empleado* getEmpleado() const;
    const Departamento* getDepartamento() const;
};
#endif
