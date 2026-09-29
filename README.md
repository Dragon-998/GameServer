# MiniGameServer

用于学习游戏服务端开发的 C++17 项目。

当前阶段实现最小登录流程：PacketCodec 解析 TCP 字节流，Dispatcher 根据协议号把 Packet 交给 LoginHandler。LoginHandler 使用固定账号 `test` / `123456` 验证，并向客户端发送登录结果。Move、Fight Handler 仍只打印日志。

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

登录请求使用 `username=test&password=123456` 格式，协议号为 `1001`；登录响应使用协议号 `2001`，数据格式为 `success=1&message=login success` 或失败消息。

当前尚未实现游戏业务、SessionManager 或多客户端并发。
