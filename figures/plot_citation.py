import matplotlib.pyplot as plt
from matplotlib.ticker import FormatStrFormatter, MultipleLocator, FixedLocator, PercentFormatter
import numpy as np
import matplotlib.ticker as mtick

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
    y = [[0.00552598, 0.0188191, 0.187693, 0.299079, 0.723906, 1.13436, 1.5239, 1.91267],
         [0.134697, 0.224023, 0.375541, 0.486439,
             0.823741, 1.11632, 1.3732, 1.65196],
         [1.16445, 4.32473, 41.1544, 122.699, 305.741, 472.866, 623.226, 747.704],
         [31.457, 47.8217, 83.5088, 154.725, 259.882, 402.66, 580.812, 722.536]]
    # y = [[0.305453,	0.568535,	1.06303, 1.3468,	2.09494,	2.56107,	3.51752,	4.57332],
    #      [0.0156666,	0.0683826,	0.494824,	0.826786,
    #          1.6332,	2.64963,	3.85975,	5.0753],
    #      [93.8655, 170.181, 241.5, 306.047, 405.948, 510.887, 599.234, 743.272],
    #      [2.12507, 10.0493, 127.851, 218.328, 388.64, 565.314, 713.675, 847.131]]
    axs.plot(x, y[0], '-c^', label="index-SF=0.1")
    axs.plot(x, y[1], '-ms', label="SieveJoin-SF=0.1")
    axs.plot(x, y[2], '-b^', label="index-SF=1")
    axs.plot(x, y[3], '-r+', label="SieveJoin-SF=1")

    axs.legend()  # loc='upper left'

    axs.set_yscale('log')
    axs.set_xlim([0, 1])
    axs.set_ylim([1e-3, 1e3])

    # majorLocator = FixedLocator(
    #     [0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0])
    ymajorLocator = FixedLocator(
        [0.001, 0.01, 0.1, 1, 10, 100, 1000])
    # axs.xaxis.set_major_locator(
    #     majorLocator)  # plt.MaxNLocator(5)
    axs.yaxis.set_major_locator(
        ymajorLocator)
    axs.xaxis.set_major_formatter(PercentFormatter(1.0))
    plt.setp(axs.get_xminorticklabels(), visible=False)

    axs.grid(axis='y')

    axs.set(xlabel='proportion of intermediate join results',
            ylabel='query response time (s)')

    # Hide x labels and tick labels for top plots and y ticks for right plots.
    axs.label_outer()

    plt.subplots_adjust(left=0.15)
    plt.subplots_adjust(bottom=0.13)
    plt.show()


def plot_queryx_comparison(sf=0.1):
    fig, axs = plt.subplots(1, 1)
    x = [0.950467506, 0.803403965, 0.57910705, 0.427232255,
         0.233631503, 0.123669406, 0.054101442, 0.006537283]
    y = [[0.00552598, 0.0188191, 0.187693, 0.299079, 0.723906, 1.13436, 1.5239, 1.91267],
         [0.134697, 0.224023, 0.375541, 0.486439,
             0.823741, 1.11632, 1.3732, 1.65196],
         [1.16445, 4.32473, 41.1544, 122.699, 305.741, 472.866, 623.226, 747.704],
         [31.457, 47.8217, 83.5088, 154.725, 259.882, 402.66, 580.812, 722.536],
         [0.002554, 0.021830, 0.222335, 0.434496,
             0.863749, 1.303641, 1.788428, 2.240768],
         [0.000426746, 0.00360698, 0.0561153, 0.135796,
             0.527769, 0.911862, 1.30654, 1.73786],
         [0.230570, 2.288659, 22.485415, 44.817197,
             88.766733, 135.886965, 179.036015, 219.126915],
         [0.0297303, 0.574212, 18.3214, 53.2559, 183.026, 372.901, 532.084, 606.438]]
    if sf == 0.1:
        axs.plot(x, y[0], '-gH', label="INL-SF=0.1")
        axs.plot(x, y[1], '-yp', label="SieveJoin-SO-SF=0.1")
        axs.plot(x, y[4], '-ms', label="Umbra-SF=0.1")
        axs.plot(x, y[5], '-c^', label="SieveJoin-EO-SF=0.1")
    if sf == 1:
        axs.plot(x, y[2], '-gH', label="INL-SF=1")
        axs.plot(x, y[3], '-yp', label="SieveJoin-SO-SF=1")
        axs.plot(x, y[6], '-ms', label="Umbra-SF=1")
        axs.plot(x, y[7], '-c^', label="SieveJoin-EO-SF=1")

    axs.legend()  # loc='upper left'

    axs.set_yscale('log')
    axs.set_xlim([0, 1])
    # axs.set_ylim([1e-4, 1e1])

    ymajorLocator = FixedLocator(
        [0.0001, 0.001, 0.01, 0.1, 1, 10, 100, 1000])
    axs.yaxis.set_major_locator(
        ymajorLocator)
    axs.xaxis.set_major_formatter(PercentFormatter(1.0))
    plt.setp(axs.get_xminorticklabels(), visible=False)

    axs.grid(axis='y')

    axs.set(xlabel='proportion of intermediate join results',
            ylabel='query response time (s)')

    # Hide x labels and tick labels for top plots and y ticks for right plots.
    axs.label_outer()

    plt.subplots_adjust(left=0.15)
    plt.subplots_adjust(bottom=0.13)
    plt.show()


def plot_response_time():
    x = range(1, 3)
    t1 = [0.00229291, 0.0947505]
    t2 = [0.000728667, 0.00925989]  # .reverse()
    t3 = [0.102845, 1.025641]
    t4 = [0.019355, 0.083570]  # .reverse()
    t5 = [0.098828, 1.539928]

    fig = plt.figure()
    width = 0.12
    ax = fig.add_subplot(1, 1, 1)
    plt.bar([i-2*width for i in x], t1, width=width,
            label='SieveJoin-EO', color='y')
    plt.bar([i-1*width for i in x], t2,
            width=width, label="SieveJoin-EO_parallel", color='k')
    plt.bar([i+0*width for i in x], t3, width=width, label='Umbra', color='m')
    plt.bar([i+1*width for i in x], t4, width=width,
            label="Umbra_parallel", color='b')
    plt.bar([i+2*width for i in x], t5, width=width,
            label="MonetDB_parallel", color='g')
    ax.set(xlabel='query type',
           ylabel='query response time (s)')
    ax.set_xticks(x, ('3-clique', '4-clique'))
    ax.set_yscale('log')
    plt.legend(loc="upper left")
    plt.subplots_adjust(left=0.15)
    plt.subplots_adjust(bottom=0.13)
    plt.show()


def plot_size():
    x = range(1, 3)
    t1 = [352807, 352807]
    t2 = [3639, 10396]  # .reverse()
    t3 = [289065, 266447]
    t4 = [314795, 265650]  # .reverse()
    t5 = [279079, 247747]
    t6 = [0, 264708]

    fig = plt.figure()
    width = 0.12
    ax = fig.add_subplot(1, 1, 1)
    plt.bar([i-2*width for i in x], t1, width=width,
            label='citation', color='y')
    plt.bar([i-1*width for i in x], t2,
            width=width, label="citation-Type II", color='k')
    plt.bar([i+0*width for i in x], t3, width=width,
            label='citation-Type I-1', color='m')
    plt.bar([i+1*width for i in x], t4, width=width,
            label="citation-Type I-2", color='b')
    plt.bar([i+2*width for i in x], t5, width=width,
            label="citation-Type I-3", color='g')
    plt.bar([i+3*width for i in x], t6, width=width,
            label="citation-Type I-4")
    ax.set(xlabel='query type',
           ylabel='table size')
    ax.set_xticks(x, ('3-clique', '4-clique'))
    # ax.set_yscale('log')
    plt.legend(loc="center left")
    plt.subplots_adjust(left=0.20)
    plt.subplots_adjust(bottom=0.13)
    plt.show()


if __name__ == "__main__":

    plot_response_time()
    # plot_size()
