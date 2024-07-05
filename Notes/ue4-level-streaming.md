# UE4 流式关卡 (LevelString)

这个特性让地图文件在游戏运行状态下根据它们的可见性从内存中加载卸载，将一个世界分割成更小的几块，只有有关联的部分才占用资源，然后在某些位置渲染出来，如果设置合理，这可以制作一个非常大型且无缝切换的游戏世界，让玩家在里面感觉就像真的在一个那样大的世界中一样。注意需要取消World Composition才能使用Level Streaming。

## PersistentLevel

Persistent Level 被认为是主关卡，并用来管理流入流出的关卡，一般是当前打开的关卡。

## StreamingLevels

Streaming Levels通过Levels window来管理，它们能与Persistent Level重叠，或者创造更大的世界，每个Streaming Level 均可被设置为Always Loaded或者Blueprint这两种类型，可以在Levels window通过右键子关卡来设置。

当被设置为Always Loaded时，它总会和Persistent Level一块加载，会与Persistent Level一同可见，它会忽略分配给它的Streaming volumes，也会忽略蓝图或C++的加载卸载请求，这种类型子关卡通常被用来划分Persistent Level中的内容为多个层，所以艺术家们可以同时工作而不会阻碍其他人，在游戏中，该类型的子关卡永远不会被Stream out，除非这个改变了Persistent Level。

## 动态Streaming方法

蓝图类型的 Streaming Levels 可以通过 Level Streaming Volumes 来控制加载卸载，使用蓝图或C++能让这些关卡动态地被加载卸载。

## LevelStreamingVolumes

它让 level streaming 更简单，就一个思路：是否发送一个流送关卡的加载卸载请求基于视角是否在该流送关卡的Level Streaming Volumes中。通过两种方式：

- 在游戏中，导致关卡是否加载卸载是基于玩家的视角是否在关卡流送体积中。
- 在编辑器中，导致流送关卡是否隐藏的是透视视口摄像机的位置。

基于体积的关卡流送是容易使用的，且不需要脚本，更进一步，基于体积的关卡流送比脚本流送更简单，取决于流送体积的大小。

它有几个属性：

- Streaming Levels	显示被该体积影响的关卡
- Editor Previs Only	该流送体积是否只用来编辑器的流送关卡预览。
- Disabled	如果被设置为true，该流送体积会被忽略，让它同某个关卡分离开，让蓝图能控制该流送体积。
- Streaming Usage	决定该流送体积是否能控制加载，加载并可见，或者只是可见。

## 重要细节

- 所有的关卡流送体积必须都得在Persistent level中，在其他关卡中的流送体积不能用来关卡流送，会生成警告。
- 如果一个关卡有一个关卡流送体积，那么该关卡的其他流送方法将不能正确地运行。
- 一个流送关卡体积可以影响多个关卡，一个关卡可被多个流送关卡体积影响。
- 为了防止在关卡体积交接处频繁加载卸载关卡，会有延迟加载时间，该属性为Min Time Between Volume Unload Requests，默认是2s。

## ScriptedLevelStreaming

如果你想建立更复杂的加载卸载关卡的行为，就像是开门这样的小场景，我们需要达成某些条件(比如拿到钥匙)才能开门，你可以用蓝图或者C++来写关卡的加载卸载逻辑，主要用到的函数是Load Stream Level和Unload Stream Level，可通过关卡名字来指定加载哪个。除了这两个函数，Get Streaming Level函数能激活更多地动态流送行为。因为它让你能获取到真正的流送关卡对象，你可以改变或者获取它的状态，同Create Instance结合，你可以创建和流送某个特定子关卡的拷贝，通过应用transforms，发送参数给这些复制，你能创建动态的世界。

## 参考

- [UE4 流送关卡 ](https://muyunsoft.com/blog/Unreal4/Basic/LevelStreaming.html#levelstreaming)