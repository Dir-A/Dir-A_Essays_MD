# [CMVS引擎] 紫社引擎 折腾日记 Purple software

写于：2022-08-05

紫社引擎折腾日记

前些日子在B站录了一套Hook教程，当时选实战例子的时候有点头疼
最后选了 Lunaris Filia～キスと契約と深紅の瞳～ 这个游戏的汉化版
因为汉化补丁加了vmp 后面win8以后好像就gg了
这游戏原版还是加 AlphaRom 也就是说加壳的还是TMD
Yuris 这个引擎也比较难搞，这一套下来确实有实战那味道了。
而且也体现了这种情况真正需要Hook，而不是为了Hook而Hook
又或是自己瞎jb挂了一堆没用的dll。

不过讲一个怎么够，还是少了点啥。

某天在群里吹逼，听说马上 アマツツミ 要发英文了，还是无码的
这必须移植到日文啊。趁还没发我就先看看这个叫CMVS的引擎
之前是听说过，听说比较难？主要是因为这引擎就他们社自己用
也就没特别大的兴趣去看，（太难我也不会啊

不过我的目标也不是逆什么封包结构，图片解析，和VM，OP之类的
封包的话，早就被人干过了，就没必要了
不过封包程序还没看到有开源的or工具，VM，OP的话又不是重写引擎也没啥必要

主要目的还是看看，汉化或者替换无码必须的功能可不可以实现
第一，回封 or 不封包读取
第二，脚本回封，字符串长度限制
第三，引擎中文支持
第四，图片解析，回封

首先得看封包这个
解包自然不用担心，GarBro肯定行，毕竟这个社也不算冷门
问题就出在封包上了，我溜了一圈GarBro的issues，
看起来这个封包的回封还是有点难度，主要是复杂
当然对于我来说，毫无回封的想法，
能hook我还痛苦面具好几天写个回封，傻呗吧。

下了个日文版的游戏，先观察一下游戏的整体结构
一个启动exe，和一个子目录里拖了一堆封包和没封包的东西.ogg音频，还有视频
外面还带了一个启动配置位置，还要键位文件。
大概也就这些
然后抄起GarBro解包看看，能观察到游戏的图片主要是.pb3后缀
游戏的脚本主要是.ps3后缀,还有个.cmv后缀的视频GarBro无法解析。

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BCMVS%E5%BC%95%E6%93%8E%5D%20%E7%B4%AB%E7%A4%BE%E5%BC%95%E6%93%8E%20%E6%8A%98%E8%85%BE%E6%97%A5%E8%AE%B0%20Purple%20software/1.png)

值得注意的是，在data\pack\目录，一堆封包里，居然有个start.ps3文件
瞧这个文件名，嗯，就很关键。

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BCMVS%E5%BC%95%E6%93%8E%5D%20%E7%B4%AB%E7%A4%BE%E5%BC%95%E6%93%8E%20%E6%8A%98%E8%85%BE%E6%97%A5%E8%AE%B0%20Purple%20software/2.png)

这个时候经验和思想就很重要了，你自然可以拿起ida f5死命按
然后把封包整个结构逆出来，后封回去，又或者看GarBro的源码
痛苦面具好几天写一个出来。

显然我并不是什么大牛，所以我选择观察他没封包的情况
为什么有些文件他不封包，肯定有那么一个地方来处理这个情况
而且游戏开发的时候他总不可能每次测试都封回去吧,这得多费劲啊
我一直都认为，他能读封包，就一定能不封包读，只是你还不知道他怎么个逻辑来读取。
所以我们先对那些没封包的.ogg啊，这类的文件跟踪一下。

那要读取文件嘛，很简单，先x64dbg打开，搜一下导入了啥函数
file一打，嗯，createfile，断了再说。![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BCMVS%E5%BC%95%E6%93%8E%5D%20%E7%B4%AB%E7%A4%BE%E5%BC%95%E6%93%8E%20%E6%8A%98%E8%85%BE%E6%97%A5%E8%AE%B0%20Purple%20software/3.png)

除去一些配置文件，启动后第一个观察到的就是读取了
data\pack\start.ps3
显然这个文件很关键，为啥他不先读封包，而是直接读了这个没封包的脚本？
先跟到返回，随即就看到 LocalAlloc ReadFile FlushFileBuffers
相信写过Windows程序的都不陌生，即使你没写过，看名字也知道干嘛
返回的地方很快就CloseHandle后ret了，先跟这读取出来的buffer走一下

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BCMVS%E5%BC%95%E6%93%8E%5D%20%E7%B4%AB%E7%A4%BE%E5%BC%95%E6%93%8E%20%E6%8A%98%E8%85%BE%E6%97%A5%E8%AE%B0%20Purple%20software/4.png)

ret后就好玩了，来到个神奇的地方，上面一堆lstrlen lstrcat cmp 0x5C
然后ret后的位置刚好在一个jmp上，jmp跳过了一个虚函数的调用
然后下面还有个jb跳转，也就是往回跳。

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BCMVS%E5%BC%95%E6%93%8E%5D%20%E7%B4%AB%E7%A4%BE%E5%BC%95%E6%93%8E%20%E6%8A%98%E8%85%BE%E6%97%A5%E8%AE%B0%20Purple%20software/5.png)

没事别急，盯着buffer看先走一个，刚下来没几步就到了一个解密循环
飘了一眼，size，就是 start.ps3 文件的大小。
这玩意对ps3文件往下0x30个字节进行了一通xor解密。
先下个断点，执行到解密结束。

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BCMVS%E5%BC%95%E6%93%8E%5D%20%E7%B4%AB%E7%A4%BE%E5%BC%95%E6%93%8E%20%E6%8A%98%E8%85%BE%E6%97%A5%E8%AE%B0%20Purple%20software/6.png)

结束后隐约能看到一些封包的名字，而且整个数据块里也没什么00
推测是有可能压缩了。接着走一点，马上就 LocalAlloc 了
那可以确定这玩意肯定要进行一些数据的转移或者解压操作，不然他alloc个鬼
然后他Alloc到Free直接就调了一个call，这个call就不进去了，
先看看他处理完后buffer里的数据，同时观察一下，alloc的地址
显然buffer里的数据没变化，我们得看alloc的地方

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BCMVS%E5%BC%95%E6%93%8E%5D%20%E7%B4%AB%E7%A4%BE%E5%BC%95%E6%93%8E%20%E6%8A%98%E8%85%BE%E6%97%A5%E8%AE%B0%20Purple%20software/7.png)

alloc的地址处，直接复制进来了buffer里的前0x30个字节，
紧接着执行过那个call，alloc的位置出现了一堆 很多 00 的数据
观察大小，往下翻一下，能看到一堆完整的字符串

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BCMVS%E5%BC%95%E6%93%8E%5D%20%E7%B4%AB%E7%A4%BE%E5%BC%95%E6%93%8E%20%E6%8A%98%E8%85%BE%E6%97%A5%E8%AE%B0%20Purple%20software/8.png)

data\music\
data\video\
data\pack\script.cpz
data\pack\event.cpz
data\pack\ps.cpz
。。。。。。
欸，这不就咱的路径和封包啊。
重复以上操作，就可以观察到其它脚本经过上述解密后可以看到明文。

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BCMVS%E5%BC%95%E6%93%8E%5D%20%E7%B4%AB%E7%A4%BE%E5%BC%95%E6%93%8E%20%E6%8A%98%E8%85%BE%E6%97%A5%E8%AE%B0%20Purple%20software/9.png)

好了，可以总结 or 脑补一下上面观察的现象了
首先这个游戏会去读取 data\pack\start.ps3 这个文件
至于为什么是这个路径，我又仔细翻了一下游戏的配置文件 cmvs.cfg
这里面有个 SCRIPT_INIT_PATH=data\pack\ 定义了这个路径
然后通过CreateFile -> LocalAllo-> ReadFile -> FlushFileBuffers
一通操作后马上就ret回上一层，回去后马上一个jmp跳转到了解密.ps3脚本的地方
解密是先在这个文件0x30大小的文件头的第0xC个字节开始读取一个密钥
来生成xor的密钥，然后从0x30开始xor以后的数据，接着进入一个解压函数解压0x30以后的数据
具体不展开说了。分析到这，我们就有个概念了，也有想法可以让引擎读取没加密的明文.ps3文件

那就动手呗，首先，很直接的，先跳过解密循环（这正好一个je天赐良机），上面那个是密钥生成可以不理他
下面还有个解压，那个是通过判断文件头里的压缩后大小和压缩前大小判断的，反有个je直接改成jmp
好了，解压后循环解密都跳过了，那现在应该是可以直接读取解密的.ps3文件了，
dump出游戏解密好了ps3文件然后覆盖掉之前的，执行修改好后的exe，确实可以，正常执行

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BCMVS%E5%BC%95%E6%93%8E%5D%20%E7%B4%AB%E7%A4%BE%E5%BC%95%E6%93%8E%20%E6%8A%98%E8%85%BE%E6%97%A5%E8%AE%B0%20Purple%20software/10.png)

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BCMVS%E5%BC%95%E6%93%8E%5D%20%E7%B4%AB%E7%A4%BE%E5%BC%95%E6%93%8E%20%E6%8A%98%E8%85%BE%E6%97%A5%E8%AE%B0%20Purple%20software/11.png)

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BCMVS%E5%BC%95%E6%93%8E%5D%20%E7%B4%AB%E7%A4%BE%E5%BC%95%E6%93%8E%20%E6%8A%98%E8%85%BE%E6%97%A5%E8%AE%B0%20Purple%20software/12.png)

运行后就开始读取封包里的文件了，不过这个地方有个问题，就是封包里的文件读取出来，还是加密的
也得进 密码生成->xor->解压 才行，这样直接跳过是会gg的，当然了我们可以hook这个位置，
然后来控制是否进行解密，不过这个都是后话了，那有没有，不hook的方法呢？ 我们继续研究。

刚刚我们也看到了start.ps3文件解密后就能看到很多明文字符串，还是和封包名称有关
我们再来看一眼这些字符串

data\music\
data\video\
data\pack\script.cpz
data\pack\event.cpz
data\pack\ps.cpz
对比文件目录，我们可以看到
没封包的路径和封包的路径
这个时候我们可能回去断这些字符串的读取，然后进入vm痛苦循环。

显然，可以试试 or 乱想的事情，一律先不要先开始痛苦。
我们先插入一个东西，前面提到，GarBro可以解包了
那么我们就可以先通过GarBro解包一下script.cpz这个封包
解包后观察一下.ps3文件，发现他解包后的ps3文件就是解密的
直接就能看到明文。

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BCMVS%E5%BC%95%E6%93%8E%5D%20%E7%B4%AB%E7%A4%BE%E5%BC%95%E6%93%8E%20%E6%8A%98%E8%85%BE%E6%97%A5%E8%AE%B0%20Purple%20software/13.png)

好，这个时候就要有点前后联想的能力了
刚刚是不是说了，lstrlen lstrcat 0x5C，下面还有个jmp和jb
0x5C是什么，这不就是 \ 这个路径的字符吗？
lstrlen lstrcat 显然和路径字符串处理有关
再看看上面 data\music\ ，data\video\ 结尾有个 \ 吧
data\pack\script.cpz 没有 \
所以说有没可能，我们把 .cpz 改成 \ 就能直接读取script这个目录的内容了呢？
而且大家不要忘了，start.ps3 这个文件就是没封包读取的

当然这一切都是猜测，我们先在刚刚那个字符串处理的地方下个断点看看读取情况。
好了，一下断点跑两下就清晰了，我们可以观察到，如果传入的路径末尾有 \
就会走读取 start.ps3 文件读取的那个call出来一个jmp越过那个虚函数
如果传入的路径末尾不是 \ ，也就说是封包的路径，就会走那个虚函数调用
而且不管走哪一条，返回的eax值都指向ps3文件的 signature 也就是文件头的第一个成员。

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BCMVS%E5%BC%95%E6%93%8E%5D%20%E7%B4%AB%E7%A4%BE%E5%BC%95%E6%93%8E%20%E6%8A%98%E8%85%BE%E6%97%A5%E8%AE%B0%20Purple%20software/14.png)

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BCMVS%E5%BC%95%E6%93%8E%5D%20%E7%B4%AB%E7%A4%BE%E5%BC%95%E6%93%8E%20%E6%8A%98%E8%85%BE%E6%97%A5%E8%AE%B0%20Purple%20software/15.png)

至此我们可以知道，这玩意确实有一套走非封包读取文件的逻辑。
为了激活这个逻辑，我们传入的路径末尾必须是 \ 也就是一个文件夹
那么好了 data\pack\script.cpz -> data\pack\script\
这样一改，是不是就有了 \ 这个字符？
当然这一切都还只是猜测，反正试一试也不会怎么样，失败了继续调嘛

好，找到我们dump出来解密的那个start.ps3,按照上面把 script.cpz 改成 script\ 多余的字符填 00
接着用GarBro解包script.cpz的全部文件，然后新建一个文件夹 data\pack\script\ 放入解包的所有文件
运行我们修改过的exe，跑一下看看，哎发现居然进去了，没有任何问题！！！

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BCMVS%E5%BC%95%E6%93%8E%5D%20%E7%B4%AB%E7%A4%BE%E5%BC%95%E6%93%8E%20%E6%8A%98%E8%85%BE%E6%97%A5%E8%AE%B0%20Purple%20software/16.png)

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BCMVS%E5%BC%95%E6%93%8E%5D%20%E7%B4%AB%E7%A4%BE%E5%BC%95%E6%93%8E%20%E6%8A%98%E8%85%BE%E6%97%A5%E8%AE%B0%20Purple%20software/17.png)

至此我们就可以得到一个没有封包的 script 目录和一堆明文的 .ps3 文件。
既然 script.cpz可以这样改，那 event.cpz 自然也可以。

不过 event.cpz 里都是pb3文件，这是CMVS引擎自己特有的一种图片格式。
GarBro可以解析，但是如果你按照上面的方法，通过GarBro解包后，游戏是读取不了的。
通过跟踪图片文件的读取，可以发现，GarBro解包的图片文件，并不是pb3在封包里的原始格式
和ps3文件分析一样，在解压完后，可以看到pb3文件的文件头有很多的数据，并不是GarBro那样一堆00

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BCMVS%E5%BC%95%E6%93%8E%5D%20%E7%B4%AB%E7%A4%BE%E5%BC%95%E6%93%8E%20%E6%8A%98%E8%85%BE%E6%97%A5%E8%AE%B0%20Purple%20software/18.png)

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BCMVS%E5%BC%95%E6%93%8E%5D%20%E7%B4%AB%E7%A4%BE%E5%BC%95%E6%93%8E%20%E6%8A%98%E8%85%BE%E6%97%A5%E8%AE%B0%20Purple%20software/19.png)

继续分析可知，后面会对文件进行解密，通过一轮解密后就变成了GarBro提取的那样了。
为了解决这个问题，我们有几种方法

1、直接跳过解密循环
2、尝试dump原始的pb3文件（此时GarBro就不能读取了）
3、看GarBro源码，复原那一层加密。

此时，正好，我发现kf有人放流了，英文版的 アマツツミ 我就先下了这游戏
好家伙，这玩意好多文件也不封包，对start.ps3文件进行dump（因为GarBro是不能直接解密ps3文件的）
发现里面居然和我刚刚改的逻辑一样。往下瞄一下字符串 “1.03“ 1.03是什么鬼，先不理他。
那这样的话，如果英文版没跳加密的话，pb3图片应该是可以正常读取的
然后换了一下，果然成功，先冲一发助兴。

好了，冲完了，继续看看。
看看有没解出正常pb3图片的工具？嘛，实在没有我可以自己dump嘛，也不难。
GitHub搜一下，欸 Xmoe 有个CMVS的这个项目。
里面有个解包工具，对一下看看，哎，确实就是封包里解压后的原始数据。
换一下看看，正常读取也ok了。不过问题又来了，我要怎么修改图片啊？
只有pb3图片解析，没有回封，而且这解析的源码看着还有点复杂。

这个时候你固然可以，对着pb3解析的源码，一通分析，痛苦面具几天
写个一堆bug的回封出来。然而，我比较懒，找找别的方法吧。

对于图片的特殊格式，我们要用看待封包的眼观去看，你想想
开发的时候会用这么脑残的图片格式来开发？显然不会。
那会用什么？ png jpg bmp 无外乎就这几个
那么我们如何操作呢？依据我一两年的gal汉化经验
大多数时候，特殊的图片格式，只要解析成bmp 后缀改回去，同名替换就可以正常读取
显然，这也是猜测，到底能不能读bmp呢?到底能不能读png呢?
如果你开发过windows程序，或者，写过需要png解析的项目，显然你知道解析png
需要引入一个libpng的库，那么我只需看看有没有libpng这个库即可知道这引擎有没有读取png的能力
当然这其实也没必然联系。管他，先搜一下字符串，先试一下bmp
哦，果然有，但是只有一条结果，再搜一下，png，一堆libpng的信息，显然引用了libpng的库。

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BCMVS%E5%BC%95%E6%93%8E%5D%20%E7%B4%AB%E7%A4%BE%E5%BC%95%E6%93%8E%20%E6%8A%98%E8%85%BE%E6%97%A5%E8%AE%B0%20Purple%20software/20.png)

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BCMVS%E5%BC%95%E6%93%8E%5D%20%E7%B4%AB%E7%A4%BE%E5%BC%95%E6%93%8E%20%E6%8A%98%E8%85%BE%E6%97%A5%E8%AE%B0%20Purple%20software/21.png)

好了，一通操作，显然知道了，这个引擎确实有这个解析 png 和 bmp 的能力，但是
能不能直接粗暴替换成png和bmp呢？那很自然的，要看这玩意是依据后缀判断文件类型
还是依据文件头判断文件类型，又或者，根本不判断。
显然你可以试试嘛，反正试试又不要钱，有问题再说。
先用GarBro解包成bmp，然后后缀改成pb3丢回去看看。哦呦，果然能够正常读取
再试试解包成png，后缀改成pb3放回去，哦呦，又可以。
至此，脚本 读取 和 图片读取以及修改 的问题已经解决

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BCMVS%E5%BC%95%E6%93%8E%5D%20%E7%B4%AB%E7%A4%BE%E5%BC%95%E6%93%8E%20%E6%8A%98%E8%85%BE%E6%97%A5%E8%AE%B0%20Purple%20software/22.png)

那么现在还有三个问题，
第一，脚本的字符串修改和超长文本的实现
第二，引擎中文显示支持
第三，patch功能

前两点我就简单说了，脚本的字符串是cstring类型的，也就说，遇到00结束
你只需要在字符串块上面的一堆op里找到脚本的offset，然后指向文件末尾
追加你需要的字符串即可。记得改字符串块的大小。
至于引擎的中文支持，比较简单，CreateFont 的charset改一下
然后把9F的字符边界判断改一下就好了

前两点都很好解决，第三点才头痛的地方
现在这玩意要么走封包，要么走文件夹
也就说如果你改了event.cpz这个封包，这里面的文件你得全部解包出来
显然这很痛苦，而且如你要改一张图片，就要带上一整个封包

那么如何解决呢？前面是不是提到过一个数字 1.03
是的，英文版其实是基于官方更新1.03做的
那么官方更新有何特殊的地方呢？
我们先来看看更新文件，就两个 start.ps3 update.cpz
先把这个补丁打上，然后我们先dump这个start.ps3
发现里面果然有update.cpz的封包路径
可以按照之前的方法，改成免封包的，没有问题
不过要注意的是，如果你是用GarBro解包的
得全部pb3转换成bmp或png然后后缀换回pb3，忽略GarBro里的目录
这个玩意封包中的目录只是分类用的，实际读取上没什么作用，
如果你免封包带上这个目录，是读取不了的。
当然你也可以把pb3反向加密回去来进行读取。

这个时候就比较好玩了，如果你要替换图片或文件，直接丢到这个update目录就ok了
游戏每次读取文件都会去检索这个目录，这个逻辑应该是start.ps3里那一堆op实现的
不过这个op暂时没人逆出来，分析也麻烦，所以这样的方法，只能用在游戏有更新补丁
且是update.cpz这种的，如果游戏没有更新补丁呢？那又要回到全部解包的情况了
不过庆幸的是，这个社绝大部分游戏都发过更新补丁。
当然了，还是有些没更新补丁的，那这要怎么解决呢？

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BCMVS%E5%BC%95%E6%93%8E%5D%20%E7%B4%AB%E7%A4%BE%E5%BC%95%E6%93%8E%20%E6%8A%98%E8%85%BE%E6%97%A5%E8%AE%B0%20Purple%20software/23.png)

还记得之前说的 lstrlen lstrcat cmp 0x5C 的那个地方吗
如果你仔细分析，这个地方，其实就是封包读取和不封包读取的控制部分
如果走虚函数那个call就是封包读取，如果走那个下面紧接着一个jmp的call
就是文件夹读取数据。在这个地方hook，只需要很简单的逻辑就可以实现
即使没有update.cpz这样的更新也可以灵活控制读取数据的方式。
之前发的那个 アマツツミ 无码补丁 就是用的这个逻辑。
项目已经开源 https://github.com/Dir-A/CmvsFileHook

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BCMVS%E5%BC%95%E6%93%8E%5D%20%E7%B4%AB%E7%A4%BE%E5%BC%95%E6%93%8E%20%E6%8A%98%E8%85%BE%E6%97%A5%E8%AE%B0%20Purple%20software/24.png)

至于那个叫.cmv的文件，其实内部就是一堆压缩的jbp图片，注意不是jpg
jbp这个图片其实pb3里面也有，想要解析可以去看pb3图片的解析，github都有源码。
crass也有.cmv这个解包的插件，但是注意github那个cui插件是不完整的，解析不了
你得下Zeas写的才能解析。

至此其实，一般汉化这种已经完全ok了。
后面我又分析了几天，准备搞一个动态提取全部文件
分析封包Index解析的地方和一堆关键读取数据的位置
分析是分析完了，暂时懒得写出来。

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BCMVS%E5%BC%95%E6%93%8E%5D%20%E7%B4%AB%E7%A4%BE%E5%BC%95%E6%93%8E%20%E6%8A%98%E8%85%BE%E6%97%A5%E8%AE%B0%20Purple%20software/25.png)

 

下面分享一些彩蛋：

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BCMVS%E5%BC%95%E6%93%8E%5D%20%E7%B4%AB%E7%A4%BE%E5%BC%95%E6%93%8E%20%E6%8A%98%E8%85%BE%E6%97%A5%E8%AE%B0%20Purple%20software/26.png)
