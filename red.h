#ifndef RED_H
#define RED_H

#include <vector>
#include <map>
#include <string>
#include "Enrutador.h"

class Red {
private:
    // Contenedores STL principales
    std::vector<Enrutador*> listaEnrutadores;
    std::map<char, int> mapaNombreAIndice;

    // Métodos internos de cálculo (Dijkstra)
    void dijkstra(Enrutador* fuente, std::map<Enrutador*, Enrutador*>& padre);
    std::vector<Enrutador*> construirCamino(Enrutador* origen, Enrutador* destino, const std::map<Enrutador*, Enrutador*>& padre);

public:
    Red();
    ~Red();

    // Métodos de gestión de la red
    bool existeEnrutador(char nombre) const;
    void agregarEnrutador(char nombre);
    void eliminarEnrutador(char nombre);
    void agregarEnlace(char origen, char destino, int costo);
    void eliminarEnlace(char origen, char destino);

    // Actualización y visualización
    void recalcularTablas();
    bool cargarDesdeArchivo(const std::string& nombreArchivo);
    void mostrarCaminoMasCorto(char origen, char destino);
    void mostrarTablasDeCostos(); // El nuevo método que agregamos
};

#endif // RED_H
