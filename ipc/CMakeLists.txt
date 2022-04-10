
set(TEST_ITC test_itc)

add_executable (${TEST_ITC} 
   "test_ipc.cpp"
   "EventLoop.cpp"
   "Dispatcher.cpp"
   "Event.cpp"
   "LogInfo.cpp"
   "ipc.cpp"
)
