# [Malie引擎] 寻找密钥 P1

今天下午刚考完，也没什么事干，也没钱，只能在宿舍坐着了。

坐着也坐着，上KF看看，看到个帖子 求Campus 恋音セ・ピアーチェ的完整立绘

https://kf.miaola.work/read.php?tid=980463&sf=fd2

campus这个社的游戏还行啊，点开看看，好像还有好几个相关帖子，也是封包解不开的问题
虽然这引擎也见过好几次，但是也没认真看过，反正咱也不是汉化组的，没什么必要也不会去看。

好吧，那就看看吧，看帖子说Garbro解包不了，这个挺正常的，因为这引擎的封包都是单独密钥加密的，基本上每个游戏都不太一样，所以得手动找密钥，但是Garbro的作者也不知道是为什么考虑，把基本上设计的游戏密钥的都不公开明文，也很少会说怎么找密钥。

![1](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BMalie%E5%BC%95%E6%93%8E%5D%20%E5%AF%BB%E6%89%BE%E5%AF%86%E9%92%A5/1.png)



## 编译调试Garbro

因为Garbro有一部分campus社的游戏是可以解包的，因为作者已经导入密钥，反正现在也不知道怎么找密钥，先下一个Garbro在打开封包的地方下个断点看看密钥长啥样吧（对的虽然它不公开，但是调试的时候依然可以看到，打开Garbro的项目也可以看到有一个叫SchemeBuilder的项目被删了）

![2](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BMalie%E5%BC%95%E6%93%8E%5D%20%E5%AF%BB%E6%89%BE%E5%AF%86%E9%92%A5/2.png)

在编译和运行Garbro之前还会有Nuget包管理器的问题，参考以下两个链接解决

https://stackoverflow.com/questions/57394869/missing-nuget-targets

https://learn.microsoft.com/en-us/nuget/consume-packages/package-restore

解决完成之后就可以正常编译运行了。可能还会有什么鬼签名的问题，自己搜一下吧。

## 编译exdieslib

exdieslib就比较简单了，用的是C++写的，只要到这里下.cpp即可，但是作者没提供as-util.h这个头文件，可以在FuckGalEngine的Minori/Minori/fuckpaz里找到一个。可能是FuckGalEngine项目作者自己写的。

http://asmodean.reverse.net/pages/exdieslib.html

https://github.com/Inori/FuckGalEngine/tree/master/Minori/Minori/fuckpaz

得到一个cpp和一个头文件，vs新建一个项目加进去就行了，不过exdieslib有好几个函数用的都是非标准c的函数，vs编译会报错，依据报错改过来就行了。

![3](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BMalie%E5%BC%95%E6%93%8E%5D%20%E5%AF%BB%E6%89%BE%E5%AF%86%E9%92%A5/3.png)

exdieslib也能直接看到密钥啥样。

![4](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BMalie%E5%BC%95%E6%93%8E%5D%20%E5%AF%BB%E6%89%BE%E5%AF%86%E9%92%A5/4.png)

## 先看密钥！

先找到ArcLIB.cs这个文件，然后到找到151行，在foreach处下断点。然后GARbro.GUI为调试的目标程序，点击绿色三角运行即可。之后就是在GarbroGUI的窗口里找到封包双击打开，就断下了。

![5](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BMalie%E5%BC%95%E6%93%8E%5D%20%E5%AF%BB%E6%89%BE%E5%AF%86%E9%92%A5/5.png)

我选择的游戏是 Deep Love Diary -恋人日記- パッケージ版 这个是Garbro有内置密钥支持解包的，需要注意的是有时候Garbro自身残留配置文件，会导致无法识别游戏，进而没办法解包，即使内置密钥也不行。由于我懒得找Garbro的配置文件了，之前记得在用户文件夹的什么地方，后面忘了，反出现这种情况。直接把Garbro移动到别的文件夹或路径下就可以了。

ok，准备就行我们来看看密钥，在左下角的watch窗口可以看到KnownSchemes这个结构，点+展开，就能看到里面对应的游戏名和密钥了。和exdieslib是一样的，不过Garbro的密钥大小会少4*4个字节，不过一般都是00。

![6](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BMalie%E5%BC%95%E6%93%8E%5D%20%E5%AF%BB%E6%89%BE%E5%AF%86%E9%92%A5/6.png)

可以结合exdieslib来看，exdieslib里面明明白白的写出了密钥，用c语言写的结构也更清晰。

反正两个对着看，大概是看明白了密钥长啥样，至于封包结构，暂时还不要去理这个。

## 找一个看看

现在我们已经看到了，密钥是一张表，现在的问题是怎么找到这张表。很显然，我们先找一个Garbro或exdieslib里有密钥的游戏，我们先对着这个游戏找出一样的密钥即可，这个时候随便乱找也行，没准你乱搜还真能搜到。当然这边我就不去乱搜了，我稍微看了一下exdieslib和Garbro的解析，都会先去解析封包开始的一些字节，来判断封包的标头，是不是LIB字段，以此来确定目前使用的密钥是能够解密游戏的，然后用这个能解析出正确标头的密钥进行文件索引和数据内容的解密。

依据上述的特征，我准备开始下断点看文件头了，结果发现这个社好几个游戏都有Playdrm加壳，虽然这个加壳并没什么用，但是为了简化分析还是找了没壳又有现成密钥的。

最后我选择了 神咒神威神楽 超先行 体験版 是的体验版一般都没乱七八糟的加壳，比较好分析（按Tab）

https://dlsoft.dmm.co.jp/detail/views_0363/

本来想先断一下CreateFileA/W的，结果发现断下来都是没什么用的信息，那换成ReadFile,发现它第一次ReadFile就读取了0x1000的大小，并且刚好是文件头上还是的0x1000个字节，只能说来得正好，但是不确定的是为什么要读0x1000，而且每次都读0x1000，然后翻了下导入表，原来是有msvcrt，有c的库函数fopen和fread这些，现在换成断fopen，fread，发现正常了，第一次fread就打开了指定的封包，fread则是每次读取0x10个字节。

翻了下上面两个项目的源码，发现这东西是按块解密的，就是文件分成了很多十六个字节的块，一个个解密过去。看着可能是更这个有关，算了，别理他为什么，先给读到内存里的文件头上的第一个字节和第四个字节下硬件断点。

![7](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BMalie%E5%BC%95%E6%93%8E%5D%20%E5%AF%BB%E6%89%BE%E5%AF%86%E9%92%A5/7.png)

按F9运行，断下。

![8](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BMalie%E5%BC%95%E6%93%8E%5D%20%E5%AF%BB%E6%89%BE%E5%AF%86%E9%92%A5/8.png)

发现直接就对比文件标头了。这个时候IDA也一起打开来辅助分析。

![9](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BMalie%E5%BC%95%E6%93%8E%5D%20%E5%AF%BB%E6%89%BE%E5%AF%86%E9%92%A5/9.png)

按x查找引用返回上一层函数。发现这个函数，是用来打开文件的，后面回调用fopen，打开后返回的指针存储着打开文件的信息，像是句柄啥的。第二个参数模式，有LFIE_I，还有CFI这种，第一个是文件的相对路径。接着把获得的指针传给了另一个函数，这个函数则是调用fread从封包读取数据到buffer里。

![10](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BMalie%E5%BC%95%E6%93%8E%5D%20%E5%AF%BB%E6%89%BE%E5%AF%86%E9%92%A5/10.png)

大概整理一下，是这样的

![11](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BMalie%E5%BC%95%E6%93%8E%5D%20%E5%AF%BB%E6%89%BE%E5%AF%86%E9%92%A5/11.png)

第一次读取并校验的时候是不成功的，所以进入了第二次读取。第二次比较奇怪的地方是GetBlockHandle的模式变成了CFI，这次GetBlock读取进来的的封包前0x10个字节居然是已经解密了，直接就能看到LIB字段。由于GetBlock会调用fread，观察第二次读取的fread，发现依旧是0x10个字节，而是也是文件开头的0x10个字节，与第一次读取一样，说明这个GetBlock里面堆文件标头进行了解密。

我们先对这次读进来的数据下硬件断点，F9运行。

![12](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BMalie%E5%BC%95%E6%93%8E%5D%20%E5%AF%BB%E6%89%BE%E5%AF%86%E9%92%A5/12.png)

发现fread刚刚返回就对读取进来的数据进行了位移操作，仅接着，在sub_4247F0函数运行后，观察这个函数的参数，发现数据已经解密了。继续观察这个函数，发现第二个参数就是密钥，虽然顺序有点不一样。

![13](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BMalie%E5%BC%95%E6%93%8E%5D%20%E5%AF%BB%E6%89%BE%E5%AF%86%E9%92%A5/13.png)



写一个程序把密钥顺序搞一下

```C
#include <iostream>

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		unsigned long aKey[56] = { 0 };

		FILE* fp = nullptr;
		errno_t err = fopen_s(&fp, argv[1], "rb");
		if (err || !fp) return 0;

		fread(aKey, 1, 0xE0, fp);

		for (size_t iteLine = 0; iteLine < 14; iteLine++)
		{
			for (size_t iteEle = 0; iteEle < 4; iteEle++)
			{
				printf("0x%08X", aKey[(13 - iteLine) * 4 + iteEle]);
				if (!(iteLine == 13 && iteEle == 3)) putchar(',');
			}
			putchar('\n');
		}
	}
	else
	{
		printf("KeyFormat.exe key.bin");
	}

	system("pause");
}
```

Keyformat.cpp

![14](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BMalie%E5%BC%95%E6%93%8E%5D%20%E5%AF%BB%E6%89%BE%E5%AF%86%E9%92%A5/14.png)

由于我这个是按照exdieslib来的，所以要往上翻4*4个字节，这个上面16个字节一般都是0。

好了现在密钥就有了，不过还不清楚这个密钥表是怎么来的，直接搜索也找不到这个表，于是又跟踪了一下。

发现游戏自己搞了一个读取和打开封包的玩意，就是封装这两个函数。那个上面GetBlockHandle的MODE像是fopen这种的扩展，游戏还有自己的MODE，上面CFI，FILE什么的。在第二次GetBlock跟踪走了下图这个函数。，这个函数有个奇怪的参数，写死的，而是像是一段密码一样的字符串。

![15](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BMalie%E5%BC%95%E6%93%8E%5D%20%E5%AF%BB%E6%89%BE%E5%AF%86%E9%92%A5/15.png)

继续跟踪会发现这个参数进入了一个函数，类似之前的解密函数，第一个参数也是0x80

![16](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BMalie%E5%BC%95%E6%93%8E%5D%20%E5%AF%BB%E6%89%BE%E5%AF%86%E9%92%A5/16.png)

执行完毕后，第三个参数指向的buffer就变成了密钥的那张表了。

![17](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BMalie%E5%BC%95%E6%93%8E%5D%20%E5%AF%BB%E6%89%BE%E5%AF%86%E9%92%A5/17.png)

看来真正的密钥其实是：XogRr2FjLW0waAuW，经过这个函数运算就成了Garbo和exdieslib里的那个表。

## 加壳怎么找?

其实没啥关系，因为很多所谓的加壳，都没有虚拟机代码，也就是代码其实运行后都原封不动吐出来了，只是会加密IAT之类的。

不过因为加密了IAT我们就不能从导入表中直接对fread，fopen下断点了，这有关系吗？没有，完全没有，因为这两个函数也不是在exe里实现的。不过值得注意的是，有些壳会把fread和fopen重新定向到壳子内部，绕一圈，调用CreateFileW，fread则是调用ReadFile，或者有些程序干脆就把库函数一起编译进去了，这个时候我们只能从CreateFile来定位回去。

这边以 Deep Love Diary -恋人日記- パッケージ版 为例子展示一个在Playdrm加壳的情况下找密钥，当然前提你先破解，这个不多说。

为了节省篇幅就直说了，fread和fopen是断不下来的，我们直接断CreateFileW，（注意下，如果有反调试，记得装scyllahide）

看到有 0155EDF0 01996968        L".\\data\\data9.dat" 这种就说明游戏已经开始打开封包了。这个时候直接点返回用户区代码

![18](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BMalie%E5%BC%95%E6%93%8E%5D%20%E5%AF%BB%E6%89%BE%E5%AF%86%E9%92%A5/18.png)

然后在栈里往下翻，以为fopen一般都有一个mode参数，rb wb 什么的，往下翻就能看到，有好几个，可以到压入的返回地址，就是那个红色的，地址处看调用的地方，然后你可以看到这么个结构的代码。

![19](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BMalie%E5%BC%95%E6%93%8E%5D%20%E5%AF%BB%E6%89%BE%E5%AF%86%E9%92%A5/19.png)

对的，就是这里，这个引擎调用fopen的时候会进行宽字节到窄字节的转换。

那么00683B02 | E8 C6020100                   | call malie3.693DCD                                   | 其实就是fopen了。

我们在add esp 8下断点，等他返回。因为游戏没data9这个封包，返回值也是0，没关系，游戏会多次调用fopen，直到找到封包。

我们按F7单步返回上一层函数，返回到第二层的时候可以看到游戏的两个读取模式了LFILE,CFI，并且call 0x00687B60这个函数其实就是对比文件标头的，可以点进去看，这个位置其实就是我们刚刚的InitHeader那个函数一样的地方。

![20](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BMalie%E5%BC%95%E6%93%8E%5D%20%E5%AF%BB%E6%89%BE%E5%AF%86%E9%92%A5/20.png)

这个时候就很好办了吧。只要知道GetBlock这个函数就行了，不过这里运行后这个函数就退出了，因为data9这个封包没有，所有GetBlockHandle这个函数会失效，等读到data.dat封包的时候就好了。这个时候可以配合断下ReadFile，并在读取的数据上下硬件断点。和之前一样就能跟踪到解密的地方。

当然你说有没更快的方法呢？当然有了，还记得上面那个参数写死，也就是生成密钥表的函数吗？因为那个函数会对比模式，也就是rb wb r+b这种，所有我们直接运行游戏，等游戏跑起来，我们直接来到游戏这个模块，右键搜索该模块全部字符串。注意左上角的Module：malie3.exe你别搜到别的模块去了。直接搜索w+b

![22](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BMalie%E5%BC%95%E6%93%8E%5D%20%E5%AF%BB%E6%89%BE%E5%AF%86%E9%92%A5/22.png)

可以看到只有两个结果，找到这个push的。

![23](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BMalie%E5%BC%95%E6%93%8E%5D%20%E5%AF%BB%E6%89%BE%E5%AF%86%E9%92%A5/23.png)

是不是看到了熟悉的push 80，如果你返回这个函数调用上一层，或者看函数传进来的参数，你就能看到用于生成密钥表的密文

![24](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BMalie%E5%BC%95%E6%93%8E%5D%20%E5%AF%BB%E6%89%BE%E5%AF%86%E9%92%A5/24.png)

## 第一部分结束

说了一堆 恋音セ・ピアーチェ 这个游戏都还没开始解，我知道你很急，但是你先别急。

先讲到这，有点长了，有些新的Malie引擎的游戏会出现直接没有那个密文生成密钥的函数的情况。也就没办法直接获取密钥表了，这个下一接讲 ：）

