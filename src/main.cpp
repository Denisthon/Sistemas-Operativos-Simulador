#include <iostream>
#include <fstream>
#include "json.hpp"
#include "utils.h"

using json = nlohmann::json;
using namespace std;

int main() {
    printTitle("Simulador SO - Unidad 01");

    // Leer configuración
    ifstream file("config/entrada.json");
    if (!file.is_open()) {
        cerr << "No se pudo abrir config/entrada.json\n";
        return 1;
    }
    json entrada;
    file >> entrada;

    string algoritmo = entrada["cpu"]["algoritmo"];
    int quantum = 0;
    if (entrada["cpu"].contains("quantum")) quantum = entrada["cpu"]["quantum"];

    cout << "Algoritmo CPU: " << algoritmo;
    if (algoritmo == "RR") cout << ", Quantum: " << quantum;
    cout << "\n\n";

    vector<Proceso> procesos;
    for (auto &p : entrada["procesos"]) {
        Proceso pr;
        pr.pid = p["pid"];
        pr.llegada = p["llegada"];
        pr.servicio = p["servicio"];
        pr.inicio = -1;
        pr.fin = -1;
        pr.respuesta = 0;
        pr.retorno = 0;
        pr.espera = 0;
        pr.restante = pr.servicio;
        procesos.push_back(pr);
    }

    vector<Proceso> resultado;
    if (algoritmo == "FCFS") {
        resultado = runFCFS(procesos);
    } else if (algoritmo == "SPN") {
        resultado = runSPN(procesos);
    } else if (algoritmo == "RR") {
        if (quantum < 2) {
            cerr << "Quantum inválido: debe ser >= 2 para RR\n";
            return 1;
        }
        resultado = runRR(procesos, quantum);
    } else {
        cerr << "Algoritmo desconocido. Usa FCFS, SPN o RR\n";
        return 1;
    }

    // imprimir tabla y métricas
    imprimirTabla(resultado);

    vector<int> respuestas, esperas, retornos;
    int tiempo_final = 0;
    for (auto &p : resultado) {
        respuestas.push_back(p.respuesta);
        esperas.push_back(p.espera);
        retornos.push_back(p.retorno);
        if (p.fin > tiempo_final) tiempo_final = p.fin;
    }

    cout << fixed;
    cout << "\nPromedio Respuesta: " << promedio(respuestas) << endl;
    cout << "Promedio Espera: " << promedio(esperas) << endl;
    cout << "Promedio Retorno: " << promedio(retornos) << endl;
    if (tiempo_final > 0)
        cout << "Throughput: " << resultado.size() << "/" << tiempo_final << " = " << (double)resultado.size()/tiempo_final << endl;
    else
        cout << "Throughput: N/A\n";

    return 0;
}

