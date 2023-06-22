# [YU-RIS] 免封包处理

## 0x00 背景

YURIS引擎如果大家之前有看我的文章，已经叫大家自行去官方看看了，在官方你可以下到一个demo，名称为`yu-ris_0486_007.lzh`的文件，当然也有可能是别的版本，反正你解压后是在一个文件夹里能看到`yu-ris.exe`观察结构，发现这东西没有封包也是可以运行的，而且还能看到demo里的相关素材在游戏目录下的文件夹里。

由此我们可以知道，这个引擎其实并不用封包就可以读取文件，当然其实大部分引擎都是支持的，只不过有些打包成游戏后把这种功能屏蔽了。

本篇文章的目的就是指导大家把`yuris`的免封包给搞出来，以方便对里面的图片脚本进行替换。

## 0x01 版本差异

对于`yuris`来说，老版本的基本上都是可以直接读取目录下的文件的，但是得保持目录结构。

对于新版本的来说，会对文件读取进行一些限制，也就是不会直接读取目录下的文件

新旧版本划分之前提过，不再赘述。

## 0x02 不同情况

当然，你可以别管是新版本还是老版本，其实`yuris`乱改也不是一天两天了，如果以后你看到个零几年的游戏都不能直接读取目录下的文件，也没什么惊讶的。

通常`yuris`能不能免封包读取不是固定的，也就是说，它甚至可以控制到具体某一个文件是否免封包读取，常见的情况是，你解包A封包放到目录下可以读取，但是B封包却不行，这是完全正常的。

当然要是你遇到一个情况，只有某个文件是不能免封包读取也是完全正常的

所以我们也别管是新版本还是老版本，是能读取还是不能读取，先把文件放到游戏目录下再说，如果不能读取就说明需要改东西了。

## 0x03 文件优先级

之前我们已经提到过`yscfg.ybn`这个文件，我们说过它里面有很多有趣的东西，它位于`ysbin.ypf`或`bn.ypf`封包里。这个文件里面有个参数是控制文件读取优先级的，详见这个链接[Note.txt](https://github.com/arcusmaximus/VNTranslationTools/blob/main/VNTextPatch.Shared/Scripts/Yuris/Notes.txt#L20)

```c++
yscfg.ybn (Project configuration)
=====================================
int magic = 'YSCF'
int version
int padding = 0
int compile
int screenWidth
int screenHeight
int enable
byte imageTypeSlots[8]
byte soundTypeSlots[4]
int thread
int debugMode
int sound
int windowResize
int windowFrame
int filePriorityDev
int filePriorityDebug
int filePriorityRelease
int padding = 0
short captionLength
byte caption[captionLength]
```

`filePriorityRelease filePriorityDebug filePriorityDev`这三个就是控制文件读取优先级的，这三个分别代表游戏在不同模式的下的文件优先级，一般大家下载的游戏都是Release的。

下面是某个游戏的`yscfg.ybn`文件

![图1](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/.img/%5BYURIS%E5%BC%95%E6%93%8E%5D%20%E5%85%8D%E5%B0%81%E5%8C%85%E5%A4%84%E7%90%86/%E5%9B%BE1.png)

图中红色方框圈起来的就是这三个参数，可以看到他们都是01开头的，其实最后一个红色方框的01是我改的原来是0，也就是`filePriorityRelease`是0，我们的目的正是在把`filePriorityRelease`变为1也就是和图上一样，这样就可以让游戏优先读取目录下的文件了。

由于这个`yscfg`文件没有像其它`yst`开头的`ybn`文件一样有一层`xor`，所以改好后直接放`ysbin`文件夹里就行了（通常是这样，但肯定有不通常嘛）。

当然这个上面还有一个叫 `debugMode`可以开启游戏的debug模式，有兴趣的可以去看看。

## 0x04 优先级内部原理

カサブランカの蕾 sub_463D98

```c++
  v0 = !Flag1 && !FileDebugFlag;
  v2 = Flag1 == 1 && !FileReleaseFlag;
  v3 = (1 - v2) * (1 - v0);

  if ( Flag2 != 1 && v3 )
  {
    v32 = 0;
    v33 = 1;
    v34 = 3;
    v35 = 2;
  }
  else
  {
    v32 = 0;
    v33 = 1;
    v34 = 2;
    v35 = 3;
  }
  
  for ( i = 0; i < 4; ++i )
  {
    v5 = *(&v32 + i);
    if ( *(&v32 + i) )
    {
      switch ( v5 )
      {
        case 1:
          v6 = sub_440910((int)&v28, "ysbin\\yscfg.ybn");
          if ( v6 )
            goto LABEL_23;
          break;
        case 2:
          v6 = sub_440620((int)&v28, "ysbin\\yscfg.ybn"); //封包
          if ( v6 )
            goto LABEL_23;
          break;
        case 3:
          v6 = sub_4405FC("ysbin\\yscfg.ybn"); //目录
          if ( (_BYTE)v6 )
          {
            v30 = 1;
            goto LABEL_23;
          }
          break;
      }
    }
    else
    {
      v6 = sub_440968((int)&v28, "ysbin\\yscfg.ybn");
      if ( v6 )
        goto LABEL_23;
    }
  }
```

随便简略说说，最头上`FileDebugFlag`这个就是`yscfg`里面的`filePriorityDebug` release同理，这也是为什么改`yscfg.ybn`能够让游戏变成能够直接读取目录下文件的原因。

```
    v32 = 0;
    v33 = 1;
    v34 = 3;
    v35 = 2;
```

其实这个就是一数组而已，你可以当成检索文件的顺序。底下那个switch其实就是依据这个数组里的数来找到对应的case，每个case里都有一个函数，函数参数都是文件的路径，如果某一个case里的函数执行成功就会`goto LABEL_23;`如果失败则进入下一个循环读取数组里的下一个元素， 这个LABEL_23;在后面，那一块代码是把文件读入内存的，太长没贴上来。

通常你不去动的时候，是命中case 2，也就是从封包读取，如果要走目录下的免封包得命中case 3，如果要命中case 3最简单的方法是把 `v32 = 0;`改成 = 3，从这也可以知道，无论你怎么改这个数组里的四个数，都要记住，其中一个必须有2，如果无法命中case 2也就是不能够从封包里读取，这样你目录下没文件的时候游戏会读取不到文件进而无法启动或报错。

我是不推荐你去改这个数组里的数字的，因为这个地方不止一处，有非常多类似上面代码的地方，而且都是有用的，当然也看游戏，有些游戏没几处。当然你硬要改的话，这种地方也很好定位，case 3那个走从封包读取的函数里是这样的

```C
BOOL __usercall sub_4405FC@<eax>(const CHAR *a1@<eax>)
{
  DWORD FileAttributesA; // eax

  FileAttributesA = GetFileAttributesA(a1);
  return (FileAttributesA & 0x10) == 0 && FileAttributesA != -1;
}
```

在IDA里可以对 `GetFileAttributesA()`（这是一个WINAPI）这个函数按x找到使用这个函数的地方，反正很快就可以找到上面代码差不多的地方，比如这个 `sub_4405FC`那么对这个函数按x找到使用这个函数的地方基本上就是上面文件读取优先级的代码处了。

在x64dbg里，有个搜索当前区域内所有调用WINAPI的call的功能，叫 Intermodular call 在搜字符串那个地方。搜一下可以找到exe里所有正常调用`WINAPI`的地方，然后筛选出调用`GetFileAttributesA()`的地方即可找到上面的位置

```ASM
004405FC  | 50                          | push eax                                      |
004405FD  | FF15 A0705700               | call dword ptr ds:[<&GetFileAttributesA>]     |
00440603  | A8 10                       | test al,10                                    |
00440605  | 75 0B                       | jne casablanca.440612                         |
00440607  | 83F8 FF                     | cmp eax,FFFFFFFF                              |
0044060A  | 74 06                       | je casablanca.440612                          |
0044060C  | B8 01000000                 | mov eax,1                                     |
00440611  | C3                          | ret                                           |
```

对这个 `push eax`按一下ctrl + r就能查找所有直接调用这个函数的地方了。

## 0x05 一些意外

上面已经说了，我不推荐你去改那个优先级数组里的0 1 2 3这些东西，一是太多，二是可能你改错了会导致一些文件没办法读取。

那么怎么改更好呢？刚刚我们已经说到了，把`yscfg`.ybn的`filePriorityRelease`改成1就可以，但是看看上面的代码，它在`yscfg.ybn`还没加载的时候就去判断了优先级，而且是默认为封包读取的。当然有一些游戏不会这样的，是允许`yscfg.ybn`从文件夹读取的，如果是这样的话，到这里其实就完成了。

其实最初`FileReleaseFlag`里面是0的，也就是和我们没改`yscfg.ybn`的时候是一样，那么这时候`yscfg.ybn`是不会从目录下读取的。只有当`yscfg.ybn`读取进来后，才会把`filePriorityRelease`里的数值拷贝到`FileReleaseFlag`里。那么这时候，我们单纯的把`yscfg.ybn`放到游戏目录下的`ysbin`文件夹内显然是没有效果的。

这时候，可以搜索`yscfg.ybn`字符串，然后找到它的优先级的那个数组，把第一个元素改成3即可。

```ASM
00463DF3  | 75 22                       | jne casablanca.463E17                         |
00463DF5  | C68424 68010000 00          | mov byte ptr ss:[esp+168],0  <-改成3           |
00463DFD  | C68424 69010000 01          | mov byte ptr ss:[esp+169],1                   |
00463E05  | C68424 6A010000 02          | mov byte ptr ss:[esp+16A],2                   |
00463E0D  | C68424 6B010000 03          | mov byte ptr ss:[esp+16B],3                   |
00463E15  | EB 20                       | jmp casablanca.463E37                         |
00463E17  | C68424 68010000 00          | mov byte ptr ss:[esp+168],0  <-改成3           |
00463E1F  | C68424 69010000 01          | mov byte ptr ss:[esp+169],1                   |
00463E27  | C68424 6A010000 03          | mov byte ptr ss:[esp+16A],3                   |
00463E2F  | C68424 6B010000 02          | mov byte ptr ss:[esp+16B],2                   |
00463E37  | 33DB                        | xor ebx,ebx                                   |
00463E39  | EB 01                       | jmp casablanca.463E3C                         |
```

当然我个认为这个方法还是不够好，还有别的方法，大家可以跟一下 `filePriorityRelease`的值是怎么赋给`FileReleaseFlag`的。

