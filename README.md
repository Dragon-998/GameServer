# MiniGameServer

用于学习游戏服务端开发的 C++17 项目。

当前阶段实现 Packet 分发：PacketCodec 解析 TCP 字节流，Dispatcher 根据协议号把 Packet 交给对应 Handler。Login、Move、Fight Handler 当前只打印日志。

## 构建

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Windows 使用 Visual Studio 生成器时，程序通常位于：

```powershell
.\build\Debug\MiniGameServer.exe
```

当前协议格式为 `[length(uint32)][protocolId(uint16)][data]`，长度字段表示 data 的字节数，字段使用网络字节序。

当前尚未实现游戏业务、SessionManager 或多客户端并发。
