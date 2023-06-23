# [YU-RIS] SDK编译器调用

[YU-RIS的官方](http://yu-ris.net/)是提供SDK下载的，而且他们2023年还更新了提供下载的引擎版本到0.490

这个SDK里包含了一个完整的demo工程文件，还有未编译的脚本，并附带了一些工具，比如脚本的编译器和封包工具，音频视频转换工具之类的。

最近为了研究YU-RIS引擎的脚本反编译，发现官方的SDK脚本文件虽然是明文，但是运行的时候是要调用编译器编译的，所以这里可以自己修改脚本来观察编译后的文件，这样研究反编译也方便很多，还可以对官方编译器进行逆向之类的。

不过正当想着怎么使用官方这个编译器的时候，发现，压根没有文档啊！也没控制台或GUI界面！双击启动直接没反应，真是烦人呐，唯一能观察到的就是启动官方的demo程序（需要转区不然编译器无法正常编译）会出现编译脚本的dialog，等它编译完成就进入demo了，但是检查目录下并没有任何编译后的文件。推测应该是主程序去调用了编译器程序并把编译的文件直接放在了主程序的堆里。

真是太傻X了！一个编译器还搞这样！

## 观察目录结构

SDK下载后解压到 “システム”文件夹里就是demo的项目目录了，

![](..\..\.img\[YU-RIS]_SDK编译器调用\1.png)

官方还有一份这个目录的结构说明

```
/システム/yu-ris.exe          :起動ファイル
         /エンジン設定.exe    :エンジン動作設定ファイル
         /yscfg.dat           :エンジン動作設定データ
         /system/forE-mote/   :E-mote動作用DLLファイル
                /forWin98/    :Win98以降版用EXEファイル
                /YSCom/       :コンパイルツール
                /YSConv/      :画像形式変換ツール
                /YSEnv/       :環境情報取得ツール
                /YSIcon/      :EXEアイコン書換ツール
                /YSOgg/       :OGGファイル変換ツール
                /YSOne/       :1ファイルEXE作成ツール
                /YSPac/       :ファイルパックツール
                /YSScr/       :スクリーンセーバー作成ツール
                /YSSelf/      :自己解凍EXE作成ツール
                /YSYmv/       :YMVムービー作成ツール
                /ysconfig.txt :システム設定ファイル
         /data/               :ERISサンプルプロジェクト

/マニュアル/                  :β4暫定マニュアル

/はじめに.txt                 :簡易説明テキスト(このテキスト)
```

data里就是项目文件了，包含了脚本代码和资源文件，system里则是一些工具，编译器就是YSCom

转区启动可以看到这个界面，编译完成后这个程序就退出了，demo正常运行启动，可见是在运行的时候进行了脚本的编译，不过目录下并没有编译后的文件就是了

![](..\..\.img\[YU-RIS]_SDK编译器调用\2.png)

找到编译器的主程序看看，可以看到YSCom.exe就是了，还有个YSCom.ycd的文件，这个Com应该是command的缩写，里面的结构和编译后ysbin里的ysc.ybn文件很像，那些字符串其实是按顺序的Opcode的名称和参数的名称，这个有机会再说吧。双击YScom.exe也没任何反应，控制台运行也没有任何输出。

![](..\..\.img\[YU-RIS]_SDK编译器调用\3.png)

## 方法论

这个问题的核心就是通过 CreateFileMapping 通过内核对象来共享数据



## 主程序编译器调用分析

这里只讲解关键部位的作用，具体的函数调用流程可以到[YurisTools](https://github.com/Dir-A/YurisTools/tree/main/_other/IDA_DATA)里下载IDA的文件自行查看。

yu-ris.exe就是主程序了，既然要调用YSCom.exe进行编译，肯定有个CreateProcess这样的创建进程的操作，对该API下断点是一个不错的选择。而且既然要创建进程，当然会有进程名称这个参数，所以搜索字符串YSCom.exe也是一个思路。

还有就是可能涉及到进程之间的数据传递问题，也就是编译器如何把编译后的脚本数据传递给主程序的问题。

随便思索一下，现在把yu-ris.exe拽进IDA先。进来后搜一下字符串，YSCom果然是有的，而且还是相对路径

![](..\..\.img\[YU-RIS]_SDK编译器调用\4.png)

查看一下引用，发现都来自同一个函数内部，这里函数名和变量名我稍微处理了一下，并添加了结构体偏移的信息

![](..\..\.img\[YU-RIS]_SDK编译器调用\5.png)

一看到CreateFileMapping差不多结构就出来了，这是通过内核对象来在不同进程之间传递数据啊，不懂也没关系稍后介绍吧。

先在x64dbg里断下这个函数，发现确实走了这里，并且这个函数执行过程中就出现了编译的dailog窗口，差不多就可以确定，这个函数就是调用编译器的函数了，那么我们就仔细来分析这个函数。

这里还有一点需要注意，因为编译器并不能在中文系统下正确编译，所以需要转区运行，那么转区运行怎么调试呢？Locale Emulator可以为每个程序创建配置文件，在配置文件里把 Create process with CREATE_SUSPENDED勾上就可以了，这时候进程创建后里面的线程都是挂起的，x64dbg附加后再resume thread就可以转区调试了。而且该进程就算后续再创建子进程也是可以被LE转区的，这也就是为什么这样编译器进程就可以正常编译了。

其实不转区也是可以打开程序的，只不过会在编译阶段报错，这个原因和路径有关，只要路径全为英文就可以打开，**具体原因参见[无法直接运行的解决方法](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/引擎逆向/YU-RIS/[YU-RIS] 无法直接运行的解决方法.md)**

![](..\..\.img\[YU-RIS]_SDK编译器调用\8.png)

一进来就是fopen，可以看到fopen成功后就fclose了，可见这个仅仅只是判断YSCom.exe这个文件是否存在而已

紧接着就是CreateFileMapping了，这个函数其实就是映射文件，当然具体的专业说法就自己去查吧，这里就简单说一下，CreateFileMapping创建一个内核对象，我们都知道内核对象不同于在本进程的那些对象，内核对象是可以在不同进程之间共享的，CreateFileMapping可以理解为创建了一个buffer，但是这个buffer是可以在不同进程之间来访问的，要访问这个buffer就得把这个buffer映射进当前进程的地址空间，相当于我们要得到一个buffer得new一样，不过这个是用MapViewOfFile而且buffer的内容是确定的，返回值就是指向这个buffer在当前进程的地址了，你可以修改buffer里的内容，然后别的进程就可以OpenFileMapping然后MapViewOfFile来取得里面的内容。

好了继续看代码，第一步CreateFileMappingA了一个0x200的空间，并命名为YU-RISCompilerFileMapObject，紧接着MapViewOfFile来映射到进程地址空间得到指针hdr_ptr，这个空间大小其实就512字节了，具体结构如下

```c
struct YSCOM_Compiler_HDR
{
	uint32_t uiCompileState0;          // State For Compiler Open YSCOM_Compiler_HDR            1 = Successfully
	uint32_t uiCompileState1;          // State For Engine Create YU-RISCompilerFileMapObject2  1 = Successfully
	uint32_t uiCompileState2;          // State For Compiler Compiled All Script                1 = Successfully
	uint32_t uiCompileState3;          // State For Compiler Compiled All Script                1 = Successfully
	uint32_t uiCompiledScriptCount;
	uint32_t uiCompiledScriptsSize;
	uint32_t uiUnknow1[0x1A];
	uint32_t uiEngineVersion;          // 486
	uint8_t  ucProjectPath[0x17C];
};
```

接着可以看到这里对版本号uiEngineVersion和项目目录进行了赋值，前头还有一个`sg_fnInitBufferToZero(hdr_ptr, 0x400);`其实这是一个全局的函数指针，具体指向的函数由cpu的指令集和配置文件决定，在0x00403588的函数会对这些全局的函数指针进行初始化。

紧接着就调用了CreateProcessA来运行编译器进程，值得注意的是它没给CreateProcessA的第一个参数传递编译器的程序名，而且在commandline参数里写`system\\YSCom\\YSCom.exe ys`来调用编译器，并且带有一个arg，就是ys。

由此可见，编译器进程应该会捕获arg，在YSCom的WinMain有如下代码

先是判断了args的数目，显然是两个，如果直接双击运行编译器的exe显然是一个arg，那么start_compile_flag就会变成0从而无法进入StartCompile函数来执行编译，也就是退出了程序。如果正确的添加了arg，那么会对比第二个arg是不是ys，是的话给start_compile_flag赋值true，也就可以进入StartCompile函数来进行编译了。通过对arg的观察，发现这个ys除了在这里进行验证，也没别的作用了，我们完全可以强制跳转到StartCompile函数。

![](..\..\.img\[YU-RIS]_SDK编译器调用\9.png)

回到我们的主程序，然后是一个while死循环，里面在判断这个编译器进程是否在运行，并且等待uiCompileState0，被修改为1，显然是等待编译器的进程去修改，也就说明编译器会去OpenFileMapping，然后MapViewOfFile，并修改。如果编译器进程在运行，并且修改为1，则退出循环。

那么我们可以通过搜索YU-RISCompilerFileMapObject这个内核对象的名称来在YSCom.exe里定位到这部分的代码

搜索结果有两处，第一处可以看到检查了版本号，并且把主程序当前的目录路径字符串复制了一份。但是这里并没有去修改uiCompileState0。

![](..\..\.img\[YU-RIS]_SDK编译器调用\7.png)

第二处如下

![](..\..\.img\[YU-RIS]_SDK编译器调用\10.png)

可以看到这里打开了YU-RISCompilerFileMapObject并且映射进地址空间，修改了uiCompileState0为1，这样主程序的死循环就可以退出来了。同时还传递了脚本数量和编译后buffer的总大小信息，然后对ProjectPath拷贝了一些东西，并死循环来等待uiCompileState1被修改成1

回到我们的主程序，死循环退出后获取了编译脚本的总数和总大小，并开始创建名为YU-RISCompilerFileMapObject2的新内核对象，该内核对象的大小正是刚刚传递过来的编译文件总大小。

![](..\..\.img\[YU-RIS]_SDK编译器调用\11.png)

创建完成新的内核对象，就对uiCompileState1修改成1，此时一个死循环，等待编译进程退出。

来到编译器这边

![](..\..\.img\[YU-RIS]_SDK编译器调用\12.png)

可以看到打开了新的内核对象，并有一个以脚本数量为范围的循环处理，猜测就是编译部分的代码了，不过这里好多都是函数指针数组的调用，看起来有点麻烦，需要先回去函数指针数组初始化的地方看数组对应的真正函数地址。

处理完成后给uiCompileState2和uiCompileState3都给设置成1，接着就是清理工作了，并退出程序了。

回到主程序，当检测到编译器退出后，就会malloc一个空间，并把数据都拷贝过来，然后生成一个包含描述脚本大小和脚本名称，脚本数据地址的结构，用于等下的脚本读取，也就是sg_ysbin_file_list了。

## 解析编译器返回的数据

当编译器进程退出后，map_file内buffer里的数据如下

![](..\..\.img\[YU-RIS]_SDK编译器调用\17.png)

结构也可以很简单，脚本路径字符串长度，脚本路径字符串，脚本数据大小，脚本数据，按照这个结构解析就可以得到全部编译后的文件了，值得注意的是这些编译后的文件都是加密的，也就是xor啦，用的是默认的密钥，0x96AC6FD3，也可以猜密钥，方法很多。

```C
struct YSCOM_Compiled_Script_Entry
{
	uint32_t uiScriptNameSize;         // With End 0x00, With Path
	uint32_t uiScriptFileSize;
	// file data
};

// Mapping OBJ Name YU-RISCompilerFileMapObject2 : YSCom.exe : 0x0040168C
struct YSCOM_Compiled_Script
{
	YSCOM_Compiled_Script_Entry[YSCOM_Compiler_HDR.uiCompiledScriptCount];
};
```

## 再谈YU-RIS的文件读取模式

如果你看到我之前的文章就知道，yu-ris读取文件有不同的模式，通过一个读取优先顺序的数组来控制。

![](..\..\.img\[YU-RIS]_SDK编译器调用\13.png)

可以看到有三种模式

![](..\..\.img\[YU-RIS]_SDK编译器调用\14.png)

可见GetFileViaList这个分支就是读取刚刚编译器生成的文件数据。而下面两个则是从封包和目录下读取文件。

sg_ysbin_file_list结构如下

![](..\..\.img\[YU-RIS]_SDK编译器调用\15.png)

![](..\..\.img\[YU-RIS]_SDK编译器调用\16.png)

## 实现编译器的启动器

观察完了上述调用过程，心里应该也有个大概的流程了，我们先可以自己写一个程序来调用编译器编译文件并且把编译的文件保存起来。

https://github.com/Dir-A/YurisTools/blob/main/lib/YurisStaticLibrary/YSCOM.cpp#L21C7-L21C7

当然现在编译阶段的大概位置也可以确定了，对编译过程的分析也可以找到入手点了。

还有不太明白的，没看懂的，建议自己手动调试一下。

## 总结

实际上这就是两个进程之间通过内核对象进行数据交换罢了。

主程序通过创建内核对象 YU-RISCompilerFileMapObject 来和编译器交换所需要的信息

编译器把编译后的数据通过 YU-RISCompilerFileMapObject2 来传递给主程序

接着主程序解析编译后的数据，生成一个描述文件的结构（文件名，大小，地址）的数组

引擎通过切换不同的文件读取模式，来在该数组中查找需要的文件

这也就是为什么编译后目录下没有任何编译文件的原因了，因为它全部编译的文件都存在主程序的堆里。