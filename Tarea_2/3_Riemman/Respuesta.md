# Respuestas 3
### (i): paralelización
Tengasé el codigo de `3_riemman.c`
No hay dependencias con esta paralelización
```
    #pragma omp parallel
    {
        clock_t t_thread = clock();

        int tid = omp_get_thread_num(); // thread  id
        int start = tid * loop_section;
        int end = start + loop_section;
        if (tid == num_threads - 1){ //al ser el ultimo thread
            end = N;
        }

        // operacion de suma dentro de cada seccion del arreglo
        for (int k = start; k < end; k++){
            X[k] = Riemann_Zeta(2, k);
        }

        t_thread = clock() - t_thread;
        printf("\t %d ", tid);
        printf("t: %f [s] \n", (double)t_thread /CLOCKS_PER_SEC);


    }
```

### (ii) Sobre el load balancing:
Si tomamos el codigo `3_riemman.c`, si bien no se tiene dependencias especiales, existe el problema del load balancing, pues k start y end definira el largo del loop interno.

Con esta hipotessis se le agregarón visualizaciones de tiempo para cada thread y se observa que:
```
Secci├│n paralelizada:
         0 t: 0.000000 [s]
         1 t: 0.000000 [s]
         3 t: 0.001000 [s]
         4 t: 0.002000 [s]
         6 t: 0.003000 [s]
         7 t: 0.004000 [s]
         2 t: 0.000000 [s]
         5 t: 0.002000 [s]

Tiempo: 0.020000 [s]
```
efectivamente hay un desbalance de loads debido a esta paralelización.

para arreglar este problema se implementó el codigo `32_riemman.c` para mantener el primer codigo intacto; así:

se utiliza una asignación dinamica, cuando un thread termina su trabajo 
```
#pragma omp for schedule(dynamic, CHUNK)
for (int k = 0; k < N; k++){
        X[k] = Riemann_Zeta(2, k);
}
```
La cual divide en Chunks, osea en trozos el for, cuando un thread termina con su Chunk, pasa tomar otros.
- en el caso default los chunk valen 1, por tanto hay bastante sincronización por detras
- al aumentar el chunk podemos ganar eficiencia, pero si aumentamos demasiado los chunks seran muy grandes y le costara paralelizar
    - los chunks han de ser proporcionales a la cantidad de datos e inversamente proporcionales a los threads, sin embargo con un factor que haga el chunk entre 1 y esta cantidad.

$$
\text{chunk} = \frac{1}{\alpha} \frac{N}{p}
$$

No siempre es posible saber cuanto demorara el codigo, así que no habria una forma directa para el tamaño del chunk en este caso impredecible.
Dividir por 3 o 5 puede ser un buen comienzo.

```
$ Tarea_2/ .\2riemman.exe
Seccion paralelizada:
         0 t: 0.001000 [s]
         1 t: 0.006000 [s]
         2 t: 0.017000 [s]
         3 t: 0.026000 [s]
         4 t: 0.041000 [s]
         5 t: 0.058000 [s]
         6 t: 0.092000 [s]
         7 t: 0.114000 [s]
         tiempo: 0.123000 [s]
Seccion dynamic paralelize:
         chunk size: 37
         t: 0.106000 [s]
Seccion dynamic paralelize:
         chunk size: 12
         t: 0.088000 [s]
Seccion dynamic paralelize:
         chunk size: 1
         t: 0.080000 [s]
Seccion dynamic paralelize, alpha=log10(N):
         chunk size: 4
         t: 0.084000 [s]
```

### (iii) distintos tipos de scheduling y sus tiempos
De acuerdo a `33_riemman_scheduling.c`

- Dinamic:
    - cuando un thread termina se le asigna un nuevo chunk
- Static
    - se reparten los problemas previamente, cada sección pertenece a threads, así que no requiere revisar cuales secciones del loop pertenecen a otros threads como lo hace dynamic
- Guided
    - similar a dynamic pero el tamaño del chunk va cambiando
- Auto
    - Delega la elección del scheduler al compilador

el output de `3riemman.exe`:
```
$ Tarea_2/ .\3riemman.exe
Dynamic paralelize:
         chunk size: 1
         t: 0.073000 [s]
Static paralelize:
         chunk size: 1
         t: 0.073000 [s]
Guided paralelize:
         t: 0.069000 [s]
Automatic paralelize:
         t: 0.086000 [s]
```

