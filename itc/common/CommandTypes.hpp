
#pragma once

//--------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------

#include <string>
#include <stdint.h>

//--------------------------------------------------------------------
// Class Declaration
//--------------------------------------------------------------------

namespace itc {

enum class eCommandNotificationType : uint32_t
{
    CMD_STARTED = 0,
    CMD_ABORT_STARTED,
    CMD_CANCEL_STARTED,
    CMD_PAUSED,
    CMD_RESUMED,
    CMD_FINISHED
};

enum class eCommandResult : uint32_t
{
    RESULT_SUCCESS = 0,
    RESULT_ERROR,
    RESULT_ABORTED,
    RESULT_CANCELED,
    RESULT_WATCHDOG
};

enum class eCommandsQueueNotificationType : uint32_t
{
    CQ_CHANGED = 0,
    CQ_CLEARED,
    CQ_FINISHED
};

}  //namespace itc
