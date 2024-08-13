# UE4 PhyX物理引擎简介

游戏中的物理系统游戏物理主要是解决两个问题:

- 其一是碰撞查询（Query），比如我面前有一堵墙，我还可以走多远就会被撞到；
- 其二是物理模拟（Simulation），比如游戏角度死亡后身体该如何倒下，飞驰的载具被路上的矮石墩碰到后如何表现等。

UE4物理模块集成的是Nvidia的PhysX。这里介绍下PhysX的基础对象:

- PxScene场景对象，同一个场景中的物体才有可能进行物理碰撞等，因此场景是一个场景物体的管理者
- PxRigidActor 刚体对象（Rigidbody）的封装	
- PxShape 几何体的封装,每个 PxRigidActor 可以绑定多个 PxShape 对象, 一般是通过 PxRigidActor 来创建 PxShape

## UE4中是如何建立物理系统

- UE4.21前的版本采用的是 NVIDIA 的PhysX做为其默认的物理引擎,用于计算3D世界的碰撞查询与物理模拟。
- 自4.21版本开始改物理调用接口。在2019GDC大会上放出了正在自建Chaos物理系统的消息, 从4.22预览版中已经可以看到相关代码;官方预计4.23就可以开始使用Chaos物理系统。
- UE4自身提供 PrimitiveComponent; 拥有BodyInstance;上面记录了PhysX所需要的物理数据;会将这些信息传递给PhysX所创建的物理世界;然后由PhyX进行处理;并得到其返回的结果。



## 参考

- [UE4 PhyX物理引擎应用介绍](https://blog.csdn.net/zzZZ20150101/article/details/117127659)
- [UE4 物理系统实现](https://blog.csdn.net/qq_23030843/article/details/112495676)
- [Chaos Physics概述](https://dev.epicgames.com/documentation/zh-cn/unreal-engine/chaos-physics-overview?application_version=4.27)
- [NVIDIA PhysX SDK 4.1](https://github.com/NVIDIAGameWorks/PhysX)
- [NVIDIA PhysX 5](https://github.com/NVIDIA-Omniverse/PhysX)
