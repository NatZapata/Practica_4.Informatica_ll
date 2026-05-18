#include "Red.h"
#include <iostream>
#include <queue>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <climits>

using namespace std;

Red::Red() {}

Red::~Red() {
    for (auto enrutador : listaEnrutadores) delete enrutador;
}

bool Red::existeEnrutador(char nombre) const {
    return mapaNombreAIndice.find(nombre) != mapaNombreAIndice.end();
}

void Red::agregarEnrutador(char nombre) {
    if (existeEnrutador(nombre)) {
        cout << "El enrutador " << nombre << " ya existe.\n";
        return;
    }

    int id = listaEnrutadores.size();
    Enrutador* nuevo = new Enrutador(nombre, id);

    listaEnrutadores.push_back(nuevo);
    mapaNombreAIndice[nombre] = id;

    recalcularTablas();
}

void Red::eliminarEnrutador(char nombre) {
    auto it = mapaNombreAIndice.find(nombre);
    if (it == mapaNombreAIndice.end()) {
        cout << "El enrutador no existe.\n";
        return;
    }

    int indice = it->second;
    Enrutador* victima = listaEnrutadores[indice];

    for (auto r : listaEnrutadores)
        r->eliminarVecino(victima);

    listaEnrutadores.erase(listaEnrutadores.begin() + indice);
    delete victima;

    mapaNombreAIndice.clear();
    for (int i = 0; i < (int)listaEnrutadores.size(); i++) {
        listaEnrutadores[i]->idEnrutador = i;
        mapaNombreAIndice[listaEnrutadores[i]->nombreEnrutador] = i;
    }

    recalcularTablas();
}

void Red::agregarEnlace(char origen, char destino, int costo) {
    if (!existeEnrutador(origen) || !existeEnrutador(destino)) {
        return;
    }

    Enrutador* a = listaEnrutadores[mapaNombreAIndice[origen]];
    Enrutador* b = listaEnrutadores[mapaNombreAIndice[destino]];

    a->agregarVecino(b, costo);
    b->agregarVecino(a, costo);

    recalcularTablas();
}

void Red::eliminarEnlace(char origen, char destino) {
    if (!existeEnrutador(origen) || !existeEnrutador(destino)) return;

    Enrutador* a = listaEnrutadores[mapaNombreAIndice[origen]];
    Enrutador* b = listaEnrutadores[mapaNombreAIndice[destino]];

    a->eliminarVecino(b);
    b->eliminarVecino(a);

    recalcularTablas();
}

void Red::dijkstra(Enrutador* fuente, map<Enrutador*,Enrutador*>& padre) {
    padre.clear();
    for (auto r : listaEnrutadores) r->reiniciarValores();

    fuente->distanciaMinima = 0;

    priority_queue<pair<int,Enrutador*>> cola;
    cola.push({0, fuente});

    while (!cola.empty()) {
        auto top = cola.top(); cola.pop();
        int distanciaNeg = top.first;
        Enrutador* actual = top.second;

        int distanciaReal = -distanciaNeg;

        if (actual->yaVisitado) continue;
        if (distanciaReal > actual->distanciaMinima) continue;

        actual->yaVisitado = true;

        for (auto &par : actual->listaVecinos) {
            Enrutador* vecino = par.first;
            int costo = par.second;

            if (vecino->yaVisitado) continue;

            int nuevaDistancia = actual->distanciaMinima + costo;

            if (nuevaDistancia < vecino->distanciaMinima) {
                vecino->distanciaMinima = nuevaDistancia;
                padre[vecino] = actual;
                cola.push({-nuevaDistancia, vecino});
            }
        }
    }
}

vector<Enrutador*> Red::construirCamino(
    Enrutador* origen,
    Enrutador* destino,
    const map<Enrutador*,Enrutador*>& padre)
{
    vector<Enrutador*> camino;

    if (destino->distanciaMinima == INT_MAX) return camino;

    Enrutador* actual = destino;
    while (actual) {
        camino.push_back(actual);

        auto it = padre.find(actual);
        if (it == padre.end()) break;

        actual = it->second;
    }

    reverse(camino.begin(), camino.end());

    if (camino.empty() || camino[0] != origen)
        camino.clear();

    return camino;
}

void Red::recalcularTablas() {
    int n = listaEnrutadores.size();
    if (n == 0) return;

    map<Enrutador*,int> indices;
    for (int i = 0; i < n; i++)
        indices[listaEnrutadores[i]] = i;

    for (int i = 0; i < n; i++) {
        Enrutador* origen = listaEnrutadores[i];

        map<Enrutador*,Enrutador*> padre;
        dijkstra(origen, padre);

        origen->tablaCostos.assign(n, INT_MAX);
        origen->tablaCaminos.assign(n, vector<char>());

        for (int j = 0; j < n; j++) {
            Enrutador* destino = listaEnrutadores[j];
            origen->tablaCostos[j] = destino->distanciaMinima;

            if (destino->distanciaMinima == INT_MAX) continue;

            auto ruta = construirCamino(origen, destino, padre);
            for (auto r : ruta)
                origen->tablaCaminos[j].push_back(r->nombreEnrutador);
        }
    }
}

bool Red::cargarDesdeArchivo(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);

    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo.\n";
        return false;
    }

    for (auto r : listaEnrutadores) {
        r->listaVecinos.clear(); // Vaciar los enlaces viejos de cada enrutador
        delete r;                // Liberar la memoria del objeto
    }
    listaEnrutadores.clear();
    mapaNombreAIndice.clear();

    // cant de nodos
    int cantidad;
    if (!(archivo >> cantidad)) return false;

    // Crear nuevos enrutadores
    for (int i = 0; i < cantidad; i++) {
        char nombre;
        archivo >> nombre;

        int id = listaEnrutadores.size();
        Enrutador* nuevo = new Enrutador(nombre, id);
        listaEnrutadores.push_back(nuevo);
        mapaNombreAIndice[nombre] = id;
    }

    // cargar nuevos enlaces
    char a, b;
    int costo;
    while (archivo >> a >> b >> costo) {
        // Validar que los nodos leidos existan en el mapa
        if (mapaNombreAIndice.find(a) != mapaNombreAIndice.end() &&
            mapaNombreAIndice.find(b) != mapaNombreAIndice.end()) {

            Enrutador* enrA = listaEnrutadores[mapaNombreAIndice[a]];
            Enrutador* enrB = listaEnrutadores[mapaNombreAIndice[b]];

            enrA->agregarVecino(enrB, costo);
            enrB->agregarVecino(enrA, costo);
        }
    }

    // recarcular
    recalcularTablas();
    return true;
}
void Red::mostrarCaminoMasCorto(char origen, char destino) {
    if (!existeEnrutador(origen) || !existeEnrutador(destino)) {
        cout << "Alguno de los enrutadores no existe.\n";
        return;
    }

    Enrutador* ori = listaEnrutadores[mapaNombreAIndice[origen]];
    int indiceDestino = mapaNombreAIndice[destino];

    int costoTotal = ori->tablaCostos[indiceDestino];
    if (costoTotal == INT_MAX) {
        cout << "No existe ruta entre " << origen << " y " << destino << ".\n";
        return;
    }

    cout << "Camino mas corto: ";
    const auto& camino = ori->tablaCaminos[indiceDestino];

    for (size_t i = 0; i < camino.size(); i++) {
        cout << camino[i];
        if (i + 1 < camino.size()) cout << " -> ";
    }

    cout << "\nCosto total: " << costoTotal << "\n";
}

void Red::mostrarTablasDeCostos() {
    if (listaEnrutadores.empty()) {
        cout << "La red esta vacia.\n";
        return;
    }

    cout << "\n--- MATRIZ DE COSTOS DE ENRUTAMIENTO ---\n\n";

    cout << "   |";
    for (auto destino : listaEnrutadores) {
        cout << "\t" << destino->nombreEnrutador;
    }
    cout << "\n---|";
    for (size_t i = 0; i < listaEnrutadores.size(); i++) {
        cout << "\t---";
    }
    cout << "\n";

    for (auto origen : listaEnrutadores) {
        cout << " " << origen->nombreEnrutador << " |";

        for (auto destino : listaEnrutadores) {
            int idxDestino = mapaNombreAIndice[destino->nombreEnrutador];
            int costo = origen->tablaCostos[idxDestino];

            if (costo == INT_MAX) {
                cout << "\tINF";
            } else {
                cout << "\t" << costo;
            }
        }
        cout << "\n";
    }
    cout << endl;
}
