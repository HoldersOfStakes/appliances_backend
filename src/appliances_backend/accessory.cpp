#include <appliances_backend/accessory.h>


namespace appliances_backend
{
  Accessory::Accessory(AccessoryType accessory_type)
    : accessory_type_{ accessory_type }
  {
  }

  AccessoryType Accessory::getType() const
  {
    return accessory_type_;
  }
}
