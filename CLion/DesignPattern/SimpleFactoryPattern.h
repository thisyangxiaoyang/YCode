//
// Created by 84865 on 2022/10/19.
//

#ifndef DESIGNPATTERN_SIMPLEFACTORYPATTERN_H
#define DESIGNPATTERN_SIMPLEFACTORYPATTERN_H


// 抽象产品类
class Operation {
public:
    Operation() {
        m_left_val = 0.;
        m_right_val = 0.;
    }
    virtual ~Operation() = default;
    virtual void set_val(double _left_val, double _right_val) = 0;
    virtual double getResult() = 0;

protected:
    double m_left_val;
    double m_right_val;
};

// 具体产品
class AddOperation : public Operation {
public:
    AddOperation() {
        m_left_val = 0.;
        m_right_val = 0.;
    }
    void set_val(double _left_val, double _right_val) override {
        m_left_val = _left_val;
        m_right_val = _right_val;
    }
    double getResult() override {
        return m_left_val + m_right_val;
    }
};

class SubOperation : public Operation {
public:
    SubOperation() {
        m_left_val = 1.;
        m_right_val = 1.;
    }

    void set_val(double _left_val, double _right_val) override {
        m_left_val = _left_val;
        m_right_val = _right_val;
    }
    double getResult() override {
        return m_left_val - m_right_val;
    }
};

class MulOperation : public Operation {
public:
    MulOperation() {
        m_left_val = 0.;
        m_right_val = 0.;
    }

    void set_val(double _left_val, double _right_val) override {
        m_left_val = _left_val;
        m_right_val = _right_val;
    }
    double getResult() override {
        return m_left_val * m_right_val;
    }
};

class DevOperation : public Operation {
public:
    DevOperation() {
        m_left_val = 0.;
        m_right_val = 1.;
    }

    void set_val(double _left_val, double _right_val) override {
        m_left_val = _left_val;
        m_right_val = _right_val;
    }
    double getResult() override {
        return m_left_val / m_right_val;
    }
};

// 工厂类
class OperationFactory {
public:
    static Operation *createOperation(char c) {
        switch (c) {
            case '+':
                return new AddOperation;
                break;
            case '-':
                return new SubOperation;
                break;
            case '*':
                return new MulOperation;
                break;
            case '/':
                return new DevOperation;
                break;
            default:
                return nullptr;
                break;
        }
    }
};

#endif //DESIGNPATTERN_SIMPLEFACTORYPATTERN_H
