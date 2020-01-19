#ifndef __APPLIANCES_BACKEND_LOG_H__
#define __APPLIANCES_BACKEND_LOG_H__


// System
#include <iostream>
#include <string>
#include <ctime>


namespace appliances_backend
{
  class Log
  {
  public:
    enum class Severity
    {
      Debug = 0,
      Info,
      Warning,
      Error,
      Critical
    };
    
    Log(std::string prefix);
    Log(std::string prefix, unsigned int level);
    virtual ~Log() = default;

    template<typename TValueType>
    Log& operator<<(const TValueType& value)
    {
      getCurrentStream() << generateLineStart() << value;
      is_at_beginning_of_line_ = false;

      return *this;
    }

    Log& operator<<(std::ostream&(*f)(std::ostream&))
    {
      getCurrentStream() << generateLineStart() << "\033[0m";

      f(getCurrentStream());
      is_at_beginning_of_line_ = true;

      return *this;
    }

    Log& operator<<(const Severity& severity)
    {
      current_severity_ = severity;

      return *this;
    }

    Log deriveLogLevel();

  private:
    Severity current_severity_;
    bool is_at_beginning_of_line_;
    std::string prefix_;
    unsigned int level_;

    std::string generateLineStart();
    std::ostream& getCurrentStream();
  };
}


#endif /* __APPLIANCES_BACKEND_LOG_H__ */
