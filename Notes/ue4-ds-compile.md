# UE4 DS 设置及编译构建

## 设置服务器构建目标

创建 [ProjectName].Target.cs 的副本，并将其重命名为 [ProjectName]Server.Target.cs 

内容参考如下：

```
public TestProjectServerTarget(TargetInfo Target) : base(Target) //根据你的项目名称更改此行
{
    Type = TargetType.Server;
    DefaultBuildSettings = BuildSettingsVersion.V2;
    ExtraModuleNames.Add("TestProject"); //根据你的项目名称更改此行
}
```


## 设置起始地图和默认地图

- 在 内容浏览器 中，请确保将所有 关卡（Level） 文件都重新放置到 内容/地图（Content/Maps） 文件夹中
- 设置正确的游戏默认地图和服务器默认地图

## 构建编译

### Windows 系统

直接构建打包，最终会多生成一个  [ProjectName]Server.exe 

### Linux 系统

**Step 1 安装 交叉编译工具 下载并安装 clang**

https://docs.unrealengine.com/4.27/en-US/SharingAndReleasing/Linux/GettingStarted/

安装完成后使用如下命令验证是否安装成功：

```
%LINUX_MULTIARCH_ROOT%x86_64-unknown-linux-gnu\bin\clang++ -v
```

**Step 2 编译**


## 参考

- [UE4 不同版本对应的交叉编译工具链](https://docs.unrealengine.com/4.27/en-US/SharingAndReleasing/Linux/GettingStarted/)
- [设置专用服务器](https://docs.unrealengine.com/4.27/zh-CN/InteractiveExperiences/Networking/HowTo/DedicatedServers/)
- [UE4命令行编译工程入门 ](https://www.cnblogs.com/WoodJim/p/15965121.html)
- [UE4 如何用命令行来打包你的游戏](https://cloud.tencent.com/developer/article/2194610)
- [Automate deployment with the Unreal Engine using the Unreal Automation Tool (UAT)](https://blog.mi.hdm-stuttgart.de/index.php/2017/02/11/uat-automation/)
- [构建操作：烘焙、打包、部署与运行](https://docs.unrealengine.com/4.27/zh-CN/SharingAndReleasing/Deployment/BuildOperations/)
- [UE4 UAT BuildCookRun命令行参数](https://zhuanlan.zhihu.com/p/41931214)

