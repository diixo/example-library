
#pragma once

namespace itc
{

class CTypeCounter
{
   CTypeCounter();

   static uint32_t counter;

public:

   CTypeCounter(uint32_t& id)
   {
      if (id == 0)
      {
         id = ++counter;
      }
   }
};

uint32_t CTypeCounter::counter = 0;

template <typename T>
class TClassId : public CTypeCounter
{
   static uint32_t id;
   typedef typename T value_type;

public:

   TClassId() : CTypeCounter(id)
   {
   }

   inline uint32_t get_id() const
   {
      return id;
   }
};

template <class T>
uint32_t TClassId<T>::id = 0;

}
