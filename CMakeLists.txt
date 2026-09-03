cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(gn10_stm32_fdcan_driver STATIC
    src/fdcan_driver.cpp
    src/can_driver.cpp
)

target_include_directories(gn10_stm32_fdcan_driver PUBLIC
    include     
)
target_link_libraries(gn10_stm32_fdcan_driver PRIVATE stm32cubemx)
target_link_libraries(gn10_stm32_fdcan_driver PUBLIC gn10_can)
