set(base_SRCS 
  Timestamp.cc
  Exception.cc
  Thread.cc
)

add_library(muduo_base ${base_SRCS}) 
target_link_libraries(muduo_base pthread rt) 

if(NOT CMAKE_BUILD_NO_EXAMPLES) 
  add_subdirectory(tests) 
endif() 