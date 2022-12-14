# [C++] C++文本处理基础知识

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

为了实现这种功能，我们接下来将介绍和学习一些C++标准库的内容。

## 0x01 C++对数据封装的基本概念

详见：初步了解C++对数据封装的设计思维

了解完了C++的这种设计思维，我们来介绍一些在处理文本过程中需要用到的知识点。

## 0x02 `string`

类似于我们前面介绍的 string是标准库提供的模板类

所谓的标准库其实就是已经帮你写好的一堆功能的集合

你可以调用里面那些写好的功能来完成各种事情

我们之前没说过模板的概念，其实模板这个大家现在可以暂时忽略掉

有兴趣的可以去网上查查资料。

反正简单来说string是一个类，这个类可以用来方便的处理字符串。

### 0x00 创建对象

类似于我们前面自己写的 Student 类

创建一个string 类的对象可以这样写

```C++
string str;
```

可以类比`int a;`这样

但是这时候你写出来可能会给你打一个红色波浪线

```
std::string str;
```

这样写的话就正常了。

但是如果每次用string都得加一个`std::`是不是很麻烦？

这时候我们可以在外面写一个 `using namespace std;`

这样就可以不用每次都加`std::`了

这个原因和`名称空间`有关，有兴趣可以自己去查查资料

现在看起来就是这样的了

```C++
#include <iostream>
using namespace std;

int main()
{
	string str;
}
```

和我们之前的`Student`是很像的，只不过这里我们没有自己定义`string`这个类

因为`string`这个类，已经在`iostream`里面定义过了，我们可以直接拿来用。



### 0x01 赋值

好了，现在我们已经创建出一个`str`对象了，相当于之前我们已经创建出来一个`xiaoming`

现在可以对`str`赋值了，也就是把数据给他

```c++
string str;
str = "Dir-A";
```

也可以两句合成一句

```c++
string str = "Dir-A";
```

这里大家可以类比于构造函数，这个和构造函数是差不多的（反汇编的时候甚至是一样的）

只不过构造函数的时候我们写了括号，这里用了等于号。

也可以用另一个对象给他赋值

```c++
string str = "Dir-A";
string str2 = str;
```

执行完成后`str2`就和`str`的内容一样了。



### 0x02 查找字符串

现在我们已经把数据给他了，当然也可以调用它里面的一些方法

比如 `str.find()`这个方法，这是一个字符串查找的方法

如果它能找到你指定的字符串，会返回字符串的位置，相反的找不到会返回` string::npos`

我们可以来简单的查找一下

```C++
int main()
{
	string str = "Dir-A";
	if (str.find("ir-A") != string::npos)
	{
		//会进来吗？
	}
}
```

!=的意思是不等于，也就是说，不等于没查找到，也就是说查找到了

也就是说，如果找到了`"ir-A"`就会进入这个if里面

当然你也可以指定开始查找的位置

```C++
int main()
{
	string str = "Dir-A";
	if (str.find("ir-A",2) != string::npos)
	{
		//会进来吗？
	}
}
```

这样的意思就是从第三个字符开始查找

不写的时候默认是第0个字符（参考数值的编号）

这个时候也就是说从`"r-A"`里找`"ir-A"`显然是找不到的，不会进入if里面。



### 0x03 拼接字符串

下面为了整体的简洁明快，将会简洁的介绍一些功能，不再赘述。

拼接两个字符串

```C++
int main()
{
	string str = "Dir-A";
	string str2 = "Hello";
	string str3 = str + str2;
}
```

执行完毕后str3就变成了 `"Dir-AHello"`

这样直接加到str2上也是可以的，执行完成后str2的情况和str3一样

当然如果你写成 `str2 += str`相当于`str2 = str2 + str1`执行完成的情况Hello会在前面。

```C++
str2 = str1 + str2;
```

当然也可以这样，不一定要加string类的对象，也可以直接加双引号的字符串

执行完成后 str2 就变成了 `"HelloGood"`

```C++
str2 = str2 + "Good";
```

值得注意的是加号不只是一个，后面加好几个加号也行。

```C++
str2 = str2 + "Good" + "Get" + "This" + "Text" + str;
```



### 0x04 复制字符串

其实说复制也不是很贴切，但是说剪切就完全错了，姑且就说复制吧。

复制字符串将用到`string`类中`substr()`这个方法

从`str2`跳过三个字符开始复制字符到str1中

执行完后 `str1` 为 `"loGoodGetThisTextDir-A"`

```C++
int main()
{
	string str = "Dir-A";
	string str2 = "Heelo";
	str2 = str2 + "Good" + "Get" + "This" + "Text" + str;
	string str1 = str2.substr(3);
}
```

当然你也可以指定拷贝的长度

比如我想从`str2`跳过4个字符，复制3个字符到`str1`

执行完后`str1`为`"oGo"`

```C++
string str1 = str2.substr(4,3);
```



### 0x05 获取C-Style String

所谓的 C-Style String 其实就是 C语言下定义的以00结尾的字符串

比如

```C
const char* cstr = "Dir-A";
char cstr2[] = "Dir-A";
```

当我们调用`printf()`或WINAPI的时候需要传递这种类型的字符串

那么如何把 `std::string` 类型的字符串转换成 `c-style`类型的呢？

很简单，我们只需要调用该类提供的一个方法即可

```C++
string str = "1.txt";
char* cstr = "1.txt";
printf("%s\n",cstr);
printf("%s\n",str.c_str());
```

可以看到 `cstr` 和 `str.c_str()` 是等价的

其实本质上 `str.c_str()` 的实现就是返回 `str` 对象内部存储的字符串的首地址



### 0x06 单字符访问

在C语言中定义一个字符串的本质其实就是在一个char类型的数组里填入一个个的字符

那么既然是数组，那就支持通过下标对字符串中的字符进行访问

理所当然的，`string`也支持这种访问形式

```C++
string strx = "Dir-A";
char c = strx[1];
char cx = 'i';
```

cx 和 c 是等价的。



## 0x02 `vector`

相信大家对数组都不陌生，定义一个数组像是这样

```c++
int arr[10] = { 0 };//定义一个长度为10个int的数组并初始化全部元素为0
```

这个10是固定的，也就是编译完成后这个`arr`就只有10个`int`的大小

如果在程序运行的时候有不确定数量的的数据要存入数组，这个显然不行

要么就把数组定义得非常大，其实也许运行阶段要存入的输入可能比你定义的还要大。

为了解决这种不确定数量的数据要存入数组的情况，

就是说当我们处理文本的时候可能会有不确定行数的文本，

有些文本可能只有几百行有些可能有上万行，

所以我们需要一个可以动态改变大小的数组，用来存放我们的数据

那么vector就是这么一个动态的数组。



### 0x00 创建对象

在使用vector之前需要先引入相关的头文件

```
#include <vector>
```

当然还有前面说的`std::`的问题，此后不再提及。

vector的对象创建和之前稍有区别，这是因为模板类的原因

需要多提供一个用尖括号括起来的元素类型参数

类<元素类型> 对象名

这里我们创建了一个vector，里面的每一个元素都是string

```c++
#include <iostream>
#include <vector>

int main()
{
	vector<string> vecString;
}
```



### 0x01 存入元素

存入元素的方法有多种，我们只介绍一种常用的即可。

在C语言下的数组，因为大小是确定的，所有我们可以通过下标进行访问对应的位置来存入元素

而vector是动态的，默认没有大小。

要存入数据我们可以调用里面的相关方法。

比如我现在有如下的字符串需要存入vector

```C++
string str0 = "Dir-A";
string str1 = "Now";
string str2 = "Teaching";
string str3 = "You";
string str4 = "Vector";
```

此处我们调用`push_back();`方法来存入元素

```c++
vecString.push_back(str0);
vecString.push_back(str1);
vecString.push_back(str2);
vecString.push_back(str3);
vecString.push_back(str4);
```

存入完毕后，可以在debug模式观察`vecString`的大小，已经变成了5，也就是存入元素的数量。



### 0x02 访问元素

和C语言下的数组一样，如果要访问vector里的元素，只需要`变量名[下标]`

比如访问刚刚存入的第2个元素,取出的元素就是string类型的

下面两者是等价的。

```c++
string m0 = vecString[1];
string m1 = "Now";
```

当然vector的功能不止于此，大家可以自行搜索更多用法，我们这里只价绍需要用到的部分。



## 0x03 `fstream`

所谓的`fstream`其实就是和`fopen` `createfile`差不多的东西

只不过C++又包了一层，用起来会更加方便

使用`fstream`需要先引入相关头文件

```c++
#include <fstream>
```



### 0x00 输入数据

输入数据的时候需要打开文件我们用到的是`ifstream`

```C++
ifstream iFile("1.txt")；//打开一个1.txt的文件
string str;
iFile >> str; //从文件中读出一行到str
```

`>>`这个符号表示数据从`iFile`流到`str`里，

在这里也就是从1.txt中读取一行到`str`里，并且会自动指向下一行

也就是再次 `iFile >> str;` 会读取第二行

当然还可以指定打开的模式，比如以二进制模式打开（默认不填是文本形式）

```C++
ifstream iFile("1.txt",ios::binary);
```

下面列出了一些模式，可以看看，不用记住，一般情况也不用去指定模式

创建对象的时候会给一个默认的模式，一般够用

模式也是可以同时使用的，比如 `ios::app | ios::binary`

```C++
ios::in			为输入(读)而打开文件
ios::out		为输出(写)而打开文件
ios::ate		初始位置：文件尾
ios::app		所有输出附加在文件末尾
ios::trunc		如果文件已存在则先删除该文件
ios::binary		二进制方式(不写以文本方式打开)
ios::_Nocreate	如果文件不存在则不创建
ios::_Noreplace	如果文件已存在则不替换
```



### 0x01 输出数据

当我们要输出数据到文件的时候，就需要用到`ofstream`

涉及到写出数据一般有两种了，

要么直接覆盖原文件（也就是创建一个新的文件），

要么在原有文件的基础上进行数据的输出，

`ofstream`默认是会覆盖原有文件的，当然这个设置一般都没问题

如果你需要在文件末尾追加数据，而不是覆盖，就可以指定模式` ios::app`

这个感兴趣可以自己去查一下，我们一般都是用默认的模式。

```C++
ofstream oFile("1.txt"); // 输出数据到文件1.txt(如果文件存在默认覆盖)
oFile << "Dir-A" << endl; // 向文件中写入字符串 "Dir-A" 并且换行
```

<<表示数据流入`oFile`里，也就是把这个字符串写入到文件里，`endl`表少换行。



### 0x02 判断文件是否可用

在我们使用`fopen`的时候会检查文件指针和`errno_t `来确定文件是否可用或成功打开/创建

使用`CreateFile`的时候我们会检查返回的句柄

那么`fstream`通过什么检查文件是否为可用状态呢？答案是.is_open();

```C++
ofstream oFile("1.txt");
if (oFile.is_open())
{
	//打开成功
}
```

`ifstream`同理



### 0x03 文件的刷新与关闭

当我们使用`ifstream`读取文件的时候，不会涉及到文件刷新的问题

但是`ofstream`会有这个问题，也就是写入的数据没有实时同步到硬盘

类似于`fflush`，当我们写入数据完成后应该对缓冲区进行刷新

也就是把缓冲区的数据强行刷到硬盘上，以避免直接退出程序导致数据丢失

刷新完数据，我们需要关闭文件(对于`ifstream`只需要close()即可)。

至此我们就有了文件读写的两个基本框架代码

```C++
ofstream oFile("1.txt");//输出数据
if (oFile.is_open())
{
	//在这里写
	oFile.flush();
	oFile.close();
}
```

```C++
ifstream iFile("1.txt");//输入数据
if (iFile.is_open())
{
	//在这里写
	iFile.close();
}
```



### 0x04 编码控制

不像是`CreateFile`打开的是二进制文件，`fstream`使用文本模式会涉及到文本的编码问题

比如我要打开的文本文件可能是UTF8，UTF16，亦或者GBK，SJIS

如果是游戏脚本的时候大部分情况肯定是希望输入的是SJIS的原始字符串，

输出的则是UTF8/16这种编码格式，方便进行翻译和修改。

除了涉及到输入后的编码转换，在打开文件时和输出数据时也是一个需要控制编码的地方。

对此，大家只需要直接使用下面的代码即可，不必深究，不然会大大提高使用的难度。



首先需要引入头文件 `codecvt`然后在函数体开头贴上这一段代码

这一行代码看起来复杂，实际上，最终就是生成了一个叫做`cvtUTF8`的变量(姑且这么说)

```C++
auto cvtUTF8 = locale(locale::empty(), new codecvt_utf8<wchar_t, 0x10ffff, codecvt_mode(consume_header | generate_header | little_endian)>);
```

等下就可以拿着 `cvtUTF8`这个东西给`fstream`创建的对象用，

只要套上了这个东西就变成以UTF8的编码来进行数据的输入输出

那么怎么套上去呢？

```C++
wofstream transFile("1.txt");
transFile.imbue(cvtUTF8);
```

对的，就是在打开文件后调用`imbue();`这个方法，传入`cvtUTF8`这个变量即可



当然这里也有一个小细节，可以看到`ofstream`变成`wofstream`了(`ifstream`也一样)，

代表是一个宽字节版本的，同样的，如果要读入或写入一行文本，`string`得改成`wstring`

值得注意的是，如果不去套这个东西，默认是窄字节读取的，通俗点就是GBK和SJIS这种

所以说，如果你需要读取SJIS编码的游戏脚本的时候就不用套这个了。



## 0x04 Range-based for loop

在以前当我们需要遍历整个数组的时候，

我们需要获取其数组的元素个数（或开头结尾），并且用for循环遍历

如果有写过C语言的其实可以感觉到比较繁琐

在C++下可能会更加繁琐

对此从C++11开始引入了 Range-based for loop

可以自动遍历，比如刚刚的vector：

```c++
for (auto p : vecString)
{
	//P
}
```

只需要这样就可以自动遍历`vecString`，p相当于每次取出来的元素

在我们写入文本的时候会很有用

其实 `auto` 就是 `string`,

只不过这边使用`auto`可以进一步简化代码，让编译器自己推断类型

不过这里可能看起来体会不深，

如果大家后面有写C++遇到一些类型比较复杂的情况

会有更加深刻的体会。



## 0x05 编码转换

这一块本质就是调用了`winapi`没什么好说的，直接用我写好的代码即可。

https://github.com/Dir-A/ACV1TextEditor/blob/main/ACV1TextEditor/ACV1TextEditor/ConvertString.cpp

里面有两套，一套是`winapi`实现的，另一个是c++标准库实现的，其实本质上后面还是调用了`winapi`

所以用第一套就行了。

