#include <iostream>

// target 是准备要提供request服务的类
class Target
{
public:
    Target() {}
    virtual ~Target() {}

    virtual void Request(void) {
        std::cout << "Target Request. " << std::endl;
    }
};
// Adaptee是能够提供SpecificRequest服务的类
// Adaptee最常见的是第三方工具
class Adaptee
{
public:
    Adaptee() {}
    ~Adaptee() {}

    void SpecificRequest(void) {
        std::cout << "Adaptee SpecificRequest. " << std::endl;
    }
};
//现在客户要求Target 提供SpecificRequest服务。
//target显然是无法提供，就去找Adaptee 。有两种方法解决1、一个是继承类的方法，一个是对象的方法。
//对象的方法就是Target派生类Adapter构造函数带参数和私有Adaptee对象，将Adaptee实现传递给Adapter。这里不用代码表示了
class adapter : public Target, private Adaptee
{
public:
    adapter() {}
    ~adapter() {}
    void Request(void) {
        SpecificRequest();
        std::cout << "adapter SpecificRequest. " << std::endl;
    }
};

int main(int argc, char const *argv[])
{
    Target* t = new adapter();
    t->Request();
    delete t;
    t = nullptr;
    return 0;
}