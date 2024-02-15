# HzwElegantCppHelper说明文档
## 说明
本项目本着增强可读性、减小开发难度的原则，声明了一些宏、类，帮助Cpp编程更加顺畅。包含函数、类常用的语法。
## 具体内容
### UseStdIO宏
本宏即使用std::cin、std::cout、std::endl
### H_RUN_ONCE宏
本宏的功能是使某个函数主体只运行一次，运行一次之后再运行就返回默认值
返回的默认值是该宏的参数，可以不填
### autoReturn
本结构体类似函数声明时的auto返回值。与auto不一样的是，本宏实用程度不如auto。需要函数声明时声明返回何类型，并且只能是含有无参构造函数的类。

使用例子：
```C++
vector<int> func(){return autoReturn();} 
```
建议尽量使用C++本来的auto语法
### getter和setter
getter和setter在别的高级语言里有，目前还没有在C++里实现。通过GET宏和SET宏，我们可以为一个类设置非共有变量的getter和setter

getter有GET、mGET、crGET、rGET四种，分别返回值、右值引用、常值引用、左值引用；这些宏的参数只有被get的成员变量名，函数体未定。

使用例子：
```C++
private: 
    int member; 
public: 
    GET(member) const {return member+1;}
```
setter只有SET宏；宏参数有两个：被set的成员变量名及set函数的参量（目前只能有一个）

使用例子：
```C++
private:
    int member;
public:
    SET(member, int a) {member=a+1;}
```
宏将会生成一个get_member(rget_member、crget_member)或者set_member成员函数，以供调用。
```C++
T obj;
obj.set_member(1);
```
### H_INTERFACE 接口宏
接口interface的概念在别的高级语言里很常见。我们也可以在C++里类似的实现。一个接口说明了一个类需要完成什么方法。

H_INTERFACE的参数是接口（父类）的名字：
```C++
H_INTERFACE(TComparable){
    virtual int compare(const T &obj) const=0;
};
```
在H_INTERFACE声明的“接口”类里，用T代表继承该接口的类。在别的语言里，这个T应该是接口名；在C++里我们用模板的T来表示。

继承接口的类需要把自己类名填入到模板参数里，以便继承实现接口的函数。
```C++
class BigNum:public TComparable<BigNum>{
public:
    int compare(const BigNum &num) const override;
};
```
### H_IOABLE、H_INPUTABLE、H_OUTPUTABLE类标签
该宏的内容是声明std::istream/std::ostream与本类的输入/输出运算符友元函数。参数是类名字。
```C++
class T{
public:
    H_IOABLE(T)
};
```
### H_HAS_POINTER_MEMBER类标签
声明拷贝构造、移动构造、赋值符号的宏，因为有指针成员一般都需要实现这些方法来保证内存安全。参数是类名字。
### H_COMPARABLE类标签
声明一个compare函数以及所有的比大小符号。与**hstd::TComparable**接口的功能差不多。但接口还需要写继承、在类里声明重载compare函数。该宏可以不继承地声明。

如果需要interface的一些优势，就用**hstd::TComparable**接口类；如果只是单纯需要比大小，完全可以用本宏。
### TComparable接口
是一个抽象类，用H_INTERFACE声明的。用法就和接口用法一样，子类只需要实现compare函数。
### memFunc函数
一个把成员函数变普通“函数”的函数。参数是成员函数指针（形如&T::func）；

C++已经实现了std::mem_fun，但是没有不定参数版本的。本函数就是依据那个原理写了一个不定参数版本，适用于任何成员函数。
例：
```C++
class T{
public:
    int a;
    int f(int b){return a+b;}
};
auto func=memFunc(T::f);
T obj; obj.a=10;
cout<<func(obj, 1)<<endl;
//输出11
```
### PRED宏
一个把普通函数变成“成员函数”的宏。灵感来源于C++20管道运算符。实际上是管道运算符的一种实现。

旨在把filter(filter(a, cond1),cond2)变成可读性较好的a|filter(cond1)|filter(cond2)。其实a.filter(cond1).filter(cond2)可读性更好，但是不让重载"."。

PRED取自单词predicate谓语，因为这个管道运算符的写法，看起来函数就像谓语一样。

所有能被转换成PRED的函数需要有至少一个参数。这样转成PRED之后就可以把这个参数写在"|"前面
用法：
```C++
auto f=PRED(/*一个函数，可以是已经声明好的函数或lambda，第一个参数是decltype(obj)类型*/);
obj | f(/*填入剩余参数*/) | pred2(/*...*/);
```
具体用法、注意事项可以看main.cpp里的实例以及hPredicate.h里的函数注释。尽量不要在第一个参数里填入指针类型。
## 注意事项
- 所有的类都在namespace hstd里。