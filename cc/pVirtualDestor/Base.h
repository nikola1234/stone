#pragma once

/* 区别1：
   #ifndef #define #endif  打开这个文件，然后判断该文件是否被define过.
   #pragma once,它是编译器来提供保证，它的判断机制是物理上的文件是否相同，
   而不是内容相同的两个文件，这样就省去了重复打开文件操作。所以推荐使用#pragma once 这种写法。
*/

 #include<iostream>   // std::cout

class StdBase  // standard base class
{
public:
    StdBase() { std::cout << "StdBase constructor!" << std::endl; };
    virtual ~StdBase() { std::cout << "StdBase destructor!" << std::endl; };  //一般是虚函数

    virtual void DoSomething() { std::cout << "Do something in class StdBase!" << std::endl; };
};

class PureBase {  // pure virtual destor class
  public:
   virtual ~PureBase() = 0;  //想要把PureBase定义为抽象基类，但是PureBase类中没有可以定义为纯虚函数的成员函数。
                             //所以，就可以将析构函数定义为纯虚函数
};

PureBase::~PureBase() {}     //当析构函数为纯虚函数，则需要在类外部实现析构函数，否则会报virtual functions are pure的错误