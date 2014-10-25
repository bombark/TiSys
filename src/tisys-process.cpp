#include "tisys.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

using namespace std;



TiProcess::~TiProcess(){
	this->finalize();
}

bool TiProcess::start(std::string cmd){
	int wpipe[2];//write to child pipe
	int rpipe[2];//read from child pipe

	char aux[8096];
	for (int i=0; i<cmd.size(); i++){
		
	}


	//initiate pipes here
	if(pipe(wpipe)){
		this->lasterror = "write pipe creation failed";
		return false;
	}
	if(pipe(rpipe)){
		this->lasterror = "read pipe creation failed";
		return false;
	}
	pid_t t_id = fork();

	//child
	if(t_id==0){
		//close unused pipe ends
		close(wpipe[1]);//read from wpipe
		close(rpipe[0]);//write to rpipe
		//handle stdin & stdout pipes
		dup2(wpipe[0],STDIN_FILENO);//reroute stdin to wpipe[0]
		dup2(rpipe[1],STDOUT_FILENO);//reroute stdout to rpipe[1]
		//close pipes since they are copied??
		close(wpipe[0]);
		close(rpipe[1]);
		//prepare array for execv
		char** args;
		args = new char*[2];
		args[0] = "ls";
		args[1] = NULL;

		for ()



		int test = execv("simples",args); 
		if(test==-1){
			this->lasterror = strerror(errno);
		}
		//delete[] args[0];
		delete[] args;
		exit(0);
	} else if(t_id>0) {//parent
		//close unused pipe ends
		close(wpipe[0]);  //write to wpipe
		close(rpipe[1]);  //read from rpipe
		this->id  = t_id;
		this->wstream = fdopen(wpipe[1],"w");
		this->rstream = fdopen(rpipe[0],"r");
	} else {//failure to fork
		cout << "Fork failed" << endl;
		return false;
	}
	return true;
}

int TiProcess::exec(std::string& out, std::string cmd){
	this->start(cmd);
	int status = this->recvAll(out);
	return status;
}

void TiProcess::send(std::string data){
	fprintf(wstream,"%s", data.c_str());
}

void TiProcess::recv(std::string& out){
	out.resize(1024);
	char* ptr = (char*)out.c_str();
	fgets(ptr, 1024, this->rstream);
}

int TiProcess::recvAll(std::string& out){
	int status = this->wait();
	out = "";
	char buffer[4096];
	while ( !feof(this->rstream) ){
		fgets(buffer, 4096, this->rstream);
		out += buffer;
	}
	this->finalize();
	return status;
}

void TiProcess::recvObj(){
	char buffer[1024];
	string obj_str;
	unsigned read = fread(buffer, sizeof(char), 1024, this->rstream);
	for (int i=0; i<read; i++){
		if ( buffer[i] == '{' ){
		}
	}
}

void TiProcess::flush(){
	fflush(this->wstream);
}

int TiProcess::wait(){
	int state;
	waitpid(this->id,&state,0);
	return state;
}

void TiProcess::finalize(){
	if (wstream){
		fclose(wstream);
		this->wstream = NULL;
	}
	if (rstream){
		char buffer[1024];
		while ( !feof(this->rstream) ){
			fread(buffer,sizeof(char), 1024, this->rstream);
		}
		fclose(rstream);
		this->rstream = NULL;
	}
}


