#include <iostream>
#include "tisys.hpp"

using namespace std;


int main(int argc, char** argv){
	Filesystem fs;
	if ( argc == 1 )
		cout << fs.node_type(".") << endl;
	else
		cout << fs.node_type( argv[1] ) << endl;
	return 0;
}
