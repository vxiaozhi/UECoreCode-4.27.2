# UE4 网络性能分析

## 方案一 Network Profiler

Network Profiler 是一款独立工具，可用于显示网络流量和性能信息，上述信息可在游戏运行时由引擎捕捉。它是一种极其有效的方式，可识别多人游戏中占用极高带宽的区域，因为您可以查看各个 actor、PRC 和属性在总带宽中的占比。

代码
```
Engine\Source\Runtime\Engine\Public\Net\NetworkProfiler.h
Engine\Source\Runtime\Engine\Private\NetworkProfiler.cpp
```

支持通过控制台命令动态启停

- netprofile：切换录制的开启和关闭
- netprofile enable：如果尚未录制，则开始录制
- netprofile disable：如果当前在录制，则停止录制

一些字段解释

- waste 指的是CPU花费在检查复制对象是否更改属性上的不必要时间。例如，如果一个复制的岩石角色在你的游戏中闲置不动，很可能不会发生变化，那么这将是浪费时间。但它成为了像休眠这样优化技术的一个好候选对象。

## 方案二 Networking Insights

作为 UE4 Insight 工具的一部分实现， 在UE4.25之后的版本支持。

## 参考
- [Networking Insights概览](https://docs.unrealengine.com/4.27/zh-CN/TestingAndOptimization/PerformanceAndProfiling/UnrealInsights/NetworkingInsights/)
- [网络性能分析工具](https://docs.unrealengine.com/4.27/zh-CN/InteractiveExperiences/Networking/NetworkProfiler/)
- [How to interpret Network Profiler results?](https://forums.unrealengine.com/t/how-to-interpret-network-profiler-results/436222)
- [UE4 Replication 简单优化](https://blog.csdn.net/maxiaosheng521/article/details/88292496)
