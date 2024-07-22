# UE4 Android 调试

## Android Studio

## VS

## gdb

## 备注

Android SDK 与 NDK 区别：

- android  sdk （Android Software Development Kit, 即android软件开发工具包）可以说只要你使用java去开发Android这个东西就必须用到。他包含了SDK Manager 和 AVD Manage，对于android系统的一些开发版本的管理以及模拟器管理。它只能运行纯java程序，有了它模拟器才可以使用。
- ndk （Native Development Kit）跟sdk差不多的是它也是一个开发工具包。用它开发c/c++是很方便的。他有一个强大的编译集合。Java调C、C++（jni接口），是一些java如何调C的代码。它会把C代码编译成一个.SO的动态库，通过jni接口用java代码去调用它，有了它我们可以直接在android代码中去添加C代码。

出现ndk的原因：

很早以前android是只有sdk的。并没有ndk。这就意味着一旦android的开发者要使用c/c++的三方库或者需要用到c/c++就必须使用非官方的法子，用java的jni去调用c/c++。就像耍小聪明走后门一样。而ndk的出现就意味着jni调用的这种方法转正了变成官方了以后你不需要再走后面大路正面随你走。如果要操作底层直接操作内存，操作地址那不得不去使用c/c++因为java这块想做这些有点困难。所以ndk是必须需要出现的。对于android来说sdk和ndk是同种语言的2种不同时期的必须品。

## 参考

- [Android Studio中三方so库的源码调试](https://www.jianshu.com/p/2a67e657b2dc)
- [Adnroid so文件动态调试技巧](https://nanshihui.github.io/2017/05/05/%E5%AE%89%E5%8D%93so%E5%8A%A8%E6%80%81%E8%B0%83%E8%AF%95%E6%8A%80%E5%B7%A7/)
- [Android Studio 调试已经编译好的带符号SO](https://blog.csdn.net/guozhongrui000/article/details/82390078)
- [UE4 Android 调试](https://docs.unrealengine.com/4.27/zh-CN/SharingAndReleasing/Mobile/Android/AndroidDebugging/)
- [UE4引擎在Android下的C++调试](https://github.com/DsoTsin/gamedev/blob/master/8.misc/debug_ue4_on_android.md)
- [VS 真机调试 Android](https://ue5wiki.com/wiki/6b9bc0ab/)
- [Android gdb调试原生代码](https://wrlus.com/android-security/android-gdb-native-debug/)
- [高效调试：命令行参数启动 UE Android App](https://imzlp.com/posts/29169/)
