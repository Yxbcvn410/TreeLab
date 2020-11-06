//
// Created by alexander on 07.11.2020.
//

#include "AbstractTree.cpp"

// TODO(Yxbcvn410) Здесь будет красно-чёрное дерево

template<typename T>
class BlackRedTree : public AbstractTree<T> {
public:
    BlackRedTree() {}

    BlackRedTree(BlackRedTree &other) {} // Конструктор копирования

    BlackRedTree(BlackRedTree &&other) {} // Конструктор перемещения

    ~BlackRedTree() {}

    void add(T value) {}

    bool check(T value) {}

    void remove(T value) {}
};