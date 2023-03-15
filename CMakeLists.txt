cmake_minimum_required(VERSION 3.16)

project(DLRF_CPU)
# 指定C++标准为C++17
set(CMAKE_CXX_STANDARD 17)
#将./include目录添加到包含文件的搜索路径中
include_directories(./include)
if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    if (CMAKE_SIZEOF_VIOD_P EQUAL 4)
        # 检查SSE2指令是否可用
        include(CheckCXXCompilerFlag)
        check_cxx_compiler_flag("/arch:SSE2" HAS_SSE2)
    else ()
        set(HAS_SSE2 ON)
    endif ()

    if (HAS_SSE2)
        # set __SSE2__ and __XOP__ macros 设置__SSE2__和__XOP__宏
        add_definitions(-D__SSE2__ -D__XOP__)
    endif ()
endif ()

# 构建demo子项目
option(BUILD_DEMO ON)
if (BUILD_DEMO)
    MESSAGE(STATUS "BUILD DEMO PROJECT")
    add_subdirectory(demo)
endif ()

# 查找并加载必要的第三方库：benchmark、OpenMP、Armadillo、glog、BLAS和LAPACK。
# 报错请apt install cmake, openblas-devel, lapack-devel, arpack-devel, SuperLU-devel
find_package(benchmark REQUIRED) # 性能测试
find_package(OpenMP REQUIRED)# OpenMP是一种跨平台的并行化技术，可以在多种操作系统和编译器上运行
find_package(Armadillo REQUIRED)# Armadillo是一个C++的开源线性代数库
find_package(glog REQUIRED) # log
find_package(BLAS REQUIRED)   # 线性代数计算的软件库
find_package(LAPACK REQUIRED) # 线性代数计算的软件库

# 将指定目录中的所有源文件（即.cpp文件）添加到相应的变量中。
aux_source_directory(./source/data DIR_DATA) # 张量类Tensor的实现和Tensor初始化方法
aux_source_directory(./source/runtime DIR_PARSER) # 计算图结构，解析和运行时相关
aux_source_directory(./source/layer/abstract DIR_ABSTRACT_LAYER) # 算子的实现
aux_source_directory(./source/layer/details DIR_BINOCULAR_LAYER)
aux_source_directory(./source/parser DIR_PARSER) #Pnnx表达式的解析类

set(link_lib glog::glog)
IF (!WIN32)
#    如果不是window，需要链接pthread
    set(link_lib ${link_lib} pthread)
ENDIF ()

# 定义需要链接的数学库
set(link_math_lib ${ARMADILLO_LIBRARIES} ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES})
# 构建共享库DLRF_CPU
add_library(DLRF_CPU SHARED ${DIR_DATA} ${DIR_PARSER} ${DIR_ABSTRACT_LAYER} ${DIR_BINOCULAR_LAYER} ${DIR_PARSER})
# 链接需要链接的库
target_link_libraries(DLRF_CPU ${link_lib} ${link_math_lib} OpenMP::OpenMP_CXX)

# 添加需要包含的头文件路径
target_include_directories(DLRF_CPU PUBLIC ${benchmark_INCLUDE_DIRS})
target_include_directories(DLRF_CPU PUBLIC ${glog_INCLUDE_DIR})
target_include_directories(DLRF_CPU PUBLIC ${GTest_INCLUDE_DIR})
target_include_directories(DLRF_CPU PUBLIC ${Armadillo_INCLUDE_DIR})

# 添加编译选项
# SSE_MATHFUN_WITH_CODE表示使用SSE指令集来加速数学函数库的运算，并且需要将其实现代码添加到编译中。
# USE_SSE_AUTO表示让编译器自动决定是否使用SSE指令集来加速运算。
add_compile_definitions(SSE_MATHFUN_WITH_CODE USE_SSE_AUTO)

# 本项目的开发者请使用set(DEVELOPMENT ON)或者在cmake中添加-DDEVELOPMENT=ON将选项打开
option(DEVELOPMENT OFF) # ON
if (${DEVELOPMENT})
    message(STATUS "DEVELOPMENT MODE ON")
    enable_testing()# 启用测试支持
    add_subdirectory(bench)
    add_subdirectory(test)
endif ()