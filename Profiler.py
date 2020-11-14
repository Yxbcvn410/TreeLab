import os
import subprocess
import matplotlib.pyplot as pl

from TestGenerator import generate_tests

test_dir = 'tests/'
benchmark_dir = 'benchmarks/'
exec_path = '/home/alexander/CLionProjects/TreeLab/cmake-build-debug/TreeLab'  # TODO

ns = [5000 * 2 ** k for k in range(0, 8)]
m = 20

TEST_TYPE_TITLES = {
    0: 'ADD',
    1: 'CHECK',
    2: 'DELETE'
}
TREES = ['BlackRedTree', 'AVLTree']
TREE_TYPE = TREES[0]


def spawn_process(tree_type, tree_sz, test_no):
    if not os.path.exists(benchmark_dir):
        os.mkdir(benchmark_dir)
    if not os.path.exists(test_dir + f'{tree_sz}_tree'):
        return  # no tree detected
    tree_filename = os.path.abspath(test_dir) + os.path.sep + f'{tree_sz}_tree'
    i = 1
    while os.path.exists(test_dir + f'{tree_sz}_test_#{i}'):
        test_filename = os.path.abspath(test_dir) + os.path.sep + f'{tree_sz}_test_#{i}'
        benchmark_filename = os.path.abspath(benchmark_dir) + os.path.sep + f'{tree_sz}_benchmark_#{i}'
        print(f'Running test {i + 1} of {m} at N={tree_sz}')
        with subprocess.Popen([exec_path], stdin=subprocess.PIPE, universal_newlines=True) as proc:
            proc.stdin.write(tree_type + '\n')
            proc.stdin.write(tree_filename + '\n')
            proc.stdin.write(test_filename + '\n')
            proc.stdin.write(benchmark_filename + '\n')
            proc.stdin.write(str(test_no) + '\n')
        i += 1
    print('-' * 50)


def extract_data(tree_sz):
    datas = ([], [], [])
    i = 1
    while os.path.exists(benchmark_dir + f'{tree_sz}_benchmark_#{i}'):
        benchmark_filename = benchmark_dir + f'{tree_sz}_benchmark_#{i}'
        with open(benchmark_filename) as benchmark_reader:
            for data in datas:
                new_line = [float(a) for a in benchmark_reader.__next__().split()]
                data.extend(new_line)
        i += 1
    return tuple(sorted(data) for data in datas)


def generate_plot_data(tree_sizes):
    y_data = ([], [], [])
    for sz in tree_sizes:
        data = extract_data(sz)
        for i in TEST_TYPE_TITLES:
            y_data[i].append(data[i])
    return y_data


def plot_boxes(y_data, x_data, title=''):
    fig, ax = pl.subplots()
    pl.title(title)
    ax.boxplot(y_data, labels=x_data)
    pl.show()


if __name__ == '__main__':
    generate_tests(ns, 100000, test_dir)

    # Perform tests
    for n in ns:
        spawn_process(TREE_TYPE, n, m)

    plot_data = generate_plot_data(ns)
    for i in TEST_TYPE_TITLES:
        plot_boxes(plot_data[i], ns, f'Test {TEST_TYPE_TITLES[i]} on {TREE_TYPE}')
