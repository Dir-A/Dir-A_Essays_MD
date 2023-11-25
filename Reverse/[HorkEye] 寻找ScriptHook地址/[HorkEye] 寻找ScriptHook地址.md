# [ACV1引擎] 寻找ScriptHook地址

## 0x00 前言

为了实现不封包即对游戏的脚本文件进行修改

此处需要Hook相关位点

Hook功能已经在[ACV1FileHook](https://github.com/Dir-A/ACV1FileHook)中实现

但是需要寻找两个地址

第一个地址是LoadScript函数的地址，该函数传入脚本文件的Hash文件名和一个记录信息的结构体地址

第二个地址是DecScript函数的地址，该函数对脚本文件进行解密

```C
DWORD CDECL DecScript(
    PBYTE pDecBuffer, 
    PDWORD pdwDecSize, 
    PBYTE pRawBuffer, 
    DWORD dwRawSize);
DWORD CDECL LoadScript(
    DWORD dwHashHigh, 
    DWORD dwHashLow, 
    PBYTE &UnknownStruct);
```

```
LoadScript
{
	ReadRawScript
	DecScript
	InitScript
}
```



## 0x01 寻找地址

首先搜索字符串 script/

这是因为LoadScript函数的实现末尾有一个对脚本进行加载的函数InitScript

该函数中的一个参数是该字符串常量，此处IDA识别有点问题。

我们可以通过该函数的参数定位出LoadScript函数



下面可以看到InitScript函数，DecScriptFile就在其上面不远处

DecScriptFile其下面会有个 ulltos32 这是明显的特征

换句话说，你只要找到 script/ 使用处

然后往上翻阅，到第二个 ulltos32 call的地方，其上那个函数就是DecScriptFile

```C
  }
  if ( DecScriptFile((PBYTE)decBuffer, &decSize, (PBYTE)rawBuffer, szie) )
  {
    v32 = 1;
    v19 = ulltos32((int)v37, dwHashLow, hash_high_1);
    UnknownStruct = (PBYTE *)&v25;
    v42 = 1;
    sub_417E50(&v25, v19, &unk_5C1B7C);
    errord(v25, v26, v27, v28, v29, v30, v31, v32);
    if ( v38 >= 0x10 )
      operator delete(v37[0]);
    free(rawBuffer);
    return 0;
  }
  free(rawBuffer);
  v20 = ulltos32((int)v37, dwHashLow, hash_high_1);
  if ( *(_DWORD *)(v20 + 20) >= 0x10u )
    v20 = *(_DWORD *)v20;
  v21 = (char *)&dword_C855B8 - v20;
  do
  {
    v22 = *(_BYTE *)v20;
    v21[v20] = *(_BYTE *)v20;
    ++v20;
  }
  while ( v22 );
  if ( v38 >= 0x10 )
    operator delete(v37[0]);
  v32 = (int)"script/";
  InitScript(
    decBuffer_1,
    decSize,
    (void (__cdecl *)(int, unsigned int, unsigned int, int))ProcessScript,
    (char *)UnknownStruct);
  return 1;
```

