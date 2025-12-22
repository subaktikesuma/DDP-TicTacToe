cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

project(ddp_tictactoe VERSION 1.0.0 LANGUAGES C)

add_compile_definitions(_DEFAULT_SOURCE)

add_executable(${PROJECT_NAME}
    src/main.c
    src/ui.c
    src/core.c
    src/utils.c
    src/bot.c
    src/audio.c
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(NOT WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE m)
endif()

install(TARGETS ddp_tictactoe
    RUNTIME DESTINATION bin)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<C_COMPILER_ID:GNU>:-Wall -Wextra>
    $<$<C_COMPILER_ID:MSVC>:/W4>
    $<$<C_COMPILER_ID:Clang>:-Wall -Wextra>
)