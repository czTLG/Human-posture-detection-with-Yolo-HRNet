cmake_minimum_required(VERSION 3.5)
#定义项目名称和语言
project(DLRF_CPU LANGUAGES CXX)

find_package(GTest REQUIRED)
find_package(glog REQUIRED)
find_package(Armadillo REQUIRED)
find_package(BLAS REQUIRED)
find_package(LAPACK REQUIRED)

#将指定目录中的源文件加入到DIR_TEST_DATA变量中
aux_source_directory(../test/test_data DIR_TEST_DATA)
aux_source_directory(../test/test_layer DIR_TEST_LAYER)
aux_source_directory(../test/test_net DIR_TEST_NET)
aux_source_directory(../test/test_runtime DIR_TEST_RUNTIME)

#定义要链接的库
set(link_lib glog::glog GTest::gtest)

#如果不是Windows系统，则添加pthread库
if(!WIN32)
set(link_lib "${link_lib} pthread")
endif()

#定义要链接的数学库
set(link_math_lib ${ARMADILLO_LIBRARIES} ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES})

#添加可执行文件test_DLRF
add_executable(test_DLRF test_main.cpp ${DIR_TEST_DATA} ${DIR_TEST_LAYER} ${DIR_TEST_NET} ${DIR_TEST_RUNTIME})

#如果是MSVC，则设置编译选项
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O0 /openmp:llvm")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fopenmp -march=native")
endif()

#链接需要的库
target_link_libraries(test_DLRF ${link_lib} ${link_math_lib})

#添加可执行文件的链接目录
target_link_directories(test_DLRF PUBLIC ${PROJECT_SOURCE_DIR}/lib)

#链接DLRF库在CMake中，每个target都可以通过target_link_libraries命令链接到其他的target或库文件。
#因此，将子项目target_link_libraries到主项目，可以将主项目中的库链接到子项目中，以便子项目可以使用主项目中的库。
target_link_libraries(test_DLRF DLRF)

#如果是MSVC，则复制DLRF库到可执行文件目录
if (MSVC)
add_custom_command(TARGET test_DLRF POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_if_different
"$<TARGET_FILE_DIR:DLRF>/DLRF.dll"
$<TARGET_FILE_DIR:test_DLRF>)
endif()

#添加头文件包含路径
target_include_directories(test_DLRF PUBLIC ${glog_INCLUDE_DIR})
target_include_directories(test_DLRF PUBLIC ${GTest_INCLUDE_DIR})
target_include_directories(test_DLRF PUBLIC ${Armadillo_INCLUDE_DIR})