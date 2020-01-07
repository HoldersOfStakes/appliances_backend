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

    template<class ... Args>
    void addAccessory(std::string name, Args ... args)
    {
      if(accessories_.find(name) != accessories_.end())
      {
	throw std::runtime_error("Accessory name already present: '" + name + "'");
      }

      accessories_[name] = std::make_shared<Accessory>(name, std::forward<Args>(args)...);
    }

    std::vector<std::string> getAccessoryNames();
    std::shared_ptr<Accessory> getAccessory(std::string name);

  private:
    std::map<std::string, std::shared_ptr<Accessory>> accessories_;
  };
}


#endif /* __APPLIANCES_BACKEND_ACCESSORIES_MANAGER_H__ */
