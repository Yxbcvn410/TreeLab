//
// Created by alexander on 07.11.2020.
//

#include <memory>
#include <iostream>
#include "AbstractTree.cpp"

template<typename T>
struct AVLTreeNode {
    typedef std::shared_ptr<AVLTreeNode> NodePtr;
    typedef std::weak_ptr<AVLTreeNode> WeakNodePtr;

    NodePtr leftChild = nullptr;
    NodePtr rightChild = nullptr;
    WeakNodePtr parent;

    bool isLeftChild = false;

    T value;
    std::int32_t height = 0;

    NodePtr getBrother() {
        return parent.lock() == nullptr ? nullptr :
               isLeftChild ? parent.lock()->rightChild :
               parent.lock()->leftChild;
    }
};

template<typename T>
class AVLTree : public AbstractTree<T> {
    typename AVLTreeNode<T>::NodePtr root = nullptr;

    typename AVLTreeNode<T>::NodePtr getNodeByValue(T value, typename std::shared_ptr<AVLTreeNode<T>> node) {
        while (node != nullptr) {
            if (node->value == value)
                return node;
            if (node->value < value) {
                if (node->rightChild == nullptr)
                    break;
                node = node->rightChild;
            } else {
                if (node->leftChild == nullptr)
                    break;
                node = node->leftChild;
            }
        }
        return node;
    }

    void remove_rec(const T& value, typename std::shared_ptr<AVLTreeNode<T>> node) {
        if (node == nullptr) return;

        auto parent = node->parent.lock();
        if (node->value < value) remove_rec(value, node->rightChild);
        else if (node->value > value) remove_rec(value, node->leftChild);
        //two children
        else if (node->leftChild != nullptr && node->rightChild != nullptr) {
            if (height(node->leftChild) <= height(node->rightChild)) {
                auto temp = node->rightChild;
                while (temp->leftChild != nullptr) {
                    temp = temp->leftChild;
                }
                node->value = temp->value;
                remove_rec(node->value, node->rightChild);
            } else {
                auto temp = node->leftChild;
                while (temp->rightChild != nullptr) {
                    temp = temp->rightChild;
                }
                node->value = temp->value;
                remove_rec(node->value, node->leftChild);
            }
        }

        //one or no child
        else {
            bool is_left = node->isLeftChild;
            if (node->leftChild != nullptr) {
                node->leftChild->parent = node->parent.lock();
                node = (node->leftChild);
            } else {
                if (node->rightChild != nullptr) node->rightChild->parent = node->parent.lock();
                node = (node->rightChild);
            }
            if (parent != nullptr) {
                if (is_left) {
                    parent->leftChild = node;
                    if (node != nullptr) node->isLeftChild = true;
                } else {
                    parent->rightChild = node;
                    if (node != nullptr) node->isLeftChild = false;
                }
            } else {
                root = node;
                node->isLeftChild = false;
            }
        }
        if (parent != nullptr) balance(parent);
    }

    std::int32_t height(const typename std::shared_ptr<AVLTreeNode<T>>& node) {
        return node == nullptr ? -1 : node->height;
    }

    void balance(typename std::shared_ptr<AVLTreeNode<T>> node) {
        if(node == nullptr) return;

        node->height = std::max(height(node->leftChild), height(node->rightChild)) + 1;

        if(height(node->leftChild) - height(node->rightChild) > ALLOWED_IMBALANCE) {
            if(height(node->leftChild->leftChild) >= height(node->leftChild->rightChild))
                rotateWithLeftChild(node);
            else
                doubleWithLeftChild(node);
        }
        else if(height(node->rightChild) - height(node->leftChild) > ALLOWED_IMBALANCE) {
            if(height(node->rightChild->rightChild) >= height(node->rightChild->leftChild))
                rotateWithRightChild(node);
            else
                doubleWithRightChild(node);
        }

        node->height = std::max(height(node->leftChild), height(node->rightChild)) + 1;
        balance(node->parent.lock());
   }

    void rotateWithLeftChild(typename std::shared_ptr<AVLTreeNode<T>> node)
    {
        auto left = (node->leftChild);

        if (left->rightChild != nullptr) {
            left->rightChild->parent = node;
            left->rightChild->isLeftChild = true;
        }
        node->leftChild = (left->rightChild);

        node->height = std::max(height(node->leftChild), height(node->rightChild)) + 1;
        left->height = std::max(height(left->leftChild), node->height) + 1;

        if (node->parent.lock() != nullptr) {
            if (node->isLeftChild) {
                node->parent.lock()->leftChild = left;
            } else {
                node->parent.lock()->rightChild = left;
            }
        }

        left->parent = node->parent.lock();
        node->parent = left;

        left->isLeftChild = node->isLeftChild;
        node->isLeftChild = false;

        left->rightChild = (node);
        node = (left);
        while (root->parent.lock() != nullptr) {
            root = root->parent.lock();
        }
    }

    void rotateWithRightChild(typename std::shared_ptr<AVLTreeNode<T>> node)
    {
        auto right = (node->rightChild);

        if (right->leftChild != nullptr) {
            right->leftChild->parent = node;
            right->leftChild->isLeftChild = false;
        }
        node->rightChild = (right->leftChild);

        node->height = std::max(height(node->leftChild), height(node->rightChild)) + 1;
        right->height = std::max(height(right->rightChild), node->height) + 1;

        if (node->parent.lock() != nullptr) {
            if (node->isLeftChild) {
                node->parent.lock()->leftChild = right;
            } else {
                node->parent.lock()->rightChild = right;
            }
        }


        right->parent = node->parent.lock();
        node->parent = right;

        right->isLeftChild = node->isLeftChild;
        node->isLeftChild = true;

        right->leftChild = (node);
        node = (right);

        while (root->parent.lock() != nullptr) {
            root = root->parent.lock();
        }
    }

    void doubleWithLeftChild(typename std::shared_ptr<AVLTreeNode<T>> node)
    {
        rotateWithRightChild(node->leftChild);
        rotateWithLeftChild(node);
    }

    void doubleWithRightChild(typename std::shared_ptr<AVLTreeNode<T>> node)
    {
        rotateWithLeftChild(node->rightChild);
        rotateWithRightChild(node);
    }

    void print_rec(typename std::shared_ptr<AVLTreeNode<T>> node)
    {
        if (node != nullptr) {
            std::cout << " { ";
            print_rec(node->leftChild);
            std::cout << node->value;
            print_rec(node->rightChild);
            std::cout << " } ";
        }
    }

public:
    AVLTree() = default;
    static const int ALLOWED_IMBALANCE = 1;

    void print()
    {
        print_rec(root);
    }

    void add(T value) {
        auto parent = getNodeByValue(value, root);
        auto newPtr = std::make_shared<AVLTreeNode<T>>();
        newPtr->value = value;

        if (parent == nullptr) {
            root = newPtr;
            return;
        }

        if (parent->value == value) {
            return;
        }

        newPtr->parent = parent;
        if (parent->value < value) {
            parent->rightChild = newPtr;
            balance(parent);
            return;
        }

        newPtr->isLeftChild = true;
        parent->leftChild = newPtr;
        balance(parent);
    }

    bool check(T value) {
        auto node = getNodeByValue(value, root);
        return node == nullptr ? false : node->value == value;
    }

    void remove(T value) {
        remove_rec(value, root);
    }
};