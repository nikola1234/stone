#include <iostream>
#include <memory>
#include <string.h>

//抽象产品基类
class Iproduct
{
public:
    Iproduct(){};
    virtual ~Iproduct(){};
    
public:
    virtual void func(void) = 0;
};
// 抽象工厂基类
class abstractfactory
{
public:
    abstractfactory() {};
    ~abstractfactory() {};

public:
    virtual std::unique_ptr<Iproduct> CreateProduct(std::string des) = 0;
};

// A产品类 实体类
class CaProduct : public Iproduct
{
public:
    CaProduct() {};
    ~CaProduct() {};
    
public:
    void func(void) {
        std::cout << "ca func!" << std::endl;
    }
};

// B产品类 实体类
class CbProduct : public Iproduct
{
public:
    CbProduct() {};
    ~CbProduct() {};
    
public:
    void func(void) {
        std::cout << "cb func!" << std::endl;
    }
};

// C产品类 实体类
class CcProduct : public Iproduct
{
public:
    CcProduct() {};
    ~CcProduct() {};
    
public:
    void func(void) {
        std::cout << "cc func!" << std::endl;
    }
};

//工厂实现类1
class ConcreteFactory1 : public abstractfactory
{
public:
    ConcreteFactory1(){};
    ~ConcreteFactory1(){};

    std::unique_ptr<Iproduct> CreateProduct(std::string des) {
        if (des == "A") {
            return std::unique_ptr<Iproduct>(new CaProduct());
        } else if (des == "B") {
            return std::unique_ptr<Iproduct>(new CbProduct());
        } else {
            return nullptr;
        }
    }
};

//工厂实现类2
class ConcreteFactory2 : public abstractfactory
{
public:
    ConcreteFactory2(){};
    ~ConcreteFactory2(){};

    std::unique_ptr<Iproduct> CreateProduct(std::string des) {
        if (des == "A") {
            return std::unique_ptr<Iproduct>(new CaProduct());
        } else if (des == "B") {
            return std::unique_ptr<Iproduct>(new CbProduct());
        } else {
            return nullptr;
        }
    }
};

//工厂实现类3
class ConcreteFactory3 : public abstractfactory
{
public:
    ConcreteFactory3(){};
    ~ConcreteFactory3(){};

    std::unique_ptr<Iproduct> CreateProduct(std::string des) {
        if (des == "A") {
            return std::unique_ptr<Iproduct>(new CaProduct());
        } else if (des == "B") {
            return std::unique_ptr<Iproduct>(new CbProduct());
        } else if (des == "C") {
            return std::unique_ptr<Iproduct>(new CcProduct());
        } else {
            return nullptr;
        }
    }
};

int main(int argc, char const *argv[])
{
    /* code */
    ConcreteFactory1 cf1;
    std::unique_ptr<Iproduct> a1 = cf1.CreateProduct("A");
    std::unique_ptr<Iproduct> b1 = cf1.CreateProduct("B");
    std::cout << "工厂实现类1 生产 ：" << std::endl;
    a1->func();
    b1->func();
    std::cout << "===================" << std::endl;
    ConcreteFactory2 cf2;
    std::unique_ptr<Iproduct> a2 = cf2.CreateProduct("A");
    std::unique_ptr<Iproduct> b2 = cf2.CreateProduct("B");
    std::cout << "工厂实现类2 生产 ：" << std::endl;
    a2->func();
    b2->func();
    std::cout << "===================" << std::endl;
    ConcreteFactory3 cf3;
    std::unique_ptr<Iproduct> a3 = cf3.CreateProduct("A");
    std::unique_ptr<Iproduct> b3 = cf3.CreateProduct("B");
    std::unique_ptr<Iproduct> c3 = cf3.CreateProduct("C");
    std::cout << "工厂实现类3 生产 ：" << std::endl;
    a3->func();
    b3->func();
    c3->func();
    std::cout << "===================" << std::endl;
    return 0;
}