#include <iostream>
#include <tiobj.hpp>
#include <tiarg.hpp>
#include "tisys.hpp"

using namespace std;



int main(int argc, char** argv){
	TiObj args, folder;
	getArgs(args, argc, argv);

	
	if ( args.has("_Akk") ){
		TiSys::info(folder, args["_Akk"].Str());
		cout << folder;
	}





	/*if ( argc < 2 ){
		cout << "ERROR {\n";
		cout << "\tmsg: \"Syntax: " << argv[0] << " File ...\";\n";
		cout << "}\n";
		return 1;
	}
	TiObj folder;
	if ( argc == 2 ){
		TiSys::info(folder, argv[1]);
		cout << folder;
	} else {
		for (int i=1; i<argc; i++){
			TiSys::info(folder, argv[i]);
		}
		return 0;
	}*/
}

