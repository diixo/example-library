#include "ICommand.hpp"

/**
 * @brief ICommand debug to string
 * 
 * @param out[in,out] trace stream
 * @param value[in] ICommand member
 * @return std::ostream& 
 */
std::ostream& operator<<(std::ostream& out, const itc::ICommand& value)
{
    return out << value.getName() << "_" << value.getCmdUID() << "(" << value.getTypeId() << ")";
}

/**
 * @brief ICommandListener debug to string
 * 
 * @param out[in,out] trace stream
 * @param value[in] ICommandListener member
 * @return std::ostream& 
 */
std::ostream& operator<<(std::ostream& out, const itc::ICommandListener& value)
{
    return out << "ICommandListener";
}
