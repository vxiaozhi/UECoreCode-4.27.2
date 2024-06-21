# UE4 中的配置文件(ini)


## 简介

### 1. 常见存储路径

- \Engine\Config\
- \Engine\Saved\Config\ (运行后生成)
- [ProjectName]\Config\
- [ProjectName]\Saved\Config (运行后生成)
- 打包文件
    - Development/Debug 打包目录\项目名称\Saved\Config\WindowsNoEditor
    - Shipping C:\Users\用户名\AppData\Local\项目名称\Saved\Config\WindowsNoEditor

### 2. 常见配置文件

- DefaultEditor.ini
- DefaultEngine.ini
- DefaultGame.ini
- DefaultInput.ini


## 代码分析

读写 ini 文件相关的类：

```
FConfigCacheIni

Engine\Source\Runtime\Core\Private\Misc\ConfigCacheIni.cpp
```



## 参考

 - [【UE4 C++】 Config Settings配置文件(.ini) ](https://www.cnblogs.com/shiroe/p/14771446.html)
 - [UE4 Config配置文件详解](https://gwb.tencent.com/community/detail/121619)
