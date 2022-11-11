import matplotlib.pyplot as plt
from matplotlib.ticker import FormatStrFormatter, MultipleLocator, FixedLocator
import numpy as np

import matplotlib.pylab as pylab
params = {'legend.fontsize': 'x-large',
          # 'figure.figsize': (15, 5),
          'axes.labelsize': 'x-large',
          'axes.titlesize': 'x-large',
          'xtick.labelsize': 'x-large',
          'ytick.labelsize': 'x-large'}
pylab.rcParams.update(params)


def plot_rst():
    fig, axs = plt.subplots(2, 2)
    x = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    y = [[0.000670417, 0.000806958, 0.00118162, 0.00157813, 0.00355133,
         0.00339592, 0.00472558, 0.00608958, 0.00812046, 0.0105823],
         [0.00226137, 0.00668071, 0.00924779, 0.0147078, 0.020685,
             0.0253485, 0.0385605, 0.0642402, 0.0791447, 0.100133],
         [0.0252255, 0.0471201, 0.0807212, 0.126867, 0.183331,
             0.307251, 0.470255, 0.560036, 0.765524, 0.945007],
         [0.201521, 0.481279, 1.13565, 1.82739, 2.87178, 4.01972, 5.23455, 7.0411, 8.68439, 10.8517]]
    axs[0, 0].plot(x, y[0], '-c^', label="qplus")
    axs[0, 0].plot(x, y[1], '-ms', label="qjoin")
    axs[0, 0].plot(x, y[2], '-b^', label="haha")
    axs[0, 0].plot(x, y[3], '-r+', label="kaka")
    axs[0, 0].set_title('$r=10^4$', y=0.98, x=0.18, pad=-14)
    axs[0, 1].plot(x, y[1], '-gs', label="qplus")  # 'tab:orange'
    axs[0, 1].set_title('$r=10^5$', y=0.98, x=0.18, pad=-14)
    axs[1, 0].plot(x, y[2], '-gp', label="qplus")  # , 'tab:green'
    axs[1, 0].set_title('$r=10^6$', y=0.98, x=0.18, pad=-14)
    axs[1, 1].plot(x, y[3], '-g+', label="qplus")  # , 'tab:red'
    axs[1, 1].set_title('$r=10^7$', y=0.98, x=0.18, pad=-14)

    # plt.plot([1, 2, 3, 4], 'o-', label='QJoin')
    # plt.ylabel('some numbers')

    axs[0, 0].legend(loc='upper left', bbox_to_anchor=(
        -0.06, 1.28), ncol=4, fancybox=True, shadow=False,  prop={'size': 13})

    for i in range(2):
        for j in range(2):
            axs[i, j].set_yscale('log')
            axs[i, j].set_xscale('log')
            axs[i, j].set_xlim([1, 10])
            axs[i, j].set_ylim([1e-4, 10])

            majorLocator = FixedLocator([1, 2, 3, 4, 5, 6, 7, 8, 10])
            ymajorLocator = FixedLocator(
                [0.0001, 0.001, 0.01, 0.1, 1, 10])
            axs[i, j].xaxis.set_major_locator(
                majorLocator)  # plt.MaxNLocator(5)
            axs[i, j].yaxis.set_major_locator(
                ymajorLocator)
            axs[i, j].xaxis.set_major_formatter(FormatStrFormatter('%d'))
            plt.setp(axs[i, j].get_xminorticklabels(), visible=False)

            axs[i, j].grid(axis='y')

    for ax in axs.flat:
        ax.set(xlabel='duplicates d',
               ylabel='response time (s)')

    # Hide x labels and tick labels for top plots and y ticks for right plots.
    for ax in axs.flat:
        ax.label_outer()

    plt.subplots_adjust(left=0.15)
    plt.subplots_adjust(bottom=0.13)
    plt.show()


if __name__ == "__main__":
    plot_rst()
