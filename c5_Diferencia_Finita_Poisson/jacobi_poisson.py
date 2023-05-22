import numpy as np
import matplotlib.pyplot as plt

def generate_phi(n=100, m=100):
    # Generar la matriz phi inicial con valores aleatorios
    phi = np.random.rand(n, m)
    return phi

def generate_rho(n=100,m=100):
    # Generar el vector de carga rho
    x = np.linspace(-1, 1, n)
    y = np.linspace(-1, 1, m)
    xx, yy = np.meshgrid(x, y, sparse=True)
    rho = np.sin(np.pi * xx) * np.sin(np.pi * yy)
    return rho

def jacobi_poisson(phi, rho, h, max_iter=1000, tol=1e-6):
    """
    A partir del metodo iterativo de jacobi, resuelve
    la ecuación de poisson y realiza un analisis extra
    para obtener el radio espectral.
    """


    # Inicialización
    nx, ny = phi.shape
    phi_new = np.zeros_like(phi)
    num_iter = 0
    err = np.inf
    
    # Matrices de iteración
    a = 1 / h ** 2
    D = -4 * a * np.eye(nx * ny)
    L = a * np.eye(nx * ny, k=-1)
    U = a * np.eye(nx * ny, k=1)
    for i in range(ny):
        L[i * nx, i * nx - 1] = 0
        U[i * nx - 1, i * nx] = 0
    G = D - L - U
    invD = np.diag(-1 / np.diag(D))
    T = invD @ G
    
    # Radio espectral
    rho_T = np.max(np.abs(np.linalg.eigvals(T)))
    print(f"Radio espectral de T: {rho_T:.4f}")
    
    # Iteración
    while num_iter < max_iter and err > tol:
        for i in range(1, nx-1):
            for j in range(1, ny-1):
                phi_new[i,j] = (phi[i+1,j] + phi[i-1,j] + phi[i,j+1] + phi[i,j-1]) / 4 \
                            - a*rho[i,j]
        err = np.max(np.abs(phi_new - phi))
        phi[:] = phi_new[:]
        num_iter += 1
        
    return phi, num_iter, err


n = 50
m = 50

phi = generate_phi(n,m)
rho = generate_rho(n,m)

h = 1e-2

phi, num_iter, err = jacobi_poisson(phi, rho, h, max_iter=1000, tol=1e-6)


print(num_iter, err)

# ======= graficar phi
# Definir los límites de los ejes x e y
xmin, xmax, ymin, ymax = 0, n, 0, m

# Graficar la matriz phi
plt.imshow(phi, extent=[xmin, xmax, ymin, ymax])

# Agregar una barra de colores a la derecha
plt.colorbar()

# Agregar etiquetas a los ejes x e y
plt.xlabel('X')
plt.ylabel('Y')

# Agregar un título
plt.title(r'$\phi$')

# Mostrar el plot
plt.show()
