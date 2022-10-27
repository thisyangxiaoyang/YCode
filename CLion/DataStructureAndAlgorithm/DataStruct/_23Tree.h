//
// Created by 84865 on 2022/10/27.
//

#ifndef DATASTRUCTUREANDALGORITHM__23TREE_H
#define DATASTRUCTUREANDALGORITHM__23TREE_H

#include <cstddef>
#include <iostream>
#include <memory.h>

using namespace std;

namespace yang {

    template<typename T>
    class _23Tree {
        struct TreeNode {
            int count; // 标记当前节点类型(2,3,4)
            T data_arr[3]; // 2节点只用[0], 3节点用[0],[1], 4节点都用
            TreeNode *p_node_arr[4]; // 2节点用[0],[1], 3节点用[0],[1],[2], 4节点都用

            TreeNode() {
                count = 0;
                memset(data_arr, 0, sizeof(T) * 3);
                memset(p_node_arr, 0, sizeof(ptrdiff_t) * 4);
            }
        };

        using tree_node = TreeNode;

    public:
        _23Tree() : m_p_root(nullptr) {}

        ~_23Tree() = default;

        // 插入一个节点倒树中
        void insert(const T &_data) {
            if (nullptr != m_p_root) {
                _insert(m_p_root, nullptr, _data);
            } else {
                m_p_root = new tree_node;
                m_p_root->count = 1;
                m_p_root->data_arr[0] = _data;
            }
        }

        void test() {
            _foreach_print(m_p_root);
        }

    private:
        void _insert(tree_node *_p_node, tree_node *_p_parent, const T &_data);

        void _foreach_print(const tree_node *_p_node);

    private:
        tree_node *m_p_root; // 指向树根的指针
    };

    template<typename T>
    void _23Tree<T>::_insert(_23Tree::tree_node *_p_node, _23Tree::tree_node *_p_parent, const T &_data) {
        if (nullptr == _p_node) return;
        if (0 == _p_node->count) { // 如果当前节点是新建立的
            _p_node->data_arr[0] = _data;
            ++_p_node->count;
            return;
        }
        if (1 == _p_node->count) { // 第二个节点
            if (_data < _p_node->data_arr[0]) { // 往左边添加
                if (_p_node->p_node_arr[0]) { // 如果有孩子
                    _insert(_p_node->p_node_arr[0], _p_node, _data);
                } else { // 如果没有孩子
                    _p_node->data_arr[1] = _p_node->data_arr[0];
                    _p_node->data_arr[0] = _data;
                    ++_p_node->count;
                }
            } else { // 往右边添加
                if (_p_node->p_node_arr[0]) { // 如果有孩子
                    _insert(_p_node->p_node_arr[1], _p_node, _data);
                } else { // 如果没有孩子
                    _p_node->data_arr[1] = _data;
                    ++_p_node->count;
                }
            }
        } else { // 第三个节点
            if (_data < _p_node->data_arr[0]) { // 往最左边添加
                if (_p_node->p_node_arr[0]) { // 右孩子
                    _insert(_p_node->p_node_arr[0], _p_node, _data);
                } else { // 没有孩子
                    _p_node->data_arr[2] = _p_node->data_arr[1];
                    _p_node->data_arr[1] = _p_node->data_arr[0];
                    _p_node->data_arr[0] = _data;
                    ++_p_node->count;
                }
            } else if (_data < _p_node->data_arr[1]) { // 往中间添加
                if (_p_node->p_node_arr[0]) { // 右孩子
                    _insert(_p_node->p_node_arr[1], _p_node, _data);
                } else { // 没有孩子
                    _p_node->data_arr[2] = _p_node->data_arr[1];
                    _p_node->data_arr[1] = _data;
                    ++_p_node->count;
                }
            } else { // 往右边添加
                if (_p_node->p_node_arr[0]) { // 右孩子
                    _insert(_p_node->p_node_arr[2], _p_node, _data);
                } else { // 没有孩子
                    _p_node->data_arr[2] = _data;
                    ++_p_node->count;
                }
            }
        }
        if (3 == _p_node->count) { // 4 节点拆分
            // 1 先拆成 3 个 2 节点
            // 1.1 创建两个新节点
            auto *p_node1 = new tree_node;
            auto *p_node2 = new tree_node;
            // 1.2 左右两边
            // 1.2.1 p_node1 是 _p_node 左边那个
            p_node1->data_arr[0] = _p_node->data_arr[0];
            p_node1->count = 1;
            p_node1->p_node_arr[0] = _p_node->p_node_arr[0];
            p_node1->p_node_arr[1] = _p_node->p_node_arr[1];
            // 1.2.2 p_node2 是 _p_node 右边那个
            p_node2->data_arr[0] = _p_node->data_arr[2];
            p_node2->count = 1;
            p_node2->p_node_arr[0] = _p_node->p_node_arr[2];
            p_node2->p_node_arr[1] = _p_node->p_node_arr[3];

            // 2 3个2节点中的父节点对当前节点的父节点作插入
            // 2.1 临时存储中间那个(3个2节点中的父节点中存储的数据)
            T tmp = _p_node->data_arr[1];
            if (_p_parent) { // 如果有父节点
                // 找位置
                if (tmp < _p_parent->data_arr[0]) { // 左边
                    if (_p_parent->p_node_arr[2]) { // 右边有孩子
                        // 挪数据
                        _p_parent->data_arr[2] = _p_parent->data_arr[1];
                        _p_parent->data_arr[1] = _p_parent->data_arr[0];
                        _p_parent->data_arr[0] = tmp;
                        // 挪指针
                        _p_parent->p_node_arr[3] = _p_parent->p_node_arr[2];
                        _p_parent->p_node_arr[2] = _p_parent->p_node_arr[1];
                        _p_parent->p_node_arr[1] = p_node2;
                        _p_parent->p_node_arr[0] = p_node1;
                    } else if (_p_parent->p_node_arr[1]) { // 中间有孩子
                        // 挪数据
                        _p_parent->data_arr[1] = _p_parent->data_arr[0];
                        _p_parent->data_arr[0] = tmp;
                        // 挪指针
                        _p_parent->p_node_arr[2] = _p_parent->p_node_arr[1];
                        _p_parent->p_node_arr[1] = p_node2;
                        _p_parent->p_node_arr[0] = p_node1;
                    }

                } else if (1 == _p_parent->count ||
                           1 < _p_parent->count && tmp < _p_parent->data_arr[1]) { // 中间
                    if (_p_parent->p_node_arr[2]) { // 右边有孩子
                        // 挪数据
                        _p_parent->data_arr[2] = _p_parent->data_arr[1];
                        _p_parent->data_arr[1] = tmp;
                        // 挪指针
                        _p_parent->p_node_arr[3] = _p_parent->p_node_arr[2];
                        _p_parent->p_node_arr[2] = p_node2;
                        _p_parent->p_node_arr[1] = p_node1;
                    } else if (_p_parent->p_node_arr[1]) { // 中间有孩子
                        // 挪数据
                        _p_parent->data_arr[1] = tmp;
                        // 挪指针
                        _p_parent->p_node_arr[2] = p_node2;
                        _p_parent->p_node_arr[1] = p_node1;
                    }

                } else if (2 == _p_parent->count ||
                           2 < _p_parent->count && tmp < _p_parent->data_arr[2]) { // 右边
                    if (_p_parent->p_node_arr[2]) {
                        // 挪数据
                        _p_parent->data_arr[2] = tmp;
                        // 挪指针
                        _p_parent->p_node_arr[3] = p_node2;
                        _p_parent->p_node_arr[2] = p_node1;
                    }
                }
                ++_p_parent->count;
                delete _p_node; // 释放当前节点，因为已经插入到父节点中去了

            } else { // 如果没有父节点
                // 当前节点数据改变
                memset(_p_node->data_arr, 0, sizeof(T) * 3); // 清空
                _p_node->data_arr[0] = tmp;
                _p_node->count = 1;
                // 两个孩子指针指向两个新节点
                memset(_p_node->p_node_arr, 0, sizeof(ptrdiff_t) * 4); // 清空
                _p_node->p_node_arr[0] = p_node1;
                _p_node->p_node_arr[1] = p_node2;
            }
        }
    }

    template<typename T>
    void _23Tree<T>::_foreach_print(const tree_node *_p_node) {
        if (nullptr == _p_node->p_node_arr[1] && nullptr == _p_node->p_node_arr[2]) {
            for (int i = 0; i < _p_node->count; ++i) {
                cout << _p_node->data_arr[i] << " ";
            }
        } else if (2 == _p_node->count) {
            _foreach_print(_p_node->p_node_arr[0]);
            cout << _p_node->data_arr[0] << " ";
            _foreach_print(_p_node->p_node_arr[1]);
            cout << _p_node->data_arr[_p_node->count - 1] << " ";
            _foreach_print(_p_node->p_node_arr[2]);
        } else {
            _foreach_print(_p_node->p_node_arr[0]);
            for (int i = 0; i < _p_node->count; ++i) {
                cout << _p_node->data_arr[i] << " ";
            }
            _foreach_print(_p_node->p_node_arr[1]);
        }
    }
}

#endif //DATASTRUCTUREANDALGORITHM__23TREE_H
