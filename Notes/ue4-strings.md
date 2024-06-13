# UE4 中的字符串处理

## 字符串相关的类

- TCHAR    UE4 中对 char 和 wchar_t 的封装
- FName    常用作标识符等不变的字符串（如：资源路径/资源文件类型/骨骼名称/表格行名等）
- FText    是一种静态字符串，在UE4中可以负责处理文本本地化、UI 文本显示等
- FString    不像FName和FText，FString是唯一一个允许操作字符串的类，有很多方法可以操作字符串，包括改变大小写，摘取子字符串，或者反转字符串，FStrings可以被查找，改变和其他字符串比较。但是，这些操作让FStrings比那些不改变字符串的类花销更大。



## 参考

- [UE4 字符串系统FString研究](https://muyunsoft.com/blog/Unreal4/UE4CodeTheory/FString.html#%E5%88%9B%E5%BB%BAfstrings)