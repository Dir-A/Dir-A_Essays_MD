# [QLIE引擎] 封包接口Hook

这个主题快拖了半个月，中间一直没空写，今天看着实在有点久了，必须写一写了，不然就快忘记了。

## 起因

前不久 Happy Live Show Up 发了官中，但是又搞的奇奇怪怪的加密，很是无聊，稍微调了下和之前 ハミダシクリエイティブ 官中是挺像的。

team的dll是用Themida保护的，其实就是Themida自己手动加载dll，目录下那个dll实际上是没用的，不过和内存里dump出来的是一样的，和Steamworks官方提供的dll好像也是一样的，这都不算什么，毕竟Alpharom十多年前就玩过了，小套路而已，但是这玩意比Alpharom强的是懂得代码虚拟化，调着真是依托shit啊。

![3](C:\Users\Xse\Desktop\image\3.png)

不过GitHub好像是有几个项目能还原Themida的虚拟机，但是我本身不怎么研究脱壳加壳，要搞这个也得有点时间，而且就算搞定，也没用，因为我就没研究过steam游戏的验证机制 ：），他要是稍微搞点一般steam emulator过不了的东西那我也搞不懂，到头还得研究steam验证机制谁有空啊。

ハミダシクリエイティブ只能说是凑巧，steam验证上没什么难度，但是自己开了个线程疯狂检查干掉就结束。

反正 Happy Live Show Up 官中是看着头大，对Themida加载的steam dll全部api都hook重定向到Goldberg貌似是能过steam验证的，但是返回到游戏exe里然后跳到虚拟化代码里饶了几圈就寄了，看着像是被检测到了，中途还能看到检测steam.exe是否存在的各种奇奇怪怪的检测，函数调用和返回疯狂往虚拟机里跳，从游戏exe走到，SteamAPI_RestartAppIFNecessary居然能把x64dbg的tracing的默认5w步走完还多8000多步，根个病毒一样，一般这个步数撑死都不会超过500，有多慢就可想而知了。

![2](C:\Users\Xse\Desktop\image\2.png)

后面发现把他检测线程干掉了，能正常走到游戏显示启动项的地方，但是提示读取资源错误。感觉要么是有其它检测，要么是被检测到随机返回了错误的数据给游戏。

![1](C:\Users\Xse\Desktop\image\1.png)

至于具体是什么问题就很难判断了，因为我这根本没steam账号，根本跑不起来这个游戏，跟踪更没可能了。还是丢回收站，右键清空回收站实在。

好吧，先别急着丢，看看能不能直接把封包结构逆出来，直接解包一步到位。

稍微去GitHub看了下QLIE的封包结构，再次对比官中的封包，看着结构很像，其实不然，官中好像只有一个封包能用garbro读出几个文件名，解包也是意义不明的数据，看着就像是改过了封包结构。

ok别管，开始跟踪解密流，跟踪的时候就明了了，官中的封包有点像是之前Malie的形式，在原格式的外面套了一层加密，通过Hook 读取文件的WinAPI和开辟堆空间的API来实现一个自己的文件系统，对封包文件的外层加密在游戏读取的时候解密掉，在游戏引擎看来就像是没加密，想着估计只是对封包的索引加密了，我先把解密后的封包文件头搞出来，然后把hash表搞出来，然后一拍脑袋发现不对，这东西整个都加密了，太狠。看着单纯提取封包的结构怕是没办法还原出来解密了。只能跟一下解密算法了。

下面就是一部分的加密算法部分，粗略统计应该有七次解密循环，只有第一次的xor是良心的，后面都是一坨。而且这么多重的解密应该也不像是自己写的，可能是套用了什么现成的算法或变种，当然具体不清楚，唯一清楚的是，这东西跑起来巨卡。后面刚好网上有人放了破解补丁，但是我已经对破解不感兴趣了，因为这玩意的数据加密，再加上QLIE初始化的时候会遍历全部封包的文件，导致游戏一打开卡的一坨。虽然是单核战士，并不会给你电脑卡死，除非你是单核的电脑，但是游戏确实卡。

![4](C:\Users\Xse\Desktop\image\4.png)

## 躺平

无所谓，没人会想去理清楚，别人随手在地上画的乱七八糟的线条

好吧他确实是懂加密的，不仅加了能让游戏卡死的数据加密，还加了能让电脑累死的虚拟机，顺便开了个线程检测，专业！（虽然基本都是themida的功劳）

可是他没有QLIE的源码，这就从本质上决定了，加密的无效性。当然其实有源码也很难大改，如果真的改了那就不是QLIE了，当然这个只是站在程序的角度，如果站在系统的角度，那同样也是从本质上决定了加密的无效性。

## 原理

那么为什么说是从本质上决定？

就这么说吧，比如你这小子特别欠打，我邀了几个人放学后在校门口怼你，假设只有一个校门，而且围栏都通电，你只能从这一个校门出。那我搬一个凳子坐在校门口，就能蹲到你，当然你不出来可以吗？可以完全可以，那游戏也别运行了。

这个校门就像是游戏的文件接口，通常的游戏特别是Gal这种，会有封包，封包就是一种自定义的文件系统，试想你自己定义了一个封包，然后你要让游戏引擎读取，你是不是得提供函数或者接口才能读取？你不提供接口那游戏怎么读取你的封包？反正我是不知道。所以必然有这么一个接口给游戏引擎读取，不管你的封包加密有多么复杂，你也不可能带着加密的文件一路跑到结束，就算你的游戏引擎说，我一帧能跑就行，全程带着加密跑，ok完全ok，但是当你调用图形api的时候系统api的时候你还怎么带着加密文件一路跑？显然是不科学和不可能的。

通常来说一个游戏有自己私有的封包或图片文件格式，在游戏引擎里都有对应的读取封包和解析图片文件的方法，这其实是废话，就和妈妈生的一样，当然你要说克隆那我也没办法了。

所以对于一个单机的游戏，无论封包是什么加密，有多么复杂，在游戏引擎里必然有解密的方法。这和你网上下个7z压缩包要密码那可完全不一样。封包是加密了，但是解密的东西也给你了，只不过这个解密的东西是在一个exe里，并且是编译过了，全是机器码，所以就看你有没本事从这一堆机器码里找到解包的算法了。如果是你网上下个7z压缩包要密码，那就只能硬猜了。

## 收集

开始逆向前得多收集信息，往往这些信息可以起到事半功倍的效果。

由于我一开始把QLIE当成了C/C++开发的，一个jmp干过去，dump完了数据，准备收工，后来仔细思索发现有点不像是C/C++开发的，一查是Delphi，那这不就妥了。

![5](C:\Users\Xse\Desktop\image\5.png)

来请IDR https://github.com/crypto2011/IDR

![6](C:\Users\Xse\Desktop\image\6.png)

这可比IDA爽多了。很多函数名已经还原了，接着导出这些函数名和标记到IDA

![7](C:\Users\Xse\Desktop\image\7.png)

可以发现封包的处理是属于TFilePack  THashFile这种名称函数的，由于Delphi我甚至都没用过，只是在函数调用约定里见过Pascal调用这种，具体细节不是很清楚，不过一开始我已经分析到了QLIE循环读取封包里文件名的地方，所以从这个地方往回翻，跟踪Create Init Get这种后缀的函数大致可以推断出结构，由于我们的目的仅仅是调用接口解包，所以没必要全部分析出来，除非你想移植QLIE引擎。

![8](C:\Users\Xse\Desktop\image\8.png)

## QLIE Extract

那么大致解释一下，有兴趣可以自己下一个来跟踪，挺简单的。

数据和实现也已经开源，可以从这里下载到 https://github.com/Dir-A/QLIE_Extract

### 调用约定

首先需要了解一下调用约定
不知道是Delphi的原因还是编译器的原因，里面的所有的函数传参都很诡异，不过大致符合Pascal的传参，但又不是，第一个参数是通过eax，第二个edx， 第三个ecx，然后都通过栈传递，但是遵循Pascal的顺序。如果要C语言调用需要转换成stdcall或fastcall这种。

### 初始化封包

![13](C:\Users\Xse\Desktop\image\13.png)

```C
LPTPack_PackEntry THashFilePack_Create(LPTPack_InitTable pFileList, CHAR cUn0, LPTPack_Buffer pKeyFile, CHAR cUn1, CHAR cUn2, PCWCHAR lpPackName)
```

THashFilePack_Create 这个函数是用来初始化封包的，可以理解为打开封包，打开成功则返回TPack_PackEntry对象指针，内部的实现会去读取封包里的全部文件名。函数调用结束后紧接着就保存返回的对象。

游戏启动的时候会多次调用这个函数，初始化全部封包，保存全部的TPack_PackEntry

![12](C:\Users\Xse\Desktop\image\12.png)

当然下面的结构不一定是对的，因为我只关心提取文件，没有把全部成员的具体作用都观察完

```C
typedef struct TPack_Handle
{
	PDWORD            pTable;
	HANDLE            hFile;
	PWCHAR            lpPackName;
}*LPTPack_Handle;

typedef struct TPack_ResEntry
{
	DWORD             dwOffset;
	DWORD             dwUn0;
	DWORD             dwCompSize;
	DWORD             dwDempSize;
	DWORD             dwCompFlag;
	DWORD             dwEncFlag;
	DWORD             dwDecKey;
}*LPTPack_ResEntry;

typedef struct TPack_ResIndex
{
	PDWORD            pTable;
	LPTPack_InitTable pTPack_InitTable;
	LPTPack_Handle    pPackHandle;
	LPTPack_ResEntry* pResInfoEntry;
	PWCHAR*           pResNameEntry;
	DWORD             dwResCount;
}*LPTPack_ResIndex;

typedef struct TPack_PackEntry
{
	PDWORD            pTable;
	DWORD             dwUn0;
	PDWORD            dwUn1;
	LPTPack_ResIndex  pResIndex;
	WCHAR*            lpHashFileName;//L"GameData\\data1.hash"
	WCHAR*            lpPackFileName;//L"GameData\\data1.pack"
}*LPTPack_PackEntry;
```



![10](C:\Users\Xse\Desktop\image\10.png)



![11](C:\Users\Xse\Desktop\image\11.png)



![9](C:\Users\Xse\Desktop\image\9.png)

从这里开始就可以导出全部的文件名了。
通过TPack_ResIndex的pResNameEntry和dwResCount成员就可以得到文件名和文件数量信息

![14](C:\Users\Xse\Desktop\image\14.png) 

![23](C:\Users\Xse\Desktop\image\23.png)

### 读取资源

继续跟踪，观察到THashFilePack_Get这个函数，在调用之前，调用了Exists来查询文件是否存在，并且该函数只有两个参数非常简单。注意，这时候游戏已经初始化完毕，停在启动设置上，需要点开始游戏才会读取文件。

```C
TPack_Buffer *__usercall THashFilePack_Get(TPack_PackEntry *pPackEntry, void *lpFileName)
```

第一个就是之前 THashFilePack_Create  返回的TPack_PackEntry，第二个就是要读取的文件名，成功则返回一个TPack_Buffer对象指针，Hook这里就可以动态dump文件，当然我们的目标不止于此。

```C
typedef struct TPack_Buffer
{
	PDWORD            pTable;
	PDWORD            pBuffer;
	DWORD             dwSize;
}*LPTPack_Buffer;
```

![15](C:\Users\Xse\Desktop\image\15.png)

### 释放资源

现在这个函数的两个参数我们都有，那么是不是意味着我们可以使用该函数来提取全部文件？确实，参数齐全，但是还有一步很重要，就是该函数返回的对象如何析构，也就是在内存中申请空间要如何释放？如果不找到释放的地方，我们就算可以成功调用该函数来提取文件，也会出现内存溢出和内存无法回收的情况。

![16](C:\Users\Xse\Desktop\image\16.png)

先IDA里看看能不能捡漏，看着好像没有，那只能往上翻了，按一下X发现很多引用，还是回到x64dbg往下跟踪吧，实在不行还能下断点。不过运气比较好，第二次返回RET就看到了叫 TObject.Free

![18](C:\Users\Xse\Desktop\image\18.png)

发现就是调用pTable指向的一堆函数地址的表里的个函数，调用之后buffer和size都清空了

![19](C:\Users\Xse\Desktop\image\19.png)

ok至此释放的函数也找到了。



### 调用接口

这里我直接把THashFilePack_Get的其中一个参数的文件名改一下，然后用Garbro来解包看看返回的对象的buffer是不是对应文件名的那个文件。

![17](C:\Users\Xse\Desktop\image\17.png)

改了后发现直接读取失败了，难道有什么蹊跷？仔细观察发现它的字符串是Pascal格式的，即前面有一个长度标识把这个长度也写上就正常了。但是这样每次都要计算一下长度生成Pascal格式的字符串也不是很方便。

那就继续跟踪，进来发现是封装了TFilePack_Get和TFilePack_Get0这两个函数，TFilePack_Get的第一个参数是LPTPack_ResIndex，第二个是一个序号，对应为 pResNameEntry 里的顺序，比如我要读取第一个文件，传0就可以了，第三个参数固定1，返回值就是LPTPack_Buffer了。

TFilePack_Get0则是对TFilePack_Get的封装，里面其实是用另一个函数来查序号。

![20](C:\Users\Xse\Desktop\image\20.png)

![21](C:\Users\Xse\Desktop\image\21.png)

好了，现在我们只需要调用TFilePack_Get传文件名和序号就可以提取全部资源了。

![22](C:\Users\Xse\Desktop\image\22.png)

当然保存完成文件后别忘了释放。

![24](C:\Users\Xse\Desktop\image\24.png)

![25](C:\Users\Xse\Desktop\image\25.png)

### 结束

官中和日文版都是通用的，官方只是hook了日文版里的一些函数来实现自己的加密，所以文件应该也是差不多的。

由于新版本的QLIE是采用Unicode编码来处理文本的，那要把官中的文本搬到日文版就改两字节和复制文件的事。

反正最终分离出来，除去视频文件，就30多mb大的补丁。

![26](C:\Users\Xse\Desktop\image\26.png)
