add_executable(sensor_cmds main.c)

install(
  TARGETS sensor_cmds
  RUNTIME DESTINATION bin
)
