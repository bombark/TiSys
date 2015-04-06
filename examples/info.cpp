#include <iostream>
#include "../src/tisys.hpp"


int main(int argc, char** argv){
	cout << "Example" <<endl;
	Filesystem fs;

	if ( argc == 1 )
		fs.info(".");
	else
		fs.info( argv[1] );

	cout << fs;
	return 0;
}
