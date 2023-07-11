project(raylibtest)

add_executable(main main.cpp)

target_link_libraries(main
  PUBLIC
  # /include/raylib/libraylib.a
  libraylib.a
  )
