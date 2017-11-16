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

//简单工厂类
class simplefactory
{
public:
    simplefactory(){};
    ~simplefactory(){};
    
    std::unique_ptr<Iproduct> CreateCaProduct(void) {
       return new CaProduct();
    }
    std::unique_ptr<Iproduct> CreateCbProduct(void) {
       return new CbProduct();
    }
    std::unique_ptr<Iproduct> CreateCcProduct(void) {
       return new CcProduct();
    }
    static std::unique_ptr<Iproduct> CreateProduct(std::string des) {
        if (des == "A") {
            return new CaProduct();
        } else if (des == "B") {
            return new CbProduct();
        } else if (des == "C") {
            return new CcProduct();
        } else {
            return nullptr;
        }
    }
};

/* 
 * 优点：一个工厂类可以获取多个产品的实例
 * 缺点：每当要增加产品或者修改产品的构造时，还需要修改工厂类
 */
//blog.csdn.net/markl22222/article/details/17055101
int main(int argc, char const *argv[])
{
    /* code */
    simplefactory sf;
    std::unique_ptr<Iproduct> a = sf.CreateCaProduct();
    std::unique_ptr<Iproduct> b = sf.CreateCbProduct();
    std::unique_ptr<Iproduct> c = sf.CreateCcProduct();
    a.func();
    b.func();
    c.func();
    std::cout << "--------------------"<<std::endl;
    simplefactory::CreateProduct("A")->func();
    simplefactory::CreateProduct("B")->func();
    simplefactory::CreateProduct("C")->func();
    return 0;
}