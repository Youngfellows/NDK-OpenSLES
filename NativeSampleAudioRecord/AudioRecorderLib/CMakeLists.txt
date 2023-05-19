
# 设置cmake 支持最小版本
cmake_minimum_required(VERSION 3.4.1)

# 设置文件目录
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src/main/cpp)

# 指定头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR})


# 加载编译的源文件
file(GLOB ALL_FILES "${PROJECT_SOURCE_DIR}/*.cpp" "${PROJECT_SOURCE_DIR}/audiorecord/*.cpp" "${PROJECT_SOURCE_DIR}/opensles/*.cpp" "${PROJECT_SOURCE_DIR}/base/*.cpp")
add_library(audiorecord SHARED ${ALL_FILES})

# 设置so目录,为在主app工程的libs位置，根据实际情况调整distribution_DIR的值
set(distribution_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(audiorecord PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${distribution_DIR}/libs/${ANDROID_ABI})

# 查找父目录3rd
get_filename_component(JNI_LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../.. ABSOLUTE)
message(STATUS "JNI_LIBS_DIR::${JNI_LIBS_DIR}")

# 设置动态库生成目录
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${distribution_DIR}/jniLibs/${ANDROID_ABI})

# 查找依赖库
find_library(log-lib log)
find_library(OpenSLES-lib OpenSLES)

# 连接依赖库
target_link_libraries(audiorecord ${log-lib} ${OpenSLES-lib})