#include <mutex>
#include <iostream>

//懒汉式-时间换空间
class LazySingleton
{
public:
    ~LazySingleton() {
        if(m_instance != nullptr) {
            delete m_instance;
            m_instance = nullptr;
        }
    };
    LazySingleton(const LazySingleton&) = delete;   //或者私有化
    LazySingleton(LazySingleton&&) = delete;
    LazySingleton& operator=(const LazySingleton&) = delete;
    LazySingleton& operator=(LazySingleton&&) = delete;

    static LazySingleton *instance() {
        if (m_instance == nullptr) {
            //线程锁，确保singleton被实例化一次
            static std::mutex m;
            std::lock_guard<std::mutex> lock(m);
            //再次检查singleton是否被实例化
            if (m_instance == nullptr) {
                m_instance = new LazySingleton();
            }
        }
        return m_instance;
    }
    void function(void) {
        std::cout << "LazySingleton function." << std::endl;
    }

protected:
    LazySingleton(){};

private:
    static LazySingleton* m_instance;
};
// 类外声明
LazySingleton* LazySingleton::m_instance = nullptr;


//饿汉式-空间换时间
class HungrySingleton
{
public:
    ~HungrySingleton() {
        if(m_instance != nullptr) {
            delete m_instance;
            m_instance = nullptr;
        }
    };

    HungrySingleton(const HungrySingleton&) = delete;   //或者私有化
    HungrySingleton(HungrySingleton&&) = delete;
    HungrySingleton& operator=(const HungrySingleton&) = delete;
    HungrySingleton& operator=(HungrySingleton&&) = delete;

    static HungrySingleton *instance() {
        return m_instance;
    }

    void function(void) {
        std::cout << "HungrySingleton function." << std::endl;
    }
protected:
    HungrySingleton(){};

private:
    static  HungrySingleton* m_instance;
};
// 类外声明
HungrySingleton* HungrySingleton::m_instance = new HungrySingleton();

int main(int argc, char const *argv[])
{
    LazySingleton::instance()->function();
    HungrySingleton::instance()->function();
    return 0;
}