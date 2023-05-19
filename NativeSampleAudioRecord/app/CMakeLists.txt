# 设置cmake 支持最小版本
cmake_minimum_required(VERSION 3.4.1)

# 设置文件目录
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src/main/cpp)

# 指定头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/base)
include_directories(${PROJECT_SOURCE_DIR})

# 设置lib工程在主工程的项目位置
set(audiorecord_DIR ${CMAKE_SOURCE_DIR}/../AudioRecorderLib)

# 加载lib需要暴露的头文件
include_directories(${audiorecord_DIR}/src/main/cpp/include)
include_directories(${audiorecord_DIR}/src/main/cpp/base)

# 导入lib工程的so库
add_library(audiorecord SHARED IMPORTED)
set_target_properties(audiorecord PROPERTIES IMPORTED_LOCATION ${audiorecord_DIR}/libs/${ANDROID_ABI}/libaudiorecord.so)

# 加载编译的源文件
file(GLOB ALL_FILES "${PROJECT_SOURCE_DIR}/*.cpp" "${PROJECT_SOURCE_DIR}/base/*.cpp")

# 生成自己的动态库文件
add_library(speex_audiorecord SHARED ${ALL_FILES})

# 查找依赖库
find_library(log-lib log)
find_library(android-lib android)

#将lib的so连接到你的native开发工程中
target_link_libraries(speex_audiorecord audiorecord ${android-lib} ${log-lib})
