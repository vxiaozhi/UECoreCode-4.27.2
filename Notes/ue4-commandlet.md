# UE4 Commandlet 简介

## 创建 Commandlet

为项目或插件创建 Commandlet，通常会在模块类型为 Editor 的 Module 中添加，因为执行 Commandlet 的逻辑，很少会涉及到运行时，都是在引擎环境下执行的。

创建自定义的 Commandlet，需要创建一个继承自 UCommandlet 的 UObject 类：

`ResScannerCommandlet.h`

```
  
DECLARE_LOG_CATEGORY_EXTERN(LogResScannerCommandlet, All, All);  
  
UCLASS()  
class RESSCANNER_API UResScannerCommandlet :public UCommandlet  
{  
   GENERATED_BODY()  
  
public:  
   virtual int32 Main(const FString& Params)override;
};
```
其中的 Main 函数，就是当通过 -run= 命令启动该 Commandlet 时，会执行到的逻辑，就像前面纯 C++ 的 main 函数一样。

不过，需要注意的是，因为 Commandlet 其实是完整的引擎环境，所以执行时会拉起已注册模块的启动，相当于有很多的前置逻辑需要执行，等执行到 Main 函数时，就是完整的引擎状态了。

可以在这个函数之内做自定义的操作，此时它具有完整的引擎环境，可以根据自己的需要进行数据导出或资源处理。

## 运行 Commandlet

前面已经提到了，运行一个 Commandlet 需要用以下的命令形式：

`UE4Editor-cmd.exe PROJECT_NAME.uproject -run=CMDLET_NAME`

启动项目中的 Commandlet 时，必须要指定项目路径，因为要去加载工程及插件中的模块。

参数 -run=，指定的是要执行的 Commandlet 的名字，这个名字和前面创建的继承自 UCommandlet 类名字直接相关：如 UResScannerCommandlet 它的 Commandlet 的名字就为 ResScanner，规则就是去掉头部的 U 以及尾部的 Commandlet。

引擎内接收到 -run= 的参数，会去查找 UClass，并会自动拼接 Commandlet 后缀


## 参考

- [UE 插件与工具开发：Commandlet](https://imzlp.com/posts/27475/)
