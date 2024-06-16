# UE4 打包流程详解

当在 UE4 编辑器中， 执行打包（点击 File -> Project -> Package Project） 后，UE4 编辑器背后都干了什么？

本文视图剖析这背后的流程。

## 源码追踪

跟踪源码前，先思考以下几个问题：

- 菜单 Package Project 是在哪里注册的？
- 菜单 被点击时触发了哪个函数执行？
- 该执行函数都做了什么？

顺着这个思路， 在代码中搜索关键字： Package Project。

菜单注册是在 FMainMenu::RegisterFileProjectMenu 中完成的。

```
void FMainMenu::RegisterFileProjectMenu()
{
	if (!GetDefault<UEditorStyleSettings>()->bShowProjectMenus)
	{
		return;
	}

	UToolMenus* ToolMenus = UToolMenus::Get();
	UToolMenu* MainTabFileMenu = ToolMenus->ExtendMenu("MainFrame.MainTabMenu.File");
	FToolMenuSection& Section = MainTabFileMenu->AddSection("FileProject", LOCTEXT("ProjectHeading", "Project"), FToolMenuInsert("FileLoadAndSave", EToolMenuInsertType::After));

	Section.AddMenuEntry( FMainFrameCommands::Get().NewProject );
	Section.AddMenuEntry( FMainFrameCommands::Get().OpenProject );

	FText ShortIDEName = FSourceCodeNavigation::GetSelectedSourceCodeIDE();

	Section.AddMenuEntry( FMainFrameCommands::Get().AddCodeToProject,
		TAttribute<FText>(),
		FText::Format(LOCTEXT("AddCodeToProjectTooltip", "Adds C++ code to the project. The code can only be compiled if you have {0} installed."), ShortIDEName)
	);

	Section.AddSubMenu(
		"PackageProject",
		LOCTEXT("PackageProjectSubMenuLabel", "Package Project"),
		LOCTEXT("PackageProjectSubMenuToolTip", "Compile, cook and package your project and its content for distribution."),
		FNewMenuDelegate::CreateStatic( &FPackageProjectMenu::MakeMenu ), false, FSlateIcon(FEditorStyle::GetStyleSetName(), "MainFrame.PackageProject")
	);
    // 。。。
```


最终绑定了 回调函数FMainFrameActionCallbacks::PackageProject， 该函数代码较长， 大概流程如下：
```

void FMainFrameActionCallbacks::PackageProject( const FName InPlatformInfoName )
{
  //...
  // 获取项目设置
  UProjectPackagingSettings* PackagingSettings = Cast<UProjectPackagingSettings>(UProjectPackagingSettings::StaticClass()->GetDefaultObject());
  // ...
  // 分析设置，组装成命令参数
if (PackagingSettings->FullRebuild)
	{
		OptionalParams += TEXT(" -clean");
	}

	if ( PackagingSettings->bCompressed )
	{
		OptionalParams += TEXT(" -compressed");
	}
// ...
FString ProjectPath = FPaths::IsProjectFilePathSet() ? FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath()) : FPaths::RootDir() / FApp::GetProjectName() / FApp::GetProjectName() + TEXT(".uproject");
	FString CommandLine = FString::Printf(TEXT("-ScriptsForProject=\"%s\" BuildCookRun %s%s -nop4 -project=\"%s\" -cook -stage -archive -archivedirectory=\"%s\" -package -clientconfig=%s -ue4exe=\"%s\" %s -utf8output"),
		*ProjectPath,
		GetUATCompilationFlags(),
		FApp::IsEngineInstalled() ? TEXT(" -installed") : TEXT(""),
		*ProjectPath,
		*PackagingSettings->StagingDirectory.Path,
		*Configuration,
		*FUnrealEdMisc::Get().GetExecutableForCommandlets(),
		*OptionalParams
	);
  // 创建 UAT task
	IUATHelperModule::Get().CreateUatTask( CommandLine, PlatformInfo->DisplayName, LOCTEXT("PackagingProjectTaskName", "Packaging project"), LOCTEXT("PackagingTaskName", "Packaging"), FEditorStyle::GetBrush(TEXT("MainFrame.PackageProject")) );
}
```


CreateUatTask 最终根据传入的参数， 调用了 RunUAT 命令。
```
virtual void CreateUatTask( const FString& CommandLine, const FText& PlatformDisplayName, const FText& TaskName, const FText &TaskShortName, const FSlateBrush* TaskIcon, UatTaskResultCallack ResultCallback )
{
		// make sure that the UAT batch file is in place
	#if PLATFORM_WINDOWS
		FString RunUATScriptName = TEXT("RunUAT.bat");
		FString CmdExe = TEXT("cmd.exe");
	#elif PLATFORM_LINUX
		FString RunUATScriptName = TEXT("RunUAT.sh");
		FString CmdExe = TEXT("/bin/bash");
	#else
		FString RunUATScriptName = TEXT("RunUAT.command");
		FString CmdExe = TEXT("/bin/sh");
	#endif
   // ...
}
```

## UAT 详细执行流程

查看 Engine/Source/Programs/AutomationTool ，不难看出， 日志打印非常完善， 例如，如下代码会打印 AutomationTool 执行时的命令参数。
```
private static void ParseCommandLine(string[] Arguments, List<CommandInfo> OutCommandsToExecute, out string OutScriptsForProjectFileName, List<string> OutAdditionalScriptsFolders)
{
			// Initialize global command line parameters
			GlobalCommandLine.Init();

			ParseProfile(ref Arguments);

			Log.TraceInformation("Parsing command line: {0}", CommandUtils.FormatCommandLine(Arguments));
           // 。。。
}
```
因此我们可以结合 打包 时的日志信息来辅助学习 AutomationTool 到底干了啥

