# MiniGameServer

用于学习游戏服务端开发的 C++17 项目。

当前阶段包含最小登录、移动和 1v1 战斗流程：PacketCodec 解析 TCP 字节流，Dispatcher 根据协议号把 Packet 交给对应 Handler。登录使用固定账号 `test` / `123456`；移动使用简单的 `dx/dy` 请求；战斗使用默认地图中的训练 DummyEnemy，并通过 Battle、Warrior、BasicAttackSkill 和 BattleReport 完成一场回合制战斗。

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

战斗请求使用协议号 `1003`，数据格式为 `targetEntityId=1`；默认地图中训练 DummyEnemy 的实体 ID 为 `1`。战斗响应使用协议号 `2003`，数据中按行包含 battleId、回合、攻击、伤害、剩余 HP 和胜者。战斗属性由服务器固定为 HP=100、Attack=20、Defense=5、Speed=10。

当前尚未实现多人战斗、复杂技能、战斗持久化、SessionManager 或多客户端并发。
