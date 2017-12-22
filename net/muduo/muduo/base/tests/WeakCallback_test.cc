#include <muduo/base/WeakCallback.h>
#include <muduo/base/noncopyable.h>

#include <iostream>
#include <string>
#include <memory>

namespace muduo
{

class MuduoCb : noncopyable, 
                public std::enable_shared_from_this<MuduoCb>
{
public:
    typedef std::function<void()> Callback;
    MuduoCb() { std::cout << "MuduoCb constr. " << std::endl; };
    ~MuduoCb() { std::cout << "MuduoCb descont. " << std::endl; };


    void func() {
        std::cout << "function called. " << std::endl;
    }

    void run() {
       //                                object          function
       std::shared_ptr<MuduoCb> m = shared_from_this();  // 保证 callback函数执行时对象存在
       std::cout << "m use_count : "<<m.use_count() << std::endl;  // use_count 2
       Callback cb = makeWeakCallback(m, &MuduoCb::func);
       std::cout << "Callback set. " << std::endl;
       cb();
    }
};
typedef std::shared_ptr<MuduoCb> MuduoCbPtr;
}

int main(int argc, char const *argv[])
{
     std::weak_ptr<muduo::MuduoCb> object_;
    {
        muduo::MuduoCbPtr cb(new muduo::MuduoCb());
        std::cout << "use_count: "<< cb.use_count() << std::endl;  //  use_count 1
        object_ = cb;
        cb->run();
    }
     muduo::MuduoCbPtr ob  = object_.lock();
    if (ob) {
        ob->run();
    }
    return 0;
}
