# Eushully (AGE) 过注册检查

Eushully 的AGE引擎(ARCGameEngine)有一种类似于CatSystem2的本机安装标识文件，也就防止你直接拷贝游戏到别的机器上运行，区别在于CatSystem2的检查是直接不让你运行，而AGE则是运行后随机弹窗来恶心你

## 目标游戏：魔導巧殻
 调用检查函数

![1](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BAGE%E5%BC%95%E6%93%8E%5DEushully%20%E8%BF%87%E6%B3%A8%E5%86%8C%E6%A3%80%E6%9F%A5/1.png)

 00486E30 CheckIsReg 

![2](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BAGE%E5%BC%95%E6%93%8E%5DEushully%20%E8%BF%87%E6%B3%A8%E5%86%8C%E6%A3%80%E6%9F%A5/2.png)

返回1表示注册，返回0表示未注册，返回值随后会写入都内部的变量（set:IsReggist）里存储

CheckIsReg 内部通过读取 SYS4RK.BIN 或 SYS3RK.BIN 文件来计算注册信息
SYS4RK.BIN文件是唯一一个游戏目录下安装在不同机器上会修改的文件
该文件相当于标识机器的作用，如果你拷贝游戏到别的机器上，
这个文件的校验值计算后将会不能通过CheckIsReg
也就是说，会返回0，从而让游戏识别到，
当前机器使用的版本为未注册版本，也就是拷贝的盗版
但是此时游戏引擎并不会有任何提示。

当游戏进入战斗后，会在消息循环函数里先读取set:IsReggist的值，并且在循环过程中不断验证

00411FC0 消息循环函数

0041207A 读取set:IsReggist的值

![3](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/image/%5BAGE%E5%BC%95%E6%93%8E%5DEushully%20%E8%BF%87%E6%B3%A8%E5%86%8C%E6%A3%80%E6%9F%A5/3.png)

读取后进行消息循环，并验证set:IsReggist的值，如果该值为未注册的，则会进入随机显示错误消息弹窗。

## 过检测

很简单，我们只需要搜索字符串 set:IsReggist 并下断点，第一个读取这个字符串的函数是保存验证flag的
也就是这个保存验证flag函数的倒数第二个参数，倒数第一个参数就是验证flag
我们只需要向上找到给这个flag赋值的函数，就是CheckIsReg了，然后把CheckIsReg的值改为1即可。

或者直接搜索 SYS4RK.BIN 字符串，定位到 CheckIsReg 里面。
