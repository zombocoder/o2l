#include "Value.hpp"

#include <sstream>
#include <string>

#include "EnumInstance.hpp"
#include "ErrorInstance.hpp"
#include "ListInstance.hpp"
#include "ListIterator.hpp"
#include "MapInstance.hpp"
#include "MapIterator.hpp"
#include "MapObject.hpp"
#include "ObjectInstance.hpp"
#include "RecordInstance.hpp"
#include "RecordType.hpp"
#include "RepeatIterator.hpp"
#include "ResultInstance.hpp"
#include "SetInstance.hpp"
#include "SetIterator.hpp"
#include "FFI/FFITypes.hpp"

// Helper function to convert Long (__int128) to string
std::string longToString(o2l::Long value) {
#ifdef __SIZEOF_INT128__
    if (value == 0) return "0";

    std::string result;
    bool negative = value < 0;
    if (negative) {
        value = -value;
    }

    while (value > 0) {
        result = char('0' + (value % 10)) + result;
        value /= 10;
    }

    if (negative) {
        result = "-" + result;
    }

    return result;
#else
    return std::to_string(static_cast<long long>(value));
#endif
}

namespace o2l {

std::string valueToString(const Value& value) {
    return std::visit(
        [](const auto& v) -> std::string {
            using T = std::decay_t<decltype(v)>;

            if constexpr (std::is_same_v<T, Int>) {
                return std::to_string(v);
            } else if constexpr (std::is_same_v<T, Long>) {
                return longToString(v);
            } else if constexpr (std::is_same_v<T, Float>) {
                return std::to_string(v);
            } else if constexpr (std::is_same_v<T, Double>) {
                return std::to_string(v);
            } else if constexpr (std::is_same_v<T, Text>) {
                return v;
            } else if constexpr (std::is_same_v<T, Bool>) {
                return v ? "true" : "false";
            } else if constexpr (std::is_same_v<T, Char>) {
                return std::string(1, v);
            } else if constexpr (std::is_same_v<T, std::shared_ptr<ObjectInstance>>) {
                return "Object(" + v->getName() + ")";
            } else if constexpr (std::is_same_v<T, std::shared_ptr<EnumInstance>>) {
                return "Enum(" + v->getEnumName() + ")";
            } else if constexpr (std::is_same_v<T, std::shared_ptr<RecordType>>) {
                return "RecordType(" + v->getRecordName() + ")";
            } else if constexpr (std::is_same_v<T, std::shared_ptr<RecordInstance>>) {
                return v->toString();
            } else if constexpr (std::is_same_v<T, std::shared_ptr<ListInstance>>) {
                return v->toString();
            } else if constexpr (std::is_same_v<T, std::shared_ptr<ListIterator>>) {
                return v->toString();
            } else if constexpr (std::is_same_v<T, std::shared_ptr<RepeatIterator>>) {
                return v->toString();
            } else if constexpr (std::is_same_v<T, std::shared_ptr<MapInstance>>) {
                return v->toString();
            } else if constexpr (std::is_same_v<T, std::shared_ptr<MapIterator>>) {
                return v->toString();
            } else if constexpr (std::is_same_v<T, std::shared_ptr<SetInstance>>) {
                return v->toString();
            } else if constexpr (std::is_same_v<T, std::shared_ptr<SetIterator>>) {
                return v->toString();
            } else if constexpr (std::is_same_v<T, std::shared_ptr<MapObject>>) {
                return v->toString();
            } else if constexpr (std::is_same_v<T, std::shared_ptr<ErrorInstance>>) {
                return v->toString();
            } else if constexpr (std::is_same_v<T, std::shared_ptr<ResultInstance>>) {
                return v->toString();
            } else if constexpr (std::is_same_v<T, std::shared_ptr<ffi::PtrInstance>>) {
                return v->toString();
            } else if constexpr (std::is_same_v<T, std::shared_ptr<ffi::CBufferInstance>>) {
                return v->toString();
            } else if constexpr (std::is_same_v<T, std::shared_ptr<ffi::CStructInstance>>) {
                return v->toString();
            } else if constexpr (std::is_same_v<T, std::shared_ptr<ffi::CArrayInstance>>) {
                return v->toString();
            } else if constexpr (std::is_same_v<T, std::shared_ptr<ffi::CCallbackInstance>>) {
                return v->toString();
            } else {
                return "UnknownValue";
            }
        },
        value);
}

std::string getTypeName(const Value& value) {
    return std::visit(
        [](const auto& v) -> std::string {
            using T = std::decay_t<decltype(v)>;

            if constexpr (std::is_same_v<T, Int>) {
                return "Int";
            } else if constexpr (std::is_same_v<T, Long>) {
                return "Long";
            } else if constexpr (std::is_same_v<T, Float>) {
                return "Float";
            } else if constexpr (std::is_same_v<T, Double>) {
                return "Double";
            } else if constexpr (std::is_same_v<T, Text>) {
                return "Text";
            } else if constexpr (std::is_same_v<T, Bool>) {
                return "Bool";
            } else if constexpr (std::is_same_v<T, Char>) {
                return "Char";
            } else if constexpr (std::is_same_v<T, std::shared_ptr<ObjectInstance>>) {
                return v->getName();
            } else if constexpr (std::is_same_v<T, std::shared_ptr<EnumInstance>>) {
                return v->getEnumName();
            } else if constexpr (std::is_same_v<T, std::shared_ptr<RecordType>>) {
                return "RecordType";
            } else if constexpr (std::is_same_v<T, std::shared_ptr<RecordInstance>>) {
                return v->getTypeName();
            } else if constexpr (std::is_same_v<T, std::shared_ptr<ListInstance>>) {
                return "List<" + v->getElementTypeName() + ">";
            } else if constexpr (std::is_same_v<T, std::shared_ptr<ListIterator>>) {
                return "ListIterator";
            } else if constexpr (std::is_same_v<T, std::shared_ptr<RepeatIterator>>) {
                return "RepeatIterator";
            } else if constexpr (std::is_same_v<T, std::shared_ptr<MapInstance>>) {
                return "Map<" + v->getKeyTypeName() + ", " + v->getValueTypeName() + ">";
            } else if constexpr (std::is_same_v<T, std::shared_ptr<MapIterator>>) {
                return "MapIterator";
            } else if constexpr (std::is_same_v<T, std::shared_ptr<SetInstance>>) {
                return "Set<" + v->getElementTypeName() + ">";
            } else if constexpr (std::is_same_v<T, std::shared_ptr<SetIterator>>) {
                return "SetIterator";
            } else if constexpr (std::is_same_v<T, std::shared_ptr<MapObject>>) {
                return "MapObject";
            } else if constexpr (std::is_same_v<T, std::shared_ptr<ErrorInstance>>) {
                return "Error";
            } else if constexpr (std::is_same_v<T, std::shared_ptr<ResultInstance>>) {
                return "Result<" + v->getValueTypeName() + ", " + v->getErrorTypeName() + ">";
            } else if constexpr (std::is_same_v<T, std::shared_ptr<ffi::PtrInstance>>) {
                return "Ptr<Void>";
            } else if constexpr (std::is_same_v<T, std::shared_ptr<ffi::CBufferInstance>>) {
                return "CBuffer";
            } else if constexpr (std::is_same_v<T, std::shared_ptr<ffi::CStructInstance>>) {
                return "CStruct";
            } else if constexpr (std::is_same_v<T, std::shared_ptr<ffi::CArrayInstance>>) {
                return "CArray";
            } else if constexpr (std::is_same_v<T, std::shared_ptr<ffi::CCallbackInstance>>) {
                return "CCallback";
            } else {
                return "Unknown";
            }
        },
        value);
}

bool valuesEqual(const Value& a, const Value& b) {
    if (a.index() != b.index()) {
        return false;
    }

    return std::visit(
        [](const auto& lhs, const auto& rhs) -> bool {
            using T = std::decay_t<decltype(lhs)>;
            using U = std::decay_t<decltype(rhs)>;

            if constexpr (std::is_same_v<T, U>) {
                if constexpr (std::is_same_v<T, std::shared_ptr<ObjectInstance>>) {
                    return lhs.get() == rhs.get();  // Pointer equality for objects
                } else if constexpr (std::is_same_v<T, std::shared_ptr<EnumInstance>>) {
                    return lhs.get() == rhs.get();  // Pointer equality for enums
                } else if constexpr (std::is_same_v<T, std::shared_ptr<RecordType>>) {
                    return lhs.get() == rhs.get();  // Pointer equality for record types
                } else if constexpr (std::is_same_v<T, std::shared_ptr<RecordInstance>>) {
                    return lhs->equals(*rhs);  // Structural equality for record instances
                } else if constexpr (std::is_same_v<T, std::shared_ptr<ListInstance>>) {
                    return lhs.get() == rhs.get();  // Pointer equality for list instances
                } else if constexpr (std::is_same_v<T, std::shared_ptr<ListIterator>>) {
                    return lhs.get() == rhs.get();  // Pointer equality for list iterators
                } else if constexpr (std::is_same_v<T, std::shared_ptr<RepeatIterator>>) {
                    return lhs.get() == rhs.get();  // Pointer equality for repeat iterators
                } else if constexpr (std::is_same_v<T, std::shared_ptr<MapInstance>>) {
                    return lhs.get() == rhs.get();  // Pointer equality for map instances
                } else if constexpr (std::is_same_v<T, std::shared_ptr<MapIterator>>) {
                    return lhs.get() == rhs.get();  // Pointer equality for map iterators
                } else if constexpr (std::is_same_v<T, std::shared_ptr<SetInstance>>) {
                    return lhs.get() == rhs.get();  // Pointer equality for set instances
                } else if constexpr (std::is_same_v<T, std::shared_ptr<SetIterator>>) {
                    return lhs.get() == rhs.get();  // Pointer equality for set iterators
                } else if constexpr (std::is_same_v<T, std::shared_ptr<MapObject>>) {
                    return lhs.get() == rhs.get();  // Pointer equality for map objects
                } else if constexpr (std::is_same_v<T, std::shared_ptr<ErrorInstance>>) {
                    return lhs.get() == rhs.get();  // Pointer equality for error instances
                } else if constexpr (std::is_same_v<T, std::shared_ptr<ResultInstance>>) {
                    return lhs.get() == rhs.get();  // Pointer equality for result instances
                } else {
                    return lhs == rhs;
                }
            } else {
                return false;
            }
        },
        a, b);
}

}  // namespace o2l