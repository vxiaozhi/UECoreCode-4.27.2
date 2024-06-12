# UE4 网络性能分析

## 方案一 Network Profiler

Network Profiler 是一款独立工具，可用于显示网络流量和性能信息，上述信息可在游戏运行时由引擎捕捉。它是一种极其有效的方式，可识别多人游戏中占用极高带宽的区域，因为您可以查看各个 actor、PRC 和属性在总带宽中的占比。

代码
```
Engine\Source\Runtime\Engine\Public\Net\NetworkProfiler.h
Engine\Source\Runtime\Engine\Private\NetworkProfiler.cpp
```
支持通过 Exec 命令动态启停

## 方案二 Networking Insights

作为 UE4 Insight 工具的一部分实现， 在UE4.25之后的版本支持。

## 参考
- [Networking Insights概览](https://docs.unrealengine.com/4.27/zh-CN/TestingAndOptimization/PerformanceAndProfiling/UnrealInsights/NetworkingInsights/)
- [网络性能分析工具](https://docs.unrealengine.com/4.27/zh-CN/InteractiveExperiences/Networking/NetworkProfiler/)
