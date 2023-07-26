import numpy as np
import matplotlib.pyplot as plt

def read_data(filename):
    data = np.loadtxt(filename)
    return data

def combine_data(processes, rows, cols, file):
    full_data = np.empty((rows* processes , cols))
    for i in range(processes):
        print(i)
        filename = file+f"_{i}.txt"
        data = read_data(filename)
        full_data[i*rows:(i+1)*rows,:] = data
    return full_data


# Número de procesos y dimensiones de la matriz
num_processes = 6
rows = 60
cols = 60
rows = int(rows / num_processes)

# Combinar los datos de los procesos en una sola matriz
input_data = combine_data(num_processes, rows, cols, "initial_data_process")
full_data = combine_data(num_processes, rows, cols, "final_data_process")


# Graficar la matriz completa
plt.imshow(input_data, cmap='hot', origin='lower')
plt.colorbar()
plt.title("Input data")
plt.xlabel("Columnas")
plt.ylabel("Filas")
plt.show()


plt.imshow(full_data, cmap='hot', origin='lower')
plt.colorbar()
plt.title("Resultado final (Buffer)")
plt.xlabel("Columnas")
plt.ylabel("Filas")
plt.show()
