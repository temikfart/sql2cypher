#pragma once

#include <iostream>

#include "SCC/ast/nodes/inode.h"
#include "SCC/config/scc_args.h"
#include "SCC/config/scc_config.h"
#include "SCC/lexer/lexer.h"
#include "SCC/log/log.h"
#include "SCC/translator/translator.h"
#include "SCC/dump/tree_dump.h"
#include "SCC/parser/parser.h"

#include "logger/initializers/console_appender_initializer.hpp"
#include "logger/initializers/file_appender_initializer.hpp"
#include "logger/log.hpp"
