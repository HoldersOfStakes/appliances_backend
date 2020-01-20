#ifndef __APPLIANCES_BACKEND_LOGGING_BASE_H__
#define __APPLIANCES_BACKEND_LOGGING_BASE_H__


// Private
#include <appliances_backend/log.h>


namespace appliances_backend
{
  class LoggingBase
  {
  protected:
    LoggingBase(Log log);
    virtual ~LoggingBase() = default;

    Log& log();

  private:
    Log log_;
  };
}


#endif /* __APPLIANCES_BACKEND_LOGGING_BASE_H__*/
