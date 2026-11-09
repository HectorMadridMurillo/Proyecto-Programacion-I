// ===============================================================
// Departamento.cpp
// Aqui implemento como agrego puestos a cada departamento y evito numeros de puesto repetidos dentro de ese departamento.
// ===============================================================
#include "Departamento.h"
Puesto::Puesto(int i, const std::string& n) : id(i), nombre(n) {}
int Puesto::getId() const { return id; }
const std::string& Puesto::getNombre() const { return nombre; }
Departamento::Departamento(int i, const std::string& n) : id(i), nombre(n) {}
int Departamento::getId() const { return id; }
const std::string& Departamento::getNombre() const { return nombre; }
const Lista<Puesto>& Departamento::getPuestos() const { return puestos; }
// Recorro los puestos existentes antes de insertar el nuevo. Si el ID esta repetido devuelvo false y no modifico la lista.
bool Departamento::agregarPuesto(int puestoId, const std::string& nombrePuesto) {
    // Reviso que no exista otro puesto con el mismo numero en este departamento.
    for (Nodo<Puesto>* n = puestos.inicio(); n != NULL; n = n->siguiente)
        if (n->dato.getId() == puestoId) return false;
    puestos.agregar(Puesto(puestoId, nombrePuesto));
    return true;
}
