# Clase 5 - Metodo de Jacobi

Se presenta el estilo de progamación utilizando pointers para asi implementar el metodo iterativo de Jacobi

Tengasé la ecuación de Poisson en 2D

$$
\nabla^2 \phi[x,y] = \rho[x,y]
$$

Debe notarse que discretizar nos da soluciones, pero *esa solución puede no ser la misma producto de resolver la ecuación analiticamente*
- hemos entonces de hacer comparaciones con soluciones analiticas

Al discretizar la derivada de segundo orden se resuelve con la formula de diferencia central, la cual tiene un error menor.


### Diferencias Finitas Centradas

$$
\phi' = \frac{\phi_{i+1} - \phi_{i-1}}{2h} + \mathcal O(h^2)
$$

$$
\phi'' = \frac{\phi_{i+1} - 2 \phi_i + \phi_{i-1}}{h^2} + \mathcal O(h^2)
$$

la cual entonces reemplazamos en la ecuación de Poisson,

$$
\nabla^2 \phi[x,y] = \frac{\phi^{i+1} - 2 \phi^i + \phi^{i-1}}{h_x^2}  + \frac{\phi_{j+1} - 2 \phi_j + \phi_{j-1}}{h_y^2}  +\mathcal O(h^2)
$$

nota:
$ \phi_j = \phi_j^i$, lo que se refiere a los indices `phi[i+j*N]`
utilizamos entonces $\phi^i = \phi^x_y = \phi[x,y]$ y $\phi^{i+1} = \phi[x+h, y] $

entonces igualamos el nabla a la funcion $\rho[x,y] = \rho_y^x$ `= rho[x,y]`
- recordar que escribiremos los arreglos como `rho[x,y] = rho[x + y*N] = rho[i + j * N]`

Entonces podemos despejar la solución actual de $\phi^x_y$, si primero hacemos igual $h_x = h_y$
$$
\nabla^2 \phi[x,y] = \frac{- 4 \phi^i_j + \phi^{i+1}  + \phi^{i-1} + \phi_{j+1}  + \phi_{j-1}}{h^2}  +
\mathcal O(h^2)
$$

despejamos así el estado actual del sistema $\phi^i_j$

$$
\phi_j^i = \frac{\phi^{i+1}  + \phi^{i-1} + \phi_{j+1}  + \phi_{j-1}}{4} - \frac{h^2}{4}\nabla^2 \phi + \mathcal O(h^2) 
$$

reemplazando $\nabla^2 \phi = \rho$

$$
\phi_j^i = \frac{\phi^{i+1}  + \phi^{i-1} + \phi_{j+1}  + \phi_{j-1}}{4} - \frac{h^2}{4} \rho + \mathcal O(h^2) 
$$

___

### Algoritmo de Jacobi
- nuestro sistema puede ser representado por una matriz $A \phi = \rho$
- la matriz puedeser descompuesta en el componente diagonal (D) y no diagonales (L + U):

$$
A = D + L + U
$$

- al tener una aproximación o guess $\phi^{(0)}$ podemos computar mejores aproximacones:

$$
\phi^{(k+1)} = D^{-1} (\rho - (L+U)  \phi^{(k)})
$$

en este caso tomaremos esas ideas para le ecuación de Poisson:


#### Reglas empiricas de convergencia
hay algunas reglas empíricas para la convergencia del método de Jacobi. Estas son:

- Si la matriz A es diagonalmente dominante, entonces el método de Jacobi convergerá para cualquier vector inicial $\phi^{(0)}$.

- Si la matriz A es simétrica y definida positiva, entonces el método de Jacobi convergerá para cualquier vector inicial $\phi^{(0)}$.

- En general, se puede utilizar el radio espectral de la matriz de iteración del método de Jacobi para estimar la velocidad de convergencia. Si el radio espectral es menor que 1, entonces el método convergerá.

- Se puede utilizar la siguiente fórmula para estimar el número de iteraciones necesarias para alcanzar un cierto nivel de precisión:
    - $n$ la cantidad de iteraciones
    - donde $\epsilon$ es la tolerancia deseada
    - $|\phi^{(k)} - \phi^{(k-1)}|$ la norma del vector
    - $\rho$ es el radio espectral de la matriz de iteración
    

$$
n \geq log_{10}(
\frac{\epsilon}{|\phi^{(k)} - \phi^{(k-1)}|}
)  \frac{1}{log_{10}(1/ρ)}
$$


*El radio espectral de una matriz se define como la magnitud del valor propio máximo de la matriz.* y se ha demostrado que es una medida de convergencia, una forma de calcularlo:

```python
import numpy as np

def spectral_radius(iteration_matrix, tolerance=1e-10, max_iterations=1000, num_vectors=10):
    # Calcula el radio espectral de una matriz de iteración

    # Crea una matriz identidad de la misma dimensión que la matriz de iteración
    n = iteration_matrix.shape[0]
    identity = np.identity(n)

    max_ratio = 0
    for i in range(num_vectors):
        # Inicializa un vector aleatorio
        x = np.random.rand(n)
        x = x / np.linalg.norm(x)

        # Itera la matriz de iteración hasta que la norma del error sea menor que la tolerancia
        for j in range(max_iterations):
            x_new = np.dot(iteration_matrix, x)
            e = np.linalg.norm(x_new - x)
            if e < tolerance:
                break
            x = x_new / np.linalg.norm(x_new)

        # Calcula el cociente entre la norma del error en la iteración actual y la iteración anterior
        if i > 0:
            ratio = e / prev_e
            if ratio > max_ratio:
                max_ratio = ratio

        prev_e = e

    # Calcula el radio espectral como el valor absoluto del máximo cociente obtenido
    spectral_radius = np.abs(max_ratio)

    return spectral_radius

```



Es importante tener en cuenta que estas son solo reglas empíricas y no garantizan la convergencia en todos los casos.

____

La regla de iteración es:
$$
(\phi^{(k+1)})_j^i = \frac{(\phi^{(k)})^{i+1}_j  + (\phi^{(k)})^{i-1}_j + (\phi^{(k)})^i_{j+1}  + (\phi^{(k)})^i_{j-1}}{4} - \frac{h^2}{4} \rho^i_j + \mathcal O(h^2) 
$$

Recordemos los terminos

$$
(\phi_j^{i+1} + \phi_j^{i-1} + \phi^i_{j+1} + \phi^i_{j-1})/4
$$ 