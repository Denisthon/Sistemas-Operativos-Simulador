#include "utils.h"
#include <iostream>
#include <iomanip>
#include <numeric>
#include <algorithm>
#include <queue>

using namespace std;

void printTitle(const string &title) {
    cout << "\n==== " << title << " ====\n";
}

void imprimirTabla(const vector<Proceso> &procesos) {
    cout << "PID | Llegada | Servicio | Inicio | Fin | Respuesta | Espera | Retorno\n";
    cout << "----|---------|----------|--------|-----|-----------|--------|--------\n";
    for (auto &p : procesos) {
        cout << setw(3) << p.pid << "  | "
             << setw(6) << p.llegada << "  | "
             << setw(6) << p.servicio << "  | "
             << setw(6) << p.inicio << "  | "
             << setw(3) << p.fin << "  | "
             << setw(9) << p.respuesta << "  | "
             << setw(6) << p.espera << "  | "
             << setw(6) << p.retorno << "\n";
    }
}

double promedio(const vector<int> &valores) {
    if (valores.empty()) return 0.0;
    double suma = 0.0;
    for (int v : valores) suma += v;
    return suma / valores.size();
}

/* ---------------- FCFS (no expropiativo) ---------------- */
std::vector<Proceso> runFCFS(std::vector<Proceso> procesos) {
    // ordenar por llegada (estable)
    sort(procesos.begin(), procesos.end(), [](const Proceso &a, const Proceso &b){
        if (a.llegada != b.llegada) return a.llegada < b.llegada;
        return a.pid < b.pid;
    });

    int tiempo = 0;
    for (auto &p : procesos) {
        if (tiempo < p.llegada) tiempo = p.llegada;
        p.inicio = tiempo;
        tiempo += p.servicio;
        p.fin = tiempo;
        p.respuesta = p.inicio - p.llegada;
        p.retorno = p.fin - p.llegada;
        p.espera = p.retorno - p.servicio;
    }
    return procesos;
}

/* ---------------- SPN (Shortest Process Next) - no expropiativo ----------------
   Atenderá siempre, cuando la CPU quede libre, al proceso con menor servicio
   entre los que ya han llegado; en empate -> menor llegada -> menor pid.
*/
std::vector<Proceso> runSPN(std::vector<Proceso> procesos) {
    // copia de trabajos sin modificar original orden
    vector<Proceso> restantes = procesos;
    // marcar campos por defecto
    for (auto &p : restantes) { p.inicio = -1; p.fin = -1; p.respuesta = 0; p.retorno = 0; p.espera = 0; }

    int tiempo = 0;
    int n = restantes.size();
    int completados = 0;

    // ordenar por llegada para usar al avanzar tiempo
    sort(restantes.begin(), restantes.end(), [](const Proceso &a, const Proceso &b){
        if (a.llegada != b.llegada) return a.llegada < b.llegada;
        return a.pid < b.pid;
    });

    while (completados < n) {
        // construir lista de candidatos: llegaron y no completados
        vector<int> cand_idx;
        for (int i = 0; i < n; ++i) {
            if (restantes[i].fin == -1 && restantes[i].llegada <= tiempo) cand_idx.push_back(i);
        }

        if (cand_idx.empty()) {
            // si no hay candidatos, avanzar al siguiente evento (proxima llegada)
            int proxLleg = INT_MAX;
            for (int i = 0; i < n; ++i) if (restantes[i].fin == -1) proxLleg = min(proxLleg, restantes[i].llegada);
            tiempo = max(tiempo, proxLleg);
            continue;
        }

        // seleccionar el de menor servicio; en empate, menor llegada (ya ordenado), luego pid
        sort(cand_idx.begin(), cand_idx.end(), [&](int i, int j){
            if (restantes[i].servicio != restantes[j].servicio) return restantes[i].servicio < restantes[j].servicio;
            if (restantes[i].llegada != restantes[j].llegada) return restantes[i].llegada < restantes[j].llegada;
            return restantes[i].pid < restantes[j].pid;
        });

        int idx = cand_idx.front();
        Proceso &p = restantes[idx];

        // despachar por su servicio completo (no expropiativo)
        if (tiempo < p.llegada) tiempo = p.llegada;
        p.inicio = tiempo;
        tiempo += p.servicio;
        p.fin = tiempo;
        p.respuesta = p.inicio - p.llegada;
        p.retorno = p.fin - p.llegada;
        p.espera = p.retorno - p.servicio;
        completados++;
    }

    // devolver en orden por pid original para consistencia en salida
    sort(restantes.begin(), restantes.end(), [](const Proceso &a, const Proceso &b){ return a.pid < b.pid; });
    return restantes;
}

/* ---------------- Round Robin (expropiativo) ----------------
   Quantum debe ser >= 2 (validación en main). Implementación típica de cola.
*/
std::vector<Proceso> runRR(std::vector<Proceso> procesos, int quantum) {
    int n = procesos.size();
    for (auto &p : procesos) {
        p.inicio = -1;
        p.fin = -1;
        p.retorno = 0;
        p.espera = 0;
        p.respuesta = -1;
        p.restante = p.servicio;
    }

    // Orden original no es crítico, pero usaremos orden por llegada para inserción inicial
    sort(procesos.begin(), procesos.end(), [](const Proceso &a, const Proceso &b){
        if (a.llegada != b.llegada) return a.llegada < b.llegada;
        return a.pid < b.pid;
    });

    queue<int> q; // indices en 'procesos'
    int tiempo = 0;
    int idx_next_arrival = 0;
    int completados = 0;

    // encolar los que llegan en tiempo 0
    while (idx_next_arrival < n && procesos[idx_next_arrival].llegada <= tiempo) {
        q.push(idx_next_arrival++);
    }
    // si cola vacía, avanzar al primer llegada
    if (q.empty() && idx_next_arrival < n) {
        tiempo = procesos[idx_next_arrival].llegada;
        q.push(idx_next_arrival++);
    }

    while (completados < n) {
        if (q.empty()) {
            // avanzar al siguiente proceso que no llegó aún
            if (idx_next_arrival < n) {
                tiempo = max(tiempo, procesos[idx_next_arrival].llegada);
                q.push(idx_next_arrival++);
                continue;
            } else break;
        }

        int i = q.front(); q.pop();
        Proceso &p = procesos[i];

        // primer despacho -> registrar inicio y respuesta
        if (p.inicio == -1) {
            p.inicio = tiempo;
            p.respuesta = p.inicio - p.llegada;
        }

        int ejec = min(quantum, p.restante);
        // ejecutar 'ejec' unidades de tiempo
        tiempo += ejec;
        p.restante -= ejec;

        // encolar nuevos llegados que hayan llegado hasta tiempo actual
        while (idx_next_arrival < n && procesos[idx_next_arrival].llegada <= tiempo) {
            q.push(idx_next_arrival++);
        }

        if (p.restante == 0) {
            p.fin = tiempo;
            p.retorno = p.fin - p.llegada;
            p.espera = p.retorno - p.servicio;
            completados++;
        } else {
            // re-enqueue this process (RR)
            q.push(i);
        }

        // edge: si cola vacía y hay procesos futuros, avanzar al siguiente llegada
        if (q.empty() && idx_next_arrival < n) {
            tiempo = max(tiempo, procesos[idx_next_arrival].llegada);
            q.push(idx_next_arrival++);
        }
    }

    // devolver en orden por pid para salida consistente
    sort(procesos.begin(), procesos.end(), [](const Proceso &a, const Proceso &b){ return a.pid < b.pid; });
    return procesos;
}