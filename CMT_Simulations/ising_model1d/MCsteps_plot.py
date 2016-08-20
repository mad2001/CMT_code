"""Script for plotting data from the Ising model simulation.

Created July 7, 2016
Morgan A. Daly (mad2001@wildcats.unh.edu)
"""

import matplotlib.pyplot as plt
import numpy as np


def output():
    fname = 'MCsteps.txt'
    data = np.loadtxt(fname, delimiter=', ')

    plt.rc('text', usetex=True)
    plt.rc('font', family='serif')

    plt.semilogx(data[:, 0], data[:, 1], 'o')
    plt.xlabel(r'Number of Measurements')
    plt.ylabel(r'Average Energy')
    plt.xlim(-10, 1000000)
    plt.ylim(-1.496, -1.482)
    plt.grid()

    plt.title(r'Energy vs Number of Measurements at T=1')
    plt.savefig('MCsteps_plot')


if __name__ == '__main__':
    output()
