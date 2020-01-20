#include <appliances_backend/utilities/mqtt_client.h>


namespace appliances_backend
{
  namespace utilities
  {
    unsigned int MqttClient::usage_counter_ = 0;
    std::mutex MqttClient::global_initialization_access_;


    static void handleMqttMessageWrapper(struct mosquitto* mosquitto_handle, void* user_data, const struct mosquitto_message* message)
    {
      if(message != nullptr)
      {
	MqttClient* mqtt_client = static_cast<MqttClient*>(user_data);
	mqtt_client->handleMessage(message);
      }
    }

    static void handleMqttLogWrapper(struct mosquitto* mosquitto_handle, void* user_data, int level, const char* message)
    {
      if(message != nullptr)
      {
	MqttClient* mqtt_client = static_cast<MqttClient*>(user_data);
	mqtt_client->handleLog(level, message);
      }
    }

    static void handleMqttConnectWrapper(struct mosquitto* mosquitto_handle, void* user_data, int return_code)
    {
      MqttClient* mqtt_client = static_cast<MqttClient*>(user_data);
      mqtt_client->handleConnect(return_code);
    }


    MqttClient::MqttClient(std::string host, unsigned short port)
      : host_{ host }
      , port_{ port }
    {
    }

    MqttClient::~MqttClient()
    {
      mosquitto_disconnect(mosquitto_handle_);
      mosquitto_loop_stop(mosquitto_handle_, false);
      mosquitto_destroy(mosquitto_handle_);

      deinitialize();

      std::lock_guard<std::mutex> lock(received_messages_access_);
      for(const std::pair<std::string, std::deque<std::pair<char*, ssize_t>>>& received_messages_entry : received_messages_)
      {
	for(const std::pair<char*, ssize_t>& received_message_entry : received_messages_entry.second)
	{
	  delete[] received_message_entry.first;
	}
      }
    }

    void MqttClient::connect()
    {
      using namespace std::chrono_literals;

      initialize();

      mosquitto_handle_ = mosquitto_new(nullptr, true, this);
      mosquitto_connect_callback_set(mosquitto_handle_, handleMqttConnectWrapper);
      mosquitto_message_callback_set(mosquitto_handle_, handleMqttMessageWrapper);
      mosquitto_log_callback_set(mosquitto_handle_, handleMqttLogWrapper);

      mosquitto_loop_start(mosquitto_handle_);

      int result = mosquitto_connect(mosquitto_handle_, host_.c_str(), port_, 1);
      if(result != MOSQ_ERR_SUCCESS)
      {
	throw std::runtime_error(std::string("Failed to start connecting to MQTT: ") + mosquitto_strerror(result));
      }

      // Waiting for the connection to be established or failed.
      std::unique_lock<std::mutex> connection_established_lock(connection_established_mutex_);
      if(connection_established_condition_.wait_for(connection_established_lock, 15s) == std::cv_status::timeout)
      {
	throw std::runtime_error(std::string("Timeout while connecting to MQTT: ") + mosquitto_strerror(connection_return_code_));
      }
      
      if(connection_return_code_ != 0)
      {
	throw std::runtime_error(std::string("Failed to connect to MQTT: ") + mosquitto_strerror(connection_return_code_));
      }
    }

    void MqttClient::publish(std::string topic, char* data, ssize_t length)
    {
      mosquitto_publish(mosquitto_handle_, nullptr, topic.c_str(), length, data, 0, false);
    }

    void MqttClient::subscribe(std::string topic)
    {
      received_messages_[topic] = std::deque<std::pair<char*, ssize_t>>();
      mosquitto_subscribe(mosquitto_handle_, nullptr, topic.c_str(), 0);
    }

    bool MqttClient::wasDataReceivedOnTopic(std::string topic)
    {
      std::lock_guard<std::mutex> lock(received_messages_access_);

      if(received_messages_.find(topic) != received_messages_.end())
      {
	return !received_messages_[topic].empty();
      }

      return false;
    }

    std::pair<char*, ssize_t> MqttClient::getNextReceivedDataOnTopic(std::string topic)
    {
      std::lock_guard<std::mutex> lock(received_messages_access_);

      std::pair<char*, ssize_t> next_data = received_messages_[topic].front();
      received_messages_[topic].pop_front();

      return next_data;
    }

    void MqttClient::handleMessage(const struct mosquitto_message* message)
    {
      std::lock_guard<std::mutex> lock(received_messages_access_);

      if(message != nullptr && message->topic != nullptr && message->payload != nullptr)
      {
	std::string topic = message->topic;

	if(received_messages_.find(topic) != received_messages_.end())
	{
	  char* received_payload = new char[message->payloadlen];
	  memcpy(received_payload, static_cast<char*>(message->payload), message->payloadlen);

	  received_messages_[topic].push_back(std::make_pair(received_payload, static_cast<ssize_t>(message->payloadlen)));
	}
      }
    }

    void MqttClient::handleLog(int log_level, std::string message)
    {
      // TODO(fairlight1337): Add logging facility and forward messages there.
    }

    void MqttClient::handleConnect(int return_code)
    {
      connection_return_code_ = return_code;

      // Subscribe to any already present subscription topics should
      // this have been a reconnect.
      std::lock_guard<std::mutex> lock(received_messages_access_);

      for(const std::pair<std::string, std::deque<std::pair<char*, ssize_t>>>& received_messages_entry : received_messages_)
      {
	mosquitto_subscribe(mosquitto_handle_, nullptr, received_messages_entry.first.c_str(), 0);
      }
      
      connection_established_condition_.notify_all();
    }

    void MqttClient::initialize()
    {
      std::lock_guard<std::mutex> lock(global_initialization_access_);

      if(usage_counter_ == 0)
      {
	mosquitto_lib_init();
      }

      usage_counter_++;
    }

    void MqttClient::deinitialize()
    {
      std::lock_guard<std::mutex> lock(global_initialization_access_);

      usage_counter_--;

      if(usage_counter_ == 0)
      {
	mosquitto_lib_cleanup();
      }
    }
  }
}
