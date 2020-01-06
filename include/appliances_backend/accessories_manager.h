#ifndef __APPLIANCES_BACKEND_ACCESSORIES_MANAGER_H__
#define __APPLIANCES_BACKEND_ACCESSORIES_MANAGER_H__


// System
#include <string>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

// Private
#include <appliances_backend/accessory.h>


namespace appliances_backend
{
  class AccessoriesManager
  {
  public:
    AccessoriesManager() = default;
    virtual ~AccessoriesManager() = default;

    void addAccessory(std::string name, AccessoryType accessory_type);

    std::vector<std::string> getAccessoryNames();
    std::shared_ptr<Accessory> getAccessory(std::string name);

  private:
    std::map<std::string, std::shared_ptr<Accessory>> accessories_;
  };
}


#endif /* __APPLIANCES_BACKEND_ACCESSORIES_MANAGER_H__ */
