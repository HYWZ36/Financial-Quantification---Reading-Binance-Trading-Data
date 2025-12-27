#ifndef ORDER_UPDATE_HPP
#define ORDER_UPDATE_HPP

// 64字节对齐，匹配 CPU Cache Line 大小以提高内存访问性能
// 64-byte alignment to match CPU Cache Line size for better memory access performance
struct alignas(64) OrderUpdate {
    double price;         // 价格 / Price
    double quantity;      // 数量 / Quantity
    long long event_time; // 事件时间戳 / Event timestamp
    char side;            // 买卖方向：'b'=买价, 'a'=卖价 / Side: 'b'=bid, 'a'=ask
};

#endif