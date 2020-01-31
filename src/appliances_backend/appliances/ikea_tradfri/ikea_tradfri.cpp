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
      while(should_run_)
      {
	usleep(100000);
      }
    }

    void IkeaTradfri::setVariable(std::list<std::string> variable_parts, nlohmann::json value)
    {
      // ...
    }
  }
}
