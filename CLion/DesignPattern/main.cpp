#include <iostream>
using namespace std;
#include "SingletonPattern.h"
#include "SimpleFactoryPattern.h"

void test01() {
    SingletonPattern<A>::instance()->show();
    SingletonPattern<B>::instance()->show();
}

void test02() {
    double left_val = 9.0;
    double right_val = 3.0;
    cout << "left_val = " << left_val << ", " << "right_val = " << right_val << endl;
    Operation* add = OperationFactory::createOperation('+');
    add->set_val(left_val, right_val);
    cout << "add: " << add->getResult() << endl;
    delete add;
    Operation* sub = OperationFactory::createOperation('-');
    sub->set_val(left_val, right_val);
    cout << "sub: " << sub->getResult() << endl;
    delete sub;
    Operation* mul = OperationFactory::createOperation('*');
    mul->set_val(left_val, right_val);
    cout << "mul: " << mul->getResult() << endl;
    delete mul;
    Operation* dev = OperationFactory::createOperation('/');
    dev->set_val(left_val, right_val);
    cout << "dev: " << dev->getResult() << endl;
    delete dev;
}

int main() {
    cout << "设计模式" << endl;
    test01();
//    test02();


    return 0;
}
