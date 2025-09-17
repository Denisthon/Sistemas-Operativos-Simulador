#include <iostream>
#include <fstream>
#include "include/json.hpp"

using json = nlohmann::json;
using namespace std;

struct Proceso {
    int pid;
    int llegada;
    int servicio;
};

int main() {
    cout << "Simulador SO - Unidad 01\n";

    // Abrir archivo JSON
    ifstream archivo("config/entrada.json");
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir config/entrada.json\n";
        return 1;
    }

    // Leer el JSON
    json j;
    archivo >> j;

    // Mostrar algoritmo y quantum
    string algoritmo = j["cpu"]["algoritmo"];
    int quantum = j["cpu"]["quantum"];
    cout << "Algoritmo CPU: " << algoritmo << ", Quantum: " << quantum << "\n";

    // Leer procesos
    vector<Proceso> procesos;
    for (auto& p : j["procesos"]) {
        procesos.push_back({p["pid"], p["llegada"], p["servicio"]});
    }

    // Mostrar procesos cargados
    cout << "Procesos cargados:\n";
    for (auto& p : procesos) {
        cout << "PID=" << p.pid << " Llegada=" << p.llegada << " Servicio=" << p.servicio << "\n";
    }

    return 0;
}
