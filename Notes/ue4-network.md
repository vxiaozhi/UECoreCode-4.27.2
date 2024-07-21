# UE4 网络模块          {#page-network}
=============

## 术语概念

### 1. 基本网络通信

- NetDriver 网络驱动，实际上我们创建使用的是他的子类IPNetDriver，里面封装了基本的同步Actor的操作，初始化客户端与服务器的连接，建立属性记录表，处理RPC函数，创建Socket，构建并管理当前Connection信息，接收数据包等等基本操作。NetDriver与World一一对应，在一个游戏世界里面只存在一个NetDriver。UE里面默认的都是基于UDPSocket进行通信的。
- Connection 表示一个网络连接。服务器上，一个客户端到一个服务器的一个连接叫一个ClientConnection。在客户端上，一个服务器到一个客户端的连接叫一个ServerConnection。
- LocalPlayer 本地玩家，一个客户端的窗口ViewportClient对应一个LocalPlayer，Localplayer在各个地图切换时不会改变。
- Channel 数据通道，每一个通道只负责交换某一个特定类型特定实例的数据信息。ControlChannel：客户端服务器之间发送控制信息，主要是发送接收连接与断开的相关消息。在一个Connection中只会在初始化连接的时候创建一个该通道实例。
- VoiceChannel：用于发送接收语音消息。在一个Connection中只会在初始化连接的时候创建一个该通道实例。
- ActorChannel：处理Actor本身相关信息的同步，包括自身的同步以及子组件，属性的同步，RPC调用等。每个Connection连接里的每个同步的Actor都对应着一个ActorChannel实例。
- PlayerController 玩家控制器，对应一个LocalPlayer，代替本地玩家控制游戏角色。同时对应一个Connection，记录了当前的连接信息，这和RPC以及条件属性复制都是密切相关的。另外，PlayerController记录他本身的ViewTarget（就是他控制额Character），通过与ViewTarget的距离（太远的Actor不会同步）来进行其他Actor的同步处理。
- World 游戏世界，任何游戏逻辑都是在World里面处理的，Actor的同步也受World控制，World知道哪些Actor应该同步，保存了网络通信的基础设施NetDriver。
- Actor 在世界存在的对象，没有坐标。UE4大部分的同步功能都是围绕Actor来实现的。
- Dormant 休眠，对于休眠的Actor不会进行网络同步


### 2. 底层通信：

- ACK(ACKnowledge Character) 在数据传输通信中，接收方发送给发送方的一种传输控制字符，它表示确认发来的数据已经接受无误。UE4中Ack是与Bunch同级别概念的网络数据串，用于实现UDP的可靠数据传输。
- NAK(Negative Acknowlegement) 是否定应答或者非应答的缩写。它是一个用于数据通信中确认数据收到但有小错误的信号。
- RawPacket 收到的 UDP 原始数据包 经由 NetDriver 转发给 Connection, RawPacket 在 Connection 中经过中间件 PacketHandler 的过滤（比如解压缩等），得到 Packet。
- Packet 从Socket读出来/输出的数据，一个Packet里面可能有多个Bunch数据或者Ack数据
- RawBunch 经过 ActorChannel 的排序合并，得到 Bunch
- Bunch 一个Bunch里面主要记录了Channel信息，NGUID。同时包含其他的附属信息如是否是完整的Bunch，是否是可靠等，可以简单理解为一个从逻辑上层分发下来的同步数据包，该数据包的数据可能不完整，Bunch分为属性Bunch以及RPCBunch。继承自FNetBitWriter InBunch：从Channel接收的数据流串 ，UNetConnection::ReceivedPacket的时候创建 OutBunch：从Channel产生的数据流串，UActorChannel::ReplicateActor()的时候创建
- FBitWriter 字节流书写器，可以临时写入比特数据用于传输，存储等，继承自FArchive
- FSocket 所有平台Socket的基类。 FSocketBSD：使用winSocket的Socket封装
- UPackageMap 生成与维护Object与NGUID的映射，负责Object的序列化。每一个Connection对应一个UPackageMap
- PacketHandler ：网络包预处理，比如加密，前向纠错，握手等。里面有一个或多个HandlerComponents来执行特殊的数据处理。目前内置的包括加密组件RSA，AES，以及必备的握手组件StatelessConnectHandlerComponent
（Packet与Bunch的区别：Bunch是Packet子集，Packet里面可能不包含Bunch信息，只包含Ack数据）

### 3. 属性同步相关

- FObjectReplicator 属性同步的执行器，每个Actorchannel对应一个FObjectReplicator，每一个FObjectReplicator对应一个对象实例。设置ActorChannel通道的时候会创建出来。
- FRepState 针对每个连接同步的历史数据，记录同步前用于比较的Object对象信息，存在于FObjectReplicator里面。
- FRepLayOut 同步的属性布局表，记录所有当前类需要同步的属性，每个类或者RPC函数有一个。
- FRepChangedPropertyTracker 属性变化轨迹记录，一般在同步Actor前创建，Actor销毁的时候删掉。
- FReplicationChangelistMgr 存放当前的Object对象，保存属性的变化历史记录


## 关键类

![net-class](imgs/ue4-network-class.png)

其中：

- NetDriver 管理 Socket 和 NetConnection
- NetConnection 管理 Channel
- ControlChannel 会将消息转发给 FNetworkNotify（实际接收者是 UWorld 或 UPendingNetGame）
- PacketHandler 是充当一个网络通信中间件的概念，负责握手、数据包过滤等. 也是个管理者的角色，实际干活的是 HandlerComponent
- 值得注意的是，不同 Connection 可以有不同的 Packethandler 实例

## 报文结构

参考 QQ文档 [UE4协议报文结构定义](https://docs.qq.com/sheet/DSldjT1JuQ1lqTFRu?tab=BB08J2)


## 连接建立过程

## 网络同步

## 源码分析

### 1. packet 收包发包入口

收包 

```
void UNetConnection::ReceivedRawPacket( void* InData, int32 Count )
{
    //...
    FBitReader Reader(Data, BitSize);

    // Set the network version on the reader
    Reader.SetEngineNetVer( EngineNetworkProtocolVersion );
    Reader.SetGameNetVer( GameNetworkProtocolVersion );

    if (Handler.IsValid())
    {
        Handler->IncomingHigh(Reader);
    }

    if (Reader.GetBitsLeft() > 0)
    {
        ReceivedPacket(Reader);

        // Check if the out of order packet cache needs flushing
        FlushPacketOrderCache();
    }
    //...
}
```
```

void UNetConnection::ReceivedPacket( FBitReader& Reader, bool bIsReinjectedPacket)
{
    // ...
    // Read packet header
		FNetPacketNotify::FNotificationHeader Header;
		if (!PacketNotify.ReadHeader(Header, Reader))
		{
			CLOSE_CONNECTION_DUE_TO_SECURITY_VIOLATION(this, ESecurityEvent::Malformed_Packet, TEXT("Failed to read PacketHeader"));
			return;
		}
    // ...
}

	/**
	 * @brief 这是网络数据的包头结构，每个数据包会携带当前的序列号信息。
	 * OutSeq 是发送序列号，当 FlushNet 发包的时候才会自增；
	 * InAckSeq 是接收序列号，当我们收包的时候，不管是 Ack 还是 Nak，都会自增；
	 * WrittenHistoryWordCount 是记录的历史序列号的数量对 BitsPerWord 求余的结果，最小是1，最大是8。
	 * 
	 * 包头序列化的时候会压缩在一个 uint32 中，14 位的 Seq，14 位的 AckedSeq，4位的 HistoryWordCount。 
	 * 4位是因为历史记录数组最大数量是8，14位是因为兼容历史？
	 * 
	 */
	struct FNotificationHeader
	{
		SequenceHistoryT History;
		SIZE_T HistoryWordCount;  // = WrittenHistoryWordCount
		SequenceNumberT Seq;	// = OutSeq
		SequenceNumberT AckedSeq;	// = InAckSeq
	};



```

```
// These methods must always write and read the exact same number of bits, that is the reason for not using WriteInt/WrittedWrappedInt
bool FNetPacketNotify::WriteHeader(FBitWriter& Writer, bool bRefresh)
{
	// we always write at least 1 word
	SIZE_T CurrentHistoryWordCount = FMath::Clamp<SIZE_T>((GetCurrentSequenceHistoryLength() + SequenceHistoryT::BitsPerWord - 1u) / SequenceHistoryT::BitsPerWord, 1u, SequenceHistoryT::WordCount);

	// We can only do a refresh if we do not need more space for the history
	if (bRefresh && (CurrentHistoryWordCount > WrittenHistoryWordCount))
	{
		return false;
	}

	// How many words of ack data should we write? If this is a refresh we must write the same size as the original header
	WrittenHistoryWordCount = bRefresh ? WrittenHistoryWordCount : CurrentHistoryWordCount;
	// This is the last InAck we have acknowledged at this time
	WrittenInAckSeq = InAckSeq;

	SequenceNumberT::SequenceT Seq = OutSeq.Get();
	SequenceNumberT::SequenceT AckedSeq = InAckSeq.Get();

	// Pack data into a uint
	uint32 PackedHeader = FPackedHeader::Pack(Seq, AckedSeq, WrittenHistoryWordCount - 1);

	// Write packed header
	Writer << PackedHeader;

	// Write ack history
	InSeqHistory.Write(Writer, WrittenHistoryWordCount);

	UE_LOG_PACKET_NOTIFY(TEXT("FNetPacketNotify::WriteHeader - Seq %u, AckedSeq %u bReFresh %u HistorySizeInWords %u"), Seq, AckedSeq, bRefresh ? 1u : 0u, WrittenHistoryWordCount);

	return true;
}

bool FNetPacketNotify::ReadHeader(FNotificationHeader& Data, FBitReader& Reader) const
{
	// Read packed header
	uint32 PackedHeader = 0;	
	Reader << PackedHeader;

	// unpack
	Data.Seq = FPackedHeader::GetSeq(PackedHeader);
	Data.AckedSeq = FPackedHeader::GetAckedSeq(PackedHeader);
	Data.HistoryWordCount = FPackedHeader::GetHistoryWordCount(PackedHeader) + 1;

	// Read ack history
	Data.History.Read(Reader, Data.HistoryWordCount);

	UE_LOG_PACKET_NOTIFY(TEXT("FNetPacketNotify::ReadHeader - Seq %u, AckedSeq %u HistorySizeInWords %u"), Data.Seq.Get(), Data.AckedSeq.Get(), Data.HistoryWordCount);

	return Reader.IsError() == false;
}
```


### 2. 握手流程（连接层握手）

### 3. 握手流程（应用层握手）

### 4. UNetConnection 创建管理的
- 什么时机创建
- 创建后保存位置



## 参考

- [《Exploring in UE4》网络同步原理深入（上）](https://blog.uwa4d.com/archives/USparkle_Exploring.html)
- [《Exploring in UE4》网络同步原理深入（下）](https://blog.uwa4d.com/archives/USparkle_Exploring1.html)
- [UnrealEngine - 网络同步之连接篇](https://www.cnblogs.com/lawliet12/p/17332897.html)
- [UE4 网络模块剖析](https://www.zhyingkun.com/ue4network/ue4network/)
- [UDP可靠性详解](https://github.com/qqwx1986/ue4_doc/blob/master/UDP%E5%8F%AF%E9%9D%A0%E6%80%A7%E8%AF%A6%E8%A7%A3.md)
- [UE4 网络同步框架源码解析](https://zhuanlan.zhihu.com/p/640723352)
- [《Exploring in UE4》关于网络同步的理解与思考[概念理解]](https://www.gameres.com/806296.html)
- [UE4 UDP是如何进行可靠传输的](https://zhuanlan.zhihu.com/p/372375535)
- [[UE4]一分钟实现聊天系统](https://www.cnblogs.com/timy/p/9992030.html)
- [UE4 RPC在C++中的使用简例](https://gwb.tencent.com/community/detail/123007)
- [UE4 RPC中的序列化](https://lawnight.github.io/server/serialization/)
- [UE4-多人游戏框架理解](https://stonelzp.github.io/ue4-multiplay-framework/)
