# Simulador SO - Unidad 01

Simulador de planificación de procesos (FCFS, SPN, Round Robin) y asignación de memoria (First-Fit, Best-Fit).

## Estructura del repositorio
- `src/` : código fuente en C++.
- `config/` : archivos JSON de ejemplo (entrada).
- `docs/` : informe técnico y documentación.
- `README.md` : guía rápida.

## Cómo compilar (ejemplo con g++)
g++ -std=c++17 src/main.cpp -o simulador
./simulador

## Próximos pasos
1. Implementar lectura JSON (nlohmann/json).
2. Implementar planificadores FCFS / SPN / RR.
3. Implementar First-Fit / Best-Fit.
