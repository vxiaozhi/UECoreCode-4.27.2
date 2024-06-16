# UE4 Lua 脚本插件

## 简介

UE 使用的是 C++ 这种编译型语言，在编译之后就成了二进制，只有通过玩家重新安装才能打到更新游戏的目的。

但是对于游戏业务而言，对于需求调整和 bug 修复时间要求非常迫切，频繁地让玩家更新 App 是不能接受的，游戏项目一般使用 Lua 作为游戏业务的脚本语言，是为了把运行时不可变的 C++ 代码变成运行时可更新的 Lua 代码。

UE 官方没有提供 Lua 的支持，UnLua是Tencent针对UE4的脚本解决方案，其目标是使用lua脚本来代替蓝图来编写业务逻辑，提升开发效率和降低维护成本。目前已在github上开源。


## 功能特性

1. 可在lua中通过UE4反射系统零胶水代码访问UCLASS, UPROPERTY, UFUNCTION, USTRUCT, UENUM
2. 可使用unlua提供的宏来静态导出反射体系外的类、成员函数、成员变量、全局函数和枚举
3. 可在lua中重写（Override ）c++中带有BlueprintImplementableEvent、BlueprintNativeEvent修饰的成员函数，
4. 可在lua中重写（Override ）蓝图中的所有事件（Event）和函数（Function）
5. 可在lua中重写（Override ）Replication Notify函数
6. 可在lua中重写（Override ）Animation Notify函数
7. 可在lua中重写（Override ）Input Event事件
8. lua中高效调用UE4引擎UFUCNTION（持久化参数缓存、参数传递、非常量引用参数和返回值处理）
9. lua中高效使用容器（TArray、TSet、TMap）
10. lua中支持delegate的使用
11. lua中高效访问结构体（struct）
12. 支持修饰符BlueprintCallable、BlueprintPure、Exec的UFUNCTION的缺省参数
13. 支持自定义碰撞（collision）枚举
14. 支持编辑器server/clients模拟
15. 支持从蓝图生成Lua template模板lua文件
16. 支持协程（coroutine）实现的UE4的Latent函数，同步写法完成异步逻辑

## 参考

- [A feature-rich, easy-learning and highly optimized Lua scripting plugin for UE.](https://github.com/Tencent/UnLua)
- [基于 UnLua 的 Lua 编程指南](https://ue5wiki.com/wiki/36659/)
- [Unlua编程基础](https://www.cnblogs.com/kekec/p/15110742.html)
