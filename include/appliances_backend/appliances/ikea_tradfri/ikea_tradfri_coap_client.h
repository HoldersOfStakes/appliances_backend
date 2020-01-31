#ifndef __APPLIANCES_BACKEND_IKEA_TRADFRI_COAP_CLIENT_H__
#define __APPLIANCES_BACKEND_IKEA_TRADFRI_COAP_CLIENT_H__


// System
#include <iostream>
#include <string>
#include <unistd.h>

// Private
#include <appliances_backend/utilities/coap_client.h>


namespace appliances_backend
{
  class IkeaTradfriCoapClient
  {
  public:
    IkeaTradfriCoapClient(std::string host, std::string security_code);
    virtual ~IkeaTradfriCoapClient() = default;

  private:
    utilities::CoapClient coap_client_;
    std::string security_code_;
  };
}


#endif /* __APPLIANCES_BACKEND_IKEA_TRADFRI_COAP_CLIENT_H__ */
