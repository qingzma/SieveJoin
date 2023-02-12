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
    # y = [[0.000670417, 0.000806958, 0.00118162, 0.00157813, 0.00355133,
    #      0.00339592, 0.00472558, 0.00608958, 0.00812046, 0.0105823],
    #      [0.00226137, 0.00668071, 0.00924779, 0.0147078, 0.020685,
    #          0.0253485, 0.0385605, 0.0642402, 0.0791447, 0.100133],
    #      [0.0252255, 0.0471201, 0.0807212, 0.126867, 0.183331,
    #          0.307251, 0.470255, 0.560036, 0.765524, 0.945007],
    #      [0.201521, 0.481279, 1.13565, 1.82739, 2.87178, 4.01972, 5.23455, 7.0411, 8.68439, 10.8517]]
    y = [[0.000701621, 0.000861311, 0.00097785, 0.00155279, 0.00218908, 0.0026799, 0.00414783, 0.00434758, 0.00524219, 0.00663672],
         [0.00192706, 0.00362337, 0.00636762, 0.00992103, 0.0182043,
             0.0199562, 0.0259782, 0.0310069, 0.04169, 0.0483575],
         [0.012743, 0.0285731, 0.0497722, 0.0736743, 0.120383,
             0.145708, 0.172175, 0.237443, 0.295016, 0.390122],
         [0.127651, 0.304785, 0.623084, 0.772751, 1.10117, 1.61639, 2.23422, 2.77625, 3.54992, 4.51862,]]
    yq = [[0.0025264, 0.00390939, 0.00806264, 0.0135874, 0.0244933, 0.0308965, 0.0419365, 0.0530391, 0.0740668, 0.0917784],
          [0.0147791, 0.0405832, 0.0901201, 0.141017, 0.226754,
              0.306729, 0.419618, 0.540697, 0.738217, 0.913077],
          [0.183658, 0.496855, 0.892106, 1.54656, 2.31941,
              3.19995, 4.17376, 5.45203, 7.48338, 9.21112],
          [1.97781, 5.92834, 11.5248, 19.5735, 28.1663, 40.0474, 56.0138, 73.1838, 92.7161, 116.338]]
    ya = [[0.231316, 1.713806, 2.223775, 2.858917, 3.107949, 4.008492, 4.347302, 4.746305, 5.123605, 6.595236],
          [0.266152, 1.808859, 2.437907, 3.272724, 4.011665,
              5.440156, 6.591605, 7.971647, 9.679269, 12.527022],
          [0.492621, 1.465222, 3.278662, 6.166018, 10.520653,
              16.751855, 25.938525, 37.333896, 52.323100, 69.614001],
          [3.463342, 9.687129, 23.842403, 48.848001, 88.891393, 148.665806, 227.903852, 332.314109, 481.760886, 652.308499]]
    pg = [[0.888, 5.814, 11.355, 16.903, 24.022, 31.941, 41.139, 52.129, 66.234, 72.027],
          [1.286, 6.608, 12.36, 18.006, 25.657,
              35.404, 46.362, 57.763, 75.111, 89.605],
          [1.313, 7.501, 14.476, 16.164, 38.316, 57.834, 80.624, 110.729, 0, 0],
          [5.221, 18.300, 45.156, 92.470, 0, 487.807, 594.298, 776.952, 1018.487, 1313.165]]
    axs[0, 0].plot(x, yq[0], '-c^', label="qplus")
    axs[0, 0].plot(x, ya[0], '-ms', label="Umbra")
    axs[0, 0].plot(x, pg[0], '-b+', label="Postgres")
    # axs[0, 0].plot(x, y[1], '-ms', label="qjoin")
    # axs[0, 0].plot(x, y[2], '-b^', label="haha")
    # axs[0, 0].plot(x, y[3], '-r+', label="kaka")
    axs[0, 0].set_title('$r=10^4$', y=0.98, x=0.18, pad=-14)
    axs[0, 1].plot(x, yq[1], '-c^', label="qplus")  # 'tab:orange'
    axs[0, 1].plot(x, ya[1], '-ms', label="Umbra")  # 'tab:orange'
    axs[0, 1].plot(x, pg[1], '-b+', label="Postgres")
    axs[0, 1].set_title('$r=10^5$', y=0.98, x=0.18, pad=-14)
    axs[1, 0].plot(x, yq[2], '-c^', label="qplus")  # , 'tab:green'
    axs[1, 0].plot(x, ya[2], '-ms', label="Umbra")  # , 'tab:green'
    axs[1, 0].plot(x[:8], pg[2][:8], '-b+', label="Postgres")
    axs[1, 0].set_title('$r=10^6$', y=0.98, x=0.18, pad=-14)
    axs[1, 1].plot(x, yq[3], '-c^', label="qplus")  # , 'tab:red'
    axs[1, 1].plot(x, ya[3], '-ms', label="Umbra")  # , 'tab:red'
    axs[1, 1].plot(x[:4]+x[5:], pg[3][:4]+pg[3][5:], '-b+', label="Postgres")
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
            axs[i, j].set_ylim([10**(-3+i*2), 10 ** 3])
            # axs[i, j].set_ylim([10**(-3+i*2), 10 ** (i+2)])

            # [1, 2, 3, 4, 5, 6, 7, 8, 10]
            majorLocator = FixedLocator([1, 5, 10])
            ymajorLocator = FixedLocator(
                [0.0001, 0.001, 0.01, 0.1, 1, 10, 100, 1000])
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
