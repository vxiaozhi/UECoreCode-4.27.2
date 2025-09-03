# UE4 Lua 脚本插件

UE4 集成 Lua 有多种方案，已知的方案有：

**n内置ScriptPlugin**
- [UE4 内置 ScriptPlugin](https://github.com/erikluo/UnrealEngine/tree/4.23/Engine/Plugins/ScriptPlugin)

**Tencent 的 lua 方案**
- [Tencent UnLua](https://github.com/Tencent/UnLua)
- [Tencent slua-unreal](https://github.com/Tencent/sluaunreal) slua-unreal作为unreal引擎的插件，通过unreal自带蓝图接口的反射能力，结合libclang静态c++代码分析，自动化导出蓝图接口和静态c++接口，提供给lua语言，使得可以通过lua语言开发unreal游戏业务逻辑.作为腾讯PUBG手游和潘多拉系统，该系统用于腾讯UE4游戏业务.

**其它 的 lua 方案**
- [LuaMachine](https://github.com/rdeioris/LuaMachine) 该方案不像 Tencent 的那样提供 UE 内组件的绑定，而是仅支持在蓝图中调用 Lua，功能相对较弱。


**Tencent 的 js 方案**
- [Tencent PuerTS - Unity/Unreal/Dotnet 下的TypeScript编程解决方案](https://github.com/Tencent/puerts)

**C# 集成方案**
- [UnrealCLR - 虚幻引擎 .NET 6 集成](https://github.com/nxrighthere/UnrealCLR)
- [C# plugin for Unreal Engine 4](https://github.com/pixeltris/USharp)
- [C# plugin for Unreal Engine 5](https://github.com/UnrealSharp/UnrealSharp)



## UnLua

UnLua是Tencent针对UE4的脚本解决方案，其目标是使用lua脚本来代替蓝图来编写业务逻辑，提升开发效率和降低维护成本。目前已在github上开源。

UE 使用的是 C++ 这种编译型语言，在编译之后就成了二进制，只有通过玩家重新安装才能打到更新游戏的目的。

但是对于游戏业务而言，对于需求调整和 bug 修复时间要求非常迫切，频繁地让玩家更新 App 是不能接受的，游戏项目一般使用 Lua 作为游戏业务的脚本语言，是为了把运行时不可变的 C++ 代码变成运行时可更新的 Lua 代码。


### 功能特性

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

### Lua 的调试方案

- [LuaPanda](https://github.com/Tencent/LuaPanda) 是一个基于 VS Code 的 lua 代码工具

### UnLua 代码阅读笔记

**插件启动**

- UnLua::FUnLuaModule::StartupModule()
  - RegisterSettings()
  - CreateDefaultParamCollection()
  - SetActive(true)
 
    
**Lua Env 的分配**

UnLua 提供了两种 Env 分配器，分别是：

- ULuaEnvLocator
- ULuaEnvLocator_ByGameInstance

默认的分配器为ULuaEnvLocator，会将所有 `UObject` 都分配到同一个Lua环境里，这通常适用于绝大部分的应用场景。

但有时候我们可能会有环境隔离的需求，比如将同一个 `GameInstance` 下的所有对象放在同一个环境里，在游戏退出时一次性释放所有该游戏实例相关的资源。又或者一些Lua游戏UI框架没有为单进程多游戏实例设计，在单进程启动多个游戏实例时会出现对象冲突的问题，此时就可以选择使用 `ULuaEnvLocator_ByGameInstance` 来做好相互隔离。

你也可以继承 `ULuaEnvLocator` 来实现自己的分配逻辑，但要注意的是，隔离并不是沙箱，它们依然可以通过UE接口访问到其它环境中的对象。

以 ULuaEnvLocator 为例，其创建 LuaState 的流程为：
- ULuaEnvLocator::Locate
  - FLuaEnv::FLuaEnv()
    - lua_newstate

**ULuaEnvLocator::Locate 何时被调用？**


## PuerTS

吃鸡手游的成功，让手游多了一种引擎选择：UE4。于是陆续有人来问xLua的UE4版本。要做UE4版本，由于宿主语言的不同其实相当于完全重新开发。我想既然都重新开发了，能否重新考虑当年xLua的一些技术决策点，放在UE，放在那么多年后的今天是否仍然合适。

xLua的开发是在2015年初，那时苹果刚要求应用提交64位版本，unity刚为此做了il2cpp。il2cpp早期占用空间大，而同期iOS应用的允许的代码段却很小（ios7以前版本40M，ios7是60M），去掉引擎本身的占用应用捉襟见肘，那时作为一个第三方库体积是很关键的。而lua在体积方面十分优秀，100K的大小把高级语言的常用特性都支持了，所以lua在当时是个十分恰当的选择。然而到了今天，ios9以上代码段限制是900M，lua在这方面的优势已经不再明显。

宿主语言是C#的话，C#本身有运行时类型校验，也有良好的反射机制，lua的动态类型并没有太大的影响，传错类型（c#的复杂类型在lua侧都以userdata代表）顶多会抛个异常，不会有严重的后果。但在UE4，宿主语言是C++，C++本身没运行时检查，而要在lua侧记录类型信息并动态校验开销会比较大，可能因为这个原因或者别的因素不少lua方案不做校验直接传，这可能会导致十分严重的后果，比如一个c++函数参数要求的是FVector指针，并修改其Z字段，在lua那错传了FVector2D指针，由于C++不会校验指针类型，所以会产生越界写（某ue4的lua方案真实案例）。这种问题能当场crash你应该谢天谢地，最怕它只是修改了别的模块的数据，一系列连锁反应产生些随机的错误，那才是噩梦。但是在996行业背景下，这种昏头操作难以避免。

还有随着lua在游戏项目应用逐渐重度使用，甚至出现所谓的全lua游戏，lua代码量多了，即使在Unity项目其动态类型也逐渐带来一系列问题：

- 没有静态类型，IDE理论上不可能做到准确的代码提示和跳转，而且由于lua比较小众，IDE大厂都没有直接支持，对开发效率有一定的影响。
- 没有静态类型检查，大项目很难做重构，随着技术债的积累会越来越难维护。
- 一些拼写错误，类型错误，得在运行时才能发现，然后靠肉眼排查。

最后一点是lua的生态，严格来说前面说的“静态类型检查”也是生态的衍生品。生态说白了就是有多少人投入进来，人多力量大，像安德斯·海尔斯伯格这种大神可以为js生态创造一个ts，而一般人的使用和反馈bug也是一种贡献：帮来人踩坑。用lua的时候，很多时候你得自己造轮子，自己踩坑。

**why typescript？**

所以，我在UE4的脚本方案选型，加入了两点要求：

1. 支持静态类型检查；
2. 有良好的生态：包括工具链，库，文档；

最终我锁定了typescript/javascript，typescript有静态类型，由大神操刀设计，语法优雅，而且IDE有大厂支持，应用很广泛，它最近两年已经跻身github十大活跃语言：

## ScriptPlugin

## 参考

- [A feature-rich, easy-learning and highly optimized Lua scripting plugin for UE.](https://github.com/Tencent/UnLua)
- [基于 UnLua 的 Lua 编程指南](https://ue5wiki.com/wiki/36659/)
- [Unlua编程基础](https://www.cnblogs.com/kekec/p/15110742.html)
- [为什么说 Lua 5.3 中没有全局变量了](https://luyuhuang.tech/2020/03/20/lua53-environment.html)
- [lua 和 cpp 互调](https://www.jianshu.com/p/2496e646a3a7)
- [Lua与C/C++互操作](https://www.cnblogs.com/kekec/p/12879896.html)
- [Lua中的点、冒号与self](https://www.cnblogs.com/suoluo/p/7368276.html)
