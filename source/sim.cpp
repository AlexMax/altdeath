#include "sim.hpp"

extern "C" {

void UDMF_Scan(const char* p);

}

namespace sim {

void Map::load(const char* input) {
	UDMF_Scan(input);
}

}

