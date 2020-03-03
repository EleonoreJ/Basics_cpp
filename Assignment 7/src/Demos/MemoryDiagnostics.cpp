#include "MemoryDiagnostics.h"
#include "hashmap.h"
#include "error.h"
#include <sstream>
#include <unordered_map>
#include <typeindex>
#include <cxxabi.h> // Nonstandard, but supported on g++ and clang.
using namespace std;

namespace {
    /* Type --> Frequency */
    unordered_map<type_index, int>& allocationTable() {
        static unordered_map<type_index, int> instance;
        return instance;
    }

    /* Name --> Type */
    unordered_map<string, type_index>& lookupTable() {
        static unordered_map<string, type_index> instance;
        return instance;
    }
}

namespace MemoryDiagnostics {
    /* Injects the type into the appropriate table. */
    int registerSentinel(const std::type_info& type) {
        /* std::type_info does not guarantee that .name() will be at all human-readable.
         * Use this g++/clang-specific logic to "demangle" the name back into a human-readable
         * format.
         */
        int statusCode;
        auto* realName = abi::__cxa_demangle(type.name(), nullptr, nullptr, &statusCode);
        if (statusCode != 0) error("Internal error: Couldn't demangle name?");

        /* C++ type_info objects don't work well as keys in maps, but std::type_index
         * allows us to fix that.
         */
        lookupTable().insert(make_pair(string(realName), type_index(type)));

        free(realName);
        return 137; // You know, cause, why not?
    }

    void recordNew(const type_info& type) {
        allocationTable()[type_index(type)]++;
    }

    void recordDelete(const type_info& type) {
        allocationTable()[type_index(type)]--;
    }
}

/* If you were directed here by pressing F2 or fn+F2, you are looking at the implementation
 * of instancesOf rather than the prototype. Hover over instancesOf again and press F2 or
 * fn+F2 a second time to be taken to the prototype, which has all the relevant documentation.
 */
int instancesOf(const std::string& type) {
    auto itr = lookupTable().find(type);
    if (itr == lookupTable().end()) {
        /* Build a string of all the types we know of. */
        ostringstream msg;
        msg << "Unknown type passed into instancesOf. We know these types: ";

        for (const auto& entry: lookupTable()) {
            msg << entry.first << "    ";
        }

        error(msg.str());
    }

    return allocationTable()[itr->second];
}
