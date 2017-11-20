#include <iostream>
#include <memory>
#include <string.h>

class prototype
{
public:
    prototype() {}
    virtual ~prototype() {}
    prototype(const prototype& another) {}

    virtual std::unique_ptr<prototype> Clone() = 0;

    virtual void setValue(int v) = 0;
    virtual int getValue(void) const = 0;
};

class ConcretePrototype : public prototype
{
public:
    ConcretePrototype() : m_value(0) {}
    ConcretePrototype(int v) :m_value(v) {}

    ~ConcretePrototype() {}
    ConcretePrototype(const ConcretePrototype& other) {
         m_value = other.m_value;
    }
    std::unique_ptr<prototype> Clone() {
        return std::unique_ptr<ConcretePrototype>(new ConcretePrototype(*this));
    }

    void setValue(int v) {
        this->m_value = v;
    }

    int getValue(void) const {
        return m_value;
    }
private:
    /* data */
    int m_value;
};

int main(int argc, char const *argv[])
{
    ConcretePrototype proto_value(10);
    std::unique_ptr<prototype> oth_value = proto_value.Clone();

    std::cout << "clone value : " << oth_value->getValue() << std::endl;
    return 0;
}