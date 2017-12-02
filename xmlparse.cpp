#include "xmlparse.h"
#include "map.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <math.h>
#include <algorithm>
#include <cstring>

#include <rapidxml.hpp>



/*-------------------------------------------------------------------------------------*/
/*----------------------------Helper functions declarations----------------------------*/
/*-------------------------------------------------------------------------------------*/
int stringtoint(const char*);
void addTileset(const std::string&, rapidxml::xml_node<>*, std::vector<struct Map::Tileset>&);
void parseTilesetFromTSX(const char*, struct Map::Tileset&);
void configMapping(rapidxml::xml_node<>*, std::vector<int**>&, int, int);





/*-------------------------------------------------------------------------------------*/
/*----------------------------Helper functions definitions-----------------------------*/
/*-------------------------------------------------------------------------------------*/
int stringtoint(const char* sValue){
	int iValue = 0;
	std::string s(sValue);
	int digit = 0;
	//trim s based upon encounter of character '.'
	
	for(std::string::iterator i=s.end()-1; i>=s.begin(); --i){
		if(*i=='-'){
			iValue *= -1;
		}else{
			int curr = *i-48;
			if(curr>=0 && curr<=9){
				iValue+=curr*powf(10,digit);
			}else{
				throw std::runtime_error("ERROR: Non-value character");
			}
			++digit;
		}
	}
	return iValue;
}

void addTileset(const std::string& source, rapidxml::xml_node<>* element, std::vector<struct Map::Tileset>& tilesets){
	struct Map::Tileset new_tileset;
	
	new_tileset.begin_ID = stringtoint(element->first_attribute("firstgid")->value());
	new_tileset.texture = nullptr;

	rapidxml::xml_node<>* image = element->first_node("image");
	if(image!=nullptr){
		new_tileset.source = image->first_attribute("source")->value();
		new_tileset.width = stringtoint(image->first_attribute("width")->value());
		new_tileset.height = stringtoint(image->first_attribute("height")->value());
		new_tileset.end_ID = new_tileset.begin_ID + stringtoint(element->first_attribute("tilecount")->value()) - 1;
		new_tileset.column_count = stringtoint(element->first_attribute("columns")->value());
		new_tileset.row_count = stringtoint(element->first_attribute("tilecount")->value()) / new_tileset.column_count;
	}else{
		std::string tsx_path = source.substr(0, source.find_last_of('/')) + "/" + element->first_attribute("source")->value();
		parseTilesetFromTSX(tsx_path.c_str(), new_tileset);
	}

	tilesets.push_back(new_tileset);
}

void parseTilesetFromTSX(const char* source, struct Map::Tileset& new_tileset){
	std::fstream fs(source);
	std::vector<char> buffer((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');
	rapidxml::xml_document<> doc;
	doc.parse<0>(&buffer[0]);

	rapidxml::xml_node<>* tileset = doc.first_node("tileset");
	if(tileset!=nullptr){		
		new_tileset.column_count = stringtoint(tileset->first_attribute("columns")->value());
		new_tileset.row_count = stringtoint(tileset->first_attribute("tilecount")->value()) / new_tileset.column_count;
		new_tileset.end_ID = new_tileset.begin_ID + stringtoint(tileset->first_attribute("tilecount")->value());
		rapidxml::xml_node<>* image = tileset->first_node("image");
		if(image!=nullptr){
			new_tileset.source = image->first_attribute("source")->value();
			new_tileset.width = stringtoint(image->first_attribute("width")->value());
			new_tileset.height = stringtoint(image->first_attribute("height")->value());
		}else{
			throw std::runtime_error("ERROR: no image layer in tilemap or tsx file");
		}
	}else{
		throw std::runtime_error("ERROR: no tileset layer in tsx file");
	}
}

void configMapping(rapidxml::xml_node<>* element, std::vector<int**>& mapping, int width, int height){
	
	//init a new map
	int** new_map = new int*[height]();
	for(int i=0; i<height; ++i){
		new_map[i] = new int[width]();
	}

	//parse data into string into new map
	std::string data_string(element->first_node("data")->value());
	std::replace(data_string.begin(), data_string.end(), ',', ' ');
	std::cout<<data_string<<"\n";
	std::stringstream ss(data_string);
	std::string line("");
	int i = 0;
	int j = 0;
	getline(ss, line); //get rid of line break before data section
	while(getline(ss, line)){
		std::stringstream line_ss(line);
		while(line_ss>>new_map[i][j++] && j<width);
		++i;
		j = 0;
	}
	
	//push new map to mapping
	mapping.push_back(new_map);
}












/*-------------------------------------------------------------------------------------*/
/*---------------------------Namespace functions---------------------------------------*/
/*-------------------------------------------------------------------------------------*/

void XMLParse::parseToTilemap(const char* source, struct Map::Tilemap& tilemap, std::vector<struct Map::Tileset>& tilesets, std::vector<int**>& mapping){
	//load xml file to rapidxml::xml_document<> instance
	std::fstream fs(source);
	std::vector<char> buffer((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');
	rapidxml::xml_document<> doc;
	doc.parse<0>(&buffer[0]);

	//get root node
	rapidxml::xml_node<>* root = doc.first_node();
	if(root!=nullptr && strcmp(root->name(), "map")==0){
		std::cout<<"Loading map \n";
	}else{
		throw std::runtime_error("ERROR: Failed to load xml");
	}

	//get tilemap info
	tilemap.width = stringtoint(root->first_attribute("width")->value());
	tilemap.height = stringtoint(root->first_attribute("height")->value());
	tilemap.tile_width = 1.0f/tilemap.width;
	tilemap.tile_height = 1.0f/tilemap.height;

	//iterate through all elements in xml
	for(rapidxml::xml_node<>* element=root->first_node(); element!=(rapidxml::xml_node<>*)0; element=element->next_sibling()){
		std::string element_type(element->name());
		if(element_type=="tileset"){
			addTileset(std::string(source), element, tilesets);
		}else if(element_type=="layer"){
			configMapping(element, mapping, tilemap.width, tilemap.height);
		}else{
			throw std::runtime_error("ERROR: unrecognized tilemap element");
		}
	}
}
