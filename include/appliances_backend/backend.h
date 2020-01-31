#ifndef __APPLIANCES_BACKEND_BACKEND_H__
#define __APPLIANCES_BACKEND_BACKEND_H__


// System
#include <list>
#include <string>
#include <unistd.h>

// libconfig++
#include <libconfig.h++>

// Private
#include <appliances_backend/appliances/helios_kwl/helios_kwl.h>
#include <appliances_backend/appliances/ikea_tradfri/ikea_tradfri.h>
#include <appliances_backend/interfaces/homebridge/homebridge_mqtt.h>
#include <appliances_backend/appliances_manager.h>
#include <appliances_backend/interfaces_manager.h>
#include <appliances_backend/accessories_manager.h>
#include <appliances_backend/mapper.h>
#include <appliances_backend/logging_base.h>


namespace appliances_backend
{
  class Backend : public LoggingBase
  {
  public:
    enum class RunMode
    {
     Normal = 0,
     DryRun
    };

    Backend(std::string config_file_path, RunMode run_mode);
    virtual ~Backend() = default;

    void initialize();
    void deinitialize();

    void run();
    void stop();

  private:
    static std::list<std::string> split(const std::string& str, char delimeter);
    static bool fileExists(std::string file_path);
    static nlohmann::json convertConfigToJson(const libconfig::Setting& setting);

    std::atomic<bool> should_run_;
    std::string config_file_path_;
    RunMode run_mode_;

    InterfacesManager interfaces_manager_;
    AppliancesManager appliances_manager_;
    AccessoriesManager accessories_manager_;
    Mapper mapper_;

    void loadPlugins();
    void loadConfiguration(std::string config_file_path);
    void loadAppliance(const libconfig::Setting& appliance_description, Log log);
    void loadInterface(const libconfig::Setting& interface_description, Log log);
    void loadAccessory(const libconfig::Setting& accessory_description, Log log);
    void loadMapping(const libconfig::Setting& mapping_description, Log log);
  };
}


#endif /* __APPLIANCES_BACKEND_BACKEND_H__*/
