# UE4 中的工具链

UE4 中的工具都位于以下目录:

```
Engine/Source/Programs
```

下面对常用的工具进行说明

## 编译构建相关

UBT和UHT是虚幻引擎自己定义的编译构建工具，主要用来简化多平台编译，去除用户自定义平台编译项目的操作。

我们写的UE4代码不是标准的C++代码，是基于UE4源代码层层改装了很多层的，UHT将UE4代码转换成标准的C++代码，而UBT负责调用UHT来实现这个转化工作。

转化完之后UBT调用标准的C++代码的编译器来将UHT转化后的标准C++代码完成编译成二进制文件。

整体上看，UHT是UBT的编译流程的一部分

### 1. UHT(UnrealHeaderTool)

UHT:Unreal Header Tool

Unreal engine背后强大的反射机制离不开UHT

UENUM()、UCLASS()、USTRUCT()、UFUNCTION()、UPROPERTY()来标记不同的类型和成员变量

也可以标记一个含有反射类型的头文件，需要添加一个特殊的#include

```
 #include "FileName.generated.h"
```
将反射数据保存为C++代码的一个好处为可以与二进制文件保持一致，永远不会加载过期的反射数据，因为它们参与编译，永远也不会加载陈旧或过时的反射数据。
UHT被设计为一个独立的程序，自己本身不使用任何的generated headers，因此避免了先有鸡还是先有蛋的问题。

.generated.h中生成的函数包含了XXX_Implementation之类的补全，也包含了用于蓝图中调用C++函数的转换函数，并通过GENERATED_BODY()安插到我们编写的类中。

**注意：** 虽然UHT实现了近似C++解析器的功能，但毕竟只能理解一部分语法，不要用#if/#ifndef把标记抱起来，会出现错误。UE4提供了一些特殊的宏来兼容反射系统，比如WITH_EDIROT和WITH_EDITORONLY_DATA。

### 2. UBT(UnrealBuildTool)

UBT:Unreal Build Tool

Unreal Build Tool由C#编写，且作为整个虚幻编译过程中第一个编译步骤。

当你运行"GenerateProjectFiles"（一个批处理文件，用于Window平台下生成Visual Studio的解决方案和工程），第一个步骤就是在Source/Programs/UnrealBuildTool/UnrealBuiltTool.csproj工程下执行MSBuild来编译这个"Unreal Build Tool"。

所以可以理解UBT其实就是一个命令行程序，却可以完成很多事情，比如生成工程文件、执行UBT、为各种不同的平台构建风格来调用编译器（Compiler）和链接器（Linker）。

### 3. UAT(AutomationTool)

## 其它
