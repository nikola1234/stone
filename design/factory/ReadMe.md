工厂模式(Factory)
--------------  

出现的原因或者说解决的问题：  
>1、为了提高内聚和降低耦合，我们经常抽象出一些类的共用接口以形成抽象基类或接口  
>2、延迟对象实例化的时间  

此处的factory模式比较传统:一个工厂生产多个产品

>class Factory {  
>    Product* CreateAProduct();  
>    Product* CreateBProduct();  
>}  
>Factory f;  
>Product* pa = f.CreateAProduct();  
>Product* pb = f.CreateBProduct();  

简单工厂在所有工厂模式中实现上最为简便,而且并不需要很多的工厂类,同时很好的将产品类的创建过程封装,并解耦产品类实现与调用.

它最大的硬伤在于工厂类内部包含了所有具体产品类的构建.  
们需要增加某个产品类,或者修改某个产品类的构建方式时,我们就不得不修改工厂类.  


工厂方法模式(factory method)
----------------------------  
与工厂模式最大的区别就是将工厂类提取抽象基类，将工厂从封闭到开放的转变。

此处有两个抽象基类 Factory 和 Product  
>class ConcreteFactory1 {  
>    Product* CreateAProduct();  
>    Product* CreateBProduct();  
>}  
>class ConcreteFactory2 {  
>    Product* CreateAProduct();  
>    Product* CreateBProduct();  
>}   
>ConcreteFactory1 cf1;  
>Product* pa1 = f.CreateAProduct();  
>Product* pb1 = f.CreateBProduct();  
>ConcreteFactory2 cf2;  
>Product* pa2 = cf1.CreateAProduct();  
>Product* pb2 = cf2.CreateBProduct();  

若出现 CProduct就重新实现一个Factory，就不需要修改之前的Factory实现类  

此处说明：
--------
三种工厂模式有点多余。两种即可。