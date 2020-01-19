#ifndef __APPLIANCES_BACKEND_LOG_H__
#define __APPLIANCES_BACKEND_LOG_H__


// System
#include <iostream>


namespace appliances_backend
{
  class Log
  {
  public:
    Log() = default;
    virtual ~Log() = default;

    template<typename TValueType>
    Log& operator<<(const TValueType& value)
    {
      std::cout << value;
      return *this;
    }

    Log& operator<<(std::ostream&(*f)(std::ostream&))
    {
      std::cout << f << std::endl;
      (*this) << f;

      return *this;
    }
  };
}


#endif /* __APPLIANCES_BACKEND_LOG_H__ */
