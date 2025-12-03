#ifndef PARSER_H
#define PARSER_H

#include "netlist.h"

Netlist* parseNetlistFile(const char* filename, int* moduleCountOut);

#endif
