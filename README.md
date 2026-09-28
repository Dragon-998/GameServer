# MiniGameServer

用于学习游戏服务端开发的 C++17 项目。

当前阶段实现 TCP 字节流到 Packet 的最小解析流程：创建监听 Socket，绑定 `0.0.0.0:9000`，接受一个客户端连接，为它创建 Session，再由 PacketCodec 处理半包和粘包。

## 构建

```bash
cmake -S . -B build
cmake --build build
```

Windows 使用 Visual Studio 生成器时，程序通常位于：

```powershell
.\build\Debug\MiniGameServer.exe
```

当前协议格式为 `[length(uint32)][protocolId(uint16)][data]`，长度字段表示 data 的字节数，字段使用网络字节序。

当前尚未实现 Dispatcher、Handler、SessionManager、多客户端并发或业务逻辑。
