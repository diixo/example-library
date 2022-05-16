
#set(TEST_ITC test_itc_main)

set (SOURCE_ITC 
   "EventLoop.cpp"
   "Dispatcher.cpp"
   "Event.cpp"
   "LogInfo.cpp"
   "MessageQueue.cpp"
   "itc.cpp"
)

add_executable (test_itc_main 
   "test_itc.cpp"
   ${SOURCE_ITC}
)

add_executable (test_itc_app 
   "test-app.cpp"
   "App.cpp"
   ${SOURCE_ITC}
)

