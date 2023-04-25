# Programación Paralela

El curso está impartido en C; sin embargo, el libro utiliza comúnmente C++; el cual es un lenguaje más moderno y utilizado en la industria,
debido a que es retrocompatible en su mayoría los archivos están escritos en C++; sin embargo, a la hora de utilizar `malloc` es algo distinto, haz de realizar la conversión


## Lista de clases y resumen

- Clase3: OpenMP aplicado en sumas, se pasa por encima

- Clase6: Una vectorizacion simple para la ecuacion de Poisson
- Clase7: Repaso del uso de vectorizacion para resolver Poisson utilizando diferencias para calcular el error del programa
- Clase8: OpenMP, el uso de hilos en profundidad


## inicio:
versión en C
```c
double* x;
x = malloc(N * sizeof(double));
```
versión en C++, que aparte es retrocompatible, es posible correr este codigo perfectamente en C, así que es una buena practica convertir el output de malloc de `void -> pointer`
```c++
double* x;
x = (double*) malloc(N * sizeof(double)); //conversión de tipo de dato de void a double, para obtener la ubicación
```

# Carpetas
- `ClaseX` carpeta que separa una clase, dentro posee un resumen de esta en formato Markdown llamada `README.md` por lo que si la ves en github tendrás fórmulas y códigos en bellos formatos, los ficheros se encuentran en orden que fueron dictados en clases `c{X}_{orden}_descripcion.c`. Se recomienda leerlos, pues poseen varios comentarios
- `Algoritmos` implementación de algoritmos para optimizar tareas dentro de un programa, encontrarás el nombre del algoritmo en la carpeta y aquellos con apellido `cmake` utilizan el programa para crear una compilación más elegante

# Instalación
Utilizo Windows 10, por tanto posee una serie de pasos extras para poder correr limpiamente compiladores de C y otras herramientas, se utiliza:

- MYSYS2 para actualizar repositorios en una linux way
- Mingw-w64: entorno de desarrolo de software que incluye un puerto de los compiladores GCC

Revisesé el README.md dentro de la carpeta de `0_Instalacion`

# Compilación
## gcc / g++
Por tanto, es posible compilarlos utilizando `gcc` haciendo links a la librería estándar de C++; sin embargo, `gcc` al encontrar la extensión de cpp automáticamente entenderá que se trata de un archivo de este tipo 
```bash
gcc archivo.cpp -lstdc++
```

también es posible especificar el lenguaje de compilación
```bash
gcc -x c++ archivo.cpp 
```

Aunque en todo caso es recomendado, por comodidad utilizar el compilador en su versión de C++, que deberia de estar instalado:
```
g++ archivo.cpp
```

## Uso de CMake
Dentro de la carpeta de Algoritmos puedes encontrar el readme que guia para instalar CMake y Ninja, para luego aplicarlos a programas simples;
CMake se encarga del linking de distintos scripts para así escribir tus propias librerias.
```
sudo apt-get install ninja-build
sudo apt-get install cmake
```


# Libro
El Libro utilizado es:
```
Parallel Programming - Concepts and Practice | B. Schmidt., J. González-Domínguez., C. Hundt. & M. Schlarb.
```
