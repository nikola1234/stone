#include <iostream>

class test{
public:
    //static const double s_a = 6.5;  //error  double 类型不对
    static constexpr double s_a = 6.5;
    // static int s_b = 5;  // err 
    int m_c = 10;
    static int m_d;
    static const int s_f = 50;    // ok  const只能修饰整形类型
    //static constexpr std::string s_g = "hello world";    // err
    static constexpr auto s_g = "hello world";    // ok
};

int test::m_d = 20;  //非const 静态类成员需要在类外初始化

/*
 *  C++规定const静态类成员可以直接初始化
 *  其他非const的静态类成员需要在类声明以外初始化，
 */

int main () {
    test c;
    std::cout << "test a : " << test::s_a <<std::endl;
    std::cout << "test done!" <<std::endl;
}