//
// Created by alexander on 07.11.2020.
//

#include <bits/unique_ptr.h>
#include <iostream>
#include "AbstractTree.cpp"
#include "AVLTree.cpp"
#include "BlackRedTree.cpp"

typedef int T;
typedef std::unique_ptr<AbstractTree<T>> Tree;

int main() {
    Tree tree = std::make_unique<BlackRedTree<T>>();
    for (int i = 0; i < 100; ++i) {
        tree->add(i);
    }
    for (int i = 0; i < 20; ++i) {
        std::cout << (tree->check(i));
    }
    // TODO(Alsushka-lullo) Здесь добав0ить тесты.
};