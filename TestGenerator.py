import os
from random import randint

LINEARITY = 5


def generate_tests(ns, total_tests, test_dir):
    if not os.path.exists(test_dir):
        os.mkdir(test_dir)
    for n in ns:
        samples_q = int(n / 10)
        if samples_q >= total_tests:
            samples_q = total_tests
            tests_q = 1
        else:
            tests_q = int(total_tests / samples_q) + 1
            samples_q = int(total_tests / tests_q)
        export_tests_fast(n, tests_q, samples_q, test_dir)


def export_tests_fast(n, tests_q, samples_q, location=''):
    with open(location + f'{n}_tree', 'w') as tree_writer:
        print(f'Generating tree for N={n}')
        for j in range(n):
            tree_writer.write(str(j * 3 + randint(1, 5)) + ' ')
    for i in range(tests_q):
        print(f'Generating test for N={n}, {i + 1} of {tests_q}')
        with open(location + f'{n}_test_#{i + 1}', 'w') as test_writer:
            for j in range(samples_q):
                test_writer.write(str(int(n / samples_q) * j + randint(0, int(1.9 * n / samples_q))) + ' ')
    print('-' * 50)


def export_tests(n, tests_q, samples_q, location=''):
    tree = generate_test_tree(n)
    with open(location + f'{n}_tree', 'w') as tree_writer:
        print(f'Generating tree for N={n}')
        for a in tree:
            tree_writer.write(str(a) + ' ')
    for i in range(tests_q):
        print(f'Generating test for N={n}, {i + 1} of {tests_q}')
        with open(location + f'{n}_test_#{i + 1}', 'w') as test_writer:
            for a in generate_test_set(tree, samples_q):
                test_writer.write(str(a) + ' ')
    print('-' * 50)


def generate_test_tree(n):
    tree = [0]
    for i in range(1, n):
        next_el = randint(min(tree) - 3, max(tree))
        if next_el in tree:
            tree.append(max(tree) + randint(1, LINEARITY))
        else:
            tree.append(next_el)
    return tree


def generate_test_set(tree, samples_q):
    test_set = [0]
    n = len(tree)
    sparsity = int(len(tree) / samples_q)
    for i in range(samples_q):
        next_el = randint(min(test_set), max(test_set))
        idx = i * sparsity + randint(0, sparsity)
        if (next_el in test_set or randint(1, LINEARITY) == 1) and idx < len(tree):
            test_set.append(tree[idx])
        else:
            test_set.append(next_el)
    return test_set[1:]
