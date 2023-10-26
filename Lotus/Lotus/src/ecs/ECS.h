#pragma once
#include <bitset>
const unsigned int MAX_COMPONENTS = 32;
// Signature
// A bitset (1's and 0's) to keep track of which components an entity has
// helps keep track of which entities a system is interested in
typedef std::bitset<MAX_COMPONENTS> Signature;