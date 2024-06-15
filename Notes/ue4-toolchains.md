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

#### BuildConfiguration

除了添加到Config/UnrealBuildTool文件夹下生成的UE4项目之外，UnrealBuildTool还会从Windows上以下位置的XML配置文件读取设置：

- Engine/Saved/UnrealBuildTool/BuildConfiguration.xml
- User Folder/AppData/Roaming/Unreal Engine/UnrealBuildTool/BuildConfiguration.xml
- My Documents/Unreal Engine/UnrealBuildTool/BuildConfiguration.xml

#### 构建C++项目
在Unreal中构建C++项目时，您可以看到（基于vcxproj的NMakeBuildCommandLine属性）将调用与此类似的命令行

```
C:\Path\To\Your\Engine\Build.bat TargetName Win64 Debug "$(SolutionDir)$(ProjectName).uproject" -waitmutex $(AdditionalBuildArguments) -2017
```

它的背后其实又调用了UnrealBuildTool

那么，UnrealBuildTool在这儿的作用是：

- 编译目标。它在运行时编译了.Target.cs代码（使用C#编译器）来获取构建属性。这是UnrealBuildTool从中获取大部分定义和平台信息的地方。某些属性（例如bBuildEditor）表示你需要的是构建编辑器。它会创建一个WITH_EDITOR定义，然后由编译器转发到源文件。以实现源代码中的条件编译：#if WITH_EDITOR 条件编译
- 解析所有依赖模块，包含来自.Target.cs和.Build.cs（模块）的依赖
- 将编译所有依赖模块的Build.cs，以获取有关如何构建每个模块的额外属性
- 解析哪些模块使用了共享编译头(即.Build.cs文件中包含SharedPCHHeaderFile属性，比如CoreUObject,Core,Engine等)
- 解析哪些模块依赖于UObject模块
- 对所有依赖于UObject的模块运行Unreal Header Tool，这时虚幻引擎会注入一些行为到你的类中，强制你在文件中加入由Unreal Header Tool生成的“.generated.h”头文件
- 基于Unreal Header Tool生成的代码，解析所有Include路径
- 基于解析后的路径、定义、外部库等，生成一系列会在目标环境执行的命令列表
- 为共享预编译头调用编译器（CL.EXE）
- 调用编译器来编译源文件（CL.EXE）
- 调用链接器（LINK.EXE）
- 调用所有这些操作

### 3. UAT(AutomationTool)

RunUAT 工具来实现自动化的编译和打包，该工具支持 Windows,Mac以及Linux。

先来看看它打包的命令行参数吧！

「BuildCookRun」 用于给整体项目执行打包，它的打包过程是： 
- 1.先编译（build）
- 2.资源的烘焙（cook）
- 3.stage
- 4.package
-  5.archive

「基本参数」

- CrashReporter: 编译CrashReporter
- Clean: 重新完整构建
- SignedPak: 使用加密的pak文件
- SignPak: 指定加密pak文件密钥内容或者密钥文件路径
- RunAssetNativization: 将蓝图资产转成代码
- Pak: 是否将资源文件打包到pak中
- SkipPak: 使用上一次的pak文件，包含-Pak
- UTF8Output: log，控制台窗口等输出使用UTF8编码
  
「Build相关参数」

- Build: 执行编译过程
- SkipBuildClient: 不编译客户端
- SkipBuildEditor: 不编译编辑器
- NoXGE: 不使用并行编译
- ForceDebugInfo: 在非Debug版本中加入调试信息

「Cook相关参数」

- Cook: 使用 Cooked 资源
- SkipCook: 跳过cook阶段，使用上一次Cook好的资源，同时包含了 -Cook 参数
- IgnoreCookErrors: 忽略cook过程中的错误
- CookFlavor=Multi/ATC/DXT/ETC1/ETC2/PVRTC/ASTC: 指定Android Cook格式
- CookPartialGC: cook阶段磁盘空间不够不要清理所有资源
- CookInEditor: 使用editor进行cook而不是UAT
- CookOutputDir: 指定cook结果保存目录，默认在Project/Saved/Cooked
- AdditionalCookerOptions=xxx: 额外的传给cooker进程的命令行
- Compressed: 压缩
- EncryptIniFiles: 加密ini文件，只在指定了加密密钥和使用pak文件时候有效
- EncryptEverything: 加密所有的文件
- EncryptPakIndex: 加密pak索引
- UnversionedCookedContent: 不包含版本号
- IterativeCooking(-Iterate): 增量 cook
- CookAll: cook content 目录下的所有文件
- CookMapsOnly: 只 cook 关卡和关卡引用到的资源，只会在同时指定了-CookAll 时生效
- MapsToCook=map1+map2+map3: 指定要cook的关卡，多个之间用+号连接
- SkipCookingEditorContent: 不cook引擎编辑器使用到的资源
- NumCookersToSpawn=n: 指定cooker进程的数量
- FastCook: 开启FastCook，如果项目支持的话
  
「Stage相关参数」

- Stage: 保存构建过程中的中间结果
- SkipStage: 不保存构造过程中的中间结果，使用上一次的结果，这个命令行同时包含了-Stage
- StagingDirectory: 构建过程中中间结果保存目录，默认在 ProjectPath\Saved\StagedBuilds
- NoDebugInfo: 不拷贝调试信息文件到 Stage 目录，最终打出来的包中不包含调试信息
- NoCleanStage: 新构建不会清理之前 StagingDirectory 中的文件, 如果指定了-clean 则一定会清理掉
- StageCommandline: 放在UE4CommandLine.txt中的一系列命令行，打出来的包在运行时会从这个文件中读取命令行并执行
  
「Package相关参数」

- Package: 执行打包
- Distribution: 打的包是发布版本
- Prereqs: 将依赖打包到一起
  
「Archive相关参数」

- Archive: 是否将构建结果归档到指定目录
- ArchiveDirectory: 归档目录
- CreateAppBundle: 当目标平台是Mac时，指定这个参数可以归档成一个 .app 文件
  
「Deploy相关参数」

- Deploy: 部署
- DeployFolder: 部署路径

#### BuildCookRun 命令打包

编译客户端

```
RunUAT BuildCookRun -project="full_project_path_and_project_name.uproject" -noP4 -platform=Win64 -clientconfig=Development -serverconfig=Development -cook -allmaps -build -stage -pak -archive -archivedirectory="Output Directory"
```

cook客户端

```
RunUAT BuildCookRun -project="full_project_path_and_project_name.uproject" -noP4 -platform=Win64 -clientconfig=Development -serverconfig=Development -cook -allmaps -NoCompile -stage -pak -archive -archivedirectory="Output Directory"
```

编译服务器

```
RunUAT BuildCookRun -project="full_project_path_and_project_name.uproject" -noP4 -platform=Win64 -clientconfig=Development -serverconfig=Development -cook -server -serverplatform=Win64 -noclient -build -stage -pak -archive -archivedirectory="Output Directory"
```

cook服务器

```
RunUAT BuildCookRun -project="full_project_path_and_project_name.uproject" -noP4 -platform=Win64 -clientconfig=Development -serverconfig=Development -cook -server -serverplatform=Win64 -noclient -NoCompile -stage -pak -archive -archivedirectory="Output Directory"
```

## 其它

## 参考

- [Build Configuration](https://docs.unrealengine.com/4.26/en-US/ProductionPipelines/BuildTools/UnrealBuildTool/BuildConfiguration/)
