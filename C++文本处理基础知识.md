# C++文本处理基础知识

本文将介绍C++基础知识，以便快速上手文本处理

## 0x00 为什么要进行文本处理？

当我们得到一些游戏脚本时会出现如下情况

```
WA 3000, , 0

CG 0,cg/黒枠用棒, 250, @-40, @-100@1000
CG 1,cg/黒枠用棒, 250, @580, @100@1000

【虎春】「――くっ！　まさかこれを躱すとはっ！」

ST0 004, S005_1CA2AA_070_M, 1, 480@-1@0@0
ST.DF 004, 3, 20@20, 1
ST.CF 004, 1, 0x888888

俺の操作するアバターが絶好のタイミングで放った一撃は[n]見事に空を切った。

BG.XY 1, , 0
CG.DEL 7, 1
CG.DEL 8, 1
CG.DEL 9, 1

CG 2,cg/神姫戦演出１, 250
WA 250, , 0
```

可以看到，该段游戏脚本夹杂了一部分代码和需要加载的资源文件名

当我们对游戏进行翻译的时候肯定是不希望对着这样文本来进行翻译的

所以我们需要过滤文本，最好是能变成如下这样

~~~
Raw:【虎春】「――くっ！　まさかこれを躱すとはっ！」
Tra：这是一段翻译的文本1

Raw:俺の操作するアバターが絶好のタイミングで放った一撃は[n]見事に空を切った。
Tra:这是一段翻译的文本2
~~~

当我们把处理好的文本交给翻译，翻译完成后再把 `Tra:` 其后的文本导回到正确的地方

## 0x01 初步了解C++对数据封装的设计思维

这里只是给大家传递这么一个概念，并不是教授C++的基础知识，有些部分我没有讲到

重在概念的讲解，看完后可以找其它C++的入门视频看看能有更好的理解

### 0x00 我们需要一个容器来存储数据

假设我们需要对学生的一些信息进行处理

每个学生有 学号 年龄 科目A成绩 科目B成绩 这四个信息

在C语言下我们可以定义一个结构体来表示这些信息的一个数据类型（为了简化表达这里全用`int`）

```c++
struct Student
{
    int ID;
    int Age;
    int SubA;
    int SubB;
};
```

这段的意思其实就是定义一个`Student`的类型，

这个类型描述了一个学生的信息，大小为4*4字节(一个`int`4字节(x32))

大家通常定义一个变量会这样写

```C
int a = 0;
```

这段的含义是，定义一个`int`类型的变量 `a`，并给其初始化为0

那么同样的

```c
Student xiaoming = { 0 };
```

这段的含义是，定义一个`Student`类型的变量 `xiaoming`，并给其初始化为0

`{ 0 }`的意思是给结构体初始化，他的意思其实相当于上面的 `a = 0` 

其实`int a = { 0 };`的写法也是可以的。

```c++
struct Student
{
    int ID;
    int Age;
    int SubA;
    int SubB;
};

int main()
{
    //相当于 int a = 0;
    Student xiaoming = { 0 };
}
```

好了，我们现在定义了一个叫`Student`的数据类型

同时在内存中划分了一个 4*4 字节大小的空间来存储我们这个数据类型的数据

这个空间我们给他了一个名字叫做`xiaoming`并且给他里面的数据全部写成了0

### 0x01 给这个`xiaoming` 同学一些信息吧

现在我们来把这个s学生的一些信息存进去

比如他的学号是 10086

比如他的年龄是 20

比如他的科目A成绩是 80

比如他的科目B成绩是 60

那么我们怎么把这些数据存进去呢？

很简单通过 `.` 点这个符号，可以对这个s同学的一些数据进行访问

```c
xiaoming.ID = 10086;
xiaoming.Age = 20;
xiaoming.SubA = 80;
xiaoming.SubB = 60;
```

`.` 点这个符号，我们称为成员访问运算符，说白了就是你要访问某个同学的一些数据就在后面加一个 `.` 符号

当然后面我们马上会谈到，访问的不只是数据，还能访问函数，也就是处理数据的一些方法。

```c++
struct Student
{
    int ID;
    int Age;
    int SubA;
    int SubB;
};

int main()
{
    //相当于 int a = 0;
    Student xiaoming = { 0 };
    xiaoming.ID = 10086;
    xiaoming.Age = 20;
    xiaoming.SubA = 80;
    xiaoming.SubB = 60;
}
```

### 0x02 算算`xiaoming`同学的科目平均分为几何

好了现在s同学的信息以及录入电脑了

现在我们来算是他的平均分吧！

平均分当然是`SubA` + `SubB`然后除以2对吧，相信大家都会

但是一个班级里面可不止一个同学

如果我们每次都是` (xiaoming.SubA + xiaoming.SubB) / 2`是不是很麻烦？

先要写一个同学的名字`xiaoming`然后要点一下访问具体科目的成绩，拿出来相加后平均

这个时候也许可以借助一个通用的函数，输入给这个函数的是两个分数，返回的是他们的平均值

```c++
int Average(int A, int B)
{
    int aver = (A + B) / 2;
    return aver;
}
```

这个叫 `Average()` 的函数需要输入两个`int`类型的数据，返回一个`int`类型的数据

输入的两个数据相当于我们的科目A和B的分数，然后他会在里面计算平均值 ，

并且把这个平均值 赋值给`aver`这个变量，然后返回这个变量的数值

```c++
struct Student
{
    int ID;
    int Age;
    int SubA;
    int SubB;
};

int Average(int A, int B)
{
    int aver = (A + B) / 2;
    return aver;
}

int main()
{
    //相当于 int a = 0;
    //划分空间存储s同学的信息
    Student xiaoming = { 0 };
    //给s同学一些信息
    xiaoming.ID = 10086;
    xiaoming.Age = 20;
    xiaoming.SubA = 80;
    xiaoming.SubB = 60;
    //计算一下s同学的平均分
    int c = Average(xiaoming.SubA, xiaoming.SubB);
}
```

现在这个`c`里面存储的数值就是`xiaoming`同学的平均分了

### 0x03 如何写的更优雅一些？

这段代码看起来也没什么问题，然后再来一个`x`同学，

他也可以通过`Average()`这个函数来进行求两科目的平均分

但是这个`Average()`函数是全局的，也就说，

不是你班的同学也可以调用，甚至你可以输入的两数据不是分数也行

```
int err = Average(xiaoming.ID, xiaoming.Age);
```

像这样，`xiaoming`同学的学号和年龄相加是什么鬼，没有任何意义

要是来了一个别的班的同学，科目A和科目B可能和咱班的不一样，也可以进行取平均分

也许他们班的这两科更简单，他们的平均分不就比咱们高了？



上述的代码看着就像是一个没有壳子的机器，各种管线和螺丝裸露在外面，

随时都可以拆下来这个机器上的任何东西，也可以加上你想要的东西。



大家买的手机电脑都是没有外壳的吗？电池和线路板都裸露在外面的吗？

显然不是对吧，你可能买的手机有金属边框，玻璃背板，电脑有金属的外壳

摸起来都是很顺滑的，有些手机甚至还有防水功能，丢在水里都能使用。



那么现在我们就给这个程序加一个外壳吧

看这个 `Average()` 函数他是干什么的？显然是给我们班的同学来进行求平均分的

他不适合给别的班求平均分，更不适合把年龄和学号拿来求平均

我们刚刚不是定义了`Student`这个数据类型吗？现在我们加点东西

```c++
struct Student
{
    int ID;
    int Age;
    int SubA;
    int SubB;

    int Average(int A, int B)
    {
        int aver = (A + B) / 2;
        return aver;
    }
};
```

这样看起来是不是感觉`Average()`就是属于`Student`这个类型的东西了

我们也可以通过成员访问运算符 `.` 来访问这个函数，用法和之前一样

只不过`int c = Average(xiaoming.SubA, xiaoming.SubB);` 

得写成 `int c = xiaoming.Average(xiaoming.SubA, xiaoming.SubB);`

但是这样，看着还是不行啊，这个函数还是可以任意输入两个数据来求平均

还是可以把年龄和学号求平均啊

那我们再改造一下

```c++
struct Student
{
    int ID;
    int Age;
    int SubA;
    int SubB;

    int Average()
    {
        int aver = (SubA + SubB) / 2;
        return aver;
    }
};
```

现在这个函数的输入参数直接变成没有了，他再也不可能输入数据了，他只可能输出数据。

这个 `aver`这个求平均的中间变量其实可以省略掉，

同时我们把函数名改一下，看着更形象一些，现在变成这样了

这个写法在C语言下看起来有点怪，因为`SubA`和`SubB`都没传入`GetAverage()`这个函数

但是却能直接访问，那么对C语言来说有可能是全局变量，但是这里却不是

这个里可以直接访问我们可以理解为`GetAverage()`这个函数是属于`Student`的，当然可以访问了

我自己的东西我自己还不能拿吗？

其实本质上这个函数是接受了一个参数的，这里我们不展开。

```c++
struct Student
{
    int ID;
    int Age;
    int SubA;
    int SubB;

    int GetAverage()
    {
        return (SubA + SubB) / 2;
    }
};
```

那么现在我们求`xiaoming`同学的平均分就变成这样了

```
 int c = xiaoming.GetAverage();
```

现在我们再回过头来看看这个`Student`的类型

他似乎已经不算单纯的数据类型了，他不仅定义了数据的存储类型也定义了处理数据的方法

学生的学号，年龄，科目A成绩，科目B成绩，是定义数据的存储类型，我们给他们起个名字，叫做`数据成员`

`GetAverage()`这个函数是拿来处理`Student`种定义的数据的，我们给他一个名字，叫做`成员函数/函数成员`

我们可以把`Student`这种混合了数据定义和处理数据方法定义的类型

称之为` 类 `所谓的`类`就是定义了数据的同时也定义了处理数据的方法（也就是如何处理数据）



需要注意的是 类 只是一种类似标准的东西，并不是真实存在于内存中的

也就是相当于，生产食品有一个标准和规范，但是这个标准和规范并不是实际上生产出来的食品

`Student` 这个 `类` 就是规范了 `xiaoming` 这个实际存在的东西是什么样的，要怎么制作出这个东西。

专业的说法 `xiaoming`就被称为 `对象`,也就是说 `类` 可以创建出一堆的 `对象`，

道理也很简单，标准一般只有一份，不然也不叫标准了

标准执行出现的东西有很多，比如生产某面包只按照一个标准生产，生产出来的面包却有非常多。

而这些面包都是按照同一个标准生产的。

```c
Student xiaoming;
Student zhangsan;
Student wangwu;
```

像这样就是在内存中实际创建出了 三个学生对象，这些对象多有相同的数据结构，和相同的处理数据的方法

### 0x04 如何写的更更优雅一些？

```C
    xiaoming.ID = 10086;
    xiaoming.Age = 20;
    xiaoming.SubA = 80;
    xiaoming.SubB = 60;
```

这样给`xiaoming`同学导入数据是不是丑了一些，也很繁琐？

```c
Student xiaoming(10086,20,80,60);
```

我们想象如果能这样就好了，看着就优雅多了，对吧！

大家如果还记得结构体的赋值，应该也能这样

```C
Student xiaoming = { 10086,20,80,60 };
```

事实上第二种是可以的,不过第一种方法现在是不行的

```c++
struct Student
{
    int ID;
    int Age;
    int SubA;
    int SubB;

    int GetAverage()
    {
        return (SubA + SubB) / 2;
    }
};

int main()
{
    Student xiaoming = { 10086,20,80,60 };
    int c = xiaoming.GetAverage();
}
```

现在看着优雅多了，但也不是很完美，比如，年龄如果输入成2000了呢？

如果是上述的写法，2000他也会进行赋值，那`xiaoming`就变成2000岁了，这也太可怕了。

那么我们的做法是什么呢？肯定是需要在对`xiaoming`信息输入的时候进行一个判断

如果是判断的话，就是对数据进行一些操作，类似于`GetAverage()`这个函数

因为我们是对`xiaoming`的输入信息进行判断，`xiaoming`他的信息是`Student`这个类型的

那我们就要在这个类型里面定义处理`xiaoming`信息的方法，

这样看起来才是符合逻辑，一体的，同时也不会把我们处理信息的方法被别人拿去使用。

那么这时候我们可能也会像是之前一样定义一个像是这样的函数

bool 也是一个数据类型，表少对或者错，这里是返回一个对或错，来说明输入的数据有没问题

```c
bool InputInfo(int ID,int Age, int SubA, int SubB)
```

然后在里面对年龄和分数什么的进行判断



理论上是可行的，但是别忘了，如果你在`Student`里面去定义这样一个函数

那你得先在内存里划分一块空间，然后给这个同学一个名字，然后你才能通过

这个同学的名字通过 `.` 点这个符号来访问里面提供的对数据处理的方法

划分空间的时候就涉及到给划分的空间填数据，如果你不填数据的话，那里面的数据是默认的

默认的数据有可能是0，也有可能是别的数值

如果是这样的话，代码就是这样的

````C
    Student xiaoming;
    xiaoming.InputInfo(10086, 20, 80, 60);
````

那能不能把 `.InputInfo`写到上面的 `xiaoming`后面呢？答案是不能

因为`xiaoming`这个东西都还不存在，你怎么可能去访问里面的东西呢？

就好比现实中，都没这个人，你怎么可能去查他的信息和叫他干活？



针对这一系列问题，C++引入了一个叫做`构造函数`的东西，所谓的`构造函数`

其实就是普通的函数，只不过他的主要用途是在你创建对象的时候干一些事情

也就是对这个对象内部的一些数据以及处理数据的方法进行一些初始化的操作和调用。



构造函数的返回值类型不用写 `构造函数` 的名称和类的名称一致 `构造函数`可以有多个参数且支持重载

那么依据构造函数这个标准，我们应该可以写出来，除了最后一条大家可能还不是很懂

那么写出 `Student` 类的构造函数就是这样

```c++
Student()
{

}
```

那么我们给这个构造函数一些参数，也就是一些输入的数据

```C++
Student(int iID, int iAge, int iSubA, int iSubB)
{

}
```

需要注意的是，构造函数的参数也就是传入构造函数的数据和对象里面的数据没有任何关系

单纯就是一个普通的函数，比如传入了一个 `iAge` 的数据，这个`iAge`和`xiaoming`的`Age`没有任何关系

即使把这个`iAge`改成`Age`也和`xiaoming`的`Age`没有任何关系，只不过这样的话，这两个`Age`就混起来了

编译器都不知道怎么区分了，如果需要区分的话需要用到this，这个大家可以自行去了解。



好的，我们加一些简单的逻辑判断，现在看起来就是这样的了

```C
struct Student
{
    int ID;
    int Age;
    int SubA;
    int SubB;

    int GetAverage()
    {
        return (SubA + SubB) / 2;
    }

    Student(int iID, int iAge, int iSubA, int iSubB)
    {
        if (iAge <= 256 && iAge > 0)
        {
            Age = iAge;
        }
        else
        {
            Age = 0;
        }
        ID = iID;
        SubA = iSubA;
        SubB = iSubB;
    }
};
```

这里只对年龄做了判断，我查了一下世界上最长寿的人，说是256岁

这里判断年龄如果是大于0岁并且小于等于256岁，那么就把这个输入的年龄赋值到对象的年龄上

也就是把`xiaoming`的年龄写为`iAge`的数值，相反的，就把年龄赋值了0

其它就是把输入的学号、科目A、科目B的成绩依次赋值，当然也可以做具体判断，这里就不写了。

前面提到这个构造函数，就是一个普通的函数，所以你可以在里面干任何普通函数可以干的事情。

比如你可以在构造函数里调用`GetAverage()`这是允许的，也可以调用不属于`Student`的函数



那么有了构造函数，就可以用利用构造函数来初始化`对象`了

那么如何使用构造函数？

```C
struct Student
{
    int ID;
    int Age;
    int SubA;
    int SubB;

    int GetAverage()
    {
        return (SubA + SubB) / 2;
    }

    Student(int iID, int iAge, int iSubA, int iSubB)
    {
        if (iAge <= 256 && iAge > 0)
        {
            Age = iAge;
        }
        else
        {
            Age = 0;
        }
        ID = iID;
        SubA = iSubA;
        SubB = iSubB;
    }
};



int main()
{
    Student xiaoming(10086,20,80,60);
    int c = xiaoming.GetAverage();
}
```

可以看到刚刚说的第一种用括号的形式可以用了，这个括号就是代表调用构造函数

但是这个时候你会发现

```c
Student xiaoming;
```

这样写变成不允许了，这个时候相当于你创建对象的时候，需要调用一个没有参数的构造函数

可以这样理解，如果你没写构造函数的时候编译器认为你是没必要采用构造函数的，默认也没调用构造函数

刚刚大家也看到了，一个结构体变成了类，如果默认强制或者自动生成一个构造函数显然是不合理的

当然如果你没理解这其中的关系也没关系，反正大家记住，如果你定义了一个有参数且没有默认值的构造函数

还想用上面这种方法创建对象，那么就得提供一个没有参数的构造函数，

至于这个构造函数里面写啥都行，不写也行。说到这，也就出现了，函数重载

两个同名的函数因为参数的不同是可以同时存在的。其实平时除了构造函数，很少写函数重载。

```C++
struct Student
{
    int ID;
    int Age;
    int SubA;
    int SubB;

    int GetAverage()
    {
        return (SubA + SubB) / 2;
    }

    Student(int iID, int iAge, int iSubA, int iSubB)
    {
        if (iAge <= 256 && iAge > 0)
        {
            Age = iAge;
        }
        else
        {
            Age = 0;
        }
        ID = iID;
        SubA = iSubA;
        SubB = iSubB;
    }

    Student()
    {

    }
};

int main()
{
    Student xiaoming;
    int c = xiaoming.GetAverage();
}
```



### 0x05 如何更Pro一点

上述已经初步展现了C++类的实现以及对数据的封装

但是还是有个问题

我们其实还是可以直接访问比如 `xiaoming`这个对象的数据成员

`xiaoming.ID`这样完全是可以访问的

为了让这个对象更有封装的特性，也就是我们不允许初始化后他的数据成员被更改

或者一些内部的方法被调用，我们需要一些访问权限的控制

想象一下，你的学号是成绩以及年龄都是可以随意改动的，这是多危险的事情啊。



首先，我们把 `struct Student`改成`class Student`

突然就看着专业了一点，class的英文意思也就是类了。

改成 `class` 后我们会发现，全部成员都没办法访问了

显然这不是我们想要的结果，我们需要灵活控制成员的访问权限

那么就需要用到 `public:` `private:`具体看代码

```
class Student
{
private:
    int ID;
    int Age;
    int SubA;
    int SubB;

public:
    int GetAverage()
    {
        return (SubA + SubB) / 2;
    }

    Student(int iID, int iAge, int iSubA, int iSubB)
    {
        if (iAge <= 256 && iAge > 0)
        {
            Age = iAge;
        }
        else
        {
            Age = 0;
        }
        ID = iID;
        SubA = iSubA;
        SubB = iSubB;
    }

    Student()
    {

    }
};

int main()
{
    Student xiaoming(10086,20,80,60);
    int c = xiaoming.GetAverage();
}
```

public后面跟着的就是能被外部访问的成员，

private后面跟着的就是私有的，不能被外部访问，但是可以内部访问的成员

public和private可以交错使用，可以写好几个

比如这样

```
private:
    int ID;
public:
    int Age;
private:
    int SubA;
    int SubB;
```

这段的意思就是

`ID`是私有的，`Age`是公开的，`SubA、SubB`都是私有的

需要注意的是，所谓的私有和公开只是编译前的一个概念，编译后在内存中没有这种东西。

class默认全部成员都是私有的，注意不要把构造函数设为私有的，

不然会没办法创建对象，当然有些设计模式是可以设有私有的。
