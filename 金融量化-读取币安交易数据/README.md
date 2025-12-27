# Binance 市场数据采集系统 / Binance Market Data Collector

## 📖 项目简介 / Project Overview

这是一个高性能的 Binance 加密货币市场数据采集系统，使用 C++17 开发，通过 WebSocket 实时连接 Binance 交易所，采集 BTC/USDT 交易对的 Best Bid and Offer (BBO) 数据，并将数据持久化存储到本地文件。

This is a high-performance Binance cryptocurrency market data collection system developed with C++17. It connects to Binance exchange via WebSocket in real-time, collects Best Bid and Offer (BBO) data for the BTC/USDT trading pair, and persists the data to local files.

### 核心特性 / Core Features

- ⚡ **高性能 / High Performance**: 使用 SIMD 指令集优化的 simdjson 库进行 JSON 解析
- 💾 **内存映射存储 / Memory Mapped Storage**: 使用 mmap 实现高效的文件 I/O
- 🔒 **缓存对齐 / Cache Alignment**: 64 字节对齐优化 CPU 缓存访问性能
- 🌐 **安全连接 / Secure Connection**: 使用 SSL/TLS 加密的 WebSocket 连接
- 📊 **实时数据 / Real-time Data**: 实时接收和处理市场行情数据

---

## 📁 项目结构 / Project Structure

```
root/
├── main.cpp           # 主程序：数据采集逻辑 / Main program: Data collection logic
├── reader.cpp         # 数据读取程序：查看存储的数据 / Data reader: View stored data
├── OrderUpdate.hpp    # 数据结构定义 / Data structure definition
├── CMakeLists.txt     # CMake 构建配置 / CMake build configuration
├── simdjson.h         # SIMD JSON 解析库头文件 / SIMD JSON parser header
└── simdjson.cpp       # SIMD JSON 解析库实现 / SIMD JSON parser implementation
```

---

## 🔧 依赖库 / Dependencies

| 库名 / Library | 版本要求 / Version | 用途 / Purpose |
|----------------|-------------------|----------------|
| Boost.Beast | 1.70+ | WebSocket 和 HTTP 通信 / WebSocket and HTTP communication |
| Boost.Asio | 1.70+ | 异步 I/O 和网络编程 / Async I/O and network programming |
| OpenSSL | 1.1+ | SSL/TLS 加密 / SSL/TLS encryption |
| simdjson | 2.0+ | 高性能 JSON 解析 / High-performance JSON parsing |
| CMake | 3.10+ | 构建系统 / Build system |
| GCC/Clang | 支持 C++17 / C++17 support | 编译器 / Compiler |

---

## 📦 安装依赖 / Installing Dependencies

### Ubuntu / Debian

```bash
# 安装 Boost 库
sudo apt-get update
sudo apt-get install libboost-all-dev

# 安装 OpenSSL
sudo apt-get install libssl-dev

# 安装 CMake
sudo apt-get install cmake

# 安装编译器（如果未安装）
sudo apt-get install build-essential g++
```

### CentOS / RHEL

```bash
# 安装 Boost 库
sudo yum install boost-devel

# 安装 OpenSSL
sudo yum install openssl-devel

# 安装 CMake
sudo yum install cmake

# 安装编译器
sudo yum groupinstall "Development Tools"
```

---

## 🚀 编译方法 / Compilation

### 方法一：使用 CMake（推荐） / Method 1: Using CMake (Recommended)

```bash
# 1. 进入项目根目录
# 1. Navigate to project root directory
cd /path/to/root

# 2. 创建构建目录
# 2. Create build directory
mkdir -p build
cd build

# 3. 配置 CMake
# 3. Configure CMake
cmake ..

# 4. 编译项目
# 4. Build project
make

# 编译成功后会生成两个可执行文件：
# After successful compilation, two executables will be generated:
# - collector: 数据采集程序 / Data collector
# - reader: 数据读取程序 / Data reader
```

### 方法二：直接使用 g++ 编译 / Method 2: Direct Compilation with g++

```bash
# 编译 collector
# Compile collector
g++ -std=c++17 -O3 \
    -I. \
    -o collector \
    main.cpp simdjson.cpp \
    -lboost_system -lssl -lcrypto -lpthread

# 编译 reader
# Compile reader
g++ -std=c++17 -O3 \
    -I. \
    -o reader \
    reader.cpp
```

---

## 📝 使用方法 / Usage

### 1. 运行数据采集程序 / Running Data Collector

```bash
# 在 build 目录下运行
# Run in build directory
./collector
```

**程序输出示例 / Example Output:**
```
Connected to Binance L3 Stream. Saving to market_data.bin...
Recorded 100 updates to disk...
Recorded 100 updates to disk...
Recorded 100 updates to disk...
...
```

**说明 / Notes:**
- 程序会持续运行，直到手动停止（Ctrl+C）
- 每接收 100 条数据输出一次日志
- 数据存储在当前目录的 `market_data.bin` 文件中

### 2. 查看采集的数据 / Viewing Collected Data

在另一个终端运行数据读取程序：

```bash
# 在 build 目录下运行
# Run in build directory
./reader
```

**输出示例 / Example Output:**
```
市场数据文件读取器
Market Data File Reader
================================================================================
文件大小: 104857600 字节
File size: 104857600 bytes
记录数量: 1638400
Record count: 1638400
实际有效记录数: 100
Actual valid records: 100
================================================================================

前 10 条记录:
First 10 records:
--------------------------------------------------------------------------------
序号   价格         数量         时间戳         方向    
--------------------------------------------------------------------------------
1      87441.04    5.76023000  1734567890123  b      
2      87441.04    5.75565000  1734567890124  b      
3      87441.04    5.75107000  1734567890125  b      
4      87441.04    5.75118000  1734567890126  b      
5      87441.04    5.73975000  1734567890127  b      
6      87441.04    5.73969000  1734567890128  b      
7      87441.04    5.75238000  1734567890129  b      
8      87441.04    5.75232000  1734567890130  b      
9      87441.04    5.75226000  1734567890131  b      
10    87441.04    5.75232000  1734567890132  b      

... (还有 90 条记录)
... (90 more records)

价格统计:
Price statistics:
  最低价格: 87441.00
  Min price: 87441.00
  最高价格: 87459.83
  Max price: 87459.83
  平均价格: 87450.50
  Avg price: 87450.50
```

---

## 📊 数据格式 / Data Format

### OrderUpdate 结构体 / OrderUpdate Structure

```cpp
struct alignas(64) OrderUpdate {
    double price;        // 价格 / Price (8 bytes)
    double quantity;     // 数量 / Quantity (8 bytes)
    long long event_time; // 事件时间戳 / Event timestamp (8 bytes)
    char side;           // 方向：b=买, s=卖 / Side: b=bid, s=ask (1 byte)
    // 剩余 39 字节为填充，用于 64 字节对齐
    // Remaining 39 bytes are padding for 64-byte alignment
};
```

### 数据文件 / Data File

- **文件名 / Filename**: `market_data.bin`
- **预分配大小 / Pre-allocated Size**: 100 MB
- **每条记录大小 / Record Size**: 64 bytes
- **最大记录数 / Max Records**: 1,638,400 条
- **存储位置 / Storage Location**: 程序运行目录

### Binance WebSocket 数据格式 / Binance WebSocket Data Format

```json
{
  "u": 400900217,
  "s": "BTCUSDT",
  "b": "87441.04000000",
  "B": "5.76023000",
  "a": "87441.05000000",
  "A": "6.75238000",
  "T": 1734567890123,
  "E": 1734567890124
}
```

**字段说明 / Field Description:**
- `u`: 更新 ID / Update ID
- `s`: 交易对 / Symbol
- `b`: 买价 / Bid price
- `B`: 买量 / Bid quantity
- `a`: 卖价 / Ask price
- `A`: 卖量 / Ask quantity
- `T`: 交易时间 / Trade time
- `E`: 事件时间 / Event time

---

## 🔍 文件详细说明 / File Details

### main.cpp

**功能 / Function:**
- 建立 WebSocket 连接到 Binance 服务器
- 解析 JSON 格式的市场数据
- 将数据写入内存映射文件

**关键类 / Key Classes:**
- `MmapStore`: 内存映射存储类，负责文件操作和数据写入

**工作流程 / Workflow:**
1. 解析 Binance WebSocket 服务器地址
2. 建立 SSL/TLS 加密连接
3. 执行 WebSocket 握手
4. 持续接收并解析 JSON 数据
5. 将解析后的数据写入内存映射文件

### reader.cpp

**功能 / Function:**
- 读取 `market_data.bin` 文件
- 过滤未初始化的数据（时间戳为 0 的记录）
- 显示前 10 条记录
- 计算并显示价格统计信息

**统计信息 / Statistics:**
- 最低价格 / Minimum price
- 最高价格 / Maximum price
- 平均价格 / Average price

### OrderUpdate.hpp

**功能 / Function:**
- 定义 `OrderUpdate` 结构体
- 使用 64 字节对齐优化缓存性能

### CMakeLists.txt

**编译配置 / Build Configuration:**
- C++17 标准
- O3 优化级别
- 链接 Boost、OpenSSL 等依赖库

---

## ⚙️ 配置说明 / Configuration

### 修改数据流 / Changing Data Stream

在 `main.cpp` 中修改 `target` 变量：

```cpp
// 订阅不同的交易对或数据流
// Subscribe to different trading pairs or data streams
const std::string target = "/ws/btcusdt@bookTicker";  // BTC/USDT BBO
const std::string target = "/ws/ethusdt@bookTicker";  // ETH/USDT BBO
const std::string target = "/ws/btcusdt@trade";       // BTC/USDT 交易流
```

### 修改存储位置 / Changing Storage Location

在 `main.cpp` 中修改文件路径：

```cpp
// 修改存储路径
// Change storage path
MmapStore store("/path/to/custom/data.bin", 1024 * 1024 * 100);
```

### 修改预分配大小 / Changing Pre-allocated Size

```cpp
// 修改预分配大小（单位：字节）
// Change pre-allocated size (in bytes)
MmapStore store("market_data.bin", 1024 * 1024 * 200);  // 200 MB
```

---

## 🐛 故障排除 / Troubleshooting

### 编译错误 / Compilation Errors

**错误 / Error:**
```
fatal error: boost/beast/core.hpp: No such file or directory
```

**解决 / Solution:**
```bash
# 安装 Boost 库
sudo apt-get install libboost-all-dev
```

### 连接错误 / Connection Errors

**错误 / Error:**
```
Error: Host not found (authoritative)
```

**解决 / Solution:**
- 检查网络连接
- 确认 DNS 解析正常
- 检查防火墙设置

### SSL 错误 / SSL Errors

**错误 / Error:**
```
Error: handshake: certificate verify failed
```

**解决 / Solution:**
- 确认 OpenSSL 已正确安装
- 检查系统时间是否正确
- 更新 CA 证书

---

## 📈 性能优化 / Performance Optimization

### 已实现的优化 / Implemented Optimizations

1. **SIMD JSON 解析 / SIMD JSON Parsing**
   - 使用 simdjson 库，利用 CPU SIMD 指令集加速 JSON 解析
   - 解析速度比传统 JSON 库快 2-3 倍

2. **内存映射 I/O / Memory Mapped I/O**
   - 使用 mmap 实现零拷贝文件访问
   - 减少用户态和内核态的数据拷贝

3. **缓存对齐 / Cache Alignment**
   - 64 字节对齐匹配 CPU Cache Line 大小
   - 提高内存访问效率

4. **编译优化 / Compilation Optimization**
   - O3 优化级别
   - 启用所有编译器优化

### 进一步优化建议 / Further Optimization Suggestions

1. **批量写入 / Batch Writing**
   - 累积多条数据后批量写入
   - 减少系统调用次数

2. **多线程处理 / Multi-threading**
   - 使用独立线程接收数据
   - 使用独立线程解析和写入数据

3. **数据压缩 / Data Compression**
   - 使用 LZ4 或 Zstd 压缩存储数据
   - 减少磁盘空间占用

---

## 📄 许可证 / License

本项目仅供学习和研究使用。

This project is for learning and research purposes only.

---

## 🤝 贡献 / Contributing

欢迎提交 Issue 和 Pull Request！

Issues and Pull Requests are welcome!

---

## 📞 联系方式 / Contact

如有问题或建议，请通过以下方式联系：

For questions or suggestions, please contact via:

- 提交 GitHub Issue / Submit GitHub Issue
- 发送邮件 / Send Email

---

## 📚 参考资料 / References

- [Binance WebSocket API](https://binance-docs.github.io/apidocs/websocket/cn/)
- [Boost.Beast Documentation](https://www.boost.org/doc/libs/release/libs/beast/)
- [simdjson Documentation](https://simdjson.org/)
- [Memory Mapped Files](https://en.wikipedia.org/wiki/Memory-mapped_file)

---

## 📅 更新日志 / Changelog

### Version 1.0.0
- ✅ 初始版本发布 / Initial release
- ✅ 支持 Binance bookTicker 数据流 / Support Binance bookTicker stream
- ✅ 实现内存映射存储 / Implement memory mapped storage
- ✅ 添加数据读取程序 / Add data reader program
- ✅ 修复时间戳问题 / Fix timestamp issue
- ✅ 添加数据过滤功能 / Add data filtering feature

---

**最后更新 / Last Updated:** 2025-11-15