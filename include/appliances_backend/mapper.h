#ifndef __APPLIANCES_BACKEND_MAPPER_H__
#define __APPLIANCES_BACKEND_MAPPER_H__


// System
#include <algorithm>
#include <map>
#include <string>
#include <list>


namespace appliances_backend
{
  class Mapper
  {
  public:
    Mapper() = default;
    virtual ~Mapper() = default;

    void mapEntities(const std::string& from_entity, const std::string& to_entity, bool bidirectional);
    void unmapEntities(const std::string& from_entity, const std::string& to_entity, bool bidirectional);

    void removeEntity(const std::string& entity);

    std::list<std::string> getMappedEntities(const std::string& from);

  private:
    std::map<std::string, std::list<std::string>> mappings_;
  };
}


#endif /* __APPLIANCES_BACKEND_MAPPER_H__ */
