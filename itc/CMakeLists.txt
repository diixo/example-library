

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

##################################

add_executable (test_itc_EventDemo
   "main-EventDemo.cpp"
   ${SOURCE_ITC}
)

add_library (lib_itc STATIC ${SOURCE_ITC})

target_link_libraries (test_itc_EventDemo  lib_itc_common_command)

##################################

add_executable (test_itc_CallDemo
   "main-CallDemo.cpp"
   ${SOURCE_ITC}
)

target_link_libraries (test_itc_CallDemo  lib_itc_common_command)

##################################

add_executable (test_itc_EventStaticDemo
   "main-EventStaticDemo.cpp"
   ${SOURCE_ITC}
)

target_link_libraries (test_itc_EventStaticDemo  lib_itc_common_command)

##################################

add_executable (test_itc_CallStaticDemo
   "main-CallStaticDemo.cpp"
   ${SOURCE_ITC}
)

target_link_libraries (test_itc_CallStaticDemo  lib_itc_common_command)

##################################

add_executable (test_itc_App 
   "main-App.cpp"
   ${SOURCE_ITC}
)

target_link_libraries (test_itc_App  lib_itc_common_command)

##################################

add_executable (test_itc_AppCmdEventDemo
   "main-AppCmdEventDemo.cpp"
   ${SOURCE_ITC}
)

target_link_libraries (test_itc_AppCmdEventDemo  lib_itc_common_command)

##################################

add_executable (test_itc_AppCmdEventExitDemo
   "main-AppCmdEventExitDemo.cpp"
   ${SOURCE_ITC}
)

target_link_libraries (test_itc_AppCmdEventExitDemo  lib_itc_common_command)

##################################

add_subdirectory(common)
