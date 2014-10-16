#include <stdio.h>
#include <iostream>
#include <tiobj.hpp>

using namespace std;




int main(int argc, char** argv){
	TiObj obj;
	TiStream stream;
	stream.open(stdin);
	while ( stream.next(obj) ){
		if ( obj.is("File") )
			cout << obj;
	}
}

