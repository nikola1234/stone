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

class Decorator
{
public:
    Decorator(Component *com) 
    virtual ~Decorator() {}
    ~Decorator();
protected:
    Component* m_com;
};