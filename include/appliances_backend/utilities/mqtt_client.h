#ifndef __APPLIANCES_BACKEND_UTILITIES_MQTT_CLIENT_H__
#define __APPLIANCES_BACKEND_UTILITIES_MQTT_CLIENT_H__


// System
#include <atomic>
#include <condition_variable>
#include <chrono>
#include <deque>
#include <iostream>
#include <string>
#include <map>
#include <mutex>
#include <unistd.h>
#include <string.h>

// libmosquitto
#include <mosquitto.h>


namespace appliances_backend
{
  namespace utilities
  {
    class MqttClient
    {
    public:
      MqttClient(std::string host, unsigned short port);
      virtual ~MqttClient();

      void connect();

      void publish(std::string topic, char* data, ssize_t length);
      void subscribe(std::string topic);

      bool wasDataReceivedOnTopic(std::string topic);
      std::pair<char*, ssize_t> getNextReceivedDataOnTopic(std::string topic);

      void handleMessage(const struct mosquitto_message* message);
      void handleLog(int log_level, std::string message);
      void handleConnect(int return_code);

    private:
      static unsigned int usage_counter_;
      static std::mutex global_initialization_access_;

      std::string host_;
      unsigned short port_;
      struct mosquitto* mosquitto_handle_;
      std::map<std::string, std::deque<std::pair<char*, ssize_t>>> received_messages_;
      std::mutex received_messages_access_;
      std::condition_variable connection_established_condition_;
      std::mutex connection_established_mutex_;
      std::atomic<int> connection_return_code_;

      void initialize();
      void deinitialize();
    };
  }
}


#endif /* __APPLIANCES_BACKEND_UTILITIES_MQTT_CLIENT_H__ */
