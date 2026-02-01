
cmake_minimum_required(VERSION 3.26)  # 3.26+ fully supports C23

project(threees C)

# Enable C23
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(COMMON_SRC src/common/game.c
               src/common/grid.c
               src/common/cli.c)

# Add your executable
add_executable(threees_server
    src/server/server_main.c
    ${COMMON_SRC}
)

add_executable(threees_client
    src/client/client_main.c
    ${COMMON_SRC}
)

target_include_directories(threees_server PUBLIC src/common)
