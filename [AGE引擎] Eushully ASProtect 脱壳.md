# [AGE引擎] Eushully ASProtect 脱壳

AGE引擎的ASProtect主要在2013年左右的版本上，2017年的天結いキャッスルマイスター已经采用了PlayDrm。ASProtect主要加在AGE.EXE和AGERC.DLL上，前者为游戏主程序，后者为游戏的资源DLL（标题菜单，模态窗口），AGERC.DLL容易被加壳软件识别为ASPack，其实内部的IAT是加密的。网上大多数讲解ASProtect脱壳的教程都是老的1.xx版本，其IAT没有什么特殊的处理，AGE的加的是2.xx的版本，FF15的Call被特殊处理过，其它代码段未见特殊处理，未见OEP虚拟化或被偷。

ASProtect由于比较古老，很多人研究，存在大量的脚本，接下来我们利用脚本对AGE的主程序和资源DLL进行脱壳即可。

## 准备工作

### 目标游戏

魔導巧殻 (1.03更新补丁，AP，都无所谓，不改变原本的EXE和DLL)

### 环境

Windows XP Pro SP3

Ollydbg （这里选用吾爱破解的版本，其它版本可能需要自行添加 ODbgScript ，OllyDump插件）

Scylla x86 v0.9.8

ASProtect unpaker script by VolX 1.15E（1.15F也可以，注意Asprvm8s.bin这个文件必须要有）

比较懒的可以直接下，吾爱破解工具包，除了脚本都有。

## 修改脚本

由于脚本需要用到 Asprvm8s.bin 文件，里面的路径是固定的，要么把文件放到脚本默认的路径中，要么自行修改路径，总共两处，在Aspr2.XX_unpacker_v1.15E.osc里搜索 Asprvm8s.bin 字段就可以找到

![1](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BAGE%E5%BC%95%E6%93%8E%5D%20Eushully%20ASProtect%20%E8%84%B1%E5%A3%B3/1.png)

![2](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BAGE%E5%BC%95%E6%93%8E%5D%20Eushully%20ASProtect%20%E8%84%B1%E5%A3%B3/2.png)

此处我修改到C盘根目录，同时把 Asprvm8s.bin 放到C盘根目录下。

![3](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BAGE%E5%BC%95%E6%93%8E%5D%20Eushully%20ASProtect%20%E8%84%B1%E5%A3%B3/3.png)

## 开脱

脚本文件和游戏EXE和DLL拽入虚拟机中

![4](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BAGE%E5%BC%95%E6%93%8E%5D%20Eushully%20ASProtect%20%E8%84%B1%E5%A3%B3/4.png)

打开Ollydbg把EXE拽进去，提示代码压缩加密，点否即可。

![5](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BAGE%E5%BC%95%E6%93%8E%5D%20Eushully%20ASProtect%20%E8%84%B1%E5%A3%B3/5.png)

如果没停在同样的窗口，点一下菜单栏边上的C按钮即可

![6](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BAGE%E5%BC%95%E6%93%8E%5D%20Eushully%20ASProtect%20%E8%84%B1%E5%A3%B3/6.png)

在菜单栏，选择插件，选择ODbgScript，选择运行脚本，打开

![7](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BAGE%E5%BC%95%E6%93%8E%5D%20Eushully%20ASProtect%20%E8%84%B1%E5%A3%B3/7.png)

![8](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BAGE%E5%BC%95%E6%93%8E%5D%20Eushully%20ASProtect%20%E8%84%B1%E5%A3%B3/8.png)

打开后会自动运行，请确认脚本中的路径已经修改，或 Asprvm8s.bin 文件已经放置在对应的路径下，否则在运行脚本的过程中，会直接卡住。

如果脚本没有运行，可以点窗口界面内的缩小窗口，在刚刚的ODbgScript插件里找到脚本运行窗口，在脚本窗口按下空格，或右键--继续来运行脚本

![9](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BAGE%E5%BC%95%E6%93%8E%5D%20Eushully%20ASProtect%20%E8%84%B1%E5%A3%B3/9.png)

![10](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BAGE%E5%BC%95%E6%93%8E%5D%20Eushully%20ASProtect%20%E8%84%B1%E5%A3%B3/10.png)

运行结束后，如图所示，提示没有被偷代码，和让我们检查log窗口的IAT信息，点确认即可。

![11](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BAGE%E5%BC%95%E6%93%8E%5D%20Eushully%20ASProtect%20%E8%84%B1%E5%A3%B3/11.png)

按下菜单栏下边的的 l 按钮，来打开log窗口，查看OEP和IAT的相关详细，如果没有则脚本无效，或者有别的问题

![13](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BAGE%E5%BC%95%E6%93%8E%5D%20Eushully%20ASProtect%20%E8%84%B1%E5%A3%B3/13.png)

确认存在OEP和IAT信息，然后，选择菜单栏，插件--->OllyDump--->脱壳在当前调试的进程

![12](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BAGE%E5%BC%95%E6%93%8E%5D%20Eushully%20ASProtect%20%E8%84%B1%E5%A3%B3/12.png)

在OllyDump窗口，确认OEP的地址和OEP的RVA，然后点击获取EIP作为OEP，再次确认地址，和log对应，则点击脱壳，然后输入新的文件名保存即可。在保存之前可以看到，多出来个de_AGE.exe的文件，不必理会这个文件，稍后可以自行删除。保存途径可能会卡住几秒，等待即可。

![15](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BAGE%E5%BC%95%E6%93%8E%5D%20Eushully%20ASProtect%20%E8%84%B1%E5%A3%B3/15.png)

把脱壳后的exe复制回物理机，运行查看效果。

![16](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BAGE%E5%BC%95%E6%93%8E%5D%20Eushully%20ASProtect%20%E8%84%B1%E5%A3%B3/16.png)

如果出现在物理机无法运行，虚拟机正常运行的情况，请尝试以下步骤。

重新操作到脚本运行完成和打开log窗口查看OEP和IAT信息的步骤

打开Scylla x86，选择当前需要脱壳的进程，并再次确认OEP和RVA信息

![17](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BAGE%E5%BC%95%E6%93%8E%5D%20Eushully%20ASProtect%20%E8%84%B1%E5%A3%B3/17.png)

填入OEP地址，IAT地址，IAT大小，然后点击获取输入表，可以看到下图输入表全部绿色勾，表示正常获取。

![18](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BAGE%E5%BC%95%E6%93%8E%5D%20Eushully%20ASProtect%20%E8%84%B1%E5%A3%B3/18.png)

此时点击 显示无效函数和显示可以函数，如果只有一个IsDialogMessageA为可疑，则无需理会，如果有大量可疑和无效，则脚本不成功。

![19](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BAGE%E5%BC%95%E6%93%8E%5D%20Eushully%20ASProtect%20%E8%84%B1%E5%A3%B3/19.png)

然后点击转储到文件，默认命名不必修改，直接保存即可

![20](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BAGE%E5%BC%95%E6%93%8E%5D%20Eushully%20ASProtect%20%E8%84%B1%E5%A3%B3/20.png)

接着点击修复转储后的文件，找到刚刚转储的文件打开即可

![21](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BAGE%E5%BC%95%E6%93%8E%5D%20Eushully%20ASProtect%20%E8%84%B1%E5%A3%B3/21.png)

此时转储文件同目录下会多出一个后缀为_SCY.exe的新文件，复制到物理机运行查看效果

![22](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BAGE%E5%BC%95%E6%93%8E%5D%20Eushully%20ASProtect%20%E8%84%B1%E5%A3%B3/22.png)

至此，基本Eushully ASProtect 的主程序都可以正常脱壳，如不行，就换Import REConstructor继续尝试。

AGERC.DLL的脱壳和上面一样，区别在于，dll是无法直接运行，所以dll在od里其实是挂在一个loader上的，如果用Scylla x86 脱壳，需要选择 loaddl.exe程序，然后选取dll，然后 修复转储后的文件 应该选择de_AGERC.DLL，而不是直接修复Scylla的转储，之后操作一样。

![23](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BAGE%E5%BC%95%E6%93%8E%5D%20Eushully%20ASProtect%20%E8%84%B1%E5%A3%B3/23.png)

