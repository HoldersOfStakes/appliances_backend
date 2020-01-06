#ifndef __APPLIANCES_BACKEND_ACCESSORY_H__
#define __APPLIANCES_BACKEND_ACCESSORY_H__


// System
#include <string>


namespace appliances_backend
{
  enum class AccessoryType
  {
    Fan
  };

  class Accessory
  {
  public:
    Accessory(AccessoryType accessory_type);
    virtual ~Accessory() = default;

    AccessoryType getType() const;

  private:
    AccessoryType accessory_type_;
  };
}


#endif /* __APPLIANCES_BACKEND_ACCESSORY_H__ */
