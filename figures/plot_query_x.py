import matplotlib.pyplot as plt
from matplotlib.ticker import FormatStrFormatter, MultipleLocator, FixedLocator, PercentFormatter
import numpy as np

import matplotlib.pylab as pylab
params = {'legend.fontsize': 'x-large',
          # 'figure.figsize': (15, 5),
          'axes.labelsize': 'x-large',
          'axes.titlesize': 'x-large',
          'xtick.labelsize': 'x-large',
          'ytick.labelsize': 'x-large'}
pylab.rcParams.update(params)


def plot_queryx():
    fig, axs = plt.subplots(1, 1)
    x = [0.950467506, 0.803403965, 0.57910705, 0.427232255,
         0.233631503, 0.123669406, 0.054101442, 0.006537283]
    y = [[0.305453,	0.568535,	1.06303, 1.3468,	2.09494,	2.56107,	3.51752,	4.57332],
         [0.0156666,	0.0683826,	0.494824,	0.826786,
             1.6332,	2.64963,	3.85975,	5.0753],
         [93.8655, 170.181, 241.5, 306.047, 405.948, 510.887, 599.234, 743.272],
         [2.12507, 10.0493, 127.851, 218.328, 388.64, 565.314, 713.675, 847.131]]
    axs.plot(x, y[0], '-c^', label="index-SF=0.1")
    axs.plot(x, y[1], '-ms', label="qjoin-SF=0.1")
    axs.plot(x, y[2], '-b^', label="index-SF=1")
    axs.plot(x, y[3], '-r+', label="qjoin-SF=1")

    axs.legend()  # loc='upper left'

    axs.set_yscale('log')
    axs.set_xlim([0, 1])
    axs.set_ylim([1e-2, 1e3])

    # majorLocator = FixedLocator(
    #     [0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0])
    ymajorLocator = FixedLocator(
        [0.01, 0.1, 1, 10, 100, 1000])
    # axs.xaxis.set_major_locator(
    #     majorLocator)  # plt.MaxNLocator(5)
    axs.yaxis.set_major_locator(
        ymajorLocator)
    axs.xaxis.set_major_formatter(PercentFormatter(1.0))
    plt.setp(axs.get_xminorticklabels(), visible=False)

    axs.grid(axis='y')

    axs.set(xlabel='proportion of intermediate join path',
            ylabel='query response time (s)')

    # Hide x labels and tick labels for top plots and y ticks for right plots.
    axs.label_outer()

    plt.subplots_adjust(left=0.15)
    plt.subplots_adjust(bottom=0.13)
    plt.show()


if __name__ == "__main__":
    plot_queryx()
