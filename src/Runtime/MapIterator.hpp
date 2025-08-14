#pragma once

#include <map>
#include <memory>
#include <string>

#include "MapInstance.hpp"
#include "Value.hpp"

// Forward declaration to avoid circular dependency
namespace o2l {
class MapObject;
}

namespace o2l {

class MapIterator {
   private:
    std::shared_ptr<MapInstance> map_instance_;
    std::map<Value, Value>::const_iterator current_iterator_;
    std::map<Value, Value>::const_iterator end_iterator_;

   public:
    MapIterator(std::shared_ptr<MapInstance> map_instance);

    // Iterator operations
    bool hasNext() const;
    Value nextKey();
    Value nextValue();
    Value nextEntry();  // Returns a Record with key and value fields
    Value MapItem();    // Returns a MapObject with key and value

    // Utility methods
    void reset();
    size_t getCurrentIndex() const;
    size_t getTotalSize() const;

    // String representation
    std::string toString() const;
};

}  // namespace o2l