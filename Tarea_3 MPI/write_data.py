import csv
import matplotlib.pyplot as plt

def create_matrix_with_charges(rows, cols, charges):
    matrix = [[0 for _ in range(cols)] for _ in range(rows)]
    for charge in charges:
        row, col, magnitude = charge
        matrix[row][col] = magnitude
    return matrix

def write_to_csv(data, filename):
    with open(filename, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        for row in data:
            writer.writerow(row)

if __name__ == "__main__":
    rows = 60
    cols = 60

    # Especificar las posiciones de las cargas junto con su magnitud
    q = 16.9
    charges = [(20, 10, -q), (40, 10, -q), (30, 28, q),
               (20, 35, -q), (40, 35, -q), (30, 58, q)]

    # Crear la matriz con las cargas especificadas
    matrix_with_charges = create_matrix_with_charges(rows, cols, charges)
    plt.imshow(matrix_with_charges, cmap='viridis', origin='lower')
    plt.colorbar()
    plt.title("Input data")
    plt.xlabel("Columnas")
    plt.ylabel("Filas")
    plt.show()

    # Escribir los datos en el archivo CSV
    csv_filename = 'input_data.csv'
    write_to_csv(matrix_with_charges, csv_filename)

    print(f"Matriz 100x100 con cargas escrita en {csv_filename}.")
