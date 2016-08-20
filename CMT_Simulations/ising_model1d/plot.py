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

    plt.figure(0)
    plt.subplot(2, 1, 1)
    plt.plot(data[:, 0], data[:, 1], 'bo')
    # plt.xlim(-1, 61)
    plt.ylabel(r'Energy')
    plt.xlabel(r'Temperature')
    plt.grid(True)

    plt.subplot(2, 1, 2)
    plt.plot(data[:, 0], data[:, 2], 'bo')
    plt.ylabel(r'Magnetization')
    plt.xlabel(r'Temperature')

    plt.grid(True)
    plt.savefig('output')

    plt.figure(1)
    plt.plot(data[:, 0], data[:, 3], 'o')
    plt.ylabel(r'Specific Heat')
    plt.xlabel(r'Temperature')
    plt.grid(True)
    plt.savefig('specific_heat')


if __name__ == '__main__':
    output()
