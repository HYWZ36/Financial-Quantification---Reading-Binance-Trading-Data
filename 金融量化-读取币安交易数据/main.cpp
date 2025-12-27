// Boost.Bast 库 - 用于 WebSocket 和 HTTP 通信
// Boost.Beast library - For WebSocket and HTTP communication
#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>

// Boost.Asio 库 - 用于网络编程和异步 I/O
// Boost.Asio library - For network programming and async I/O
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>

// 标准库
// Standard library
#include <iostream>
#include <string>
#include <chrono>  // 时间处理 / Time handling

// Linux/Unix 系统调用 - 用于文件操作和内存映射
// Linux/Unix system calls - For file operations and memory mapping
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

// 项目头文件
// Project headers
#include "simdjson.h"
#include "OrderUpdate.hpp"

// 命名空间别名，简化代码
// Namespace aliases for cleaner code
namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = net::ssl;
using tcp = net::ip::tcp;

// --- 简单的 Mmap 存储类 ---
// --- Simple Mmap (Memory Mapped) Storage Class ---
class MmapStore {
    int fd;                    // 文件描述符 / File descriptor
    size_t offset = 0;         // 写入偏移量 / Write offset
    OrderUpdate* data_ptr;    // 内存映射指针 / Memory mapped pointer
public:
    // 构造函数：打开文件并创建内存映射
    // Constructor: Open file and create memory mapping
    MmapStore(const char* path, size_t size) {
        fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0644);
        if (ftruncate(fd, size) == -1) {
            std::cerr << "Failed to truncate file" << std::endl;
            exit(1);
        }
        data_ptr = (OrderUpdate*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    }

    // 写入订单更新数据
    // Write order update data
    void write(double p, double q, long long t, char s) {
        data_ptr[offset++] = {p, q, t, s};
        if (offset % 100 == 0) std::cout << "Recorded 100 updates to disk..." << std::endl;
    }

    // 析构函数：取消内存映射并关闭文件
    // Destructor: Unmap memory and close file
    ~MmapStore() { munmap(data_ptr, 1024*1024*10); close(fd); }
};

int main() {
    try {
        // Binance WebSocket 服务器配置
        // Binance WebSocket server configuration
        const std::string host = "stream.binance.com";
        const std::string port = "443";
        // 订阅 Best Bid and Offer (BBO) 数据流
        // Subscribe to Best Bid and Offer (BBO) stream
        const std::string target = "/ws/btcusdt@bookTicker";

        // 创建 I/O 上下文和 SSL 上下文
        // Create I/O context and SSL context
        net::io_context ioc;
        ssl::context ctx{ssl::context::tlsv12_client};

        // 创建 TCP 解析器和 WebSocket 流
        // Create TCP resolver and WebSocket stream
        tcp::resolver resolver{ioc};
        websocket::stream<beast::ssl_stream<tcp::socket>> ws{ioc, ctx};

        // 1. 解析地址并连接
        // 1. Resolve address and connect
        auto const results = resolver.resolve(host, port);
        boost::asio::connect(beast::get_lowest_layer(ws), results.begin(), results.end());

        // 2. 执行 SSL 握手
        // 2. Perform SSL handshake
        ws.next_layer().handshake(ssl::stream_base::client);

        // 3. 执行 WebSocket 握手
        // 3. Perform WebSocket handshake
        ws.handshake(host, target);

        std::cout << "Connected to Binance L3 Stream. Saving to market_data.bin..." << std::endl;

        // 创建内存映射存储，预分配 100MB 空间
        // Create memory mapped storage, pre-allocate 100MB space
        MmapStore store("market_data.bin", 1024 * 1024 * 100);

        // 创建 JSON 解析器和缓冲区
        // Create JSON parser and buffer
        simdjson::ondemand::parser parser;
        beast::flat_buffer buffer;

        // 无限循环：持续接收和处理数据
        // Infinite loop: Continuously receive and process data
        for (;;) {
            // 清空缓冲区
            // Clear buffer
            buffer.consume(buffer.size());

            // 读取 WebSocket 数据
            // Read WebSocket data
            ws.read(buffer);

            // 使用 simdjson 快速解析 JSON
            // Use simdjson for fast JSON parsing
            auto json_str = beast::buffers_to_string(buffer.data());
            // 注意：生产环境应该使用 padded_string 避免拷贝，但这里为了演示
            // Note: In production, use padded_string to avoid copying, but this is for demonstration
            simdjson::padded_string padded(json_str);
            auto doc = parser.iterate(padded);

            // Binance bookTicker 格式: "b": bid price, "B": bid qty, "a": ask price, "A": ask qty
            // 注意：bookTicker 流不包含事件时间戳，使用当前时间
            // Binance bookTicker format: "b": bid price, "B": bid qty, "a": ask price, "A": ask qty
            // Note: bookTicker stream doesn't include event timestamp, use current time
            std::string_view b_price_str = doc["b"].get_string();
            std::string_view b_qty_str = doc["B"].get_string();
            double b_price = std::stod(std::string(b_price_str));
            double b_qty   = std::stod(std::string(b_qty_str));

            // 使用当前时间戳（毫秒）
            // Use current timestamp in milliseconds
            auto now = std::chrono::system_clock::now();
            auto duration = now.time_since_epoch();
            long long event_time = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

            // 写入到内存映射存储
            // Write to memory mapped storage
            store.write(b_price, b_qty, event_time, 'b');
        }
    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}