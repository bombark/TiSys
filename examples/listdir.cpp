#include <iostream>
#include "../src/tisys.hpp"

using namespace std;


int main(int argc, char** argv){
	cout << "Example" <<endl;
	Filesystem fs;

	if ( argc == 1 )
		fs.listdir(".");
	else
		fs.listdir( argv[1] );

	cout << fs;
	return 0;
}
