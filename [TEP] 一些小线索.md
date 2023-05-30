# The Enigma Protector

## 0、Dump壳Dll
可以从.data段dump出来
然后载入IDA，可以看到部分壳的函数名

## 1、一些特征码：
编译器入口点特征码
```ASM
33C06A003944240868001000000F94C050FF15????????85C0 [MicroSoft Linker 6.0]
```

可以用于定位程序的入口点，其它的自己找
壳用的Turo Linke，可以直接dump出壳dll然后用ida找到入口点。

绘制验证窗口
```ASM
558BEC83C4E0535633D2 (壳代码解密后可以搜到)
```


可以定位到绘制验证窗口的地方

DELPHI 按钮事件：

```ASM
740E8BD38B83????????FF93???????? (壳代码解密后可以搜到)
```

搜索到的结果对多个call dword pter这个下断点。
可以定位到具体按钮事件，也就是按下按钮执行的代码

## 2、直接过注册(未勾选常量加密)：

硬件断点 `kernelbase.RegCreateKeyExInternalA`

断下后能看到
```ASM
00FEF940  035B2DEC    "Software\\Enigma Protector\\29AEB4A0365755F6-B862CAE984EA4D0E"
```

这样的东西
栈窗口往下翻阅
能看到压入了大量的`00000000`
大块的`00000000`中间还能看到输入的注册信息
大约esp偏移+300以上，也就是大块压入的`00000000`往下找到第一处压入的返回地址
一般在`SEH_Record`上面，前后各有夹杂了输入的注册信息字符串

再次往下翻页还能看到格式化字符串的信息

```ASM
00FEFED8  0357013C    "Software\\Enigma Protector\\%.8x%.8x-%.8x%.8x"
00FEFEDC  01040C2C    L"Software\\Enigma Protector\\%.8x%.8x-%.8x%.8x"
00FEFEE0  01040ED4    L"Software\\Enigma Protector\\29AEB4A0365755F6-B862CAE984EA4D0E"
00FEFF08  00000000    
00FEFF0C  0103FD6C    L"c:\\users\\[user]\\desktop\\123.exe"
00FEFF10  03570034    "kernel32.dll"
00FEFF14  0357000C    "CheckRemoteDebuggerPresent"
00FEFF18  00000000    
```

转到这个上面提到的返回地址可以看到一个push和jmp
对push下断点
停住后eax=0，改成1即可直接bypass