add_executable(timestamp_unittest Timestamp_unittest.cc) 
target_link_libraries(timestamp_unittest muduo_base) 
add_test(NAME timestamp_unittest COMMAND timestamp_unittest)

add_executable(exception_test Exception_test.cc)
target_link_libraries(exception_test muduo_base)
add_test(NAME exception_test COMMAND exception_test) 

add_executable(atomic_unittest Atomic_unittest.cc)
add_test(NAME atomic_unittest COMMAND atomic_unittest) 

add_executable(Thread_unittest Thread_test.cc) 
target_link_libraries(Thread_unittest muduo_base) 
add_test(NAME Thread_unittest COMMAND Thread_unittest)

add_executable(Singleton_test Singleton_test.cc) 
target_link_libraries(Singleton_test muduo_base) 
add_test(NAME Singleton_test COMMAND Singleton_test)

add_executable(WeakCallback_test WeakCallback_test.cc) 
target_link_libraries(WeakCallback_test muduo_base) 
add_test(NAME WeakCallback_test COMMAND WeakCallback_test)

add_executable(StringPiece_test StringPiece_test.cc) 
target_link_libraries(StringPiece_test muduo_base) 
add_test(NAME StringPiece_test COMMAND StringPiece_test)

add_executable(BlockingQueue_test BlockingQueue_test.cc) 
target_link_libraries(BlockingQueue_test muduo_base) 
add_test(NAME BlockingQueue_test COMMAND BlockingQueue_test)
