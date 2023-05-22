# Cache

La primera forma de mejorar la velocidad de un programa es resolver el problema de comunicar la memoria; en lugar de constantemente mover memoria al CPU es posible utilizar una memoria interna que posee, llamada `cache` 

aqui tambien se presentan instancias donde se usa avx, para compilar se debe usar

```bash
g++ -O3 -g -march=native c4_3_mult_array_2.cpp -o 3_mult.exe
```

-march se refiere a la arquitectura del procesador, especificar native es buena idea

puedes ver la comparación entre los metodos:

```bash
$ Clase4/ .\3_mult.exe
m: 1024, n: 1024, l: 1024
m * l = 1048576
n * l = 1048576
Comenzando la suma de manera clasica, C = A . B : 
C = A . B; metodo ingenuo: 2.436000 [s]

Comenzando la suma que aproveche el cache, C = A * BT : 
Hemos de calcular la traspuesta primero:
Calculando B^T , la transpuesta: 0.008000 [s]

Entonces, C = A * BT : 
C = A * BT: 2.313000 [s] 

Sumando con AVX:
C = A * B, con avx: 0.417000 [s] 
```