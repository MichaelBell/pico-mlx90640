add_library(mlx90640 INTERFACE)

target_sources(mlx90640 INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/MLX90640_API.cpp
  ${CMAKE_CURRENT_LIST_DIR}/MLX90640_Pico_I2C.cpp )

target_include_directories(mlx90640 INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(mlx90640 INTERFACE pico_stdlib hardware_i2c)
