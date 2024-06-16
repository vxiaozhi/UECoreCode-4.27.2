# UE4 中的相机

## 简介


## 投射模式中：透视和正交的区别

举个简单的例子来说明正交投影与透视投影照相机的区别。使用透视投影照相机获得的结果是类似人眼在真实世界中看到的有“近大远小”的效果（如下图中的(a)）；而使用正交投影照相机获得的结果就像我们在数学几何学课上老师教我们画的效果，对于在三维空间内平行的线，投影到二维空间中也一定是平行的（如下图中的(b)）。

![projection-mode](imgs/projection-mode.webp)


那么，你的程序需要正交投影还是透视投影的照相机呢？一般说来，对于制图、建模软件通常使用正交投影，这样不会因为投影而改变物体比例；而对于其他大多数应用，通常使用透视投影，因为这更接近人眼的观察效果。当然，照相机的选择并没有对错之分，你可以更具应用的特性，选择一个效果更佳的照相机。


## 参考

- [UE基础-Camera系统（一）](https://zhuanlan.zhihu.com/p/642207659)
- [UE基础-Camera系统（二）](https://zhuanlan.zhihu.com/p/656535805)
- [https://dev.epicgames.com/documentation/en-us/unreal-engine/cameras-in-unreal-engine?application_version=5.4](https://dev.epicgames.com/documentation/en-us/unreal-engine/cameras-in-unreal-engine?application_version=5.4)


