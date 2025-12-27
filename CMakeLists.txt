# CMake 最低版本要求 / Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# 项目名称 / Project name
project(HFT_Collector)

# 设置 C++ 标准为 C++17
# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)

# 编译优化选项：O3 优化 + 使用本地 CPU 指令集
# Compilation optimization: O3 optimization + use native CPU instruction set
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")

# 查找 Boost 库（需要 system 组件）
# Find Boost library (requires system component)
find_package(Boost REQUIRED COMPONENTS system)

# 查找 OpenSSL 库（用于 SSL/TLS 加密）
# Find OpenSSL library (for SSL/TLS encryption)
find_package(OpenSSL REQUIRED)

# 查找线程库
# Find Threads library
find_package(Threads REQUIRED)

# 编译 simdjson 源文件
# Compile simdjson source files
add_executable(collector main.cpp simdjson.cpp)

# 编译数据读取程序
# Compile data reader program
add_executable(reader reader.cpp)

# 设置头文件包含路径
# Set header file include directories
target_include_directories(collector PRIVATE .)
target_include_directories(reader PRIVATE .)

# 链接所需的库
# Link required libraries
target_link_libraries(collector
    Boost::system      # Boost 系统库 / Boost system library
    OpenSSL::SSL       # OpenSSL SSL 库 / OpenSSL SSL library
    OpenSSL::Crypto    # OpenSSL 加密库 / OpenSSL crypto library
    Threads::Threads   # 线程库 / Threads library
)

# 读取程序不需要网络库，只需要标准库
# Reader program doesn't need network libraries, only standard library
target_link_libraries(reader)