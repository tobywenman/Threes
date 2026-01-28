
cmake_minimum_required(VERSION 3.26)  # 3.26+ fully supports C23

project(threes C)

# Enable C23
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add your executable
add_executable(threes
    src/main.c
    src/grid_impl.c
    src/grid.c
)
