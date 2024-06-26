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

## 常用命令行参数

- UE4Editor.exe "%GameDir%\MyGame.uproject"  // 启动MyGame项目编辑器
- UE4Editor.exe "%GameDir%\MyGame.uproject" -nothreading  // 以单线程的方式启动MyGame项目编辑器  注：缺省带-threading参数，以多线程的方式启动
- UE4Editor.exe "%GameDir%\MyGame.uproject" TestMap_Main -game  // 单机运行TestMap_Main地图
- UE4Editor.exe "%GameDir%\MyGame.uproject" TestMap_Main -game -norhithread  // 禁用rhi线程模式来单机运行TestMap_Main地图（在LaunchEngineLoop.cpp的PreInit方法）   注：缺省带-rhithread参数
- UE4Editor.exe "%GameDir%\MyGame.uproject" TestMap_Main -game -server  // 后台运行TestMap_Main地图ds进程  注：没有命令行窗口
- E4Editor.exe "%GameDir%\MyGame.uproject" TestMap_Main?multihome=10.32.212.47?port=7778 -game -server  // 后台运行TestMap_Main地图ds进程，监听本机IP为10.32.212.47网卡的7778端口
- UE4Editor.exe "%GameDir%\MyGame.uproject" TestMap_Main -game -server -log  // 前台运行TestMap_Main地图ds进程  注：有命令行窗口，log会实时地刷到命令行窗口中
- UE4Editor.exe "%GameDir%\MyGame.uproject" TestMap_Main -game -server MULTIHOME=10.32.212.47 Port=8888 // 绑定到10.32.212.47:8888，后台运行TestMap_Main地图ds进程
- UE4Editor.exe "%GameDir%\MyGame.uproject" 127.0.0.1 -game  // 启动游戏并联网加入本地ds对局中
- UE4Editor.exe "%GameDir%\MyGame.uproject" TestMap_Main?MaxPlayers=10?MaxSpectators=2 -game -server -log  // 运行TestMap_Main地图ds进程，并设置最大玩家数为10，最大观战玩家数为2
- UE4Editor.exe "%GameDir%\MyGame.uproject" 127.0.0.1?SpectatorOnly=1 -game  // 启动游戏并以观战者的身份联网加入本地ds对局中
- UE4Editor.exe "%GameDir%\MyGame.uproject" TestMap_Main -game -CmdLineFile=d:\test.txt // 读取d:\test.txt中的内容并加到命令行参数中   注：test.txt为-log -nosound
- UE4Editor.exe "%GameDir%\MyGame.uproject" TestMap_Main -game exec=cmd.txt  // 启动游戏后，执行引擎Engine\Binaries目录下cmd.txt中的命令序列
- UE4Editor.exe "%GameDir%\MyGame.uproject" TestMap_Main -game -ExecCmds=god,pause  // 启动游戏后，依次执行god和pause命令
- UE4Editor.exe "%GameDir%\MyGame.uproject" httpproxy=web-proxy.oa.com  // 设置http代理服务器为web-proxy.oa.com   注：在DefaultEngine.ini的HTTP标签下将HttpProxyAddress配置为web-proxy.oa.com也可以达到同样效果
- UE4Editor.exe "%GameDir%\MyGame.uproject" TestMap_Main -game -NoLoadingScreen  // 不加载Loading视频，单机运行TestMap_Main地图
- UE4Editor.exe "%GameDir%\MyGame.uproject" TestMap_Main -game -Windowed  // 单机以窗口模式运行TestMap_Main地图
- UE4Editor.exe "%GameDir%\MyGame.uproject" TestMap_Main -game -FullScreen  // 单机以全屏模式运行TestMap_Main地图
- UE4Editor.exe "%GameDir%\MyGame.uproject" TestMap_Main -game -Windowed  ResX=800 ResY=600 // 单机以窗口模式运行TestMap_Main地图  设置并移动窗体，使得渲染画面宽800高600
- UE4Editor.exe "%GameDir%\MyGame.uproject" TestMap_Main -game -Windowed  WinX=80 WinY=150 ResX=800 ResY=600 // 单机以窗口模式运行TestMap_Main地图  设置并移动窗体，使得渲染画面宽800高600，左上角屏幕坐标为(80, 150)
- UE4Editor.exe "%GameDir%\MyGame.uproject" TestMap_Main -game -Windowed ResX=1415 ResY=435 -emulatestereo  // 以左右分屏立体模式单机运行TestMap_Main地图，使得渲染画面宽1415高435
- UE4Editor.exe "%GameDir%\MyGame.uproject" TestMap_Main -game -ansimalloc // 使用Ansi内存分配器单机运行TestMap_Main地图   注：非Shipping包下有效
- UE4Editor.exe "%GameDir%\MyGame.uproject" TestMap_Main -game -tbbmalloc // 使用TBB内存分配器单机运行TestMap_Main地图（windows下缺省为该方式）
- UE4Editor.exe "%GameDir%\MyGame.uproject" TestMap_Main -game -mimalloc // 使用mimalloc内存分配器单机运行TestMap_Main地图
- UE4Editor.exe "%GameDir%\MyGame.uproject" TestMap_Main -game -stompmalloc // 使用stompmalloc内存分配器单机运行TestMap_Main地图
- UE4Editor.exe "%GameDir%\MyGame.uproject" TestMap_Main -game -binnedmalloc // 使用Binned内存分配器单机运行TestMap_Main地图
- UE4Editor.exe "%GameDir%\MyGame.uproject" TestMap_Main -game -binnedmalloc2 // 使用Binned2内存分配器单机运行TestMap_Main地图
- UE4Editor.exe "%GameDir%\MyGame.uproject" TestMap_Main -game -binnedmalloc3 // 使用Binned3内存分配器单机运行TestMap_Main地图（需为64位游戏）

## Cook打包好的windows版本

`E:\MyGame\MyGame.exe -basedir="F:\Downloads\MyGame-Win64-Test\MyGame\Binaries\Win64"`   // 使用E:\MyGame中的MyGame.exe可执行程序，加上F:\Downloads\MyGame-Win64-Test\MyGame\Binaries\Win64版本内容（其他dll、资源、配置都在该目录中），来运行游戏  注：已有的打包版本+新的exe


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
