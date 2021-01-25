cmake_minimum_required(VERSION 2.8.3)
project(h_180201036)
 
## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs genmsg)

## Declare ROS messages and services
add_message_files(FILES Num.msg)
add_service_files(FILES AddTwoInts.srv)
 
## Generate added messages and services
generate_messages(DEPENDENCIES std_msgs)
 
## Declare a catkin package
catkin_package()
 
## Build talker and listener
include_directories(include ${catkin_INCLUDE_DIRS})
 
add_executable(hakan src/hakan.cpp)
target_link_libraries(hakan ${catkin_LIBRARIES})
add_dependencies(hakan h_180201036_generate_messages_cpp)

add_executable(keles src/keles.cpp)
target_link_libraries(keles ${catkin_LIBRARIES})
add_dependencies(keles h_180201036_generate_messages_cpp)

catkin_install_python(PROGRAMS scripts/hakan.py scripts/keles.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)