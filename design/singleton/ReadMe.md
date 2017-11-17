单例模式(singleton)
--------------  

区分懒汉式和饿汉式  
懒汉式是在需要的时候实例化。有一个注意点：多线程访问的话需要加锁，不然会被实例化多次。  
饿汉式是类被加载的时候实例化。  