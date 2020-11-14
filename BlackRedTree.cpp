//
// Created by alexander on 07.11.2020.
//

#include <memory>
#include "AbstractTree.cpp"

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
bool isBlack(typename BlackRedNode<T>::NodePtr node) {
    return node == nullptr or node->isBlack;
}

template<typename T>
class BlackRedTree : public AbstractTree<T> {
    typename BlackRedNode<T>::NodePtr head = nullptr;

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
        newInPlace->isLeftChild = pt->isLeftChild;
        pt->parent = newInPlace;
        pt->isLeftChild = left;
        auto ptNewChild = left ? newInPlace->leftChild : newInPlace->rightChild;
        if (ptNewChild != nullptr) {
            ptNewChild->parent = pt;
            ptNewChild->isLeftChild = not left;
        }
        if (left) {
            pt->rightChild = ptNewChild;
            newInPlace->leftChild = pt;
        } else {
            pt->leftChild = ptNewChild;
            newInPlace->rightChild = pt;
        }
        // Обновим указатель на корень
        while (head->parent.lock() != nullptr)
            head = head->parent.lock();
    }

    void addHelper(typename BlackRedNode<T>::NodePtr node) {
        // Метод вызывается для красного node
        auto parent = node->parent.lock();
        if (parent == nullptr) {
            // Случай 1
            node->isBlack = true;
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
            addHelper(parent->parent.lock());
            return;
        }
        auto grandpa = parent->parent.lock();
        if (parent->isLeftChild) { // В зависимости от расположенности родителя...
            if (not node->isLeftChild) {
                // Случай 4
                rotate(parent, true);
                std::swap(node, parent);
            }
            // Случай 5
            rotate(grandpa, false);
            grandpa->isBlack = false;
            parent->isBlack = true;
        } else {
            if (node->isLeftChild) {
                // Случай 4
                rotate(parent, false);
                std::swap(node, parent);
            }
            // Случай 5
            rotate(grandpa, true);
            grandpa->isBlack = false;
            parent->isBlack = true;
        }
    }

    void removeHelper(typename BlackRedNode<T>::NodePtr s) {
        // Важно: s - брат узла, с которым надо разобраться. Реализовано так, поскольку возможно N == nullptr
        auto p = s->parent.lock();

        if (p == nullptr) {
            // Случай 1 - обновляем ссылку на корень
            head = s;
            return;
        }
        if (not s->isBlack) {
            // Случай 2
            p->isBlack = false;
            s->isBlack = true;
            bool k = s->isLeftChild;
            s = s->isLeftChild ? s->rightChild : s->leftChild;
            rotate(p, not k);
        }
        // Начиная отсюда s гарантированно чёрный
        if (p->isBlack
            and isBlack<T>(s->leftChild)
            and isBlack<T>(s->rightChild)) {
            // Случай 3
            s->isBlack = false;
            if (p->parent.lock() != nullptr)
                removeHelper(p->getBrother());
            return;
        }
        if (not p->isBlack
            and isBlack<T>(s->leftChild)
            and isBlack<T>(s->rightChild)) {
            // Случай 4
            s->isBlack = false;
            p->isBlack = true;
            return;
        }

        // Случай 5
        if (s->isLeftChild and isBlack<T>(s->leftChild) and not isBlack<T>(s->rightChild)) {
            s->isBlack = false;
            s->rightChild->isBlack = true;
            rotate(s, true);
            s = s->parent.lock();
        } else if (not s->isLeftChild and isBlack<T>(s->rightChild) and not isBlack<T>(s->leftChild)) {
            s->isBlack = false;
            s->leftChild->isBlack = true;
            rotate(s, false);
            s = s->parent.lock();
        }

        // Случай 6
        if (s->isLeftChild and not isBlack<T>(s->leftChild)) {
            s->leftChild->isBlack = true;
            s->isBlack = p->isBlack;
            p->isBlack = true;
            rotate(p, false);
        } else if (not s->isLeftChild and not isBlack<T>(s->rightChild)) {
            s->rightChild->isBlack = true;
            s->isBlack = p->isBlack;
            p->isBlack = true;
            rotate(p, true);
        }
    }

    typename BlackRedNode<T>::NodePtr getNodeByValue(T value) {
        std::shared_ptr<BlackRedNode<T>> pt = head;
        while (pt != nullptr) {
            if (pt->value == value)
                return pt;
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
        return pt;
    }

public:
    BlackRedTree() = default;

    // Конструкторы перемещения и т. п. не нужны - смарт-пойнтеры же!

    void add(T value) {
        auto parent = getNodeByValue(value);
        auto newPtr = std::make_shared<BlackRedNode<T>>();
        newPtr->value = value;
        if (parent == nullptr) {
            newPtr->isBlack = true;
            head = newPtr;
            return;
        } else if (parent->value == value) {
            return;
        }
        newPtr->parent = parent;
        if (parent->value < value) {
            newPtr->isLeftChild = false;
            parent->rightChild = newPtr;
        } else {
            newPtr->isLeftChild = true;
            parent->leftChild = newPtr;
        }
        addHelper(newPtr);
    }

    bool check(T value) {
        auto pt = getNodeByValue(value);
        return pt == nullptr ? false : pt->value == value;
    }

    void remove(T value) {
        auto node = getNodeByValue(value);
        if (node == nullptr or node->value != value)
            return;

        // Если у узла оба потомка, выбираем ближайший слева
        if (node->leftChild != nullptr && node->rightChild != nullptr) {
            auto nearest = node->leftChild;
            while (nearest->rightChild != nullptr)
                nearest = nearest->rightChild;
            node->value = nearest->value;
            node = nearest;
        }

        auto parent = node->parent.lock();
        auto child = node->leftChild != nullptr ? node->leftChild : node->rightChild;
        auto bro = node->getBrother();

        // Заменяем node на child
        if (parent != nullptr) {
            if (node->isLeftChild) {
                parent->leftChild = child;
            } else {
                parent->rightChild = child;
            }
        }
        // Проверяем корень
        if (node == head)
            head = child;
        if (head == nullptr)
            return;

        if (not node->isBlack)
            // node красный - потомков нет
            return;
        if (not isBlack<T>(child)) {
            // node чёрный, а child красный - смело убираем node и перекрашиваем child
            child->parent = node->parent;
            child->isLeftChild = node->isLeftChild;
            child->isBlack = true;
            return;
        }
        removeHelper(bro);
    }
};