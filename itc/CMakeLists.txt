

set (SOURCE_ITC 
   "EventLoop.cpp"
   "Dispatcher.cpp"
   "Event.cpp"
   "LogInfo.cpp"
   "MessageQueue.cpp"
   "itc.cpp"
)

add_executable (test_itc_EventDemo
   "main-EventDemo.cpp"
   ${SOURCE_ITC}
)

add_executable (test_itc_CallDemo
   "main-CallDemo.cpp"
   ${SOURCE_ITC}
)

add_executable (test_itc_App 
   "main-App.cpp"
   "App.cpp"
   "DeviceManager.cpp"
   ${SOURCE_ITC}
)

