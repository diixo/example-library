
#set(TEST_ITC test_itc_main)

add_executable (test_itc_main 
   "test_itc.cpp"
   "EventLoop.cpp"
   "Dispatcher.cpp"
   "Event.cpp"
   "LogInfo.cpp"
   "MessageQueue.cpp"
   "itc.cpp"
)

add_executable (test_itc_service 
   "test_service.cpp"
   "App.cpp"
   "EventLoop.cpp"
   "Dispatcher.cpp"
   "Event.cpp"
   "LogInfo.cpp"
   "MessageQueue.cpp"
   "itc.cpp"
)
