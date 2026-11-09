// ===============================================================
// Consola.h
// Declaro las operaciones de la interfaz. La clase Consola se encarga de pedir datos y mostrar mensajes, pero delega las reglas del negocio al Sistema.
// ===============================================================
#ifndef CONSOLA_H
#define CONSOLA_H
class Sistema;
class Consola {
private:
    Sistema& sistema;
    void crearDepartamento();
    void crearPuesto();
    void crearEmpleado();
    void mostrarDepartamentos() const;
    void mostrarEmpleados() const;
    void cambiarJornada();
    void generarComprobante();
    void mostrarComprobantes() const;
    void realizarPago();
public:
    explicit Consola(Sistema& sistema);
    void ejecutar();
};
#endif
