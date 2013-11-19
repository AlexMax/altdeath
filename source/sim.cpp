#include "sim.hpp"
#include "udmf.hpp"

namespace sim {

void Map::load(const char* input) {
	udmf::parse(input);
}

}

