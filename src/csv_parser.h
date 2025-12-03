#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include "netlist.h"

// Parse a CSV hypergraph netlist file using memory pool + adjacency list
Netlist* parseCSVNetlist(const char* filename, int* moduleCountOut);

#endif