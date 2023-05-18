12 Mayo 2023


## Sobre el curso
Que es bueno saber sobre OpenMp
- variables globales privadas
- definir operadores de reduccion 
- leer sobre monoide conmutativo
- es usado en un sistema con memoria compartida

- MPI se usa cuando tenemos memoria distribuida y hemos de enviar memoria de un lugar a otro


____

## Clase 12 de mayo
- example custom reduction (from intel compiler documentation)
    - la posibilidad de implementar una funcion OMP para reduccion
- OMP redction in detail;
    ```
    x += 1
    ```

    - especificamos el operador o funcion para reduccion
    - aqui usamos (min:x) que usa la varialbe global previamente definida y las compara con las variables locales
    - se conversa sobre que este programa no tiene un sentido logico, aunque si funciona correctamente
    - OMP no analiza, es un programa ciego

- OMP reduction in detail; 
    ```
    std::string result ={"Simon says: "};
    std::vector<std::string>
    ```

    - #pragma omp declare reduction()... sirve para declarar variables y funciones
    - #pragma omp parallel for reduction... es donde dividimos el trabajo entre los threads



- SIMD Vectorization
    - Vector Addition; es posible usar `#pragma omp simd` para implementar la vectorizacion; pero no funciona muy bien en la mayoria de los casos
    - el profesor recomienda hacer la vectorizacion a mano



___

## Tarea
Es posible hacerla en C++


## Proyecto Final del Curso
Una simulacion de N cuerpos paralelizada.
Particulas repartidas que representan la materia oscura del universo
- se debera calcular un campo de densidad; usando las particulas para obtener una densidad en puntos de la maya
- luego la ecs de Poisson para el potencial gravitatorio
    - hasta ahora se usa el metood de Jacobi (convergencia 1000 iteraciones)
    - existen otros como Gauss-Seidel pero mas dificil de paralelizar pero mas rapido en convergencia (200 iteraciones)
    - metodos como MULTIGRID, muy usado en simulaciones profesionales a grandes escala. (10 iteraciones)
        - se usan grillas de resolucion variables
    - SuccesiveOverRelaxation [SoR]; es posible agregar este metodo a cualquiera de los anteriores para hacerlo mas rapido
    - Se puede resolver tambien en espacio de Fourier; donde se vuelve un problema algebraica.

- es posible hacer trabajos en grupo para esto 
- entonces el gradiente del potencial se utiliza para obtener la fuerza

Extras:
- exista la expansion del universo, por tanto habra de incluirse
- es posible leer sobre QuadTree
- Refinamiento adaptativo de la grilla, aunque eso tambien es complicado

Luego mas adelante se pueden agregar cosas como la hidrodinamica; la presion de Gas; como varian las ecuaciones de movimiento en este caso
