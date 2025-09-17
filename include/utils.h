#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

struct Proceso {
    int pid;
    int llegada;
    int servicio;
    int inicio;     // primer despacho (-1 si no se despacho aún)
    int fin;        // momento de terminación
    int respuesta;  // inicio - llegada
    int espera;     // fin - llegada - servicio
    int retorno;    // fin - llegada
    int restante;   // para RR
};

// funciones de utilidad / planificadores
void printTitle(const std::string &title);
void imprimirTabla(const std::vector<Proceso> &procesos);
double promedio(const std::vector<int> &valores);

// planificadores (devuelven vector de procesos con campos calculados)
std::vector<Proceso> runFCFS(std::vector<Proceso> procesos);
std::vector<Proceso> runSPN(std::vector<Proceso> procesos);
std::vector<Proceso> runRR(std::vector<Proceso> procesos, int quantum);

#endif // UTILS_H