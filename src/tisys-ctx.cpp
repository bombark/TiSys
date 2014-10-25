#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <tiobj.hpp>
#include <tiarg.hpp>
#include "tisys.hpp"

using namespace std;




int main(int argc, char** argv){
	TiObj args, folder;
	getArgs(args, argc, argv);

	string classe = args.atStr("_Akk", "");
	string from   = args.atStr("from", "");

	if ( classe == "" ){
		return 0;
	}

	cout  << path_context(classe, from) << endl;
	return 0;
}

