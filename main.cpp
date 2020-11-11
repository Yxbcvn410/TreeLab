//
// Created by alexander on 07.11.2020.
//

#include <bits/unique_ptr.h>
#include <iostream>
#include <set>
#include <random>
#include "AbstractTree.cpp"
#include "AVLTree.cpp"
#include "BlackRedTree.cpp"

typedef int T;
typedef std::unique_ptr<AbstractTree<T>> Tree;

int main() {
    Tree tree = std::make_unique<BlackRedTree<T>>();
    int m = 1000;
    for (int i = 0; i < m; ++i)
        tree->add(i);
    bool flag = true;
    for (int i = 0; i < 2 * m; ++i) {
        if ((i < 1000) != tree->check(i)) {
            flag = false;
            std::cout << "ERROR: ";
            if (i < 1000)
                std::cout << i << " is claimed to be not included, but should" << std::endl;
            else
                std::cout << i << " is claimed to be included, but should not" << std::endl;

        }
    }
    std::cout << "Test " << (flag ? "passed" : "failed") << std::endl;
    // TODO(Alsushka-lullo) Здесь добавить тесты.
};