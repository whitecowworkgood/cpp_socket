cmake_minimum_required(VERSION 3.5)
project(Socket_Project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

include_directories(includes)



#클라이언트 파일 목록
set(CLIENT_SOURCE 
    src/client/ClientMain.cpp
    src/client/MyClient.cpp
)


#서버 파일 목록
set(SERVER_SOURCE 
    src/server/ServerMain.cpp
    src/server/MyServer.cpp
)

#클라이언트 빌드
add_executable("client" ${CLIENT_SOURCE})

#서버 빌드
add_executable("server" ${SERVER_SOURCE})

# 라이브러리 링크
target_link_libraries(client pthread ssl crypto stdc++fs)
target_link_libraries(server pthread ssl crypto stdc++fs)

