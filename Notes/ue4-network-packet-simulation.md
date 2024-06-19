# UE4 

网络模拟 可以模拟服务器和客户端的延迟和数据包丢失。这对于识别网络环境中可能出现的问题至关重要。


## 如何使用

### step1

要使用网络模拟，前往 项目设置（Project Settings） > 关卡编辑器（Level Editor） > 运行（Play） > 多人游戏选项（Multiplayer Options），然后将 启用网络模拟（Enable Network Emulation） 设置为 启用（enabled）。此操作将启用网络模拟设置。



### step2

接着可在控制台中通过以下命令模拟网络延迟和丢包，用来测试程序的网络性能。

- 1、Net pktLag=，模拟延迟，单位是毫秒
- 2、Net PktLagVariance=300，在模拟延迟的基础上，再上下浮动300毫秒。加上这个就会出现移动瞬移卡顿的效果
- 3、Net PKtLoss=，丢包，单位是百分比，Net PKtLoss=90就是90%会丢包，也会出现移动瞬移卡顿
- 4、Net PktOrder=1，乱序发包，会出现一定的移动瞬移，但不太明显
- 5、Net PktDup=，重复发包，单位是百分比，Net PktDup=20表示20%会出现重复发包。



## 代码实现

对应的代码实现如下：

```
void FPacketSimulationSettings::RegisterCommands()
{
	IConsoleManager& ConsoleManager = IConsoleManager::Get();
	
	// Register exec commands with the console manager for auto-completion if they havent been registered already by another net driver
	if (!ConsoleManager.IsNameRegistered(TEXT("Net PktLoss=")))
	{
		ConsoleManager.RegisterConsoleCommand(TEXT("Net PktLoss="), TEXT("PktLoss=<n> (simulates network packet loss)"));
		ConsoleManager.RegisterConsoleCommand(TEXT("Net PktOrder="), TEXT("PktOrder=<n> (simulates network packet received out of order)"));
		ConsoleManager.RegisterConsoleCommand(TEXT("Net PktDup="), TEXT("PktDup=<n> (simulates sending/receiving duplicate network packets)"));
		ConsoleManager.RegisterConsoleCommand(TEXT("Net PktLag="), TEXT("PktLag=<n> (simulates network packet lag)"));
		ConsoleManager.RegisterConsoleCommand(TEXT("Net PktLagVariance="), TEXT("PktLagVariance=<n> (simulates variable network packet lag)"));
	}
}
```

## 参考

- [使用网络模拟](https://dev.epicgames.com/documentation/en-us/unreal-engine/using-network-emulation-in-unreal-engine)
- [[UE4]模拟网络延迟和丢包](https://www.cnblogs.com/timy/p/9895354.html)