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

## 日志实例

```
UE_LOG(LogAnimation, Warning, TEXT("FBoneReference::Initialize BoneIndex for Bone '%s' does not exist in Skeleton '%s'"), *BoneName.ToString(), *GetNameSafe(SkeletonAsset));

// 日志的类别有：NoLogging、Fatal、Error、Warning、Display、Log、Verbose、VeryVerbose
// LogTimes为Local时打印出的log如下
// [989]为GFrameCounter % 1000
[2019.09.21-11.56.42:537][989]LogAnimation: Warning: FBoneReference::Initialize BoneIndex for Bone 'GunRef' does not exist in Skeleton 'F01_body_rig_Skeleton'
```

GFrameCounter % 1000 代码实现：

```
FString FOutputDeviceHelper::FormatLogLine( ELogVerbosity::Type Verbosity, const class FName& Category, const TCHAR* Message /*= nullptr*/, ELogTimes::Type LogTime /*= ELogTimes::None*/, const double Time /*= -1.0*/ )
{
	const bool bShowCategory = GPrintLogCategory && Category != NAME_None;
	FString Format;

	switch (LogTime)
	{
		case ELogTimes::SinceGStartTime:
		{																	
			const double RealTime = Time == -1.0f ? FPlatformTime::Seconds() - GStartTime : Time;
			Format = FString::Printf( TEXT( "[%07.2f][%3llu]" ), RealTime, GFrameCounter % 1000);
			break;
		}

		case ELogTimes::UTC:
			Format = FString::Printf(TEXT("[%s][%3llu]"), *FDateTime::UtcNow().ToString(TEXT("%Y.%m.%d-%H.%M.%S:%s")), GFrameCounter % 1000);
			break;

		case ELogTimes::Local:
			Format = FString::Printf(TEXT("[%s][%3llu]"), *FDateTime::Now().ToString(TEXT("%Y.%m.%d-%H.%M.%S:%s")), GFrameCounter % 1000);
			break;

		case ELogTimes::Timecode:
			Format = FString::Printf(TEXT("[%s][%3llu]"), *FApp::GetTimecode().ToString(), GFrameCounter % 1000);
			break;

		default:
			break;
	}	

	if (bShowCategory)
	{
		Format += Category.ToString();
		Format += TEXT(": ");

		if (GPrintLogVerbosity && Verbosity != ELogVerbosity::Log)
		{
			Format += ToString(Verbosity);
			Format += TEXT(": ");
		}
	}
	else if (GPrintLogVerbosity && Verbosity != ELogVerbosity::Log)
	{
#if !HACK_HEADER_GENERATOR
		Format += ToString(Verbosity);
		Format += TEXT(": ");
#endif
	}

	if (Message)
	{
		Format += Message;
	}
	return Format;
}
```

## UE4的LOG时间格式

### 方式1

Preferences -> General -> Apppearance -> Log Timestamp选择类型就可以输出时间了。

### 方式2 日志中时间格式也可以在DefaultEngine.ini中配置

```
[LogFiles]
;LogTimes=None ;日志中不打印时间
;LogTimes=UTC ;日志中时间使用utc格式
;LogTimes=SinceStart ;日志中时间使用SinceStart格式
LogTimes=Local ;日志中时间使用Local格式
;LogTimes=Timecode ;时间使用Timecode格式
```

## UE4 Log 信息中的关键词

- 搜索 `LogInit: Command Line:` 查看进程启动参数

## 参考

- [UE4 调试常用的打印日志方法](https://cloud.tencent.com/developer/article/2066023)
- [UE4-正确使用LOG](https://stonelzp.github.io/how-to-use-log/)
- [虚幻引擎应用实例分享（六）：日志打印](https://indienova.com/indie-game-development/unreal-engine-example-sharing-part-6/)
- [UE4 字符串调试日志](https://muyunsoft.com/blog/Unreal4/UE4CodeTheory/DebugInfo.html)
