cmake_minimum_required(VERSION 3.15)

project(FTPSERVER)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
add_executable(${PROJECT_NAME}  FTPTask.cpp FTPThread.cpp FTPThreadPool.cpp CMDRETR.cpp CMDSTOR.cpp FTPserverCMD.cpp CMDOPT.cpp CMDUSER.cpp CloneFactory.cpp  FTPserver.cpp)


# 添加 libevent 的链接库
target_link_libraries(${PROJECT_NAME} PRIVATE -levent  stdc++fs)
