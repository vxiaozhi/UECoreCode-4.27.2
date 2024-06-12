# UE4中的网络模式

网络模式 说明了计算机与联网多人游戏会话之间的关系。

## 模式定义

代码中定义如下：

```
enum ENetMode
{
	/** Standalone: a game without networking, with one or more local players. Still considered a server because it has all server functionality. */
	NM_Standalone,

	/** Dedicated server: server with no local players. */
	NM_DedicatedServer,

	/** Listen server: a server that also has a local player who is hosting the game, available to other players on the network. */
	NM_ListenServer,

	/**
	 * Network client: client connected to a remote server.
	 * Note that every mode less than this value is a kind of server, so checking NetMode < NM_Client is always some variety of server.
	 */
	NM_Client,

	NM_MAX,
};
```

- NM_Standalone 有一个或多个本地玩家且没有联网的游戏。仍被视为服务器，因为它包含所有服务器功能，但不接受来自远程客户端的连接。此模式用于单人和本地多人游戏。此模式可根据需要同时运行服务器端和客户端逻辑。
- NM_DedicatedServer 没有本地玩家的服务器。服务器仅接受来自远程客户端的连接，因此为了更高效地运行，服务器放弃了图形、声音、输入和其他面向玩家的功能。此模式常用于需要更持久、安全或大规模多玩家的游戏。
- NM_ListenServer 此服务器也有一个本地玩家，此玩家负责托管游戏。可供网络上的其他玩家链接。此模式常用于临时合作和竞技多人游戏。
- NM_Client 连接到远程服务器的客户端。客户端连接到网络多人游戏会话中的监听服务器或专用服务器。客户端不运行服务器端逻辑。

## 相关的获取网络模式的代码

```
FORCEINLINE_DEBUGGABLE ENetMode UActorComponent::GetNetMode() const
FORCEINLINE_DEBUGGABLE ENetMode UWorld::GetNetMode() const
FORCEINLINE_DEBUGGABLE ENetMode AActor::GetNetMode() const
ENetMode UNetDriver::GetNetMode() const
ENetMode UEngine::GetNetMode(const UWorld *World) const
```

## 监听服务器与专用服务器的区别

**监听服务器**

监听服务器 以简单易用为设计出发点，可供用户自发进行设置，因为用户只要有游戏副本，就可以在同一台计算机上启动监听服务器并运行。支持监听服务器的游戏常常采用游戏中用户界面（UI）来启动服务器或搜索要加入的现有服务器。监听服务器并非没有缺点。由于托管监听服务器的玩家是直接在服务器上玩游戏，因此相对于作为客户端在监听服务器上玩游戏的玩家来说，他们更有优势。这可能引发游戏公平性的问题。另一方面，既要作为服务器运行，同时还要支持图形和音频渲染等玩家相关系统，这也会带来额外的处理负载。考虑到这些因素，监听服务器不太适合极其考验竞技能力的环境，也不适合网络负载非常高的游戏，但对于小型玩家群体间进行临时合作和竞技多人游戏而言，却十分好用。

**专用服务器**

专用服务器 成本更高，更难配置。专用服务器需要独立于所有游戏参与玩家的计算机，并且需要完成自身网络连接。加入专用服务器的所有玩家使用远程网络连接体验游戏，这样更能保证公平性。由于专用服务器不渲染图形，也不执行仅对本地玩家相关的逻辑，因此还能够更高效地处理Gameplay事件并执行网络功能。因此，出于安全、公平、可靠方面的原因，专用服务器更适合需要大量玩家或高性能可靠服务器的游戏。这类游戏包括大型多人在线游戏（MMO）、竞技型多人在线战术竞技游戏（MOBA）或快节奏在线射击游戏。

## 参考

- [网络模式](https://docs.unrealengine.com/latest/INT/Gameplay/Networking/Overview/)
- [UE4中的网络模式](https://jashking.github.io/2018/06/13/ue4-netmode/)
