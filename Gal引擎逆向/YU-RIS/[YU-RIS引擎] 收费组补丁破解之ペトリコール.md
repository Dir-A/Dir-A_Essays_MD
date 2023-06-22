# [YU-RIS引擎\] 收费组补丁破解之**ペトリコール -Petrichor-**

以前发的贴，转到Github保存一下，略有改动。2023年5月30日

从床上爬起来看了看手机，伦敦时间5点44，差不多快6点了，自律的人应该起床了。
起来热了下饭菜，打开YouTube看看台媒下饭。
边吃边看，然后在群里吹逼了几句，到七点半了。

然后打开了昨天下的里番开始导管
混账，自律的人怎么能天天导管呢？最近导的平凡了，一天十几次，所谓暴食斯英语。
自律的我必须找点事情干（今天）！

想起KF好像有人发了个汉化，还没看有没加密
哎，反正也没事干，看截图大概是没加密，那先下一个看看吧。
下来一看，好像是没加密，就是exe大小有点不对劲，然后看着是打的原封包
那Garbro解包看看，发现Garbro解包不了，这个情况想起来之前还有个屋子的补丁没破。
![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BYU-RIS%E5%BC%95%E6%93%8E%5D%20%E6%94%B6%E8%B4%B9%E7%BB%84%E8%A1%A5%E4%B8%81%E7%A0%B4%E8%A7%A3%E4%B9%8B%E3%83%9A%E3%83%88%E3%83%AA%E3%82%B3%E3%83%BC%E3%83%AB/1.png)
（图裂了？因为是Github的链接，所以你得开代理小老弟:)）

屋子的补丁是一个叫 ペトリコール -Petrichor- 的游戏的，也是yu-ris引擎
我之前也破过一个屋子的同样的yuris引擎的游戏
不过那个就一个难点猜密钥，而且文件也没封包，直接就是解包后的文件，猜出密钥就秒杀了
这次有点不一样，封包了，而且还解包不了。当然最主要的情况是因为我不能运行这个游戏
如果可以运行那就几秒的事，因为屋子已经挂了，跑不起来就只能摁住猜了。
![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BYU-RIS%E5%BC%95%E6%93%8E%5D%20%E6%94%B6%E8%B4%B9%E7%BB%84%E8%A1%A5%E4%B8%81%E7%A0%B4%E8%A7%A3%E4%B9%8B%E3%83%9A%E3%83%88%E3%83%AA%E3%82%B3%E3%83%BC%E3%83%AB/2.png)

那个屋子补丁也是Garbro解不了，然后对着YPF的文件格式自己人肉解析了一下，发现原始数据开头是5A
对于zlib压缩的文件而言，那必不可能是5A，一般是7x多少，当然我也是不什么密码学专家
他这样一改，我有点玉玉了，你改这还能不能好好玩了，然后一直也没猜出是个什么加密方法。
如果他整体xor一层，我感觉这辈子都猜不出来了，何况可能还换了压缩的方法，而且yu-ris的脚本本身就有一层xor。
屋子是必然改了，因为汉化不可能全部文件都修改，但是xor密钥一改，全部文件都不一样了，想通过日文版的来对比
基本没戏。但是我发现其中一个yscfg文件把5A改成78后是可以正常被zlib解压的。
反正后面就研究到这就放硬盘里了，没啥思路，想着逆向不就这样，大起大落罢了，不如继续导管。
![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BYU-RIS%E5%BC%95%E6%93%8E%5D%20%E6%94%B6%E8%B4%B9%E7%BB%84%E8%A1%A5%E4%B8%81%E7%A0%B4%E8%A7%A3%E4%B9%8B%E3%83%9A%E3%83%88%E3%83%AA%E3%82%B3%E3%83%BC%E3%83%AB/3.png)

当然上面说的是之前的事情，看到这个情况脑子里一闪而过，现在我们继续看KF这个新发的汉化补丁。
既然Garbro解不了，还是报zlib的问题，说明是死在了解压上，那就看看汉化的exe吧。
看了下发现是upx，这个简单，学过脱壳的第一课应该就是脱这个，脱壳后发现exe有点小，怎么才2mb左右
比日文版的小了，1mb左右，嗨别理他，再查看一下有没别的壳，看样子是没有。

上手x64dbg调试一下，由于最近在更新YU-RIS引擎的相关教程，这个还是得心应手的，随便翻一下找到解压的地方看看。
oh，一看到这个解压函数被套了一层，到这就水落实出了，再看看它的原始数据，oh，这个更是惊喜不断
看着这个数据的头几个字节和屋子那个简直是一模一样。我赶紧在一堆导管资料里翻出了屋子那个补丁。

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BYU-RIS%E5%BC%95%E6%93%8E%5D%20%E6%94%B6%E8%B4%B9%E7%BB%84%E8%A1%A5%E4%B8%81%E7%A0%B4%E8%A7%A3%E4%B9%8B%E3%83%9A%E3%83%88%E3%83%AA%E3%82%B3%E3%83%BC%E3%83%AB/4.png)

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BYU-RIS%E5%BC%95%E6%93%8E%5D%20%E6%94%B6%E8%B4%B9%E7%BB%84%E8%A1%A5%E4%B8%81%E7%A0%B4%E8%A7%A3%E4%B9%8B%E3%83%9A%E3%83%88%E3%83%AA%E3%82%B3%E3%83%BC%E3%83%AB/5.png)
经过对比我发现该新汉化补丁，实际上还是zlib压缩，只是头3个字节xor了一个数，然后偏移4个字节xor了文件的大小（压缩大小）。
当然到这个只是解决了这个补丁搞的东西，屋子那个还没看。

我想着如果是一样的话，屋子那个封包复制过来加载一下看看？跟踪了一下，结果在这个程序里的加密方法运算后确实头一个字节变成了0x7x
感觉有点希望，后面调用解压函数的时候倒是出错了，看来还是有点问题啊，你也很难保证这两是一样的吧，没准人家就喜欢5A开头呢？

经过反复对比我发现，只要把这个新汉化补丁里的对头3个字节xor的密钥 0x0040FB22 改成 0x0040BF22 就可以成功被zlib解压了。

赶紧起手写个程序把屋子那个文件处理一下，最近又降温，写的手有点哆嗦，也有可能是导多了。
![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BYU-RIS%E5%BC%95%E6%93%8E%5D%20%E6%94%B6%E8%B4%B9%E7%BB%84%E8%A1%A5%E4%B8%81%E7%A0%B4%E8%A7%A3%E4%B9%8B%E3%83%9A%E3%83%88%E3%83%AA%E3%82%B3%E3%83%BC%E3%83%AB/6.png)
（你看导多了吧，不delete也不close）

处理完成后对原始数据用zlib解压，轻松得到YSTB文件，接着用之前为了讲YU-RIS教程写的猜密码和解密工具处理一下

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BYU-RIS%E5%BC%95%E6%93%8E%5D%20%E6%94%B6%E8%B4%B9%E7%BB%84%E8%A1%A5%E4%B8%81%E7%A0%B4%E8%A7%A3%E4%B9%8B%E3%83%9A%E3%83%88%E3%83%AA%E3%82%B3%E3%83%BC%E3%83%AB/7.png)
winhex看看效果

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BYU-RIS%E5%BC%95%E6%93%8E%5D%20%E6%94%B6%E8%B4%B9%E7%BB%84%E8%A1%A5%E4%B8%81%E7%A0%B4%E8%A7%A3%E4%B9%8B%E3%83%9A%E3%83%88%E3%83%AA%E3%82%B3%E3%83%BC%E3%83%AB/8.png)
ok轻松秒杀。

接着对日文版处理一下，把这个汉化文件搞上去，又是一个好补丁（焊化+1）

![img](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BYU-RIS%E5%BC%95%E6%93%8E%5D%20%E6%94%B6%E8%B4%B9%E7%BB%84%E8%A1%A5%E4%B8%81%E7%A0%B4%E8%A7%A3%E4%B9%8B%E3%83%9A%E3%83%88%E3%83%AA%E3%82%B3%E3%83%BC%E3%83%AB/9.png)

最后总结一波

```C
#赤红幻境汉化组 -> CH
#日文原版 -> JP
#胖次汉化组（屋子） -> WZ
#原始数据（即未解压未解密数据） -> RAW

#日文版密钥 -> 0xC41386F5
#屋子版密钥 -> 0x75F74413

----- First Four Bytes Of Raw Data -----
JP_RAW = 0x78DAXXXX
WZ_RAW = 0x5A65XXXX
CH_RAW = 0x5A21XXXX

----- Encryption Algorithms CH -----
CH_RAW + 0 xor DWORD 0x0040FB22

CH_RAW[0] xor 0x22 = 0x78
CH_RAW[1] xor 0xFB = 0xDA
CH_RAW[2] xor 0x40 = ?
CH_RAW[3] xor 0x00 = CN_RAW[3]

CH_RAW + 4 xor DWORD szFile

----- Encryption Algorithms WZ -----
WZ_RAW + 0 xor DWORD 0x0040BF22
仅修改第二个字节为BF使得即使结果为0xDA

WZ_RAW[0] xor 0x22 = 0x78
WZ_RAW[1] xor 0xBF = 0xDA
WZ_RAW[2] xor 0x40 = ?
WZ_RAW[3] xor 0x00 = WZ_RAW[3]

WZ_RAW + 4 xor DWORD szFile
```

