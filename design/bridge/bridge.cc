#include <iostream>

/*
    来源地：A1  桥  目的地：B1
    来源地：A2  桥  目的地：B2
    来源地：A3  桥  目的地：B3
    不用桥接模式怎么实现？
    需要继承多个类，或者在类中定义多个目的地成员
*/

//定义桥 抽象类
class bridge
{
public:
    bridge() {}
    virtual ~bridge() {}

    virtual void go_where() = 0;  //通过这个桥去哪，通过抽象共同方法解耦
};
// 目的地 B1 类
class B1 : public bridge
{
public:
    B1() {}
    ~B1() {}

    void go_where() {
        std::cout << "去 目的地：B1." << std::endl;
    }
};
// 目的地 B2 类
class B2 : public bridge
{
public:
    B2() {}
    ~B2() {}

    void go_where() {
        std::cout << "去 目的地：B2." << std::endl;
    }
};
// 目的地 B3 类
class B3 : public bridge
{
public:
    B3() {}
    ~B3() {}

    void go_where() {
        std::cout << "去 目的地：B3." << std::endl;
    }
};
// 来源地抽象类
class AbstractA {
public:
    AbstractA() {}
    virtual ~AbstractA() {}

    virtual void from_where() = 0;
};
// 来源地 A1
class A1 : public AbstractA
{
public:
    A1(bridge* value) {
        b = value;
    }
    ~A1() {}

    void from_where() {
        std::cout << "从 来源地：A1." << std::endl;
        b->go_where();
    }
private:
    bridge* b;
};
// 来源地 A2
class A2 : public AbstractA
{
public:
    A2(bridge* value) {
        b = value;
    }
    ~A2() {}

    void from_where() {
        std::cout << "从 来源地：A2." << std::endl;
        b->go_where();
    }
private:
    bridge* b;
};
// 来源地 A3
class A3 : public AbstractA
{
public:
    A3(bridge* value) {
        b = value;
    }
    ~A3() {}

    void from_where() {
        std::cout << "从 来源地：A3." << std::endl;
        b->go_where();
    }
private:
    bridge* b;
};
/*
    1、高内聚 、松耦合
    2、适用 多个输入 多个输出，中间用桥连接。
*/
int main(int argc, char const *argv[])
{
    bridge* b = new B3();
    AbstractA* aa = new A2(b);
    aa->from_where();
    delete b;
    delete aa;
    b = nullptr;
    aa = nullptr;
    return 0;
}