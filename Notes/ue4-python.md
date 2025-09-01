# UE Python 方案

- 官方方案：UE 内置，主要用于​​自动化、工具链和编辑器扩展​​。
- github.com/20tab/UnrealEnginePython： 不仅可以用于自动化， 而且可以用来编写 GamePlay 逻辑。

## UE 官方 python

近年来，Python事实上已成为用于制作流程和3D应用程序之间的互操作性的首选语言，在媒体和娱乐行业中尤其流行。这部分要归功于它能够支持各种各样的应用程序。制作流程的复杂程度持续急剧上升，涉及到的应用程序的数量也在不断增多，拥有通用的脚本语言可以简化创建和维护大型资源管理系统的过程。

抛开这些外部原因，或与其他应用程序一起使用的需求，如果你希望在虚幻编辑器中自动化工作流程，Python也是绝佳的选择。对于刚接触编程的人员来说，它相对比较容易入门，通过PySide等模块，它可以提供创建复杂而功能全面的用户界面的能力，而且它还向社区提供很多其他非常有用的免费模块，有助于降低你的工作难度。

通过在虚幻编辑器中使用Python，你可以：

- 构建可将虚幻编辑器与你在贵组织中使用的其他3D应用程序连接在一起的大型资源管理流程或工作流程。
- 在虚幻编辑器中使耗时的资源管理任务实现自动化，例如，为静态网格体生成细节级别（LOD）。
- 以程序化的方式将内容放置在关卡中。
- 从你自己在Python中创建的UI控制虚幻编辑器。

**能用 Python 开发游戏核心逻辑吗？**

​​在Unreal Engine中，完全不推荐也不应该使用Python来开发核心游戏逻辑。​​然而，UE确实提供了与Python的集成，主要用于​​自动化、工具链和编辑器扩展​​。
不适合的原因如下：

1. 性能差距巨大​​： 游戏逻辑（尤其是在Tick中执行的代码）需要极高的性能。Python作为解释型语言，其性能与C++或Even Blueprints（最终编译为C++）相比有数量级的差距。这将是游戏的性能瓶颈。
2. 缺乏原生支持​​：UE的整个游戏框架（AActor, UActorComponent, UObject生命周期、复制、RPC、序列化等）是深度基于C++和其反射系统构建的。Python无法直接、高效地集成到这个框架中。你无法用Python定义一个继承自 AActor的类并享受引擎的所有内置功能。
3. 调试和工作流困难​​：UE编辑器的主要调试工具（断点、监视变量、热重载等）是针对Blueprints和C++设计的。Python脚本的调试将非常困难，与标准游戏开发工作流脱节。
4. 部署和打包问题​​：如何将Python逻辑打包到最终的游戏中？你需要捆绑Python解释器，管理依赖，这极大地增加了产品的复杂性和大小，并可能引入兼容性问题。


python 插件代码路径：

```
Engine\Plugins\Experimental\PythonScriptPlugin
```

在 UE4.27 版本中还属于实验性质的功能。

## 20tab/UnrealEnginePython

截至2020年4月，该项目已暂停开发：2016至2018年间20tab公司曾投入大量资源，但Epic公司在2018年突然推出了自有实现方案，且插件原作者于2019年申请的Megagrant资助也遭拒绝。

本插件最高支持虚幻引擎4.23版本，另有适用于4.24版本的分支代码和PR提交。自4.25版本起Epic重构了UProperty子系统，如需将插件移植到≥4.25的版本，需进行大量查找替换操作（主要将UProperty重命名为FProperty、Cast替换为CastField即可）。

20tab 公司开发了新的LuaMachine项目（https://github.com/rdeioris/LuaMachine/）。

## 参考

- [使用Python脚本化运行虚幻编辑器](https://dev.epicgames.com/documentation/en-us/unreal-engine/scripting-the-unreal-editor-using-python?application_version=5.4)
- [UnrealPython基础学习](https://vannyyuan.github.io/2021/01/26/unreal/UnrealPython%E5%9F%BA%E7%A1%80%E5%AD%A6%E4%B9%A0/)
- [UnrealEnginePython](https://github.com/20tab/UnrealEnginePython)
