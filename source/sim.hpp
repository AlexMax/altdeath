#ifndef SIM_HPP
#define SIM_HPP

#include <memory>
#include <vector>

namespace sim {

struct MapVertex {
	double x;
	double y;
	MapVertex(double x, double y) : x(x), y(y) { }
};

struct MapSector {
	int heightfloor;
	int heightceiling;
};

struct MapSide {
	std::weak_ptr<MapSector> sector;
};

struct MapLine {
	std::weak_ptr<MapVertex> v1;
	std::weak_ptr<MapVertex> v2;
	std::weak_ptr<MapSide> sidefront;
	std::weak_ptr<MapSide> sideback;
};

struct Map {
	std::vector<std::shared_ptr<MapVertex> > vertexes;
	std::vector<std::shared_ptr<MapSector> > sectors;
	std::vector<std::shared_ptr<MapSide> > sides;
	std::vector<std::shared_ptr<MapLine> > lines;
	void load(const char* input);
};

}

#endif
