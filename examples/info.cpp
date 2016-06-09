#include <iostream>
#include "../src/tisys.hpp"

using namespace std;


int main(int argc, char** argv){
	cout << "Example" <<endl;
	Filesystem fs;

	TiObj res(0);
	if ( argc == 1 )
		res = fs.info(".");
	else
		res = fs.info( argv[1] );

	cout << res;
	
	res = fs.ls(".");
	cout << res;
	
	return 0;
}
