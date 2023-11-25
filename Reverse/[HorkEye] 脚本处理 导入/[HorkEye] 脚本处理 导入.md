# [HorkEye] 脚本处理 导入

## 0x00 前言

在导入脚本之前，我们先来回顾一下之前写的导出代码
```c++
#include <iostream>
#include <string>
#include <fstream>

bool DumpText(std::string strFileName)
{
	std::ofstream oText(strFileName + ".txt");
	std::ifstream iScript(strFileName);
	if (iScript.is_open() && oText.is_open())
	{
		size_t lineCount = 0;
		for (std::string perLine; std::getline(iScript, perLine); lineCount++)
		{
			if (perLine.size() <= 0)
			{
				continue;
			}

			if (((unsigned char)perLine[0] >= (unsigned char)0x7B) ||
				perLine.find("CS \"") != std::string::npos ||
				perLine.find("SELECT \"") != std::string::npos
				)
			{
				oText << "LineCount:" << lineCount << '\n';
				oText << "Raw:" << perLine << '\n';
				oText << "Tra:" << '\n' << '\n';
			}
		}

		oText.flush();
		oText.close();
		iScript.close();
		return true;
	}

	return false;
}

int main()
{
	DumpText("00015_c2da29b4bb60c0f2");
}
```

至于上次说到那个问题

```c++
perLine.find("CS \"") != std::string::npos
```

```
BGM "Load_CS "
```

```
CS "プロローグ"
```

如果是上面这样写这两个都是可以搜到并显示出来的

为此，我们需要改一下

```c++
perLine.find("CS \"") == 0
```

下面的同理，因为find的返回值是该字符串在搜索字符串中的开始位置，如果位置是0说明就是在开头。下面那个SELECT同理。

## 0x01 编码转换

现在我们的这个程序只能导出SJIS编码的文本，为了方便我们使用（就是直接打开来不乱码），我们可以加一个编码转换的操作，把输出的文本变成UTF8，当然UTF16也可以，但是C++在Windows上其实对UTF16支持不怎么样，虽然能用，但是很难用，所以就不转成UTF16了。

上次说到个`fstream`的编码问题，这个已经很大家说了，照抄即可，不必纠结。
```c++
auto cvtUTF8 = locale(locale::empty(), new codecvt_utf8<wchar_t, 0x10ffff, codecvt_mode(consume_header | generate_header | little_endian)>);
```

把`fstream`的对象套上这个东西就能读取或输出UTF8的文本（自动识别生成文件头）

我们这段代码只有`oText`需要套这个

```c++
#include <iostream>
#include <string>
#include <fstream>
#include <codecvt>
using namespace std;

bool DumpText(std::string strFileName)
{
	auto cvtUTF8 = locale(locale::empty(), new codecvt_utf8<wchar_t, 0x10ffff, codecvt_mode(consume_header | generate_header | little_endian)>);

	std::ofstream oText(strFileName + ".txt");
	oText.imbue(cvtUTF8);
```

这里我把`std`的`namespace`加上了，不然要写一堆`std::`很难受

现在我们还有个地方要改改，既然是宽字节输出，那么就要用和宽字节匹配的东西

```c++
std::wofstream oText(strFileName + ".txt");
```

```
oText << L"LineCount:" << lineCount << '\n';
oText << L"Raw:" << perLine << '\n';
oText << L"Tra:" << '\n' << '\n';
```

这样改改之后还有个问题 `perLine`这个是string并非`wstring`，但是我们不能直接改了`perLine`。因为`perLine`接受的是`iScript`也就是SJIS的游戏脚本输入的窄字节字符串，并不是宽字节的，为此，我们需要准备一个由窄字节字符串转换到宽字节字符串的函数。

这个函数照抄即可，基本上来说都是这样转换的，用C++ STL实现的其实也是去调用了这两个`API`，反正你在Windows上一般就是这个方法，别的一些函数最终其实也是调用了这个两个API，除非你自己再整一个表来转换。

```c++
#include <Windows.h>
#include <string>

std::string WStrToStr(std::wstring& wstrString, UINT uCodePage)
{
	int lenStr = 0;
	std::string result;

	lenStr = WideCharToMultiByte(uCodePage, NULL, wstrString.c_str(), wstrString.size(), NULL, NULL, NULL, NULL);
	char* buffer = new char[lenStr + 1];
	WideCharToMultiByte(uCodePage, NULL, wstrString.c_str(), wstrString.size(), buffer, lenStr, NULL, NULL);
	buffer[lenStr] = '\0';

	result.append(buffer);
	delete[] buffer;
	return result;
}

std::wstring StrToWStr(std::string& strString, UINT uCodePage)
{
	int lenWStr = 0;
	std::wstring result;

	lenWStr = MultiByteToWideChar(uCodePage, NULL, strString.c_str(), strString.size(), NULL, NULL);
	wchar_t* buffer = new wchar_t[lenWStr + 1];
	MultiByteToWideChar(uCodePage, NULL, strString.c_str(), strString.size(), buffer, lenWStr);
	buffer[lenWStr] = '\0';

	result.append(buffer);
	delete[] buffer;
	return result;
}
```

理论上要先整一个宽字节的`wstring`，但是因为`StrToWStr()`这个函数的返回值就是`wstring`，所以我们这样写就行了，这个函数第二个参数是代码页，现在是SJIS编码的字符串所以写932，如果是GBK就写936

```C++
oText << L"Raw:" << StrToWStr(perLine,932) << '\n';
```

修改完毕后运行，这时候生成的txt文件已经是`UTF8 with Bom`了，直接用记事本打开也是正常显示的。

等下我们会用到从宽字节转到窄字节的函数`WStrToStr()`,因为现在我们输出的文本是UTF8的，也就是宽字节的，等下写进去的译文也是宽字节的，输入到程序也是宽字节的，但是一般SJIS是改成GBK，而不是直接写入Unicode，所以等下导入文本的时候我们得把字符串转换到GBK然后插入到脚本中。当然这个地方其实会有个问题，有些字符串GBK下是没有的，比如音乐的那个符号，转换后会变成问号，这个和上面两个编码转换的API有个，如果大家要求不要这样就行了，不会对游戏运行造成什么影响，后期可以写个程序把这些字符筛出来，替换成别的，当然也能直接在转换的时候报错或者替换成别的字符，具体可以去查上面两API的用法。

## 0x02 文本导入

这里我们稍微讲快一点，因为很多我们前面都铺垫讲解过了，这里不再赘述。

首先我们定义文本导入函数，等下全部实现都写在里面

```c++
bool InsertText(std::string strFileName)
{

}
```

打开译文并读入vector

```c++
bool InsertText(std::string strFileName)
{
	auto cvtUTF8 = locale(locale::empty(), new codecvt_utf8<wchar_t, 0x10ffff, codecvt_mode(consume_header | generate_header | little_endian)>);

	std::vector<wstring> textLineList; //用于存放译文
	std::vector<unsigned int> textLinePositionList; //用于存放译文对应原始脚本的行数

	std::wifstream iText(strFileName + ".txt");
	iText.imbue(cvtUTF8);
	if (iText.is_open())
	{
		unsigned int textLinePosition = 0;
		for (std::wstring perLine; std::getline(iText, perLine);)
		{
			if (perLine.find(L"LineCount:") != std::wstring::npos)
			{
                //其实就是scanf而已，只不过内存里叫sscanf，宽字节的叫swscanf
				swscanf_s(perLine.c_str(), L"LineCount:%d", &textLinePosition);
				textLinePositionList.push_back(textLinePosition);
				continue;
			}

			if (perLine.find(L"Tra:") != std::wstring::npos)
			{
                //这里把Tra:这四个字符去掉了，因为这四个字符也是在行里的。
				textLineList.push_back(perLine.substr(0x4));
				continue;
			}
		}
		iText.close();
	}

	return true;
}
```

运行完后，我们将在`textLineList`和`textLinePositionList`中分别得到译文 、译文在原始脚本的位置,接下来我们按照对应的位置把译文插入回游戏脚本里。当然如果你是直接在`fstream`里插入只能从头尾插入，或改变文件指针，如果你从中间插入，数据并不会全部后移，而是直接覆盖插入位置的数据。

```
1:少しざわざわしている。
2:ウチの学園ならそういう可能性もあるわけか。

8FAD82B582B482ED82B482ED82B582C482A282E981420D0A8345836082CC8A77898082C882E782BB82A482A282A489C2945C90AB82E082A082E982ED82AF82A98142
```

在编辑器中我们可以在1:行后面随便添加文字，保存在硬盘的二进制文件就像是下面一串数字。

为此，我们的做法是先把字符串按行读出来，修改完后按行导入回去。当然这个地方我们用了vector，其实vector并不方便插入行，但是我们的目的其实并不在行操作，而是在操作某一行的内容。如果需要插入行，需要用到类似链表实现的数据结构来操作，不过在处理游戏文本的时候我们不建议去插入行，确实需要换行游戏有相应的换行符号，如果你去给脚本随便的换行增加行，可能会导致游戏奔溃。

```C++
		}
		iText.close();
	}

	std::vector<string> scriptLineList;
	std::ifstream iScript(strFileName);
	if (iScript.is_open())
	{
		for (std::string perLine; std::getline(iScript, perLine);)
		{
			scriptLineList.push_back(perLine);
		}
		iScript.close();
	}

	return true;
```

上述代码执行完毕后，`scriptLineList`里的每一个元素就是一行，由于vector是以0为基准的，所以`scriptLineList[0]`对应的其实是`sublime`里面的第一行。等下替换行的时候得清楚自己导出文本的时候是以0还是1为基准，我们教程演示都是以0为基准。

```C++
	std::ofstream oScript(strFileName + ".new");
	if (oScript.is_open())
	{
		size_t position = 0;
		std::wstring line;
		for (size_t iterPosition = 0; iterPosition < textLinePositionList.size(); iterPosition++)
		{
			position = textLinePositionList[iterPosition]; //取出位置信息
			line = textLineList[iterPosition]; //取出译文
			scriptLineList[position] = WStrToStr(line,936);
		}

		for (auto& iteLine: scriptLineList)
		{
			oScript << iteLine << std::endl;
		}
		oScript.flush();
		oScript.close();
	}
```

以上代码执行完成后，文本就正确导入了，但是其实有个问题，我们输入译文的时候，没有过滤空行，因此这里会把空行替换回去，导致导入后的文本除了译文填写的行，其它行都成了空行。当然这个最简单的解决方法就是输出双行文本，就不会出现这种情况，或者是导入的时候判断一下是否为空行。当然我们这里只是讲讲思路，具体的细节大家自己去实现的时候才能体会到。、

由于这里是用的vector实现，看着比较繁琐，我给大家的源码里写的是map，map的用法大家可以去查查，用map会更方便一点，当然vector里也是可以放结构体的，这个也会方便点。

## 0x03 总结一下文本导入

首先读取导入的信息，即导入的位置和导入的字符串

然后把游戏原始的脚本以行的方式读进来存到vector里

然后依据信息把对应的行（vector中）的字符串替换掉

最后把替换好的游戏脚本一行行写回去即可
