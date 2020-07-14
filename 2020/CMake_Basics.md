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

project(SafeIntAdd)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS -g -Wall)
message(STATUS "CMAKE_CXX_FLAGS: " "${CMAKE_CXX_FLAGS}")
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS: " "${CMAKE_CXX_FLAGS}")

add_executable(SafeIntAdd main.cpp)
```

*Cmake命令解释：*

1. cmake_minimum_required: cmake的最低版本要求
2. project: 指定项目的名称
3. set: 设置普通变量，缓存变量或环境变量
4. add_executable: 使用列出的源文件构建可执行文件

有几个需要注意的点：

1. cmake的指令不区分大小写

2. 在使用set指定指定CMAKE_CXX_FLAGS的时候通过空格来分隔多个编译选项，生成的CMAKE_CXX_FLAGS字符串是“-g;-Wall”，需要用字符串替换将分号替换为空格

3. message可以在构建的过程中向stdout输出一些信息，上面的例子中的输出信息为：

    ```bash
    -- CMAKE_CXX_FLAGS: -g;-Wall
    -- CMAKE_CXX_FLAGS: -g -Wall
    ```

4. 类似于bash脚本，在CMakeLists.txt中输出变量时要使用`“${CMAKE_CXX_FLAGS}”`的形式，而不能直接使用`CMAKE_CXX_FLAGS`

    编辑好`CMakeLists.txt`之后，我们可以新建一个build目录，并在build目录下使用cmake来进行构建，构建成功的话再使用make进行编译和链接，最终的到`safeIntAdd`这个可执行文件:

    ```bash
    wangheng@wangheng-PC:~/桌面/code/github/blogs/2020/code/safeIntAdd$ mkdir build
    wangheng@wangheng-PC:~/桌面/code/github/blogs/2020/code/safeIntAdd$ cd build/
    wangheng@wangheng-PC:~/桌面/code/github/blogs/2020/code/safeIntAdd/build$ cmake ..
    -- The C compiler identification is GNU 7.5.0
    -- The CXX compiler identification is GNU 7.5.0
    -- Check for working C compiler: /usr/bin/cc
    -- Check for working C compiler: /usr/bin/cc -- works
    -- Detecting C compiler ABI info
    -- Detecting C compiler ABI info - done
    -- Detecting C compile features
    -- Detecting C compile features - done
    -- Check for working CXX compiler: /usr/bin/c++
    -- Check for working CXX compiler: /usr/bin/c++ -- works
    -- Detecting CXX compiler ABI info
    -- Detecting CXX compiler ABI info - done
    -- Detecting CXX compile features
    -- Detecting CXX compile features - done
    -- CMAKE_CXX_FLAGS: -g;-Wall
    -- CMAKE_CXX_FLAGS: -g -Wall
    -- Configuring done
    -- Generating done
    -- Build files have been written to: /home/wangheng/桌面/code/github/blogs/2020/code/safeIntAdd/build
    wangheng@wangheng-PC:~/桌面/code/github/blogs/2020/code/safeIntAdd/build$ make
    Scanning dependencies of target SafeIntAdd
    [ 50%] Building CXX object CMakeFiles/SafeIntAdd.dir/main.cpp.o
    [100%] Linking CXX executable SafeIntAdd
    [100%] Built target SafeIntAdd
    wangheng@wangheng-PC:~/桌面/code/github/blogs/2020/code/safeIntAdd/build$ ./SafeIntAdd
    210000000 21000000
    231000000
    ```

    ## 2. 构建多个文件

    ### 2.1 使用GCC编译

    假设现在我们希望将加法函数放到单独的文件中去，并在main函数所在的源文件中包含这个文件：

    ```c++
    // main.cpp
    #include <iostream>
    #include "./lib/math.h"
    #include "./definition/error_code.h"
    
    int main() {
        int a{0}, b{0}, c{0};
        std::cin >> a >> b >> c;
        int sum(0);
        int ret_val = safeAdd(sum, a, b, c);
        std::cout << sum << std::endl;
    
        return ret_val;
    }
    ```

    ```c++
    // lib/math.h
    #include <limits>
    #include "../definition/error_code.h"
    
    template<typename ValueType>
    ValueType ValueTypeMax(ValueType) {
        return std::numeric_limits<ValueType>::max();
    }
    
    template<typename ValueType>
    int safeAdd(ValueType &sum) {
        return exit_success;
    }
    
    template<typename ValueType, typename ...ValueTypes>
    int safeAdd(ValueType &sum, const ValueType &value, const ValueTypes &...other_values) {
        int ret_val = safeAdd<ValueType>(sum, other_values...);
        if (ret_val != exit_success) {
            return ret_val;
        }
    
        if (sum > ValueTypeMax(value) - value) {
            sum = ValueTypeMax(value);
            return error_data_overflow;
        }
    
        sum += value;
        return exit_success;
    }
    ```

    ```c++
    // definition/error_code.h
    #ifndef DEFINITION_ERROR_CODE_H
    #define DEFINITION_ERROR_CODE_H
    
    constexpr int exit_success = 0;
    constexpr int exit_failure = 1;
    constexpr int error_data_overflow = 2;
    
    #endif
    ```

    再使用GCC编译的时候可以使用-I参数指定头文件所在的目录：

    ```bash
    wangheng@wangheng-PC:~/桌面/code/github/blogs/2020/code/multiplefiles$ g++ -g -Wall -std=c++11 -Ilib -Idefinition -o SafeAdd main.cpp 
    wangheng@wangheng-PC:~/桌面/code/github/blogs/2020/code/multiplefiles$ ./SafeAdd 
    20000 50000 80000
    150000
    ```

    ### 2.2 使用cmake构建

    ```cmake
    cmake_minimum_required(VERSION 3.10)
    
    project(SafeIntAdd)
    
    set(CMAKE_CXX_COMPILER "c++")
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED True)
    set(CMAKE_CXX_FLAGS -g -Wall)
    string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    
    include_directories(lib/ definition/)
    
    aux_source_directory(./ SOURCE_DIR)
    
    add_executable(SafeIntAdd ${SOURCE_DIR})
    ```

    相比于构建单个文件，我们额外使用了两个指令：

    1. <font color=red>include_directories</font>：添加多个头文件搜索路径，路径之间用空格分隔；如果将 lib 和 definition 目录都添加到到搜索路径的话，在 include 的时候就不需要使用相对路径了
    2. <font color=red>aux_source_directory</font>：在目录中查找所有源文件，并将这些源文件存储在变量 SOURCE_DIR 中；需要注意这个指令不会递归包含子目录

    接下来进入build目录进行构建：

    ```bash
    wangheng@wangheng-PC:~/桌面/code/github/blogs/2020/code/multiplefiles/build$ rm -rf *
    wangheng@wangheng-PC:~/桌面/code/github/blogs/2020/code/multiplefiles/build$ cmake ..
    -- The C compiler identification is GNU 7.5.0
    -- The CXX compiler identification is GNU 7.5.0
    -- Check for working C compiler: /usr/bin/cc
    -- Check for working C compiler: /usr/bin/cc -- works
    -- Detecting C compiler ABI info
    -- Detecting C compiler ABI info - done
    -- Detecting C compile features
    -- Detecting C compile features - done
    -- Check for working CXX compiler: /usr/bin/c++
    -- Check for working CXX compiler: /usr/bin/c++ -- works
    -- Detecting CXX compiler ABI info
    -- Detecting CXX compiler ABI info - done
    -- Detecting CXX compile features
    -- Detecting CXX compile features - done
    -- Configuring done
    -- Generating done
    -- Build files have been written to: /home/wangheng/桌面/code/github/blogs/2020/code/multiplefiles/build
    wangheng@wangheng-PC:~/桌面/code/github/blogs/2020/code/multiplefiles/build$ make
    Scanning dependencies of target SafeIntAdd
    [ 50%] Building CXX object CMakeFiles/SafeIntAdd.dir/main.cpp.o
    [100%] Linking CXX executable SafeIntAdd
    [100%] Built target SafeIntAdd
    wangheng@wangheng-PC:~/桌面/code/github/blogs/2020/code/multiplefiles/build$ ./SafeIntAdd 
    1234 3434 434321
    438989
    ```

    ## 3. 构建依赖于静态库的项目

    