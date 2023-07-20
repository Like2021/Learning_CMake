# 参考教程

`Cmake`使用教程：[知乎](https://zhuanlan.zhihu.com/p/500002865)、[官网](https://cmake.org/cmake/help/book/mastering-cmake/cmake/Help/guide/tutorial/index.html)（结合着看）



# 学习记录

## 1. 快速开始

首先`mkdir project && cd project`，构建一个项目，并在此目录下编写`CMakeLists.txt`：

```cmake
cmake_minimum_required(VERSION 3.15)

# set the project name
project(Tutorial)

# add the executable
add_executable(Tutorial tutorial.cpp)
```

`cmake_minimum_required` 指定使用`CMake`的最低版本号，`project` 指定项目名称，例如`Tutorial`，`add_executable` 用来生成可执行文件，需要指定生成可执行文件的名称，例如`Tutorial`和相关源文件，例如`tutorial.cpp`。



这里`mkdir build && cd build`创建一个`build`目录存放编译产物，可以避免编译产物与代码文件混在一起，这种叫做**外部构建**。

然后构建：

```bash
# 构建系统是需要指定 CMakeLists.txt 所在路径，此时在 build 目录下，所以用 .. 表示 CMakeLists.txt 在上一级目录。
cmake ..
```

此时在 build 目录下会生成 Makefile 文件，然后调用编译器来实际编译和链接项目：

```bash
# --build 指定编译生成的文件存放目录，其中就包括可执行文件，. 表示存放到当前目录
cmake --build .
# 或者
make
```

在`build`目录下生成了一个`Tutorial`可执行文件，接下来执行：

```bash
./Tutorial 10
> The square root of 10 is 3.16228
```



## 2. 优化`CMakeLists.txt`文件

#### set 与 PROJECT_NAME

指定了项目名后，后面可能会有多个地方用到这个项目名，如果更改了这个名字，就要改多个地方，比较麻烦，那么可以使用 `PROJECT_NAME` 来表示项目名。

```cmake
add_executable(${PROJECT_NAME} tutorial.cpp)
```

生成可执行文件需要指定相关的源文件，如果有多个，那么就用空格隔开，比如：

```cmake
add_executable(${PROJECT_NAME} a.cpp b.cpp c.cpp)
```

我们也可以用一个变量来表示这多个源文件：

```cmake
set(SRC_LIST a.cpp b.cpp c.cpp)
add_executable(${PROJECT_NAME} ${SRC_LIST})
```

`set` 命令指定`SRC_LIST`变量来表示多个源文件，用 `${var_name}` 获取变量的值。

于是原来的`CMakeLists.txt`文件就可以变成如下所示：

```cmake
cmake_minimum_required(VERSION 3.15)

# set the project name
project(Tutorial)

SET(SRC_LIST tutorial.cpp)

# add the executable
add_executable(${PROJECT_NAME} ${SRC_LIST})
```

这样看起来就很简洁。

#### 添加版本号和配置头文件

我们可以在`CMakeLists.txt`为可执行文件和项目提供一个版本号。首先，修改`CMakeLists.txt`文件，使用 project 命令设置项目名称和版本号。

```cmake
cmake_minimum_required(VERSION 3.15)

# set the project name and version
project(Tutorial VERSION 1.0.2)

configure_file(TutorialConfig.h.in TutorialConfig.h)
```

然后，配置头文件将版本号传递给源代码：

```cmake
configure_file(TutorialConfig.h.in TutorialConfig.h)
```

由于`TutorialConfig.h`文件这里被设置为自动写入 build 目录，因此需要将该目录添加到搜索头文件的路径列表中，也可以修改为写到其它目录。

将以下行添加到`CMakeLists.txt`文件的末尾：

```cmake
target_include_directories(${PROJECT_NAME} PUBLIC
                           ${PROJECT_BINARY_DIR}
                           )
```

`PROJECT_BINARY_DIR` 表示当前工程的二进制路径，即编译产物会存放到该路径，此时`PROJECT_BINARY_DIR` 就是 build 所在路径。

然后手动创建`TutorialConfig.h.in`文件，包含以下内容：

```cmake
// the configured options and settings for Tutorial
#define Tutorial_VERSION_MAJOR @PROJECT_VERSION_MAJOR@
#define Tutorial_VERSION_MINOR @PROJECT_VERSION_MINOR@
#define Tutorial_VERSION_PATCH @PROJECT_VERSION_PATCH@
```

当使用`CMake`构建项目后，会在`build`中生成一个`TutorialConfig.h`文件，内容如下：

```cmake
// the configured options and settings for Tutorial
#define Tutorial_VERSION_MAJOR 1
#define Tutorial_VERSION_MINOR 0
#define Tutorial_VERSION_PATCH 2
```

下一步在`tutorial.cpp`包含头文件`TutorialConfig.h`，最后通过以下代码打印出可执行文件的名称和版本号。

```cpp
// report version
std::cout << argv[0] << " Version " << Tutorial_VERSION_MAJOR << "."
        << Tutorial_VERSION_MINOR << std::endl;
std::cout << "Usage: " << argv[0] << " number" << std::endl;
```

#### 添加编译时间戳（存疑）

有时候我们需要知道编译时的时间戳，并在程序运行时打印出来。

那就需要在`CMakeLists.txt`中添加如下这句：

```cmake
string(TIMESTAMP COMPILE_TIME %Y%m%d-%H%M%S)
```

这表示将时间戳已指定格式保存到 COMPILE_TIME 变量中。

然后修改上面的`TutorialConfig.h.in`，添加：

```c++
#define COMPILE_TIME "@COMPILE_TIME@"
```

其中，@@符号包含起来的是`CMakeLists.txt`文件里的变量。

在构建项目后，`TutorialConfig.h`文件就会自动增加一句：

```text
#define COMPILE_TIME "20230720-143834"
```



这样就可以在源码中打印出`COMPILE_TIME`的值了。



**注：**

这里我尝试了更换`TIMESTAMP`，发现构建报错：

```bash
CMake Error at CMakeLists.txt:12 (string):
  string does not recognize sub-command TIMESSS


-- Configuring incomplete, errors occurred!
See also "/home/admin-main/Like/Learning_CMake/project/build/CMakeFiles/CMakeOutput.log".
```

猜测是之前构建的时候，`TIMESTAMP`已经占位为了时间戳，后续没做验证，存疑。



此外，定义时间戳加引号`"@COMPILE_TIME@"`，猜测是为了保证它是`string`，因为如果不加引号，打印出来的就不是头文件中的时间日期。



#### 指定 C++ 标准（可选）

接下来将 `tutorial.cpp`源码中的`atof`替换为 `std::stod`，这是 C++11 的特性，并删除 `#include<cstdlib>`。

```cpp
const double inputValue = std::stod(argv[1]);
```

在`CMake`中支持特定 C++标准的最简单方法是使用 `CMAKE_CXX_STANDARD` 标准变量。在`CMakeLists.txt`中设置 `CMAKE_CXX_STANDARD` 为11，`CMAKE_CXX_STANDARD_REQUIRED` 设置为True。确保在 `add_executable` 命令之前添加 `CMAKE_CXX_STANDARD_REQUIRED` 命令。

```cmake
cmake_minimum_required(VERSION 3.15)

# set the project name and version
project(${PROJECT_NAME} VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
```

需要注意的是，如果你的`gcc`编译器版本够高，也可以不用指定 C++ 版本为 11。从`GCC 6.1`开始，当不指定任何版本 C++ 标准时，默认版本是 C++ 14，如果你想用 C++17 的语言，还是需要指定的。

修改完成后，需要对代码进行重新编译 `cmake --build .`，此时可以不用进行项目构建。



## 3. 添加库

在`project/MathFunctions`目录下创建一个`CMakeLists.txt`文件，并添加以下一行：

```cmake
# MathFunctions/CMakeLists.txt
add_library(MathFunctions mysqrt.cpp)
```

表示添加一个叫`MathFunctions`的库文件。

`CMake`中的 target 有可执行文件和库文件，分别使用 `add_executable` 和 `add_library` 命令生成，除了指定生成的可执行文件名/库文件名，还需要指定相关的源文件。

此时文件结构为：

```text
step3/
    build/
    MathFunctions/
        CMakeLists.txt
        MathFunctions.h
        mysqrt.cpp
    CMakeLists.txt
    tutorial.cpp
    TutorialConfig.h.in
```

为了使用`MathFunctions`这个库，我们将在父目录里的`project/CMakeLists.txt`文件中添加一个 `add_subdirectory(MathFunctions)` 命令指定库所在子目录，该子目录下应包含其`CMakeLists.txt`文件和代码文件。

可执行文件要使用库文件，需要能够找到库文件和对应的头文件，可以分别通过 `target_link_libraries` 和 `target_include_directories` 来指定。

使用 `target_link_libraries` 将新的库文件添加到可执行文件中，使用 `target_include_directories` 将`MathFunctions`添加为头文件目录，添加到 Tutorial 目标上，以便`mysqrt.h`可以被找到。

`project/CMakeLists.txt`的最后几行如下所示：

```cmake
# add the MathFunctions library
add_subdirectory(MathFunctions)

# add the executable
add_executable(${PROJECT_NAME} tutorial.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC MathFunctions)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(${PROJECT_NAME} PUBLIC
                           ${PROJECT_BINARY_DIR}
                           ${PROJECT_SOURCE_DIR}/MathFunctions
                           )
```

`MathFunctions`库就算添加完成了，接下来就是在主函数使用该库中的函数，先在 tutorial.cpp 文件中添加头文件：

```cpp
#include "MathFunctions.h"
```

然后使用 `mysqrt` 函数即可：

```cpp
const double outputValue = mysqrt(inputValue);
```



## 4. 将库设置为可选择

现在将`MathFunctions`库设为可选的，虽然对于本教程来说，没有必要这样做，但对于较大的项目来说，这种情况很常见。

第一步是向顶级`project/CMakeLists.txt`文件添加一个选项。

```cmake
option(USE_MYMATH "Use tutorial provided math implementation" ON)
```

`option` 表示提供用户可以选择的选项。命令格式为：`option(<variable> "description [initial value])`。

`USE_MYMATH` 这个选项缺省值为 ON，用户可以更改这个值。此设置将存储在缓存中，以便用户不需要在每次构建项目时设置该值。

下一个更改是使`MathFunctions`库的构建和链接成为条件。于是创建一个 if 语句，该语句检查选项 `USE_MYMATH` 的值。

```cmake
if(USE_MYMATH)
  add_subdirectory(MathFunctions)
  list(APPEND EXTRA_LIBS MathFunctions)
  list(APPEND EXTRA_INCLUDES ${PROJECT_SOURCE_DIR}/MathFunctions)
endif()

# add the executable
add_executable(${PROJECT_NAME} tutorial.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS})

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(${PROJECT_NAME} PUBLIC
                           ${PROJECT_BINARY_DIR}
                           ${EXTRA_INCLUDES}
                           )
```