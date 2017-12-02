#include "map.h"



#ifndef XML_PARSE_H
#define XML_PARSE_H

namespace XMLParse{

		void parseToTilemap(const char*, struct Map::Tilemap&, std::vector<struct Map::Tileset>&, std::vector<int**>&);

};

#endif
