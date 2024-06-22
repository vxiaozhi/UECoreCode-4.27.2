# UE4 日志

## 自定义LOG的Category

头文件中声明：

```
DECLARE_LOG_CATEGORY_EXTERN(CategoryName, DefaultVerbosity, CompileTimeVerbosity);
```

- 参数 CategoryName 是你定义的类别名
- 参数 DefaultVerbosity 日志级别
- 参数 CompileTimeVerbosity 是要在代码中编辑的最大详情类别


CPP文件中定义：

```
DEFINE_LOG_CATEGORY(CategoryName);
```

使用：

```

UE_LOG(CategoryName, Log, TEXT("Test Log Message"));

```

## 快速打印提示信息

这个一个简单的调试宏，你可以在你CPP文件的开头使用它：

```
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)
```

然后你就能在这个CPP文件中像往常一样使用可爱的print();


## 设置 Log 的输出等级

### 方式1 配置文件

```
[Core.Log]
LogMyAwesomeGame=VeryVerbose
```

### 方式2 使用命令行的方式提升某个LOG输出等级

```
Console
: Log LogMyAwesomeGame Verbose //将定义的LOG提升至Verbose输出等级
```


## UE4的LOG输出时间

Preferences -> General -> Apppearance -> Log Timestamp选择类型就可以输出时间了。

## 参考

- [UE4 调试常用的打印日志方法](https://cloud.tencent.com/developer/article/2066023)
- [UE4-正确使用LOG](https://stonelzp.github.io/how-to-use-log/)
- [虚幻引擎应用实例分享（六）：日志打印](https://indienova.com/indie-game-development/unreal-engine-example-sharing-part-6/)
- [UE4 字符串调试日志](https://muyunsoft.com/blog/Unreal4/UE4CodeTheory/DebugInfo.html)
