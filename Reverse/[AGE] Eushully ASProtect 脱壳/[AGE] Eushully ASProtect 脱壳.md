# [AGE引擎] Eushully ASProtect 脱壳

AGE引擎的`ASProtect`主要在2013年左右的版本上，2017年的`天結いキャッスルマイスター`已经采用了`PlayDRM`。  
`ASProtect`主要加在 `AGE.EXE` 和 `AGERC.DLL` 上，前者为游戏主程序，后者为游戏的资源DLL（标题菜单，模态窗口），  
`AGERC.DLL`容易被加壳软件识别为`ASPack`，其实内部的`IAT`是加密的。  
网上大多数讲解`ASProtect`脱壳的教程都是老的`1.xx`版本，其`IAT`没有什么特殊的处理， 

`AGE`的加的是`2.xx`的版本，`FF15`的`Call`被VM了，其它代码段未见特殊处理，未见`OEP`虚拟化或被偷。  

`ASProtect`由于比较古老，很多人研究，存在大量的脚本，接下来我们利用脚本对AGE的主程序和资源DLL进行脱壳即可。

## 准备工作

### 目标游戏

`魔導巧殻` (1.03更新补丁，AP，都无所谓，不改变原本的EXE和DLL)

### 环境

`Windows XP Pro SP3`

`Ollydbg` （这里选用吾爱破解的版本，其它版本可能需要自行添加 `ODbgScript` ，`OllyDump`插件）

`Scylla x86 v0.9.8`

`ASProtect unpaker script by VolX 1.15E`（1.15F也可以，注意`Asprvm8s.bin`这个文件必须要有）

比较懒的可以直接下，吾爱破解工具包，除了脚本都有。

脚本和脱壳/未脱壳的文件在[此处](https://github.com/Dir-A/Dir-A_Essays_MD/tree/main/image/%5BAGE%E5%BC%95%E6%93%8E%5D%20Eushully%20ASProtect%20%E8%84%B1%E5%A3%B3/%E9%99%84%E4%BB%B6)

## 修改脚本

由于脚本需要用到 `Asprvm8s.bin` 文件，里面的路径是固定的，要么把文件放到脚本默认的路径中，要么自行修改路径，总共两处，在`Aspr2.XX_unpacker_v1.15E.osc`里搜索 `Asprvm8s.bin` 字段就可以找到

![1](image/1.png)

![2](image/2.png)

此处我修改到C盘根目录，同时把 `Asprvm8s.bin` 放到C盘根目录下。

![3](image/3.png)

## 开脱

`脚本文件`和游戏`EXE`和`DLL`拽入虚拟机中

![4](image/4.png)

打开`Ollydbg`把`EXE`拽进去，提示代码压缩加密，点否即可。

![5](image/5.png)

如果没停在同样的窗口，点一下菜单栏边上的C按钮即可

![6](image/6.png)

在菜单栏，选择插件，选择`ODbgScript`，选择运行脚本，打开

![7](image/7.png)

![8](image/8.png)

打开后会自动运行，请确认脚本中的路径已经修改，或 `Asprvm8s.bin` 文件已经放置在对应的路径下，否则在运行脚本的过程中，会直接卡住。

如果脚本没有运行，可以点窗口界面内的缩小窗口，在刚刚的`ODbgScript`插件里找到脚本运行窗口，在脚本窗口按下空格，或右键--继续来运行脚本

![9](image/9.png)

![10](image/10.png)

运行结束后，如图所示，提示没有被偷代码，和让我们检查`Log窗口`的`IAT信息`，点确认即可。

![11](image/11.png)

按下菜单栏下边的的` l 按钮`，来打开`Log窗口`，查看`OEP`和`IAT`的相关详细，如果没有则脚本无效，或者有别的问题

![13](image/13.png)

确认存在`OEP`和`IAT`信息，然后，选择菜单栏，`插件`--->`OllyDump`--->`脱壳在当前调试的进程`

![12](image/12.png)

在`OllyDump`窗口，确认`OEP的地址`和`OEP的RVA`，然后点击`获取EIP作为OEP`，再次确认地址，和`Log窗口`的信息对应，则点击`脱壳`，然后输入新的文件名保存即可。在保存之前可以看到，多出来个`de_AGE.exe`的文件，不必理会这个文件，稍后可以自行删除。保存途径可能会卡住几秒，等待即可。
![14](image/14.png)
![15](image/15.png)

把脱壳后的`EXE`复制回物理机，运行查看效果。

![16](image/16.png)

如果出现在物理机无法运行，虚拟机正常运行的情况，请尝试以下步骤。

重新操作到脚本运行完成和打开`Log窗口`查看`OEP和IAT信息`的步骤

打开`Scylla x86`，选择当前需要脱壳的进程，并再次确认`OEP和RVA信息`

![17](image/17.png)

填入`OEP地址`，`IAT地址`，`IAT大小`，然后点击获取输入表，可以看到下图输入表全部绿色勾，表示正常获取。

![18](image/18.png)

此时点击 `显示无效函数`和`显示可疑函数`，如果只有一个`IsDialogMessageA`为可疑，则无需理会，如果有大量可疑和无效，则脚本不成功。

![19](image/19.png)

然后点击`转储到文件`，默认命名不必修改，直接保存即可

![20](image/20.png)

接着点击`修复转储后的文件`，找到刚刚转储的文件打开即可

![21](image/21.png)

此时转储文件同目录下会多出一个后缀为`_SCY.exe`的新文件，复制到物理机运行查看效果

![22](image/22.png)

至此，基本Eushully ASProtect 的主程序都可以正常脱壳，如不行，就换`Import REConstructor`继续尝试。

`AGERC.DLL`的脱壳和上面一样，区别在于，DLL是无法直接运行，所以DLL在OD里其实是挂在一个`loader`上的，如果用`Scylla x86`脱壳，需要选择`loaddl.exe`程序，然后`选取DLL`，然后 `修复转储后的文件` 应该选择`de_AGERC.DLL`，而不是直接修复Scylla的转储，之后操作一样。

![23](image/23.png)

