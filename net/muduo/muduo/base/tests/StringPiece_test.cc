#include <muduo/base/StringPiece.h>
#include <iostream>

int main(int argc, char const *argv[])
{
    muduo::StringPiece str("hello");
    std::cout << "cout : " << str << std::endl;
    return 0;
}