# Simulador de Planificación de Procesos y Gestión de Memoria

## 📌 Descripción
Este proyecto es un **simulador de un sistema operativo simple**, desarrollado en C++, que permite poner en práctica los conceptos de la **Unidad 01** de Sistemas Operativos:  
- Planificación de CPU.  
- Cálculo de métricas por proceso y globales.  
- Gestión de memoria con estrategias básicas.  

La entrada del simulador se da en un archivo JSON (`config/entrada.json`) que define los procesos, la estrategia de memoria y el algoritmo de planificación a usar.  

---

## ⚙️ Funcionalidades
- Implementa el algoritmo de planificación **FCFS (First Come, First Served)**.  
- Calcula métricas por proceso:
  - **Respuesta = inicio − llegada**  
  - **Espera = fin − llegada − servicio**  
  - **Retorno (turnaround) = fin − llegada**  
- Calcula métricas globales:
  - Promedios de respuesta, espera y retorno.  
  - **Throughput** (procesos / tiempo total).  
- Lectura de configuración desde JSON.  
- Estructura lista para ampliar con **SPN** y **Round Robin**.  

---

## 📂 Estructura del proyecto
Sistemas-Operativos-Simulador/
│
├── config/
│ └── entrada.json # Configuración de entrada
│
├── include/
│ ├── json.hpp # Librería JSON
│ └── utils.h # Funciones auxiliares
│
├── src/
│ ├── main.cpp # Programa principal
│ └── utils.cpp # Implementación de funciones auxiliares
│
├── docs/
│ └── especificacion.md # Informe técnico
│
├── README.md
└── .gitignore

---

## 🚀 Compilación
En la carpeta raíz del proyecto, compilar con:

```bash
g++ -std=c++17 src/main.cpp src/utils.cpp -Iinclude -o simulador

---

## Ejecución en consola
./simulador
El programa cargará la configuración desde config/entrada.json y mostrará los resultados en pantalla.

---

## Ejemplo de entrada (config/entrada.json)
{
   "cpu": { "algoritmo": "FCFS", "quantum": 4 },
   "procesos": [
      {"pid": 1, "llegada": 0, "servicio": 12},
      {"pid": 2, "llegada": 1, "servicio": 5},
      {"pid": 3, "llegada": 2, "servicio": 8}
   ],
   "memoria": { "tam": 1048576, "estrategia": "first-fit" },
   "solicitudes_mem": [ {"pid": 1, "tam": 120000}, {"pid": 2, "tam": 64000} ]
}

---

## Ejemplo de salida
Simulador SO - Unidad 01
Algoritmo CPU: FCFS, Quantum: 4

PID | Llegada | Servicio | Inicio | Fin | Respuesta | Espera | Retorno
--- | ------- | -------- | ------ | --- | --------- | ------ | -------
1   | 0       | 12       | 0      | 12  | 0         | 0      | 12
2   | 1       | 5        | 12     | 17  | 11        | 11     | 16
3   | 2       | 8        | 17     | 25  | 15        | 15     | 23

Promedio Respuesta: 8.67
Promedio Espera: 8.67
Promedio Retorno: 17.0
Throughput: 3/25 = 0.12