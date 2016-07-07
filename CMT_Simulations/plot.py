"""Script for plotting data from the Ising model simulation.

Created July 7, 2016
Morgan A. Daly (mad2001@wildcats.unh.edu)
"""

import matplotlib.pyplot as plt
import numpy as np


def output():
    fname = 'data.txt'
    data = np.loadtxt(fname, delimiter=', ')

    plt.rc('text', usetex=True)
    plt.rc('font', family='serif')
    plt.plot(data[:, 0], data[:, 1], 'bo')
    plt.hold(True)
    # plt.xlim(-1, 61)
    plt.ylabel(r'Energy')
    plt.xlabel(r'Temperature')
    plt.title(r'Energy vs Temperature')
    plt.grid(True)
    plt.savefig('ising_model_output')
    plt.show()


if __name__ == '__main__':
    output()
