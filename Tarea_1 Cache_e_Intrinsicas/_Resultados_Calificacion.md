# Q1.

Muy buena respuesta. Reconociste que el tiempo de comunicación varía.

Nota de esta pregunta: 7.0

# Q2.

i) Entendiste la idea, pero trabajaste con un cache de 8 floats, y no 16. Pero claramente entiendes lo que hay que hacer.

ii) Aparte del problema de los 8 floats, todo bien.

iii) Muy buena respuesta.

iv) Los tiempos tienen redondeo? Son valores muy exactos... Pero no hay problemas con la respuesta.

Nota de esta pregunta:7.0

# Q8.

i) La idea acá es reconocer si hay dependencias entre las iteraciones de los ciclos. La única dependencia que causa problemas en la paralelización es el ciclo sobre el tiempo.

ii) Tú método no resulta en mucha paralelización... Estás basicamente usando las intrínsicas para implementar una reducción. Otra posibilidad sería tener 3 vectores de valores de A, uno con un desplazamiento a la izquierda, uno sin desplazamiento y otro con desplazamiento a la derecha. Así se puede sumar verticalmente 8 valores a la vez.

iii) Las intrínsicas que estás usando parecen correctas para el método elegido.

Nota de esta pregunta: 4.0

# Nota final: 6.0