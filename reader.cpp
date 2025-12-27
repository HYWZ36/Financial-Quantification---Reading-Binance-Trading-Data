// Boost.Beast 库 - 用于 WebSocket 和 HTTP 通信
// Boost.Beast library - For WebSocket and HTTP communication
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cstdint>

// 项目头文件
// Project headers
#include "OrderUpdate.hpp"

// 读取并显示市场数据文件
// Read and display market data file
void readMarketData(const char* filename) {
    // 以二进制模式打开文件
    // Open file in binary mode
    std::ifstream file(filename, std::ios::binary);
    
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        std::cerr << "Cannot open file: " << filename << std::endl;
        return;
    }

    // 获取文件大小
    // Get file size
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // 计算记录数量
    // Calculate number of records
    size_t recordCount = fileSize / sizeof(OrderUpdate);
    
    std::cout << "文件大小: " << fileSize << " 字节" << std::endl;
    std::cout << "File size: " << fileSize << " bytes" << std::endl;
    std::cout << "记录数量: " << recordCount << std::endl;
    std::cout << "Record count: " << recordCount << std::endl;
    std::cout << std::string(80, '=') << std::endl;

    // 读取所有记录
    // Read all records
    std::vector<OrderUpdate> records(recordCount);
    file.read(reinterpret_cast<char*>(records.data()), fileSize);

    // 过滤掉未初始化的记录（时间戳为 0 的记录）
    // Filter out uninitialized records (records with timestamp 0)
    std::vector<OrderUpdate> validRecords;
    for (size_t i = 0; i < recordCount; i++) {
        if (records[i].event_time != 0) {
            validRecords.push_back(records[i]);
        }
    }

    size_t actualCount = validRecords.size();
    std::cout << "实际有效记录数: " << actualCount << std::endl;
    std::cout << "Actual valid records: " << actualCount << std::endl;
    std::cout << std::string(80, '=') << std::endl;

    // 显示前 10 条有效记录
    // Display first 10 valid records
    size_t displayCount = std::min(actualCount, static_cast<size_t>(10));
    
    std::cout << "\n前 " << displayCount << " 条记录:" << std::endl;
    std::cout << "First " << displayCount << " records:" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    
    std::cout << std::left 
              << std::setw(6) << "序号"
              << std::setw(12) << "价格"
              << std::setw(12) << "数量"
              << std::setw(15) << "时间戳"
              << std::setw(8) << "方向"
              << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    
    for (size_t i = 0; i < displayCount; i++) {
        const auto& record = validRecords[i];
        std::cout << std::left 
                  << std::setw(6) << i + 1
                  << std::setw(12) << std::fixed << std::setprecision(2) << record.price
                  << std::setw(12) << std::fixed << std::setprecision(8) << record.quantity
                  << std::setw(15) << record.event_time
                  << std::setw(8) << record.side
                  << std::endl;
    }

    // 如果有效记录超过 10 条，显示统计信息
    // If more than 10 valid records, show statistics
    if (actualCount > 10) {
        std::cout << "\n... (还有 " << (actualCount - 10) << " 条记录)" << std::endl;
        std::cout << "... (" << (actualCount - 10) << " more records)" << std::endl;
        
        // 计算价格统计
        // Calculate price statistics
        double minPrice = validRecords[0].price;
        double maxPrice = validRecords[0].price;
        double sumPrice = 0;
        
        for (size_t i = 0; i < actualCount; i++) {
            minPrice = std::min(minPrice, validRecords[i].price);
            maxPrice = std::max(maxPrice, validRecords[i].price);
            sumPrice += validRecords[i].price;
        }
        
        std::cout << "\n价格统计:" << std::endl;
        std::cout << "Price statistics:" << std::endl;
        std::cout << "  最低价格: " << std::fixed << std::setprecision(2) << minPrice << std::endl;
        std::cout << "  Min price: " << std::fixed << std::setprecision(2) << minPrice << std::endl;
        std::cout << "  最高价格: " << std::fixed << std::setprecision(2) << maxPrice << std::endl;
        std::cout << "  Max price: " << std::fixed << std::setprecision(2) << maxPrice << std::endl;
        std::cout << "  平均价格: " << std::fixed << std::setprecision(2) << (sumPrice / actualCount) << std::endl;
        std::cout << "  Avg price: " << std::fixed << std::setprecision(2) << (sumPrice / actualCount) << std::endl;
    }

    file.close();
}

int main() {
    std::cout << "市场数据文件读取器" << std::endl;
    std::cout << "Market Data File Reader" << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    
    // 读取 market_data.bin 文件
    // Read market_data.bin file
    readMarketData("market_data.bin");
    
    return 0;
}