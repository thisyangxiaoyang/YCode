//
// Created by 84865 on 2022/10/25.
//

#ifndef DATASTRUCTUREANDALGORITHM_SEQUENTIALBINARYTREE_H
#define DATASTRUCTUREANDALGORITHM_SEQUENTIALBINARYTREE_H

#include <cstddef>
#include <iostream>

namespace yang {

    template<typename T>
    struct SequentialBinaryTreeNode {
        using self = SequentialBinaryTreeNode<T>;

        T data;
        self *leftChild;
        self *rightChild;

        SequentialBinaryTreeNode(const T &_data, self *_left, self *_right) {
            data = _data;
            leftChild = _left;
            rightChild = _right;
        }
    };

    template<typename T>
    class SequentialBinaryTree {
        using self = SequentialBinaryTree<T>;
        using tree_node = SequentialBinaryTreeNode<T>;
        using size_type = size_t;

    public:
        SequentialBinaryTree() : m_p_root(nullptr), m_size(0) {}

        ~SequentialBinaryTree() {
            _clear(m_p_root);
        }

        size_type size() const {
            return m_size;
        }

        void clear() {
            _clear(m_p_root);
            m_p_root = nullptr;
        }

        void insert(const T &_data) {
            if (_insert(_data, &m_p_root)) {
                ++m_size;
            }
        }

        void erase(const T &_data) {
            if (_erase(_data, &m_p_root)) {
                --m_size;
            }
        }

        friend std::ostream &operator<<(std::ostream &out, self &_self) {
            _foreach_print(_self.m_p_root);
            return out;
        }

    private:
        void _clear(tree_node *_p_root) {
            if (nullptr == _p_root) {
                return;
            }
            _clear(_p_root->leftChild);
            _clear(_p_root->rightChild);
            delete _p_root;
        }

        bool _insert(const T &_data, tree_node **_p_root) {
            if (nullptr == *_p_root) {
                *_p_root = new tree_node(_data, nullptr, nullptr);
                return true;
            } else if ((*_p_root)->data == _data) {
                return false;
            } else {
                if (_data < (*_p_root)->data) {
                    return _insert(_data, &(*_p_root)->leftChild);
                } else {
                    return _insert(_data, &(*_p_root)->rightChild);
                }
            }
        }

        bool _erase(const T &_data, tree_node **_p_root) {
            tree_node *p_del_node = _find_node(*_p_root, _data);
            tree_node *p_del_parent_node = nullptr;
            if (p_del_node) {
                // 判断是否为根节点
                if (p_del_node == *_p_root) { // 如果是根节点
                    // 判断根节点是否有右孩子
                    if (p_del_node->rightChild) { // 如果有右孩子
                        // 让根节点的左孩子成为根节点右孩子的最左孩子
                        tree_node *tmp_node = p_del_node->rightChild;
                        while (tmp_node->leftChild) {
                            tmp_node = tmp_node->leftChild;
                        }
                        tmp_node->leftChild = p_del_node->leftChild;
                        (*_p_root) = p_del_node->rightChild;
                    } else { // 如果根节点没有右孩子
                        (*_p_root) = p_del_node->leftChild; // 让根节点的左孩子根
                    }
                    delete p_del_node;
                    return true;
                } else {
                    p_del_parent_node = *_p_root;
                    // 找到要删除节点的父节点
                    while (p_del_parent_node->leftChild != p_del_node &&
                           p_del_node->rightChild != p_del_node->rightChild) {
                        if (_data < p_del_parent_node->data) {
                            p_del_parent_node = p_del_parent_node->leftChild;
                        } else {
                            p_del_parent_node = p_del_parent_node->rightChild;
                        }
                    }
                    if (p_del_node->rightChild) {
                        tree_node *tmp_node = p_del_node->rightChild;
                        while (tmp_node->leftChild) {
                            tmp_node = tmp_node->leftChild;
                        }
                        tmp_node->leftChild = p_del_node->leftChild;
                        if (p_del_parent_node->rightChild == p_del_node) {
                            p_del_parent_node->rightChild = p_del_node->rightChild;
                        } else {
                            p_del_parent_node->leftChild = p_del_node->rightChild;
                        }
                    } else {
                        if (p_del_parent_node->rightChild == p_del_node) {
                            p_del_parent_node->rightChild = p_del_node->leftChild;
                        } else {
                            p_del_parent_node->leftChild = p_del_node->leftChild;
                        }
                    }
                    delete p_del_node;
                }

            } else {
                return false;
            }
        }

        tree_node *_find_node(tree_node *_p_root, const T &_data) {
            if (nullptr == _p_root) {
                return nullptr;
            }
            tree_node *tmp = _p_root;
            while (tmp) {
                if (_data == tmp->data) {
                    return tmp;
                } else if (_data < tmp->data) {
                    tmp = tmp->leftChild;
                } else {
                    tmp = tmp->rightChild;
                }
            }
            return nullptr;
        }

        static void _foreach_print(tree_node *_p_root) {
            if (nullptr == _p_root) {
                return;
            }
            _foreach_print(_p_root->leftChild);
            std::cout << _p_root->data << " ";
            _foreach_print(_p_root->rightChild);
        }

    private:
        tree_node *m_p_root;
        size_type m_size;
    };
}


#endif //DATASTRUCTUREANDALGORITHM_SEQUENTIALBINARYTREE_H
