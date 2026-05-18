#include "Enrutador.h"
#include <climits>

Enrutador::Enrutador(char nombre, int id) : nombreEnrutador(nombre), idEnrutador(id), distanciaMinima(INT_MAX), yaVisitado(false) {}

void Enrutador::agregarVecino(Enrutador* vecino, int costo) {
    // Buscamos si ya existe el enlace con este vecino para no duplicarlo
    for (auto &par : listaVecinos) {
        if (par.first == vecino) {
            par.second = costo; // Si existe, solo actualizamos el costo
            return;
        }
    }
    // Si es un vecino totalmente nuevo, lo añadimos al vector
    listaVecinos.push_back({vecino, costo});
}

void Enrutador::eliminarVecino(Enrutador* vecino) {
    for (auto it = listaVecinos.begin(); it != listaVecinos.end();) {
        if (it->first == vecino) {
            it = listaVecinos.erase(it);
        } else {
            ++it;
        }
    }
}

void Enrutador::reiniciarValores() {
    distanciaMinima = INT_MAX;
    yaVisitado = false;
}
