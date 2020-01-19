#include <appliances_backend/log.h>


namespace appliances_backend
{
  Log::Log(std::string prefix)
    : current_severity_{ Log::Severity::Info }
    , is_at_beginning_of_line_{ true }
    , prefix_{ prefix }
    , level_{ 0 }
  {
  }

  Log::Log(std::string prefix, unsigned int level)
    : current_severity_{ Log::Severity::Info }
    , is_at_beginning_of_line_{ true }
    , prefix_{ prefix }
    , level_{ level }
  {
  }
  
  std::string Log::generateLineStart()
  {
    std::string result = "";
    std::string code = "";

    switch(current_severity_)
    {
    case Severity::Debug:
      result += "\033[0;36m";
      code = "DBG";
      break;

    case Severity::Info:
      result += "\033[0;37m";
      code = "INF";
      break;

    case Severity::Warning:
      result += "\033[0;33m";
      code = "WRN";
      break;

    case Severity::Error:
      result += "\033[0;31m";
      code = "ERR";
      break;

    case Severity::Critical:
      result += "\033[1;31m";
      code = "CRT";
      break;
    }

    if(is_at_beginning_of_line_)
    {
      std::string indentation = "";
      for(unsigned int i = 0; i < level_; ++i)
      {
	indentation += "  ";
      }
      
      std::time_t current_time = std::time(nullptr);
      char time_string_buffer[100];

      std::strftime(time_string_buffer, sizeof(time_string_buffer), "%FT%T%z", std::localtime(&current_time));
      result += "[" + code + " " + std::string(time_string_buffer) + "] " + indentation;
    }

    return result;
  }
  
  std::ostream& Log::getCurrentStream()
  {
    switch(current_severity_)
    {
    case Severity::Debug:
    case Severity::Info:
      return std::cout;
      break;

    case Severity::Warning:
    case Severity::Error:
    case Severity::Critical:
      return std::cerr;
      break;
    }
  }

  Log Log::deriveLogLevel()
  {
    Log derived_log(prefix_, level_ + 1);
    derived_log << current_severity_;

    return derived_log;
  }
}
