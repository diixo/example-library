

set (SOURCE_ITC 
   "EventLoop.cpp"
   "Dispatcher.cpp"
   "Event.cpp"
   "LogInfo.cpp"
   "MessageQueue.cpp"
   "itc.cpp"
)

add_executable (test_itc_main
   "test-main.cpp"
   ${SOURCE_ITC}
)

add_executable (test_itc_App 
   "test-app.cpp"
   "App.cpp"
   "DeviceManager.cpp"
   ${SOURCE_ITC}
)

