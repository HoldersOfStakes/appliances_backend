#include <appliances_backend/appliances/ikea_tradfri/ikea_tradfri_coap_client.h>


namespace appliances_backend
{
  IkeaTradfriCoapClient::IkeaTradfriCoapClient(std::string host, std::string security_code)
    : coap_client_{ host, 5684 }
    , security_code_{ security_code }
  {
    // ...
  }
}
