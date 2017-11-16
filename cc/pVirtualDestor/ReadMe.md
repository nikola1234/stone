纯虚析构函数  
------------
 Pure virtual destructors  

log  
------------
> + g++ -g app.cc -std=c++11 -o pvd  
> + ./pvd  
>StdBase constructor!  
>StdDerived constructor!  
>Do something in class StdDerived!  
>StdDerived destructor!  
>StdBase destructor!  

另一个测试的结果  
------------
>1、派生类的实例化是先调用基类的构造函数，再调用自己的构造函数  
>2、销毁这个实例是先调用自己的析构函数，再调用基类的析构函数  