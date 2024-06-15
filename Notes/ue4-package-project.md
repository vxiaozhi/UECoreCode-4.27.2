# UE4 打包流程详解

当在 UE4 编辑器中， 执行打包（点击 File -> Project -> Package Project） 后，UE4 编辑器背后都干了什么？
本文视图剖析这背后的流程。

菜单注册


回调函数
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
