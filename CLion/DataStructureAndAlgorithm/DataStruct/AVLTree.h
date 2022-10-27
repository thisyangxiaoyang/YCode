//
// Created by 84865 on 2022/10/26.
//

#ifndef DATASTRUCTUREANDALGORITHM_AVLTREE_H
#define DATASTRUCTUREANDALGORITHM_AVLTREE_H

#include <cstddef>
#include <iostream>

using namespace std;

namespace yang {

    template<typename T>
    struct TreeNode {
        using tree_node = TreeNode<T>;
        using size_type = size_t;

        T data;
        tree_node *p_left_child;
        tree_node *p_right_child;
        size_type height;

        explicit TreeNode(const T &_data) {
            data = _data;
            p_right_child = p_left_child = nullptr;
            height = 1;
        }
    };

    template<typename T>
    class AVLTree {
        using self = AVLTree<T>;
        using tree_node = TreeNode<T>;
        using size_type = size_t;

    public:
        AVLTree() : m_p_root(nullptr), m_size(0) {}

        ~AVLTree() {
            _clear(&m_p_root);
        }

        size_type size() const {
            return m_size;
        }

        void clear() {
            _clear(&m_p_root);
        }

        void insert(const T &_data) {
            _insert(_data, &m_p_root);
        }

        void erase(const T &_data) {
            if (_erase(_data, &m_p_root)) {
                --m_size;
            }
        }

    private:
        void _clear(tree_node **_pp_root);

        void _insert(const T &_data, tree_node **_pp_root);

        int _get_height(const tree_node *_p_root);

        bool _erase(const T &_data, tree_node **_p_root);

        tree_node *_find_node(tree_node *_p_root, const T &_data);

        static void _foreach_print(tree_node *_p_root);

        // 旋转
        tree_node *LL(tree_node *_p_root);

        tree_node *RR(tree_node *_p_root);

        tree_node *LR(tree_node *_p_root);

        tree_node *RL(tree_node *_p_root);

    private:
        tree_node *m_p_root;
        size_type m_size;
    };

    template<typename T>
    void AVLTree<T>::_clear(tree_node **_pp_root) {
        if (nullptr == *_pp_root) {
            return;
        }
        _clear(&(*_pp_root)->p_left_child);
        _clear(&(*_pp_root)->p_right_child);
        delete *_pp_root;
        *_pp_root = nullptr;
    }

    template<typename T>
    void AVLTree<T>::_insert(const T &_data, tree_node **_pp_root) {
        if (nullptr == *_pp_root) {
            *_pp_root = new tree_node(_data);
            ++m_size;
            return;
        }
        if (_data == (*_pp_root)->data) {
            return;
        } else {
            if (_data < (*_pp_root)->data) { // 向左子树插入
                _insert(_data, &(*_pp_root)->p_left_child);
                if (_get_height((*_pp_root)->p_left_child) - _get_height((*_pp_root)->p_right_child) >
                    1) { // 高度差大于1需要旋转
                    if (_data < (*_pp_root)->p_left_child->data) { // 需要右旋
                        *_pp_root = RR(*_pp_root);
                    } else { // 需要左右旋
                        *_pp_root = LR(*_pp_root);
                    }
                }
            } else { // 向右子树插入
                _insert(_data, &(*_pp_root)->p_right_child);
                if (_get_height((*_pp_root)->p_right_child) - _get_height((*_pp_root)->p_left_child) >
                    1) { // 高度差大于1需要旋转
                    if (_data > (*_pp_root)->p_right_child->data) { // 需要左旋
                        *_pp_root = LL(*_pp_root);
                    } else { // 需要右左旋
                        *_pp_root = RL(*_pp_root);
                    }
                }
            }
            // 设置当前节点高度
            size_type left_height = _get_height((*_pp_root)->p_left_child);
            size_type right_height = _get_height((*_pp_root)->p_right_child);
            (*_pp_root)->height = 1 + ((left_height > right_height) ? left_height : right_height);
        }
    }

    template<typename T>
    int AVLTree<T>::_get_height(const tree_node *_p_root) {
        if (_p_root) {
            return _p_root->height;
        }
        return 0;
    }

    template<typename T>
    bool AVLTree<T>::_erase(const T &_data, tree_node **_p_root) {
        tree_node *p_del_node = _find_node(*_p_root, _data);
        tree_node *p_del_parent_node = nullptr;
        if (p_del_node) {
            // 判断是否为根节点
            if (p_del_node == *_p_root) { // 如果是根节点
                // 判断根节点是否有右孩子
                if (p_del_node->p_right_child) { // 如果有右孩子
                    // 让根节点的左孩子成为根节点右孩子的最左孩子
                    tree_node *tmp_node = p_del_node->p_right_child;
                    while (tmp_node->p_left_child) {
                        tmp_node = tmp_node->p_left_child;
                    }
                    tmp_node->p_left_child = p_del_node->p_left_child;
                    (*_p_root) = p_del_node->p_right_child;
                } else { // 如果根节点没有右孩子
                    (*_p_root) = p_del_node->p_left_child; // 让根节点的左孩子根
                }
                delete p_del_node;
                return true;
            } else {
                p_del_parent_node = *_p_root;
                // 找到要删除节点的父节点
                while (p_del_parent_node->p_left_child != p_del_node &&
                       p_del_node->p_right_child != p_del_node->p_right_child) {
                    if (_data < p_del_parent_node->data) {
                        p_del_parent_node = p_del_parent_node->p_left_child;
                    } else {
                        p_del_parent_node = p_del_parent_node->p_right_child;
                    }
                }
                if (p_del_node->p_right_child) {
                    tree_node *tmp_node = p_del_node->p_right_child;
                    while (tmp_node->p_left_child) {
                        tmp_node = tmp_node->p_left_child;
                    }
                    tmp_node->p_left_child = p_del_node->p_left_child;
                    if (p_del_parent_node->p_right_child == p_del_node) {
                        p_del_parent_node->p_right_child = p_del_node->p_right_child;
                    } else {
                        p_del_parent_node->p_left_child = p_del_node->p_right_child;
                    }
                } else {
                    if (p_del_parent_node->p_right_child == p_del_node) {
                        p_del_parent_node->p_right_child = p_del_node->p_left_child;
                    } else {
                        p_del_parent_node->p_left_child = p_del_node->p_left_child;
                    }
                }
                delete p_del_node;
            }

        } else {
            return false;
        }
        return true;
    }

    template<typename T>
    typename AVLTree<T>::tree_node *AVLTree<T>::_find_node(tree_node *_p_root, const T &_data) {
        if (nullptr == _p_root) {
            return nullptr;
        }
        tree_node *tmp = _p_root;
        while (tmp) {
            if (_data == tmp->data) {
                return tmp;
            } else if (_data < tmp->data) {
                tmp = tmp->p_left_child;
            } else {
                tmp = tmp->p_right_child;
            }
        }
        return nullptr;
    }

    template<typename T>
    void AVLTree<T>::_foreach_print(tree_node *_p_root) {
        if (nullptr == _p_root) {
            return;
        }
        _foreach_print(_p_root->p_left_child);
        std::cout << _p_root->data << " ";
        _foreach_print(_p_root->p_right_child);
    }

    template<typename T>
    typename AVLTree<T>::tree_node *AVLTree<T>::LL(AVLTree::tree_node *_p_root) {
        cout << "LL" << endl;
        tree_node *p_tmp = _p_root->p_right_child;
        _p_root->p_right_child = p_tmp->p_left_child;
        p_tmp->p_left_child = _p_root;

        size_type left_height = _get_height(_p_root->p_left_child);
        size_type right_height = _get_height(_p_root->p_right_child);
        _p_root->height = 1 + ((left_height > right_height) ? left_height : right_height);
        left_height = _get_height(p_tmp->p_left_child);
        right_height = _get_height(p_tmp->p_right_child);
        p_tmp->height = 1 + ((left_height > right_height) ? left_height : right_height);
        return p_tmp;
    }

    template<typename T>
    typename AVLTree<T>::tree_node *AVLTree<T>::RR(AVLTree::tree_node *_p_root) {
        cout << "RR" << endl;
        tree_node *p_tmp = _p_root->p_left_child;
        _p_root->p_left_child = p_tmp->p_right_child;
        p_tmp->p_right_child = _p_root;

        size_type left_height = _get_height(_p_root->p_left_child);
        size_type right_height = _get_height(_p_root->p_right_child);
        _p_root->height = 1 + ((left_height > right_height) ? left_height : right_height);
        left_height = _get_height(p_tmp->p_left_child);
        right_height = _get_height(p_tmp->p_right_child);
        p_tmp->height = 1 + ((left_height > right_height) ? left_height : right_height);

        return p_tmp;
    }

    template<typename T>
    typename AVLTree<T>::tree_node *AVLTree<T>::LR(AVLTree::tree_node *_p_root) {
        cout << "LR" << endl;
        _p_root->p_left_child = LL(_p_root->p_left_child);

        return RR(_p_root);
    }

    template<typename T>
    typename AVLTree<T>::tree_node *AVLTree<T>::RL(AVLTree::tree_node *_p_root) {
        cout << "RL" << endl;
        _p_root->p_right_child = RR(_p_root->p_right_child);

        return LL(_p_root);
    }
}

#endif //DATASTRUCTUREANDALGORITHM_AVLTREE_H
