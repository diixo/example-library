
set (SOURCE_ITC_COMMON_COMMAND 
   CommandBase.cpp 
   CommandsQueue.cpp 
   ICommand.cpp 
   )

add_library (lib_itc_common_command STATIC ${SOURCE_ITC_COMMON_COMMAND})
