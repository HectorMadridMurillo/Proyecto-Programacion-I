// ===============================================================
// main.cpp
// Este es el punto donde comienza mi programa. Aqui conecto el almacenamiento, las reglas del hospital y el menu. Mantengo estas responsabilidades separadas para no mezclar todo en main.
// ===============================================================
#include "Repositorio.h"
#include "Sistema.h"
#include "Consola.h"
#include <iostream>

int main() {
    // Primero preparo el repositorio; asi el sistema sabe donde leer y guardar.
    RepositorioTexto archivos(".");
    Sistema hospital(archivos);
    // Al iniciar cargo lo que quedo guardado en las tres listas.
    if (!hospital.cargar())
        std::cout << "Aviso: se omitieron datos invalidos de los archivos.\n";
    Consola pantalla(hospital);
    pantalla.ejecutar();
    // Al salir se ejecutan los destructores y se libera la memoria dinamica.
    return 0;
}
