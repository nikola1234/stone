#include <iostream>

//组件类
class Component
{
public:
    virtual ~Component() {}
    virtual void Operation(void)  = 0;
protected:
    Component() {}
private:
};
//组件实现类
class ConcreteComponent : public Component
{
public:
    ConcreteComponent() {}
    ~ConcreteComponent() {}
    void Operation(void) {
        std::cout << "ConcreteComponent Operation." << std::endl;
    }
protected:

private:
};
// 继承Component实现Operation方法
class Decorator : public Component
{
public:
    Decorator(Component *com) {
        m_com = com;
        std::cout << "Decorator constructrue." << std::endl;
    }
    virtual ~Decorator() {}
    virtual void Operation(void) {}
protected:
    Component* m_com;
};

class ConcreteDecorator : public Decorator
{
public:
    ConcreteDecorator(Component *com) : Decorator(com){  // 派生类传输参数到基类中
        std::cout << "ConcreteDecorator constructrue." << std::endl;
    }
    ~ConcreteDecorator() {};
    void Operation(void) {
        m_com->Operation();
        AddOperation();
    }
    void AddOperation(void) {
        std::cout << "ConcreteDecorator AddOperation." << std::endl;
    }
};
/*
本来：
Component* com = new ConcreteComponent();
com->Operation();
将这个组件实现类放在装饰器中增加功能。
 */
 */
int main(int argc, char const *argv[])
{
    Component* com = new ConcreteComponent();
    Decorator* dec = new ConcreteDecorator(com);
    dec->Operation();
    delete com;
    com = nullptr;
    delete dec;
    dec = nullptr;
    return 0;
}