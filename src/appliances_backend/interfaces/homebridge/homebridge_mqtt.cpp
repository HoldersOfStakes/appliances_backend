#include <appliances_backend/interfaces/homebridge/homebridge_mqtt.h>


namespace appliances_backend
{
  namespace interfaces
  {
    HomebridgeMqtt::HomebridgeMqtt(std::string host, unsigned short port)
      : mqtt_client_{ host, port }
    {
    }

    void HomebridgeMqtt::run()
    {
      mqtt_client_.subscribe("topic_name");

      while(should_run_)
      {
	if(mqtt_client_.wasDataReceivedOnTopic("topic_name"))
	{
	  std::pair<char*, ssize_t> data = mqtt_client_.getNextReceivedDataOnTopic("topic_name");

	  std::cout << "Message: " << std::string(data.first, data.second) << std::endl;

	  delete[] data.first;
	}

	usleep(50000);
      }

      // TODO(fairlight1337): Unsubscribe here.
    }
  }
}
