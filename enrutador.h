#ifndef ENRUTADOR_H
#define ENRUTADOR_H

#include <vector>
#include <map>

class Enrutador {
public:
    char nombreEnrutador;
    int idEnrutador;
    int distanciaMinima;
    bool yaVisitado;

    // Contenedor STL para almacenar los vecinos: {Puntero al vecino, Costo del enlace}
    std::vector<std::pair<Enrutador*, int>> listaVecinos;

    // Tablas individuales de enrutamiento
    std::vector<int> tablaCostos;
    std::vector<std::vector<char>> tablaCaminos;

    // Constructor y métodos
    Enrutador(char nombre, int id);
    void agregarVecino(Enrutador* vecino, int costo);
    void eliminarVecino(Enrutador* vecino);
    void reiniciarValores();
};

#endif // ENRUTADOR_H
