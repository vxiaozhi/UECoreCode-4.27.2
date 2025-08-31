# UE4 DS 服务器综述

## 简介

开始前先看这篇文章, 完整讲述了 DS 搭建，属性同步及网络同步

- [【UE5】UE5 Dedicated Server专用服务器与网络同步](https://goulandis.github.io/2021/08/11/%E3%80%90UE5%E3%80%91UE5%20Dedicated%20Server%E4%B8%93%E7%94%A8%E6%9C%8D%E5%8A%A1%E5%99%A8%E4%B8%8E%E7%BD%91%E7%BB%9C%E5%90%8C%E6%AD%A5/)


### DS 服务器是什么

### 为什么叫 DS 服务器

###  UE 服务器特点

- 客户端服务器共用一套代码；
- 默认通信协议为UDP（应用层实现数据可靠的UDP）；
- 收发UDP数据包都在主线程（GameThread）执行。


### 与客户端区别

## 服务器流程

- init  - tick  - exit
- 与一般服务器的区别
- 帧同步 vs 状态同步

## 初始化

- map 加载
- wait and fork

## 定时器

- 三种tick方式

## 网络同步

- Actor  ReplicationGraph
- 属性同步 PushModel

## UDP可靠传输

- bunch  
- 

## UE服务器部署

## 参考

- [游戏服务端杂谈](https://zhuanlan.zhihu.com/p/699555798)
