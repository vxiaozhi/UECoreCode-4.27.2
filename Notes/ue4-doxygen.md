# UE4 生成 doxygen 文档

开始之前先安装相关工具，包括：

- doxygen
- graphviz

## 准备工作

注意，使用 doxygen 从 UE 源码生成文档之前，记得删除源码中所有的.h/.hpp 中的 UE 的宏 ，不然生成出来的会有问题 (比如函数名不见了，显示出来的是 UFUNCTION，或者成员变量不见了，显示出来的是 UPROPERTY)。

主要删除以下这几个 MACRO 就可以。

- "UFUNCTION",
- "DEPRECATED",
- "UCLASS",
- "UINTERFACE",
- "UPROPERTY",
- "GENERATED_BODY()",
- "GENERATED_UCLASS_BODY()",
- "GENERATED_INTERFACE_BODY()"

使用 sed 就是如下的命令：

```
# 删除源文件中的宏并保存
$ sed -i -e /UFUNCTION/d -e /DEPRECATED/d -e /UCLASS/d -e /UINTERFACE/d -e /UPROPERTY/d -e /GENERATED_BODY/d -e /GENERATED_UCLASS_BODY/d -e /GENERATED_INTERFACE_BODY/d ${filename}
# Example Actor.h
$ sed -i -e /UFUNCTION/d -e /DEPRECATED/d -e /UCLASS/d -e /UINTERFACE/d -e /UPROPERTY/d -e /GENERATED_BODY/d -e /GENERATED_UCLASS_BODY/d -e /GENERATED_INTERFACE_BODY/d Actor.h
```

## 生成文档

执行

```
GenDocs.bat
```
