project(raylibtest)

add_executable(main main.cpp)
include(CMakePrintHelpers)


set(Bullet_DIR "/home/jonatan/Documents/kod/cpp/libraries/bullet3/build_cmake/")
# set(BULLET_INCLUDE_DIR )


find_package(Bullet CONFIG REQUIRED)
# set(BULLET_INCLUDE_DIR "/home/jonatan/Documents/kod/cpp/libraries/bullet3/src/")
# include_directories(${BULLET_INCLUDE_DIR})

target_include_directories(main
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/bullet3/src/
  # ${BULLET_INCLUDE_DIR}

  )
cmake_print_variables(BULLET_INCLUDE_DIR)


target_link_libraries(main
  PRIVATE
  ${BULLET_LIBRARIES}
  PUBLIC
  # /include/raylib/libraylib.a
  libraylib.a
  # libBullet3Common.so
  )

