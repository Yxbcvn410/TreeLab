//
// Created by alexander on 07.11.2020.
//

#include <bits/unique_ptr.h>
#include "AbstractTree.cpp"
#include "AVLTree.cpp"

typedef int T;
typedef std::unique_ptr<AbstractTree<T>> Tree;

int main() {
    Tree tree(new AVLTree<int>());

    // TODO(Alsushka-lullo) Здесь добавить тесты.
};