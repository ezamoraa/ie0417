add_executable(sensor_cmds main.c)

target_link_libraries(sensor_cmds ${LIB_NAME})

install(
  TARGETS sensor_cmds
  RUNTIME DESTINATION bin
)
