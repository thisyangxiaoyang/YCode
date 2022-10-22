//
// Created by 84865 on 2022/10/19.
//

#ifndef DESIGNPATTERN_SINGLETONPATTERN_H
#define DESIGNPATTERN_SINGLETONPATTERN_H

#include <iostream>

// 单例模式
template <typename T>
class SingletonPattern {
public:
    static T* instance() { // 提供静态函数
        if (nullptr == m_instance)
            m_instance = new T();
        return m_instance;
    }

private:
    static T* m_instance; // 静态指针
    SingletonPattern() = default; // 构造函数私有化
    SingletonPattern(const SingletonPattern<T>&) = default;
    ~SingletonPattern() = default;
    SingletonPattern<T>& operator=(const SingletonPattern<T>&) = default;
};
template <typename T>
T* SingletonPattern<T>::m_instance = nullptr;

class A {
    friend class SingletonPattern<A>;
public:
    void show() const {
        std::cout << m_name << std::endl;
    }

private:
    A() : m_name("A") {}
    A(const A&) = default;
    ~A() = default;
    A& operator=(const A&) = default;
private:
    string m_name;
};

class B {
    friend class SingletonPattern<B>;
public:
    void show() const {
        std::cout << m_name << std::endl;
    }

private:
    B() : m_name("B") {}
    B(const B&) = default;
    ~B() = default;
    B& operator=(const B&) = default;
private:
    string m_name;
};


#endif //DESIGNPATTERN_SINGLETONPATTERN_H
