#include <appliances_backend/appliances/ikea_tradfri/ikea_tradfri.h>


namespace appliances_backend
{
  namespace appliances
  {
    IkeaTradfri::IkeaTradfri(std::string host, Log log)
      : ApplianceBase{ log }
    {
    }

    void IkeaTradfri::run()
    {
      using namespace std::chrono_literals;

      while(should_run_)
      {
	waitFor(100ms);
      }
    }

    void IkeaTradfri::setVariable(std::list<std::string> variable_parts, nlohmann::json value)
    {
      // ...
    }
  }
}
