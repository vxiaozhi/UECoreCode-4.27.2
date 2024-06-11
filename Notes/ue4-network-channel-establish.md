# UE4 Channel 建立过程

## 初始 Channel 的创建


```
void UNetDriver::AddClientConnection(UNetConnection * NewConnection)
{
// ...
	CreateInitialServerChannels(NewConnection);
// ...
}
```

```

// 客户端
void UNetDriver::CreateInitialClientChannels()
{
	if (ServerConnection != nullptr)
	{
		for (const FChannelDefinition& ChannelDef : ChannelDefinitions)
		{
			if (ChannelDef.bInitialClient && (ChannelDef.ChannelClass != nullptr))
			{
				ServerConnection->CreateChannelByName(ChannelDef.ChannelName, EChannelCreateFlags::OpenedLocally, ChannelDef.StaticChannelIndex);
			}
		}
	}
}

// ds 服务端
void UNetDriver::CreateInitialServerChannels(UNetConnection* ClientConnection)
{
	if (ClientConnection != nullptr)
	{
		for (const FChannelDefinition& ChannelDef : ChannelDefinitions)
		{
			if (ChannelDef.bInitialServer && (ChannelDef.ChannelClass != nullptr))
			{
				ClientConnection->CreateChannelByName(ChannelDef.ChannelName, EChannelCreateFlags::OpenedLocally, ChannelDef.StaticChannelIndex);
			}
		}
	}
}
```
ChannelDefinitions 的定义

Engine/Config/BaseEngine.ini
```
[/Script/Engine.NetDriver]
+ChannelDefinitions=(ChannelName=Control, ClassName=/Script/Engine.ControlChannel, StaticChannelIndex=0, bTickOnCreate=true, bServerOpen=false, bClientOpen=true, bInitialServer=false, bInitialClient=true)
+ChannelDefinitions=(ChannelName=Voice, ClassName=/Script/Engine.VoiceChannel, StaticChannelIndex=1, bTickOnCreate=true, bServerOpen=true, bClientOpen=true, bInitialServer=true, bInitialClient=true)
+ChannelDefinitions=(ChannelName=Actor, ClassName=/Script/Engine.ActorChannel, StaticChannelIndex=-1, bTickOnCreate=false, bServerOpen=true, bClientOpen=false, bInitialServer=false, bInitialClient=false)
```

## ActorChannel 的创建

packet 接收时
```
void UNetConnection::ReceivedPacket( FBitReader& Reader )
{
	// Read packet header
		FNetPacketNotify::FNotificationHeader Header;
		if (!PacketNotify.ReadHeader(Header, Reader))
		{
			CLOSE_CONNECTION_DUE_TO_SECURITY_VIOLATION(this, ESecurityEvent::Malformed_Packet, TEXT("Failed to read PacketHeader"));
			return;
		}
  //...
	// Process Received data
		{
			// Parse the incoming data.
			FInBunch Bunch( this );
	}
//...
// 根据 bunch.ChIndex 找到对应的 Channel
UChannel* Channel = Channels[Bunch.ChIndex];
	if (Channel == nullptr) {
//...
	Channel = CreateChannelByName( Bunch.ChName, EChannelCreateFlags::None, Bunch.ChIndex );
}
}
```

同步Actor 时
```
int32 UNetDriver::ServerReplicateActors_ProcessPrioritizedActors( UNetConnection* Connection, const TArray<FNetViewer>& ConnectionViewers, FActorPriority** PriorityActors, const int32 FinalSortedCount, int32& OutUpdated )
{
	if ( ActorInfo == NULL && PriorityActors[j]->DestructionInfo )
		{
			// ...

			UActorChannel* Channel = (UActorChannel*)Connection->CreateChannelByName( NAME_Actor, EChannelCreateFlags::OpenedLocally );
// ...
	}
}
```




