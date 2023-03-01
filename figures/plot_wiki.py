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


def plot_response_time():
    x = range(1, 3)
    t1 = [0.5687, 39.188]
    t2 = [0.0269477, 1.95643]  # .reverse()
    t3 = [0.045950,   1.2596]
    t4 = [0.010540, 0.138099]  # .reverse()
    t5 = [0.136776, 2.831473]
    t6 = [0.030587, 0.970907]
    t7 = [0.007427, 0.130801]

    fig = plt.figure()
    width = 0.1
    ax = fig.add_subplot(1, 1, 1)
    plt.bar([i-3*width for i in x], t1, width=width,
            label='SieveJoin-EO', color='y')
    plt.bar([i-2*width for i in x], t2,
            width=width, label="SieveJoin-EO_parallel", color='k')
    plt.bar([i-1*width for i in x], t3, width=width, label='Umbra', color='m')
    plt.bar([i+0*width for i in x], t4, width=width,
            label="Umbra_parallel", color='b')
    plt.bar([i+1*width for i in x], t5, width=width,
            label="MonetDB_parallel", color='g')
    plt.bar([i+2*width for i in x], t6, width=width,
            label="SieveJoin-multi")
    plt.bar([i+3*width for i in x], t7, width=width,
            label="SieveJoin-multi_parallel",)
    ax.set(xlabel='query type',
           ylabel='query response time (s)')
    ax.set_xticks(x, ('3-clique', '4-clique'))
    ax.set_yscale('log')
    plt.legend(loc="upper left", fancybox=True, framealpha=0.01)
    plt.subplots_adjust(left=0.15)
    plt.subplots_adjust(bottom=0.13)
    plt.show()


def plot_size():
    x = range(1, 3)
    t1 = [103689, 103689]
    t2 = [37171, 39174]  # .reverse()
    t3 = [57650, 57650]
    t4 = [57934, 57650]  # .reverse()
    t5 = [40421, 40421]
    t6 = [0, 57650]

    fig = plt.figure()
    width = 0.12
    ax = fig.add_subplot(1, 1, 1)
    plt.bar([i-2*width for i in x], t1, width=width,
            label='wiki', color='y')
    plt.bar([i-1*width for i in x], t2,
            width=width, label="wiki-Type II", color='k')
    plt.bar([i+0*width for i in x], t3, width=width,
            label='wiki-Type I-1', color='m')
    plt.bar([i+1*width for i in x], t4, width=width,
            label="wiki-Type I-2", color='b')
    plt.bar([i+2*width for i in x], t5, width=width,
            label="wiki-Type I-3", color='g')
    plt.bar([i+3*width for i in x], t6, width=width,
            label="wiki-Type I-4")
    ax.set(xlabel='query type',
           ylabel='table size')
    ax.set_xticks(x, ('3-clique', '4-clique'))
    # ax.set_yscale('log')
    plt.legend(loc="upper left", fancybox=True, framealpha=0.4)
    plt.subplots_adjust(left=0.20)
    plt.subplots_adjust(bottom=0.13)
    plt.show()


if __name__ == "__main__":

    plot_response_time()
#     plot_size()
