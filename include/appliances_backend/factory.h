#ifndef __APPLIANCES_BACKEND_FACTORY_H__
#define __APPLIANCES_BACKEND_FACTORY_H__


// System
#include <string>
#include <memory>
#include <functional>
#include <map>

// nlohmann
#include <nlohmann/json.hpp>


namespace appliances_backend
{
  template<class TBaseClassType>
  class Factory
  {
  public:
    Factory() = default;
    virtual ~Factory() = default;

    void registerType(std::string key, std::function<std::shared_ptr<TBaseClassType>(nlohmann::json)> generator)
    {
      registered_classes_[key] = generator;
    }

    std::shared_ptr<TBaseClassType> instantiate(std::string key, nlohmann::json parameters)
    {
      if(registered_classes_.find(key) == registered_classes_.end())
      {
	throw std::runtime_error("No class registered in factory for key '" + key + "'.");
      }

      return registered_classes_[key](parameters);
    }

  private:
    std::map<std::string, std::function<std::shared_ptr<TBaseClassType>(nlohmann::json)>> registered_classes_;
  };
}


#endif /* __APPLIANCES_BACKEND_FACTORY_H__ */
