cmake_minimum_required(VERSION 3.0)
project(gameserver)

#set(CMAKE_VERBOSE_MAKEFILE ON) # 显示详细的make命令
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")  # 定义编译的参数
# set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O2 -g -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")  # 定义编译的参数
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include_directories(${CMAKE_SOURCE_DIR}/gameserver)  # -I

set(LIB_SRC
    gameserver/Log/log.cc
    ) # 源码放在src下

add_library(gameserver SHARED ${LIB_SRC})  # 生成so/dll文件
#add_library(gameserver_static STATIC ${LIB_SRC})  # 生成a/lib
#SET_TARGET_PROPERTIES (gameserver_static PROPERTIES OUTPUT_NAME "gameserver")

add_executable(test tests/test.cc)  # 生成测试文件.o
add_dependencies(test gameserver)  # 测试文件依赖于so文件
target_link_libraries(test gameserver)  # 链接so文件

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)  # 输出生成路径
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
