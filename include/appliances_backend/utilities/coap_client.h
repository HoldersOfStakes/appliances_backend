#ifndef __APPLIANCES_BACKEND_UTILITIES_COAP_CLIENT_H__
#define __APPLIANCES_BACKEND_UTILITIES_COAP_CLIENT_H__


// System
#include <string>
#include <iostream>
#include <stdexcept>
#include <sys/time.h>

// libmodbus
#include <coap2/coap.h>


namespace appliances_backend
{
  namespace utilities
  {
    class CoapClient
    {
    public:
      CoapClient(std::string host, unsigned short port);
      virtual ~CoapClient();

    private:
    };
  }
}


#endif /* __APPLIANCES_BACKEND_UTILITIES_COAP_CLIENT_H__ */
