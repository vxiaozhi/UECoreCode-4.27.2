# UE4 DS 服务器综述

## 简介

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
- packet

