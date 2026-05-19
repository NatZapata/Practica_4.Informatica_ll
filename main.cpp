#include <iostream>
#include "Red.h"

using namespace std;

void menuArchivo(Red& red);
void menuModelarRed(Red& red);

int main() {
    Red red;
    int opcionPrincipal;

    do {
        cout << "\n-------------------------------------\n";
        cout << "            RED DE ENRUTADORES         \n";
        cout << "----------------------------------------\n";
        cout << "1. Cargar red desde archivo\n";
        cout << "2. Crear red manualmente\n";
        cout << "0. Salir del programa\n";
        cout << "-------------------------------------\n";
        cout << "Seleccione una opcion principal: ";
        cin >> opcionPrincipal;

        switch (opcionPrincipal) {
        case 1:
            menuArchivo(red);
            break;
        case 2:
            menuModelarRed(red);
            break;
        case 0:
            cout << "\nSaliendo del programa.\n";
            break;
        default:
            cout << "\nOpcion invalida en el Menu Principal.\n";
            break;
        }
    } while (opcionPrincipal != 0);

    return 0;
}

void menuArchivo(Red& red) {
    // CAMBIO: Aquí colocas la ruta directa de tu archivo entre comillas.
    // NOTA: En Windows, usa doble barra diagonal (\\) para que no se confunda con caracteres de escape.
    string nombreArchivo = "C:\\ProyectosQt\\Practica_4\\build\\Desktop_Qt_6_10_2_MinGW_64_bit-Debug\\red.txt";

    if (red.cargarDesdeArchivo(nombreArchivo)) {
        cout << "\nRed cargada exitosamente\n";

        int opSub;
        do {
            cout << "\n--- RED DESDE ARCHIVO ---\n";
            cout << "1. Ver tabla de costos actuales\n";
            cout << "2. Calcular camino mas corto entre 2 enrutadores\n";
            cout << "0. Volver al Menu Principal\n";
            cout << "Seleccione una opcion: ";
            cin >> opSub;

            char enrutadorA, enrutadorB;

            switch (opSub) {
            case 1:
                red.mostrarTablasDeCostos();
                break;
            case 2:
                cout << "Enrutador origen: ";
                cin >> enrutadorA;
                cout << "Enrutador destino: ";
                cin >> enrutadorB;
                red.mostrarCaminoMasCorto(enrutadorA, enrutadorB);
                break;
            case 0:
                cout << "Regresando...\n";
                break;
            default:
                cout << "Opcion invalida.\n";
                break;
            }
        } while (opSub != 0);
    } else {
        cout << "[Error]: Verifique que el archivo exista en la ruta especificada.\n";
    }
}

void menuModelarRed(Red& red) {
    int opSub;
    do {
        cout << "\n--- CREAR RED MANUALMENTE ---\n";
        cout << "1. Agregar enrutador (origen, vecino y costo)\n";
        cout << "2. Eliminar enrutador\n";
        cout << "3. Agregar o Modificar un enlace\n";
        cout << "4. Eliminar un enlace\n";
        cout << "5. Ver tabla de costos actuales\n";
        cout << "6. Calcular camino mas corto entre 2 enrutadores\n";
        cout << "0. Volver al Menu Principal\n";
        cout << "Seleccione una opcion: ";
        cin >> opSub;

        char enrutadorA, enrutadorB;
        int costo;

        switch (opSub) {
        case 1: {
            cout << "Ingrese el nombre del nuevo enrutador: ";
            cin >> enrutadorA;

            if (red.existeEnrutador(enrutadorA)) {
                cout << "El enrutador " << enrutadorA << " ya existe.\n";
                break;
            }

            cout << "Ingrese el nombre del enrutador vecino: ";
            cin >> enrutadorB;
            cout << "Costo del enlace: ";
            cin >> costo;

            // Agregar
            red.agregarEnrutador(enrutadorA);

            // Si el vecino no existe, se crea
            if (!red.existeEnrutador(enrutadorB)) {
                cout << " El vecino '" << enrutadorB << "' no existia. Se creara automaticamente...\n";
                red.agregarEnrutador(enrutadorB);
            } else {
                    cout << " Conectando con el vecino existente '" << enrutadorB << "'...\n";
                }

            // Crear enlace entre ambos
            red.agregarEnlace(enrutadorA, enrutadorB, costo);

            cout << "\nProceso completado. Enrutadores listos y enlazados.\n";
            break;
        }
        case 2:
            cout << "Ingrese el nombre del enrutador a eliminar: ";
            cin >> enrutadorA;
            red.eliminarEnrutador(enrutadorA);
            cout << "Enrutador eliminado con exito.\n ";
            break;

        case 3:
            cout << "Enrutador origen: ";
            cin >> enrutadorA;
            cout << "Enrutador destino: ";
            cin >> enrutadorB;
            cout << "Costo del enlace: ";
            cin >> costo;
            red.agregarEnlace(enrutadorA, enrutadorB, costo);
            cout << "\nEnlace establecido con exito.\n";
            break;

        case 4:
            cout << "Enrutador origen: ";
            cin >> enrutadorA;
            cout << "Enrutador destino: ";
            cin >> enrutadorB;
            red.eliminarEnlace(enrutadorA, enrutadorB);
            cout << "enlace eliminado con exito.\n";
            break;

        case 5:
            red.mostrarTablasDeCostos();
            break;

        case 6:
            cout << "Enrutador origen: ";
            cin >> enrutadorA;
            cout << "Enrutador destino: ";
            cin >> enrutadorB;
            red.mostrarCaminoMasCorto(enrutadorA, enrutadorB);
            break;

        case 0:
            cout << "Regresando...\n";
            break;

        default:
            cout << "Opcion invalida.\n";
            break;
        }
}   while (opSub != 0);
}
