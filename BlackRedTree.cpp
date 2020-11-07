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
        return parent == nullptr ? nullptr :
               isLeftChild ? parent.lock()->rightChild :
               parent.lock()->leftChild;
    }
};

template<typename T>
class BlackRedTree : public AbstractTree<T> {
    std::shared_ptr<BlackRedNode<T>> head = nullptr;

    void FixNode(std::weak_ptr<BlackRedNode<T>> node) {
        // TODO
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
        FixNode(newPt);
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