// ===============================================================
// Repositorio.h
// Separo el almacenamiento del resto del sistema usando una interfaz abstracta. Asi la logica de negocio no necesita saber como se escriben los archivos.
// ===============================================================
#ifndef REPOSITORIO_H
#define REPOSITORIO_H
#include <string>
class Sistema;

// Creo una interfaz para no obligar al sistema a depender de archivos de texto.
class IRepositorio {
public:
    virtual ~IRepositorio() {}
    virtual bool cargar(Sistema& sistema) = 0;
    virtual bool guardar(const Sistema& sistema) = 0;
};

class RepositorioTexto : public IRepositorio {
private:
    std::string carpeta;
public:
    explicit RepositorioTexto(const std::string& carpeta = ".");
    bool cargar(Sistema& sistema);
    bool guardar(const Sistema& sistema);
};
#endif
