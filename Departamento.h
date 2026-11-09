// ===============================================================
// Departamento.h
// Defino departamentos y puestos. Un departamento tiene una lista de puestos propia: por eso lo uso como ejemplo de composicion.
// ===============================================================
#ifndef DEPARTAMENTO_H
#define DEPARTAMENTO_H
#include "Lista.h"
#include <string>

class Puesto {
private:
    int id;
    std::string nombre;
public:
    Puesto(int id = 0, const std::string& nombre = "");
    int getId() const;
    const std::string& getNombre() const;
};

class Departamento {
private:
    int id;
    std::string nombre;
    // Composicion: los puestos viven dentro del departamento.
    Lista<Puesto> puestos;
public:
    Departamento(int id, const std::string& nombre);
    int getId() const;
    const std::string& getNombre() const;
    const Lista<Puesto>& getPuestos() const;
    bool agregarPuesto(int id, const std::string& nombre);
};
#endif
