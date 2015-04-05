#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <tiobj.hpp>
#include <tiarg.hpp>
#include "tisys.hpp"

using namespace std;


 
// ANSI-Cores
#define C_VERMELHO     "{FONTE}33[41m{FONTE}33[37m"
#define C_VERDE        "{FONTE}33[42m{FONTE}33[30m"
#define C_LARANJA      "{FONTE}33[43m{FONTE}33[30m"
#define C_AZUL         "{FONTE}33[44m{FONTE}33[37m"
#define C_ROSA         "{FONTE}33[45m{FONTE}33[37m"
#define C_CIANO        "{FONTE}33[46m{FONTE}33[30m"
#define C_BRANCO       "{FONTE}33[47m{FONTE}33[30m"
#define C_PRETO        "{FONTE}33[40m{FONTE}33[37m"
#define C_RESTAURA       "{FONTE}33[00m"


/*struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

	printf ("lines %d\n", w.ws_row);
    printf ("columns %d\n", w.ws_col);*/


void printBox(TiBox& box){
	for (uint i=0; i<box.size(); i++){
		cout << box[i].toString() << endl;
	}
}


int main(int argc, char** argv){
	TiObj args, folder;
	getArgs(args, argc, argv);
	Filesystem fs;

	string classe = args.atStr("_Akk");
	string    url = args.atStr("from", "");

	/*TiObj aaa;
	tiurl_explode(aaa, url);
	return 0;*/

	if ( classe != "" ){
		if ( classe == "User" ){
			/*TiObj aux;
			TiSys::listUsers(aux);
			cout << aux;*/
		} else {
			fs.listdir(url);
			fs.select(classe).orderby("name");
			printBox(fs.box);
		}
	} else {
		fs.listdir(folder, url);
		fs.orderby("name");
		printBox(folder.box);
	}
}

