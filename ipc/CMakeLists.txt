set(TEST_IPC test_ipc)

add_executable (${TEST_IPC} 
   "test_ipc.cpp"
   "EventLoop.cpp"
   "Dispatcher.cpp"
   "Event.cpp"
   "LogInfo.cpp"
)
