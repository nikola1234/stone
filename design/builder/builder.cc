#include <iostream>
#include <string.h>
#include <memory>

//产品类
class Product
{
public:
    Product() {};
    ~Product() {};

     std::string get_m_a(void) const {
        return m_a;
     }

     std::string get_m_b(void) const {
        return m_b;
     }

     std::string get_m_c(void) const {
        return m_c;
     }

    void Make_A(std::string a) {
        m_a = a;
        std::cout << "make A. " << a << std::endl;
    }

    void Make_B(std::string b) {
        m_b = b;
        std::cout << "make B. " << b << std::endl;
    }

    void Make_C(std::string c) {
        m_c = c;
        std::cout << "make C. " << c << std::endl;
    }
    // 使用建造者模式的原因: 产品的生产要多步骤。
    void use_product(void) {
        std::cout << "Product is " << m_a << " "
                                   << m_b << " "
                                   << m_c << std::endl;
    }
private:
    std::string m_a = "";   // 产品 A部分
    std::string m_b = "";   // 产品 B部分
    std::string m_c = "";   // 产品 C部分
};

//建造者抽象基类
class Builder
{
public:
    Builder() {};
    virtual ~Builder() {};
    virtual void BuildPartA(std::string a) = 0;
    virtual void BuildPartB(std::string b) = 0;
    virtual void BuildPartC(std::string c) = 0;
    virtual Product* GetProduct(void) const = 0;
};

//建造者1
class ConcreteBuilder1 : public Builder
{
public:
    ConcreteBuilder1() {
        m_product = new Product();
    };
    ~ConcreteBuilder1() {
        delete m_product;
        m_product = nullptr;
    };

    void BuildPartA(std::string a) {
        m_product->Make_A(a);
    }

    void BuildPartB(std::string b) {
        m_product->Make_B(b);
    }

    void BuildPartC(std::string c) {
        m_product->Make_C(c);
    }

    Product* GetProduct(void) const {
        return m_product;
    }
private:
    Product* m_product;
};

//建造者2
class ConcreteBuilder2 : public Builder
{
public:
    ConcreteBuilder2() {
        m_product = new Product();
    };
    ~ConcreteBuilder2() {
        delete m_product;
        m_product = nullptr;
    };

    void BuildPartA(std::string a) {
        m_product->Make_A(a);
    }

    void BuildPartB(std::string b) {
        m_product->Make_B(b);
    }

    void BuildPartC(std::string c) {
        //m_product->Make_C(c);
        m_product->Make_C(c + " " + m_product->get_m_b() + " added");  //此处使用了不同的建造方式
    }

    Product* GetProduct(void) const {
        return m_product;
    }
private:
    Product* m_product;
};

class  Director
{
public:
    Director() {};
    ~Director() {};

    void Command1(Builder* b) {
        b->BuildPartA("part1");
        b->BuildPartB("part2");
        b->BuildPartC("part3");
    }

    void Command2(Builder* b) {
        b->BuildPartA("parta");
        b->BuildPartB("partb");
        b->BuildPartC("partc");
    }
};

int main(int argc, char const *argv[])
{
    // 现有两个工人 Builder1和 Builder2
    Builder*  Builder1 = new ConcreteBuilder1();
    Builder*  Builder2 = new ConcreteBuilder2();

    // 还有一个设计师,设计师指挥工人工作
    std::unique_ptr<Director>  director(new Director());
    // 有两种生产产品的方式
    // 此处使用设计者两种设计方式来生产不同的产品
    // 也可以用建造者的建造方式不同，来生产不同的产品
    director->Command1(Builder1);
    director->Command2(Builder2);

    // 我得到两个产品
    Product* pa = Builder1->GetProduct();
    Product* pb = Builder2->GetProduct();

    // 我要使用这两个产品
    pa->use_product();
    pb->use_product();

    // 释放内存
    delete Builder1;
    delete Builder2;
    Builder1 = nullptr;
    Builder2 = nullptr;

    return 0;
}