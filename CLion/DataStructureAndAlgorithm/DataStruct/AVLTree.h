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
        tree_node *p_left;
        tree_node *p_right;
        size_type height;

        explicit TreeNode(const T &_data) {
            data = _data;
            p_right = p_left = nullptr;
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
            _clear(m_p_root);
        }

        size_type size() const {
            return m_size;
        }

        void clear() {
            _clear(m_p_root);
        }

        void insert(const T &_data) {
            _insert(_data, m_p_root);
        }

        void erase(const T &_data) {
            _erase(_data, m_p_root);
        }

        friend ostream& operator<<(ostream& out, const self& _self) {
            cout << "AVLTree(data): ";
            _foreach_print(_self.m_p_root);
            return out;
        }

    private:
        void _clear(tree_node *&_p_root);

        void _insert(const T &_data, tree_node *&_p_root);

        int _get_height(const tree_node *_p_root);

        void _erase(const T &_data, tree_node *&_p_root);

        tree_node *_find_node(tree_node *_p_root, const T &_data);

        tree_node *_find_node_parent(tree_node *_p_root, const T &_data);

        static void _foreach_print(tree_node *_p_root);

        // 左旋
        tree_node *LL(tree_node *_p_root);
        // 右旋
        tree_node *RR(tree_node *_p_root);
        // 左右旋
        tree_node *LR(tree_node *_p_root);
        // 右左旋
        tree_node *RL(tree_node *_p_root);

    private:
        tree_node *m_p_root; // 树根节点
        size_type m_size; // 元素个数
    };

    template<typename T>
    void AVLTree<T>::_clear(tree_node *&_p_root) {
        if (nullptr == _p_root) {
            return;
        }
        _clear(_p_root->p_left);
        _clear(_p_root->p_right);
        delete _p_root;
        _p_root = nullptr;
    }

    template<typename T>
    void AVLTree<T>::_insert(const T &_data, tree_node *&_p_root) {
        if (nullptr == _p_root) {
            _p_root = new tree_node(_data);
            ++m_size;
            return;
        }
        if (_data == _p_root->data) {
            return;
        } else {
            if (_data < _p_root->data) { // 向左子树插入
                _insert(_data, _p_root->p_left);
                if (_get_height(_p_root->p_left) - _get_height(_p_root->p_right) > 1) { // 高度差大于1需要旋转
                    if (_data < _p_root->p_left->data) { // 需要右旋 (/)
                        _p_root = RR(_p_root);
                    } else { // 需要左右旋 (<)
                        _p_root = LR(_p_root);
                    }
                }
            } else { // 向右子树插入
                _insert(_data, _p_root->p_right);
                if (_get_height(_p_root->p_right) - _get_height(_p_root->p_left) > 1) { // 高度差大于1需要旋转
                    if (_data > _p_root->p_right->data) { // 需要左旋 (\)
                        _p_root = LL(_p_root);
                    } else { // 需要右左旋 (>)
                        _p_root = RL(_p_root);
                    }
                }
            }
            // 设置当前节点高度
            size_type left_height = _get_height(_p_root->p_left);
            size_type right_height = _get_height(_p_root->p_right);
            _p_root->height = 1 + ((left_height > right_height) ? left_height : right_height);
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
    void AVLTree<T>::_erase(const T &_data, tree_node *&_p_root) {
        tree_node *p_del_node = _find_node(_p_root, _data);
        if (p_del_node) { // 如果找到了要删除的元素
            // 判断是否为根节点
            if (p_del_node == _p_root) { // 如果是根节点
                // 判断根节点是否有右孩子
                if (p_del_node->p_right) { // 如果有右孩子
                    // 让根节点的左孩子成为根节点右孩子的最左孩子
                    tree_node *p_tmp = p_del_node->p_right;
                    while (p_tmp->p_left) {
                        p_tmp = p_tmp->p_left;
                    }
                    p_tmp->p_left = p_del_node->p_left;
                    // 将根节点的右孩子作为新的根节点
                    _p_root = p_del_node->p_right;
                } else { // 如果根节点没有右孩子
                    // 将根节点的左孩子作为新的根节点
                    _p_root = p_del_node->p_left;
                }
            } else { // 如果不是根节点
                // 找到要删除节点的父节点
                tree_node *p_del_parent_node = _find_node_parent(_p_root, _data);
                // 判断要删除的节点是否有右孩子
                if (p_del_node->p_right) { // 如果要删除的节点有右孩子
                    // 让要删除节点的左孩子成为要删除节点的右孩子的最左孩子
                    tree_node *p_tmp = p_del_node->p_right;
                    while (p_tmp->p_left) {
                        p_tmp = p_tmp->p_left;
                    }
                    p_tmp->p_left = p_del_node->p_left;
                    // 将要删除节点的右孩子填补要删除节点的位置
                    if (p_del_node == p_del_parent_node->p_left) {
                        p_del_parent_node->p_left = p_del_node->p_right;
                    } else {
                        p_del_parent_node->p_right = p_del_node->p_right;
                    }
                } else { // 如果删除的节点没有右孩子
                    // 将要删除节点的左孩子填补要删除节点的位置
                    if (p_del_node == p_del_parent_node->p_left) {
                        p_del_parent_node->p_left = p_del_node->p_left;
                    } else {
                        p_del_parent_node->p_right = p_del_node->p_left;
                    }
                }
            }
            delete p_del_node;
            --m_size;
        }
    }

    template<typename T>
    typename AVLTree<T>::tree_node *AVLTree<T>::_find_node(tree_node *_p_root, const T &_data) {
        if (nullptr == _p_root) {
            return nullptr;
        }
        tree_node *p_tmp = _p_root;
        while (p_tmp && _data != p_tmp->data) {
            if (_data < p_tmp->data) {
                p_tmp = p_tmp->p_left;
            } else {
                p_tmp = p_tmp->p_right;
            }
        }

        return p_tmp;
    }

    template<typename T>
    typename AVLTree<T>::tree_node *AVLTree<T>::_find_node_parent(AVLTree::tree_node *_p_root, const T &_data) {
        if (nullptr == _p_root) {
            return nullptr;
        }
        tree_node *p_tmp = _p_root;
        tree_node *p_tmp_parent = nullptr;
        while (p_tmp && _data != p_tmp->data) {
            p_tmp_parent = p_tmp;
            if (_data < p_tmp->data) {
                p_tmp = p_tmp->p_left;
            } else {
                p_tmp = p_tmp->p_right;
            }
        }

        return p_tmp_parent;
    }

    template<typename T>
    void AVLTree<T>::_foreach_print(tree_node *_p_root) {
        if (nullptr == _p_root) {
            return;
        }
        _foreach_print(_p_root->p_left);
        cout << _p_root->data << " ";
        _foreach_print(_p_root->p_right);
    }

    template<typename T>
    typename AVLTree<T>::tree_node *AVLTree<T>::LL(AVLTree::tree_node *_p_root) {
        tree_node *p_tmp = _p_root->p_right;
        _p_root->p_right = p_tmp->p_left;
        p_tmp->p_left = _p_root;

        size_type left_height = _get_height(_p_root->p_left);
        size_type right_height = _get_height(_p_root->p_right);
        _p_root->height = 1 + ((left_height > right_height) ? left_height : right_height);
        left_height = _get_height(p_tmp->p_left);
        right_height = _get_height(p_tmp->p_right);
        p_tmp->height = 1 + ((left_height > right_height) ? left_height : right_height);
        return p_tmp;
    }

    template<typename T>
    typename AVLTree<T>::tree_node *AVLTree<T>::RR(AVLTree::tree_node *_p_root) {
        tree_node *p_tmp = _p_root->p_left;
        _p_root->p_left = p_tmp->p_right;
        p_tmp->p_right = _p_root;

        size_type left_height = _get_height(_p_root->p_left);
        size_type right_height = _get_height(_p_root->p_right);
        _p_root->height = 1 + ((left_height > right_height) ? left_height : right_height);
        left_height = _get_height(p_tmp->p_left);
        right_height = _get_height(p_tmp->p_right);
        p_tmp->height = 1 + ((left_height > right_height) ? left_height : right_height);

        return p_tmp;
    }

    template<typename T>
    typename AVLTree<T>::tree_node *AVLTree<T>::LR(AVLTree::tree_node *_p_root) {
        _p_root->p_left = LL(_p_root->p_left);

        return RR(_p_root);
    }

    template<typename T>
    typename AVLTree<T>::tree_node *AVLTree<T>::RL(AVLTree::tree_node *_p_root) {
        _p_root->p_right = RR(_p_root->p_right);

        return LL(_p_root);
    }
}

#endif //DATASTRUCTUREANDALGORITHM_AVLTREE_H
