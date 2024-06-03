#include "SCC/translator/cypher/schema.h"

namespace scc::translator::cypher {

Schema::Schema(std::string database_name) : database_name(std::move(database_name)) {}

} // scc::translator::cypher
