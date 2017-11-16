#include <>
// prefer const enum and inlines to define
// #define PI 3.14   compile 
const float PI = 3.14;
const std::string AuthorName("scott hant");

template<typename T>
void f(const T& n)
{
    std::cout << n << std::endl;
}

template<typename T>
inline void callWithMax(const T& a, const T& b)
{
    f(a > b ? a: b);
}