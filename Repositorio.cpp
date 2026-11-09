// ===============================================================
// Repositorio.cpp
// Aqui convierto objetos en lineas de texto y las lineas de texto nuevamente en objetos. Es la unica parte que necesita conocer el formato de los archivos .txt.
// ===============================================================
#include "Repositorio.h"
#include "Sistema.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <cstdio>
#include <cmath>

namespace {
// Divido cada linea por el separador | para obtener sus campos en un vector.
std::vector<std::string> separar(const std::string& linea) {
    std::vector<std::string> partes;
    std::stringstream ss(linea);
    std::string parte;
    while (std::getline(ss, parte, '|')) partes.push_back(parte);
    return partes;
}
// Convierto texto a entero y rechazo una cadena si contiene caracteres adicionales.
int entero(const std::string& valor) { std::size_t pos = 0; int x = std::stoi(valor, &pos); if (pos != valor.size()) throw 1; return x; }
// Convierto texto a double, verifico que se uso toda la cadena y evito NaN o infinito.
double decimal(const std::string& valor) { std::size_t pos = 0; double x = std::stod(valor, &pos); if (pos != valor.size() || !std::isfinite(x)) throw 1; return x; }
// Escribo primero a un archivo temporal y despues lo sustituyo para reducir cortes parciales.
// Primero escribo en un archivo temporal; despues intento sustituir el original para reducir el riesgo de un archivo truncado.
bool reemplazar(const std::string& ruta, const std::string& contenido) {
    std::string temporal = ruta + ".tmp";
    { std::ofstream f(temporal.c_str(), std::ios::binary | std::ios::trunc);
      if (!f) return false;
      f << contenido;
      f.flush();
      if (!f) { f.close(); std::remove(temporal.c_str()); return false; }
    }
    if (std::rename(temporal.c_str(), ruta.c_str()) == 0) return true;
    // En Windows, rename puede fallar si el destino ya existe.
    if (std::remove(ruta.c_str()) == 0 && std::rename(temporal.c_str(), ruta.c_str()) == 0) return true;
    std::remove(temporal.c_str());
    return false;
}
}
RepositorioTexto::RepositorioTexto(const std::string& c) : carpeta(c) {}
// Leo los tres archivos en orden: primero departamentos, luego empleados y al final comprobantes, porque dependen unos de otros.
bool RepositorioTexto::cargar(Sistema& s) {
    bool valido = true;
    std::string linea;
    // Cargo departamentos primero, porque empleados y planillas los referencian.
    std::ifstream deps((carpeta + "/departamentos.txt").c_str());
    while (std::getline(deps, linea)) {
        if (linea.empty()) continue;
        try {
            std::vector<std::string> p = separar(linea);
            if (p.size() < 3) { valido = false; continue; }
            if (p[0] == "D" && p.size() == 3) {
                if (!s.nuevoDepartamento(entero(p[1]), p[2], false)) valido = false;
            } else if (p[0] == "P" && p.size() == 4) {
                if (!s.nuevoPuesto(entero(p[1]), entero(p[2]), p[3], false)) valido = false;
            } else valido = false;
        } catch (...) { valido = false; }
    }
    // Despues leo empleados; el tipo guardado me indica cual clase hija crear.
    std::ifstream emps((carpeta + "/empleados.txt").c_str());
    while (std::getline(emps, linea)) {
        if (linea.empty()) continue;
        Empleado* e = NULL;
        try {
            std::vector<std::string> p = separar(linea);
            if (p.size() != 9) { valido = false; continue; }
            int id = entero(p[1]), d = entero(p[3]), noches = entero(p[6]), guardias = entero(p[7]);
            double salario = decimal(p[4]), horas = decimal(p[5]);
            if (p[0] == "MEDICO") e = new Medico(id, p[2], d, salario, horas, noches, guardias, p[8]);
            else if (p[0] == "ENFERMERO") e = new Enfermero(id, p[2], d, salario, horas, noches, guardias, p[8]);
            else if (p[0] == "ADMINISTRATIVO") e = new Administrativo(id, p[2], d, salario, horas, noches, guardias, p[8]);
            if (!e || !s.nuevoEmpleado(e, false)) { delete e; valido = false; }
        } catch (...) { delete e; valido = false; }
    }
    // Por ultimo cargo los comprobantes, pues necesitan empleados existentes.
    std::ifstream pags((carpeta + "/planilla.txt").c_str());
    while (std::getline(pags, linea)) {
        if (linea.empty()) continue;
        try {
            std::vector<std::string> p = separar(linea);
            if (p.size() != 10 || (p[9] != "0" && p[9] != "1") ||
                !s.agregarComprobanteCargado(entero(p[0]), entero(p[1]), entero(p[2]), p[3],
                                            decimal(p[4]), decimal(p[5]), decimal(p[6]),
                                            decimal(p[7]), p[9] == "1") || p[8] != "V1") valido = false;
        } catch (...) { valido = false; }
    }
    return valido; // Si hay filas malas, aviso, pero conservo las filas correctas.
}
// Recorro cada lista y construyo las lineas de los tres archivos usando el mismo formato que voy a leer al iniciar.
bool RepositorioTexto::guardar(const Sistema& s) {
    std::ostringstream deps, emps, pags;
    deps << std::setprecision(17); emps << std::setprecision(17); pags << std::setprecision(17);
    for (Nodo<Departamento*>* n = s.getDepartamentos().inicio(); n; n = n->siguiente) {
        const Departamento* d = n->dato;
        deps << "D|" << d->getId() << '|' << d->getNombre() << '\n';
        for (Nodo<Puesto>* p = d->getPuestos().inicio(); p; p = p->siguiente)
            deps << "P|" << d->getId() << '|' << p->dato.getId() << '|' << p->dato.getNombre() << '\n';
    }
    for (Nodo<Empleado*>* n = s.getEmpleados().inicio(); n; n = n->siguiente) {
        const Empleado* e = n->dato;
        emps << e->tipo() << '|' << e->getId() << '|' << e->getNombre() << '|'
             << e->getDepartamentoId() << '|' << e->getSalarioBase() << '|'
             << e->getHorasExtra() << '|' << e->getTurnosNocturnos() << '|'
             << e->getGuardias() << '|' << e->getDetalle() << '\n';
    }
    for (Nodo<ComprobantePago*>* n = s.getComprobantes().inicio(); n; n = n->siguiente) {
        const ComprobantePago* c = n->dato;
        pags << c->getNumero() << '|' << c->getEmpleadoId() << '|' << c->getDepartamentoId()
             << '|' << c->getPeriodo() << '|' << c->getBase() << '|' << c->getExtras()
             << '|' << c->getBonos() << '|' << c->getDeducciones() << "|V1|"
             << (c->estaPagado() ? 1 : 0) << '\n';
    }
    // Los tres archivos se guardan despues de cambios; no son una transaccion entre si.
    // Guardo cada tabla de forma separada y solo informo exito si las tres
    // escrituras terminaron bien; esto NO es una transaccion atomica.
    bool a = reemplazar(carpeta + "/departamentos.txt", deps.str());
    bool b = reemplazar(carpeta + "/empleados.txt", emps.str());
    bool c = reemplazar(carpeta + "/planilla.txt", pags.str());
    return a && b && c;
}
