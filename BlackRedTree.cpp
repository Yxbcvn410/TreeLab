//
// Created by alexander on 07.11.2020.
//


#include <memory>
#include "AbstractTree.cpp"

// TODO(Yxbcvn410) Здесь будет красно-чёрное дерево. Пока оно просто красное

template<typename T>
struct BlackRedNode {
    typedef std::shared_ptr<BlackRedNode> NodePtr;
    typedef std::weak_ptr<BlackRedNode> WeakNodePtr;

    NodePtr leftChild = nullptr;
    NodePtr rightChild = nullptr;
    WeakNodePtr parent;

    bool isLeftChild = false;
    bool isBlack = false;

    T value;

    NodePtr getBrother() {
        return parent.lock() == nullptr ? nullptr :
               isLeftChild ? parent.lock()->rightChild :
               parent.lock()->leftChild;
    }
};

template<typename T>
void rotate(typename std::shared_ptr<BlackRedNode<T>> pt, bool left) {
    auto newInPlace = left ? pt->rightChild : pt->leftChild;
    if (newInPlace == nullptr)
        throw std::exception();
    auto parent = pt->parent.lock();
    if (parent != nullptr) {
        if (pt->isLeftChild)
            parent->leftChild = newInPlace;
        else
            parent->rightChild = newInPlace;
    }
    newInPlace->parent = parent;
    pt->parent = newInPlace;
    if (left) {
        pt->rightChild = newInPlace->leftChild;
        if (pt->rightChild != nullptr)
            pt->rightChild->parent = pt;
        newInPlace->leftChild = pt;
    } else {
        pt->leftChild = newInPlace->rightChild;
        if (pt->leftChild != nullptr)
            pt->leftChild->parent = pt;
        newInPlace->rightChild = pt;
    }
}

template<typename T>
class BlackRedTree : public AbstractTree<T> {
    std::shared_ptr<BlackRedNode<T>> head = nullptr;

    void fixAdd(std::weak_ptr<BlackRedNode<T>> node) {
        // Метод вызывается для красного node
        auto pt = node.lock();
        auto parent = pt->parent.lock();
        if (parent == nullptr) {
            // Случай 1
            pt->isBlack = true;
            return;
        }
        if (parent->isBlack) {
            // Случай 2
            return;
        }
        if (not parent->isBlack and parent->getBrother() != nullptr and not parent->getBrother()->isBlack) {
            // Случай 3
            parent->isBlack = true;
            parent->getBrother()->isBlack = true;
            parent->parent.lock()->isBlack = false;
            fixAdd(parent->parent);
            return;
        }
        auto grandpa = parent->parent.lock();
        if (parent->isLeftChild) { // В зависимости от расположенности родителя...
            if (not pt->isLeftChild) {
                // Случай 4
                rotate(parent, true);
                std::swap(pt, parent);
            }
            // Случай 5
            rotate(grandpa, false);
            grandpa->isBlack = false;
            parent->isBlack = true;
        } else {
            if (pt->isLeftChild) {
                // Случай 4
                rotate(parent, false);
                std::swap(pt, parent);
            }
            // Случай 5
            rotate(grandpa, true);
            grandpa->isBlack = false;
            parent->isBlack = true;
        }
        // На случай если голова сместилась
        if (head == grandpa)
            head = grandpa->parent.lock();
    }

public:
    BlackRedTree() = default;

    // Конструкторы копирования и т. п. не нужны - смарт-пойнтеры же!

    void add(T value) {
        auto newPt = std::make_shared<BlackRedNode<T>>();
        newPt->value = value;
        if (head == nullptr) {
            newPt->isBlack = true;
            head = newPt;
            return;
        }

        std::shared_ptr<BlackRedNode<T>> pt = head;
        while (true) {
            if (pt->value == value)
                return;
            if (pt->value < value) {
                if (pt->rightChild == nullptr)
                    break;
                pt = pt->rightChild;
            } else {
                if (pt->leftChild == nullptr)
                    break;
                pt = pt->leftChild;
            }
        }
        newPt->parent = pt;
        if (pt->value < value) {
            newPt->isLeftChild = false;
            pt->rightChild = newPt;
        } else {
            newPt->isLeftChild = true;
            pt->leftChild = newPt;
        }
        fixAdd(newPt);
    }

    bool check(T value) {
        std::shared_ptr<BlackRedNode<T>> pt = head;
        while (pt != nullptr) {
            if (pt->value == value)
                return true;
            if (pt->value < value)
                pt = pt->rightChild;
            else
                pt = pt->leftChild;
        }
        return false;
    }

    void remove(T value) {
        // TODO
    }

};