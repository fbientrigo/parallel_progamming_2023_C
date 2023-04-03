import numpy as np
import matplotlib.pyplot as plt
import argparse


def main(args):

    data = np.genfromtxt(args.name)
    N = int(np.sqrt(len(data))) # asume cuadrado de datos
    data = data.reshape((N,N))


    plt.imshow(data)


    # if -force is used, only execute the export_force_function()
    if args.save:
        plt.savefig(f"./plots/{args.title}.png")
    
    plt.show()





if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Plotter for output of poisson')
    parser.add_argument('-name', type=str, help='name of the file')
    parser.add_argument('-title', type=str, help='name for output')
    parser.add_argument('--save', help='exports and plots force data', action='store_true')
    args = parser.parse_args()
    
    if args.name:
        main(args)