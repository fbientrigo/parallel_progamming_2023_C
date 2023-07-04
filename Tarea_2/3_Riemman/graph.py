import sys
import pandas as pd
import matplotlib.pyplot as plt
from numpy import pi

# Obtener el nombre del archivo CSV desde los argumentos de línea de comandos
if len(sys.argv) < 2:
    print("Debe proporcionar el nombre del archivo CSV como argumento.")
    sys.exit(1)

csv_file = sys.argv[1]

# Leer el archivo CSV utilizando pandas
data = pd.read_csv(csv_file)

# Extraer las columnas k y Valor
k = data['k']
valor = data['Valor']

# Crear el gráfico de dispersión
plt.plot(k, valor, label='C code')
plt.scatter(k, valor, alpha=0.3)
plt.hlines(pi*pi/6, min(k), max(k), colors='r',linestyles='--', label='$\zeta(2)=\pi^2/6$')
plt.xlabel('k')
plt.ylabel('Valor')
plt.title(f'Gráfico de {csv_file}')
plt.legend()
plt.grid(True)

# Mostrar el gráfico
plt.show()
