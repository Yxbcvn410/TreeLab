//
// Created by alexander on 07.11.2020.
//

#include "AbstractTree.cpp"

// TODO(fetisu): Здесь будет AVL-дерево. Конструкторы перемещения и копирования нужны!

template<typename T>
class AVLTree : public AbstractTree<T> {
public:
    AVLTree() {}

    AVLTree(AVLTree &other) {} // Конструктор копирования

    AVLTree(AVLTree &&other) {} // Конструктор перемещения

    ~AVLTree() {}

    void add(T value) {}

    bool check(T value) {}

    void remove(T value) {}
};