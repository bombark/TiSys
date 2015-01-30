#include <iostream>
#include "tisys.hpp"


int main(int argc, char** argv){
	Filesystem fs;
	string url = ".";
	if ( argc > 1 )
		url = argv[1];

	TiObj node;
	fs.info(node, url);


	return 0;
}
