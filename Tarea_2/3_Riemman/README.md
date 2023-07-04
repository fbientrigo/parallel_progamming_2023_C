# Euler Riemman
$$
\zeta (s) = \sum_{n=0}^\infty n^{-s}
$$

Otra exprsión alternativa
$$
\zeta (s) = 2^s \lim_{k \to \infty} \sum_{i=0}^k
\sum_{j=0}^k \frac{(-1)^{i+1}}{(i+j)^s}
$$

```c
double Riemann_Zeta(double s, int k) { // O(k^2)
    double result = 0.0;

    for (int i = 0; i < k; i++) {
        for (int j = 0; j <k; j++) {
            result += (2*(i&1)-1)/pow(i+j,s);
        }
    }
    return result * pow(2,s);
}
```

___

resultados conocidos de la zeta de riemann:
$$
\zeta (0) = - \frac{1}{2}
$$

$$
\zeta (2) = \frac{\pi^2}{6}
$$

$$
\zeta (4) = \frac{\pi^4}{90}
$$
