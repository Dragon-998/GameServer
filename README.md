# MiniGameServer

用于学习游戏服务端开发的 C++17 项目。

当前阶段实现最基础的 TCP 连接流程：创建监听 Socket，绑定 `0.0.0.0:9000`，开始监听并接受一个客户端连接。连接成功后，按回车即可退出。

## 构建

```bash
cmake -S . -B build
cmake --build build
```

Windows 使用 Visual Studio 生成器时，程序通常位于：

```powershell
.\build\Debug\MiniGameServer.exe
```

当前尚未实现 `recv`、`send`、Session、PacketCodec、Dispatcher、Handler、多客户端并发或业务逻辑。
