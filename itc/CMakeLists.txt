

set (SOURCE_ITC 
   "App.cpp"
   "EventLoop.cpp"
   "Dispatcher.cpp"
   "Event.cpp"
   "LogInfo.cpp"
   "MessageQueue.cpp"
   "itc.cpp"
   "DeviceManager.cpp"
)

add_executable (test_itc_EventDemo
   "main-EventDemo.cpp"
   ${SOURCE_ITC}
)

add_executable (test_itc_CallDemo
   "main-CallDemo.cpp"
   ${SOURCE_ITC}
)

add_executable (test_itc_EventStaticDemo
   "main-EventStaticDemo.cpp"
   ${SOURCE_ITC}
)

add_executable (test_itc_CallStaticDemo
   "main-CallStaticDemo.cpp"
   ${SOURCE_ITC}
)

add_executable (test_itc_App 
   "main-App.cpp"
   ${SOURCE_ITC}
)

add_executable (test_itc_AppCmdEventDemo
   "main-AppCmdEventDemo.cpp"
   ${SOURCE_ITC}
)
