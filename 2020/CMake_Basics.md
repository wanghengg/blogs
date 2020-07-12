# Cmake使用方法简介

## 0. Cmake简介

CMake 是一个跨平台的开源构建工具，使用 CMake 能够方便地管理依赖多个库的目录层次结构并生成 makefile 和使用 GNU make 来编译和连接程序。

## 1. 构建单个文件项目

### 1.1 使用GCC编译

假设现在我们希望编写一个函数来实现安全的 int 类型加法防止数据溢出，这个源文件没有任何依赖的源码或静态库：

```c++
#include <iostream>
#include <memory>
#define INT_MAX 2147483647
#define ERROR_DATA_OVERFLOW 2

int safeIntAdd(std::unique_ptr<int> &sum, int a, int b) {
    if (a > INT_MAX - b) {
        *sum = INT_MAX;
        return ERROR_DATA_OVERFLOW;
    }
    *sum = a + b;
    return EXIT_SUCCESS;
}

int main() {
    int a, b;
    std::cin >> a >> b;
    std::unique_ptr<int> sum(new int(1));
    int res = safeIntAdd(sum, a, b);
    std::cout << *sum << std::endl;

    return res;
}
```

可以直接使用一句简单的gcc命令编译这个文件并执行：

```
g++ main.cpp -g -Wall -m64 -std=c+=11 -o safeIntAdd
```

*参数解释：*

* -g——告诉 GCC 产生能被 GNU 调试器使用的调试信息以便调试你的程序。
* -Wall——生成所有警告信息
* -w——不生成任何警告信息
* -m64——指定编译为64位应用程序
* -std=c++11——设置使用c++11标准编译
* -o——生成可执行文件

### 1.2 使用cmake构建

如果要使用cmake来生成makefile的话，首先需要在源文件目录下新建一个CmakeLists.txt文件，cmake的所有配置都在这个文件中完成，CmakeLists.txt中的内容如下：

```cmake
cmake_minimum_required(VERSION 3.10)

project(safeIntAdd)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS -g -Wall)
message(STATUS "CMAKE_CXX_FLAGS: " "${CMAKE_CXX_FLAGS}")
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS: " "${CMAKE_CXX_FLAGS}")

add_executable(safeIntAdd main.cpp)
```

*Cmake命令解释：*

1. cmake_minimum_required: cmake的最低版本要求
2. project: 指定项目的名称
3. set: 设置普通变量，缓存变量或环境变量
4. add_executable: 使用列出的源文件构建可执行文件

