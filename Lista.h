// ===============================================================
// Lista.h
// Aqui implemente mi propia lista simplemente enlazada, sin usar std::list. Cada nodo apunta al siguiente; la lista guarda la direccion del primero y del ultimo. El destructor libera los nodos.
// ===============================================================
#ifndef LISTA_H
#define LISTA_H

#include <cstddef>

// Hice mi propia lista enlazada porque el proyecto pide nodos y listas manuales.
// Cada nodo guarda un dato y la direccion del siguiente nodo.
template <typename T>
class Nodo {
public:
    T dato;
    Nodo<T>* siguiente;
    explicit Nodo(const T& valor) : dato(valor), siguiente(NULL) {}
};

template <typename T>
class Lista {
private:
    Nodo<T>* primero;
    Nodo<T>* ultimo;
    std::size_t cantidad;
    // No permito copiar la lista porque duplicar los punteros puede causar errores.
    Lista(const Lista&);
    Lista& operator=(const Lista&);
public:
    Lista() : primero(NULL), ultimo(NULL), cantidad(0) {}
    ~Lista() { limpiar(); }

    // Cuando agrego, creo un nodo al final. Si la lista esta vacia, ese
    // mismo nodo se convierte en el primero; si no, enlazo el ultimo.
    void agregar(const T& dato) {
        Nodo<T>* nuevo = new Nodo<T>(dato); // Reservo memoria para el nodo nuevo.
        if (ultimo != NULL) ultimo->siguiente = nuevo;
        else primero = nuevo;
        ultimo = nuevo;
        ++cantidad;
    }
    Nodo<T>* inicio() const { return primero; }
    std::size_t tamano() const { return cantidad; }
    bool vacia() const { return primero == NULL; }
    // No basta con cambiar primero a NULL: perderia los nodos en memoria.
    // Por eso recorro la lista guardando primero el siguiente nodo.
    void limpiar() {
        while (primero != NULL) {
            Nodo<T>* siguiente = primero->siguiente;
            delete primero; // Libero cada nodo para evitar fugas de memoria.
            primero = siguiente;
        }
        ultimo = NULL;
        cantidad = 0;
    }
};
#endif
