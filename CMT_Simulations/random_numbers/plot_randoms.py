"""Script for testing random number generator.

Created July 7, 2016
Morgan A. Daly (mad2001@wildcats.unh.edu)
"""

import matplotlib.pyplot as plt
import numpy as np


def output():
    # load data from text file into numpy array
    data = np.loadtxt('random_numbers.txt', delimiter=', ')

    # sets matplotlib to use LaTeX
    plt.rc('text', usetex=True)
    plt.rc('font', family='serif')

    # plot data (with black dots for markers)
    plt.plot(data[:, 0], data[:, 1], 'k.')

    plt.title(r'Random Number Generator')
    plt.savefig('radom_number_test')


if __name__ == '__main__':
    output()
