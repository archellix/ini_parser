include_directories(. ../include)

add_library(inip inip.cpp)

# Boost Libraries
set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_DEBUG_LIBS OFF)
set(Boost_USE_RELEASE_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  target_link_libraries(inip ${Boost_LIBRARIES})
endif()