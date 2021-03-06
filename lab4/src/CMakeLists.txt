cmake_minimum_required(VERSION 2.8.3)

find_package(Boost 1.40.0 COMPONENTS filesystem system iostreams REQUIRED)

add_executable(examples
  examples.cc)
target_link_libraries(examples 
  lib_graph 
  lib_environment 
  lib_util)

add_executable(path_planning
  path_planning.cc 
  dijkstra2d.h 
  dijkstra2d.cc
  depth_first_search2d.h 
  depth_first_search2d.cc
  a_star2d.h 
  a_star2d.cc
  gnuplot-iostream.h
  gui2d.h
  gui2d.cc
  )
target_link_libraries(path_planning
  lib_graph 
  lib_environment 
  lib_util
  ${Boost_LIBRARIES})

add_executable(polynomial_planning
  polynomial_planning.cc
  )
target_include_directories(polynomial_planning PUBLIC
  ${PROJECT_SOURCE_DIR}/src/P4/src)
target_link_libraries(polynomial_planning
  lib_graph 
  lib_environment 
  lib_util
  lib_p4
  ${Boost_LIBRARIES})

add_executable(full_stack_planning
  full_stack_planning.cc 
  a_star2d.h 
  a_star2d.cc
  gnuplot-iostream.h
  gui2d.h
  gui2d.cc
  )
target_include_directories(full_stack_planning PUBLIC
  ${PROJECT_SOURCE_DIR}/src/P4/src)
target_link_libraries(full_stack_planning
  lib_graph 
  lib_environment 
  lib_util
  lib_p4
  ${Boost_LIBRARIES})
