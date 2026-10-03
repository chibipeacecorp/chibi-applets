#pragma once
#include "core.h"


void freeModFiles();
s_ModuleConfig getModuleConfig();
toml_datum_t getModuleContent();
s_Element getElementFromToml(toml_datum_t toml);