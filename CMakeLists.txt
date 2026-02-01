
cmake_minimum_required(VERSION 3.26)  # 3.26+ fully supports C23

project(threees C)

# Enable C23
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include the command that downloads libraries
include(FetchContent)

# define a function for adding git dependencies
function(include_dependency libName gitURL gitTag)
    # setup the declare
    FetchContent_Declare(${libName}
            GIT_REPOSITORY ${gitURL}
            GIT_TAG        ${gitTag}
            GIT_SHALLOW    TRUE
            GIT_PROGRESS   TRUE
    )

    set(_old_c_standard ${CMAKE_C_STANDARD})
    set(CMAKE_C_STANDARD 17)

    FetchContent_MakeAvailable(${libName})

    set(CMAKE_C_STANDARD ${_old_c_standard})
endfunction()

# add SDL3 support
find_package(SDL3 QUIET)
if (NOT SDL3_FOUND)
message(STATUS "Getting SDL3 from Github")
include_dependency(SDL3 https://github.com/libsdl-org/SDL.git release-3.4.0)
else()
message(STATUS "Using local SDL3")
endif()

find_package(SDL3_image QUIET)
if (NOT SDL3_image)
message(STATUS "Getting SDL3_image from Github")
include_dependency(SDL3_image https://github.com/libsdl-org/SDL_image.git release-3.4.0)
else()
message(STATUS "Using local SDL3_image")
endif()

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
    src/client/client_state.c
    ${COMMON_SRC}
)

target_include_directories(threees_server PUBLIC src/common)
target_include_directories(threees_client PUBLIC src/common)

# link all libraries to the project
target_link_libraries(threees_client PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)