# UE4 模块编译

在开发UE4 C++项目中，需要经常对C++类进行修改操作。

如果每次修改后都重新一次编译全部文件，则需要大量的时间。

有一种比较快速的方式，只编译修改后的文件，迅速反应到UE4 launcher中，减少开发时间。

方法如下：

分别打开UE4 C++ 工程中的 .sln 和 .uproject

首先进入VS中，修改C++中的文件。

回到UE4 Launcher中

在Window - Developer Tools - Modules 面板

点击Reload -- Recompile -- Reload，重新编译修改后的文件。

![module-compile](imgs/ue4-module-compile.png)


## 参考


