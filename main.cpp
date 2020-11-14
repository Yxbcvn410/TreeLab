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
typedef std::shared_ptr<AVLTree<T>> Tree;

int main() {
    Tree tree(new AVLTree<T>());
    int m = 150000;
    double f = 0.5;
    for (int i = 0; i < m; ++i) {
          tree->add(i);
//        std::cout << "here is the tree after addititon " << i << '\n';
//        tree->print();
//        std::cout << '\n';
    }
    bool flag = true;
    for (int i = 0; i < 2 * m; ++i) {
        if ((i < m) != tree->check(i)) {
            flag = false;
            std::cout << "ERROR on addition test: ";
            if (tree->check(i))
                std::cout << i << " is claimed to be included, but should not" << std::endl;
            else
                std::cout << i << " is claimed to be not included, but should" << std::endl;

        }
    }
    for (int i = m * f; i < m; ++i) {
        tree->remove(i);
//        std::cout << "here is the tree after removing " << i << '\n';
//        tree->print();
//        std::cout << '\n';
    }
    for (int i = 0; i < 2 * m; ++i) {
        if ((i < (int) (m * f)) != tree->check(i)) {
            flag = false;
            std::cout << "ERROR on deletion test: ";
            if (tree->check(i))
                std::cout << i << " is claimed to be included, but should not";
            else
                std::cout << i << " is claimed to be not included, but should";
            std::cout << std::endl;
        }
    }
    for (int i = 0; i < m; ++i)
        tree->add(i);
    for (int i = 0; i < 2 * m; ++i) {
        if ((i < m) != tree->check(i)) {
            flag = false;
            std::cout << "ERROR on addition test 2: ";
            if (tree->check(i))
                std::cout << i << " is claimed to be included, but should not" << std::endl;
            else
                std::cout << i << " is claimed to be not included, but should" << std::endl;

        }
    }
    std::cout << "Test " << (flag ? "passed" : "failed") << std::endl;
};