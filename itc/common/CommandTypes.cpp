
#include "CommandTypes.hpp"

std::ostream& operator<<(std::ostream& out, const ::bt::eCommandNotificationType& value)
{
    switch (value)
    {
        case bt::eCommandNotificationType::CMD_STARTED:
            return out << "CMD_STARTED";
        case bt::eCommandNotificationType::CMD_ABORT_STARTED:
            return out << "CMD_ABORT_STARTED";
        case bt::eCommandNotificationType::CMD_CANCEL_STARTED:
            return out << "CMD_CANCEL_STARTED";
        case bt::eCommandNotificationType::CMD_FINISHED:
            return out << "CMD_FINISHED";
        default:
            return out << "invaild_eCommandNotificationType(" << static_cast<int>(value) << ")";
    }
}

std::ostream& operator<<(std::ostream& out, const ::bt::eCommandResult& value)
{
    switch (value)
    {
        case bt::eCommandResult::RESULT_SUCCESS:
            return out << "RESULT_SUCCESS";
        case bt::eCommandResult::RESULT_ERROR:
            return out << "RESULT_ERROR";
        case bt::eCommandResult::RESULT_ABORTED:
            return out << "RESULT_ABORTED";
        case bt::eCommandResult::RESULT_CANCELED:
            return out << "RESULT_CANCELED";
        case bt::eCommandResult::RESULT_WATCHDOG:
            return out << "RESULT_WATCHDOG";
        default:
            return out << "invaild_eCommandResult(" << static_cast<int>(value) << ")";
    }
}

std::ostream& operator<<(std::ostream& out, const ::bt::eCommandsQueueNotificationType& value)
{
    switch (value)
    {
        case bt::eCommandsQueueNotificationType::CQ_CHANGED:
            return out << "CQ_CHANGED";
        case bt::eCommandsQueueNotificationType::CQ_CLEARED:
            return out << "CQ_CLEARED";
        case bt::eCommandsQueueNotificationType::CQ_FINISHED:
            return out << "CQ_FINISHED";
        default:
            return out << "invaild_eCommandsQueueNotificationType(" << static_cast<int>(value) << ")";
    }
}