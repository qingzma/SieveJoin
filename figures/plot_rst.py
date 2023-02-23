import matplotlib.pyplot as plt
from matplotlib.ticker import FormatStrFormatter, MultipleLocator, FixedLocator
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


def plot_training_time():
    x = range(1, 11)
    t1 = [0.870565, 1.69033, 2.48929, 3.41601, 4.13399,
          4.85573, 5.90185, 6.58842, 7.47151, 8.15952]
    t2 = [2.58773, 5.39565, 8.33781, 11.742, 14.9685,
          18.5816, 22.164, 25.9066, 29.2775, 32.9037]
    t3 = [1.81334, 4.59818, 7.3311, 11.4623, 13.1157,
          15.4444, 18.8667, 21.2278, 22.3466, 23.7932]
    fig = plt.figure()
    width = 0.2
    ax = fig.add_subplot(1, 1, 1)
    plt.bar([i-width for i in x], t1, width=width, label='loading data')
    plt.bar(x, t2, width=width, label='building indexes')
    plt.bar([i+width for i in x], t3, width=width, label='building BFs')
    ax.set(xlabel='duplicates d',
           ylabel='training time (s)')
    plt.legend()
    plt.show()


def plot_size_reduction():
    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)
    ax.yaxis.set_major_formatter(mtick.PercentFormatter())
    x = range(1, 11)
    R = [0.0001 * i for i in x]
    S = [0.002, 0.002, 0.002, 0.002, 0.002, 0.002, 0.002, 0.002, 0.002, 0.002]
    r = [i*100 for i in R]
    s = [i*100 for i in S]
    plt.plot(x, r, 'r^-', label="R")
    plt.plot(x, s, '--gH', label="S")
    plt.legend()
    plt.ylim([0.000, 0.25])
    # plt.yscale('log')
    plt.subplots_adjust(left=0.21)
    ax.set(xlabel='duplicates d',
           ylabel='Relative size change (%)')
    plt.show()


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
    so = [[0.068281, 0.233512, 0.473551, 0.581129, 0.994807,
           1.04111, 1.03395, 1.24294, 1.31322, 1.37403],
          [0.0994538, 0.301406, 0.631885, 0.794441,
           1.32401, 1.47389, 1.65935, 2.03167, 2.32672, 2.6721],
          [0.438694, 1.17499, 2.06057, 3.13101, 4.64723,
              6.00616, 7.20811, 10.2325, 11.5352, 14.8902],
          [4.25922, 10.2423, 17.7055, 28.0664, 40.9068, 55.5931, 70.4869, 87.864, 117.43, 146.563]]

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
    monet = [[0.002569, 0.02231, 0.054736, 0.075930, 0.118438, 0.124263, 0.175821, 0.188078, 0.273316, 0.295888],
             [0.044971, 0.046100, 0.107986, 0.153800, 0.226570,
                 0.350084, 0.447128, 0.564865, 0.740552, 0.962421],
             [0.056090, 0.143893, 0.315960, 0.604172, 1.04219,
                 1.682680, 2.582645, 4.376749, 6.874695, 9.093795],
             [0.119949, 0.753584, 1.988712, 4.613389, 8.819586, 16.346233, 32.676958, 62.190009]]
    axs[0, 0].plot(x, so[0], '-yp', label="QJoin-SO")
    axs[0, 0].plot(x, y[0], '-kx', label="QJoin-EO_parallel")
    axs[0, 0].plot(x, yq[0], '-c^', label="QJoin-EO")
    axs[0, 0].plot(x, ya[0], '-ms', label="Umbra")
    axs[0, 0].plot(x, pg[0], '-b+', label="Postgres")
    axs[0, 0].plot(x, monet[0], '-gH', label="Monetdb_parallel")
    # axs[0, 0].plot(x, y[1], '-ms', label="qjoin")
    # axs[0, 0].plot(x, y[2], '-b^', label="haha")
    # axs[0, 0].plot(x, y[3], '-r+', label="kaka")
    axs[0, 0].set_title('$r=10^4$', y=0.98, x=0.18, pad=-14)
    axs[0, 1].plot(x, so[1], '-yp', label="QJoin-SO")
    axs[0, 1].plot(x, y[1], '-kx', label="QJoin-EO_parallel")
    axs[0, 1].plot(x, yq[1], '-c^', label="QJoin-EO")  # 'tab:orange'
    axs[0, 1].plot(x, ya[1], '-ms', label="Umbra")  # 'tab:orange'
    axs[0, 1].plot(x, pg[1], '-b+', label="Postgres")
    axs[0, 1].plot(x, monet[1], '-gH', label="Monetdb_parallel")
    axs[0, 1].set_title('$r=10^5$', y=0.98, x=0.18, pad=-14)
    axs[1, 0].plot(x, so[2], '-yp', label="QJoin-SO")
    axs[1, 0].plot(x, y[2], '-kx', label="QJoin-EO_parallel")
    axs[1, 0].plot(x, yq[2], '-c^', label="QJoin-EO")  # , 'tab:green'
    axs[1, 0].plot(x, ya[2], '-ms', label="Umbra")  # , 'tab:green'
    axs[1, 0].plot(x[:8], pg[2][:8], '-b+', label="Postgres")
    axs[1, 0].plot(x, monet[2], '-gH', label="Monetdb_parallel")
    axs[1, 0].set_title('$r=10^6$', y=0.98, x=0.18, pad=-14)
    axs[1, 1].plot(x, so[3], '-yp', label="QJoin-SO")
    axs[1, 1].plot(x, y[3], '-kx', label="QJoin-EO_parallel")
    axs[1, 1].plot(x, yq[3], '-c^', label="QJoin-EO")  # , 'tab:red'
    axs[1, 1].plot(x, ya[3], '-ms', label="Umbra")  # , 'tab:red'
    axs[1, 1].plot(x[:4]+x[5:], pg[3][:4]+pg[3][5:], '-b+', label="Postgres")
    axs[1, 1].plot(x[:8], monet[3], '-gH', label="Monetdb_parallel")
    axs[1, 1].set_title('$r=10^7$', y=0.98, x=0.18, pad=-14)

    # plt.plot([1, 2, 3, 4], 'o-', label='QJoin')
    # plt.ylabel('some numbers')

    axs[0, 0].legend(loc='upper left', bbox_to_anchor=(
        -0.06, 1.78), ncol=2, fancybox=True, shadow=False,  prop={'size': 13})
    # axs[0, 0].legend()

    for i in range(2):
        for j in range(2):
            axs[i, j].set_yscale('log')
            axs[i, j].set_xscale('log')
            axs[i, j].set_xlim([1, 10])
            axs[i, j].set_ylim([10**(-4+i*2), 10 ** 3])
            # axs[i, j].set_ylim([10**(-4+i*2), 10 ** 3])

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
    plt.subplots_adjust(top=0.76)
    plt.show()


if __name__ == "__main__":
    plot_rst()
    # plot_size_reduction()
    # plot_training_time()
