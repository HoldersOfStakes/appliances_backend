#include <appliances_backend/logging_base.h>


namespace appliances_backend
{
  LoggingBase::LoggingBase(Log log)
    : log_{ log }
  {
  }

  Log& LoggingBase::log()
  {
    return log_;
  }
}
