# UE4 中的控制台指令

## 简介

UE4 可通过命令行参数开启或关闭一些功能， 同时部分功能也支持 在控制台中（快捷键 ` 开启）通过命令方式动态启停。

## 代码实现
UE4 的命令行参数主要在类FCommandLine中实现。 在进程启动阶段通过 调用 FCommandLine::BuildFromArgV 进行解析：

```
int32 FEngineLoop::PreInit(int32 ArgC, TCHAR* ArgV[], const TCHAR* AdditionalCommandline)
{
	FString CmdLine = FCommandLine::BuildFromArgV(nullptr, ArgC, ArgV, AdditionalCommandline);

	// send the command line without the exe name
	return GEngineLoop.PreInit(*CmdLine);
}
```

最终的解析结果存储在 全局静态变量 FCommandLine::CmdLine 中， 其它模块可调用 FCommandLine::Get() 获取该值。

```
struct CORE_API FCommandLine
{
    /** Flag to check if the commandline has been initialized or not. */
	static bool bIsInitialized;
	/** character buffer containing the command line */
	static TCHAR CmdLine[MaxCommandLineSize];
	/** character buffer containing the original command line */
	static TCHAR OriginalCmdLine[MaxCommandLineSize];
	/** character buffer containing the command line filtered for logging purposes */
	static TCHAR LoggingCmdLine[MaxCommandLineSize];
	/** character buffer containing the original command line filtered for logging purposes */
	static TCHAR LoggingOriginalCmdLine[MaxCommandLineSize];
	/** subprocess command line */
	static FString& GetSubprocessCommandLine_Internal();
};
```


## 指令解析

指令解析主要是在 FParse 类中实现的。
模块可通过调用一些辅助工具类，如 FParse 进行指令参数的判断。

- bool FParse::Command( const TCHAR** Stream, const TCHAR* Match, bool bParseMightTriggerExecution )
- bool FParse::Param( const TCHAR* Stream, const TCHAR* Param )

```
//
// Sees if Stream starts with the named command.  If it does,
// skips through the command and blanks past it.  Returns 1 of match,
// 0 if not.
//
bool FParse::Command( const TCHAR** Stream, const TCHAR* Match, bool bParseMightTriggerExecution )
{
#if !UE_BUILD_SHIPPING
	if(GConsoleCommandLibrary)
	{
		GConsoleCommandLibrary->OnParseCommand(Match);
		
		if(bParseMightTriggerExecution)
		{
			// Better we fail the test - we only wanted to find all commands.
			return false;
		}
	}
#endif // !UE_BUILD_SHIPPING

	while (**Stream == TEXT(' ') || **Stream == TEXT('\t'))
	{
		(*Stream)++;
	}

	int32 MatchLen = FCString::Strlen(Match);
	if (FCString::Strnicmp(*Stream, Match, MatchLen) == 0)
	{
		*Stream += MatchLen;
		if( !FChar::IsAlnum(**Stream))
//		if( !FChar::IsAlnum(**Stream) && (**Stream != '_') && (**Stream != '.'))		// more correct e.g. a cvar called "log.abc" should work but breaks some code so commented out
		{
			while (**Stream == TEXT(' ') || **Stream == TEXT('\t'))
			{
				(*Stream)++;
			}

			return true; // Success.
		}
		else
		{
			*Stream -= MatchLen;
			return false; // Only found partial match.
		}
	}
	else
	{
		return false; // No match.
	}
}
```
从 FCString::Strnicmp 的调用可以看出 UE4 的命令是忽略大小写的。


## 常用指令

- dumpconsolecommands console的命令非常多，将所有命令全部dump出一个列表, 使用命令dumpconsolecommands，执行后，所有可用的命令会存储在\Saved\Logs\下的log文件中。
- ShowFlag.Bounds 1/0 开启/关闭在 Editor 中查看 Actor 蓝图的 Bounds
- Show Collision 显示刚体
- stat fps 显示帧率
- t.MaxFPS 60 限制最大帧率（需要提前关闭VSync）
- stat scenerendering 显示mesh drawcall
- stat slate 显示UMG Widget drawcall
- stat Engine 显示static mesh、skeletal mesh的三角面数量
- stat rhi 显示硬件级（RHI）三角面总数和drawcall总数
- stat game 显示当前帧的CPU时间信息（各种Tick, GC Mark，Update Overlaps等）
- r.ScreenPercentage 50 设置渲染分辨率为默认大小的50%（500就表示实际viewport大小的5倍尺寸来渲染，可以用来做压力测试，判断性能瓶颈是不是像素处理）
- DisableAllScreenMessages/EnableAllScreenMessages 关闭或开启屏幕上打印信息（AddOnScreenDebugMessage）
- Taking Screenshots 
- NETWORKPROFILER enable/disable  网络性能分析工具 NetworkProfiler 启停。


## 控制台变量

一个控制台变量是一些简单的数据类型，比如float，int，string，它在引擎范围内有状态，用户可以读取和设置这些状态。控制台变量有名字，用户能在输入变量名到控制台时使用自动补全。

在引擎启动的时候，控制台变量的状态可从Engine/Config/ConsoleVariables.ini文件中读取并被加载，这个地方是为本地开发者预留的，它不应该作为项目的设置

## 如何在 DS 动态执行命令

目前应该不支持
这里有讨论：[Dedicated Server and Console Commands (Exec, Cvars)](https://forums.unrealengine.com/t/dedicated-server-and-console-commands-exec-cvars/106445)

## 参考

- [[UE4]console命令行常用命令(command)](https://dawnarc.com/2016/05/ue4console%E5%91%BD%E4%BB%A4%E8%A1%8C%E5%B8%B8%E7%94%A8%E5%91%BD%E4%BB%A4command/)
- [Unreal Engine 4 Console Variables and Commands](https://www.cnblogs.com/kekec/p/16872555.html)
- [UE4 中的控制台变量](https://muyunsoft.com/blog/Unreal4/UE4CodeTheory/ConsoleVariables.html)
- [UE4之Exec命令 ](https://www.cnblogs.com/kekec/p/17364072.html)
- [UE4命令行参数](https://www.cnblogs.com/kekec/p/14952261.html)
