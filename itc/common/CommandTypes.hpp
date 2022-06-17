
#pragma once

//--------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------

#include <string>
#include <stdint.h>
#include <vector>
#include <sstream>

namespace CommonAPI
{

   template<typename Base_>
   struct Enumeration {
      Enumeration() = default;
      Enumeration(const Base_ &_value) :
         value_(_value) {
      }

      virtual ~Enumeration() {}

      inline Enumeration &operator=(const Base_ &_value) {
         value_ = _value;
         return (*this);
      }

      inline operator const Base_() const {
         return value_;
      }

      inline bool operator==(const Enumeration<Base_> &_other) const {
         return (value_ == _other.value_);
      }

      inline bool operator!=(const Enumeration<Base_> &_other) const {
         return (value_ != _other.value_);
      }

      inline bool operator<(const Enumeration<Base_> &_other) const {
         return (value_ < _other.value_);
      }

      inline bool operator<=(const Enumeration<Base_> &_other) const {
         return (value_ <= _other.value_);
      }

      inline bool operator>(const Enumeration<Base_> &_other) const {
         return (value_ > _other.value_);
      }

      inline bool operator>=(const Enumeration<Base_> &_other) const {
         return (value_ >= _other.value_);
      }

      virtual bool validate() const = 0;

      Base_ value_;
   };

} // namespace CommonAPI

struct DeviceManagerTypes
{
   struct ProfileType : CommonAPI::Enumeration< int32_t> {
      enum Literal : int32_t {
         HFP = 0,
         A2DP = 1
      };

      ProfileType()
         : CommonAPI::Enumeration< int32_t>(static_cast< int32_t>(Literal::HFP)) {}
      ProfileType(Literal _literal)
         : CommonAPI::Enumeration< int32_t>(static_cast< int32_t>(_literal)) {}

      inline bool validate() const {
         switch (value_) {
         case static_cast< int32_t>(Literal::HFP) :
         case static_cast< int32_t>(Literal::A2DP) :
            return true;
         default:
            return false;
         }
      }

      inline bool operator==(const ProfileType &_other) const { return (value_ == _other.value_); }
      inline bool operator!=(const ProfileType &_other) const { return (value_ != _other.value_); }
      inline bool operator<=(const ProfileType &_other) const { return (value_ <= _other.value_); }
      inline bool operator>=(const ProfileType &_other) const { return (value_ >= _other.value_); }
      inline bool operator<(const ProfileType &_other) const { return (value_ < _other.value_); }
      inline bool operator>(const ProfileType &_other) const { return (value_ > _other.value_); }

      inline bool operator==(const Literal &_value) const { return (value_ == static_cast< int32_t>(_value)); }
      inline bool operator!=(const Literal &_value) const { return (value_ != static_cast< int32_t>(_value)); }
      inline bool operator<=(const Literal &_value) const { return (value_ <= static_cast< int32_t>(_value)); }
      inline bool operator>=(const Literal &_value) const { return (value_ >= static_cast< int32_t>(_value)); }
      inline bool operator<(const Literal &_value) const { return (value_ < static_cast< int32_t>(_value)); }
      inline bool operator>(const Literal &_value) const { return (value_ > static_cast< int32_t>(_value)); }
   };
   typedef std::vector< DeviceManagerTypes::ProfileType> ProfileTypes;
};


//--------------------------------------------------------------------
// Class Declaration
//--------------------------------------------------------------------

namespace bt {

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

std::ostream& operator<<(std::ostream& out, const ::bt::eCommandNotificationType& value);
std::ostream& operator<<(std::ostream& out, const ::bt::eCommandResult& value);
std::ostream& operator<<(std::ostream& out, const ::bt::eCommandsQueueNotificationType& value);

