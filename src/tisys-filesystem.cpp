/*=====================================================================================*/

#include "tisys.hpp"
#include <iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <libgen.h>
#include <dirent.h>
#include <errno.h>


// Usado em Filesystem::file_type
string VETOR[][2] = {
	{ "avi", "Video:avi" },
	{ "bmp", "Image:bmp" },
	{ "bz2", "Package:bz2" },
	{   "c", "Code:c"   },
	{ "cpp", "Code:cpp" },
	{ "css", "MarkLing:css" },
	{ "dcm", "Image:dcm" },
	{ "deb", "Package:deb" },
	{ "flv", "Video:flv" },
	{ "gif", "Image:gif" },
	{  "gz", "Package:gz" },
	{"html", "MarkLing:html"},
	{"java", "Code:java"},
	{"jpeg", "Image:jpg" },
	{ "jpg", "Image:jpg" },
	{  "js", "Code:js"},
	{"json", "MarkLing:json" },
	{ "mp3", "Audio:mp3" },
	{ "mp4", "Video:mp4" },
	{ "ogg", "Audio:ogg" },
	{ "odt", "Document:odt"},
	{ "pas", "Code:pas" },
	{ "pdf", "Book:pdf" },	
	{ "php", "Code:php"},
	{ "png", "Image:png" },
	{  "py", "Code:py"  },
	{ "rar", "Package:rar" },
	{  "sh", "Code:sh"},
	{ "tex", "MarkLing:tex" },
	{ "tgz", "Package:tgz" },
	{  "ti", "Object" },
	{ "ttf", "Font:ttf" },
	{ "txt", "Text:txt" },	
	{ "wav", "Audio:wav" },
	{ "xml", "MarkLing:xml" },
	{ "zip", "Package:zip" },
};

/*-------------------------------------------------------------------------------------*/


/*=====================================================================================*/

int csystem(std::string cmd){
	return system(cmd.c_str());
}

std::string path_add(std::string base, std::string add1){
	if ( base == "" )
		return add1;
	if ( add1 == "" )
		return base;
	if ( base[ base.size()-1 ] == '/' || add1[0] == '/' )
		return base+add1;
	else
		return base+"/"+add1;
}

std::string path_add(std::string base, std::string add1, std::string add2){
	return path_add(path_add(base,add1), add2);
}

std::string path_add(std::string base, std::string add1, std::string add2, std::string add3){
	return path_add(path_add(path_add(base,add1),add2),add3);
}

std::string path_remove(std::string url){
	size_t pos = url.find_last_of('/');
	if ( pos == string::npos ){
		return "";
	} else {
		return url.substr(1,pos-1);
	}

}

std::string path_last(std::string url){
	size_t pos = url.find_last_of('/');
	if ( pos == string::npos ){
		return url;
	} else {
		return url.substr(pos+1);
	}
}

std::string path_absolute(std::string url){
	static char strbuf[4*1024];
	realpath(url.c_str(), strbuf);
	return strbuf;
}


std::string path_context (std::string classe, std::string _url){
	string url;
	if ( _url == "" )
		url = path_absolute(".");
	else
		url = path_absolute(_url);

	Filesystem fs;
	TiObj aux;

	while (true){
		if ( url.size() == 0 )
			return "";
		aux.classe = fs.folder_type(url);
		if ( aux.is(classe) ){
			break;
		}
		
		if ( url == "/" ){
			return "";
		} else {
			int i;
			for ( i=url.size()-1; i>0; i--){
				if ( url[i] == '/' ){
					break;
				}
			}
			if ( i == 0 )
				url.resize(1);
			else
				url.resize(i);
		}
	}
	return url;
}


void tiurl_explode(TiObj& out, std::string tiurl){
	out.clear();
	if ( tiurl.size() == 0 )
		return;
	
	// Explode the URL in all the '/'
	int i=0, base=0;
	string buf, name;
	if ( tiurl[0] == '/'){
		out["flag"] = "/"; i=1; base=1;
	} else if ( tiurl[0] == '~' ){
		out["flag"] = "~"; i=1; base=1;
	}
	for (; i<tiurl.size(); i++){
		if ( tiurl[i] == '/' ){
			if ( i-base > 0 ){
				TiObj* aux = new TiObj();
				buf   = tiurl.substr(base, i-base);
				int found = buf.find_last_of(":");
				if ( found == string::npos ){
					if ( buf.find_last_of("*") != string::npos )
						aux->set("class", "list");
					else
						aux->set("class", "cd");
					aux->set("name", buf);
				} else {
					name = buf.substr(found+1);
					aux->set("type", buf.substr(0, found) );
					if ( name == "" ){
						aux->set("class", "list");
					} else if ( name.find_last_of("*") != string::npos ){
						aux->set("class", "list");
					} else
						aux->set("class", "find");
					aux->set("name", name);
				}
				out.box += aux;
			}
			base = i+1;
		}
	}
	if ( base < i ){
		TiObj* aux = new TiObj();
		buf   = tiurl.substr(base, i-base);
		int found = buf.find_last_of(":");
		if ( found == string::npos ){
			if ( buf.find_last_of("*") != string::npos )
				aux->set("class", "list");
			else
				aux->set("class", "cd");
			aux->set("name", buf);
		} else {
			name = buf.substr(found+1);
			aux->set("type", buf.substr(0, found) );
			if ( name == "" ){
				aux->set("class", "list");
			} else if ( name.find_last_of("*") != string::npos ){
				aux->set("class", "list");
			} else
				aux->set("class", "find");
			aux->set("name", name);
		}
		out.box += aux;
	}
	cout << out;
}


void tiurl_sysobj (TiObj& out, std::string tiurl){
	out.clear();
	out.loadFile( path_add(tiurl, ".sysobj.ti") );
}

/*-------------------------------------------------------------------------------------*/


/*=====================================================================================*/

Filesystem::Filesystem(){
}

Filesystem::Filesystem(std::string cur_path, string root){
	this->cur_path = cur_path;
}


bool Filesystem::listdir(TiObj& out, std::string url){
	this->log("listdir");
	out.clear();

	string real_url = this->path_set(url);
	DIR *dp;
	struct stat buf;
	struct dirent *ep;    
	dp = opendir (real_url.c_str());
	if (dp != NULL){
		string item, item_url;
		while ( ep = readdir(dp) ){
			if ( strcmp(ep->d_name,".")==0 || strcmp(ep->d_name,"..")==0 ) 
				continue;
			item = path_add(url, ep->d_name);
			item_url = path_add(real_url, ep->d_name);
			if ( stat(item_url.c_str(), &buf) != -1 ){
				if (S_ISREG (buf.st_mode) ){
					string tmp = ep->d_name;
					TiObj *novo = new TiObj();
					novo->classe = this->file_type(item_url);
					novo->set("name", tmp);
					novo->set( "url", item);
					out.box += novo;
				} else if (S_ISDIR (buf.st_mode) ){
					TiObj *novo = new TiObj();
					novo->classe = this->folder_type(item_url);
					novo->set("name", ep->d_name);
					novo->set( "url", item);
					out.box += novo;
				}
			}

		}
		closedir(dp);
	} else {
		TiObj* error = new TiObj();
		error->classe = "ERROR";
		error->set("id", errno);
		error->set("msg", strerror(errno));
		error->set("url", url);
		out.box += error;
		return false;
	}

	return true;
}

bool Filesystem::listdirtree(TiObj& out, std::string url){
	this->log("listdirtree");
	TiObj buffer;
	this->listdir(buffer, url);
	for ( int i=0; i<buffer.box.size(); i++){
		TiObj& node = buffer.box[i];
		if ( node.is("Folder") ){
			this->listdirtree( out, path_add(url, node["name"].str) );
		} else {
			out.box += node;
		}
	}
	return true;
}

bool Filesystem::info(TiObj& out, std::string url){
	this->log("info");
	out.clear();
	struct stat buf;
	string file_url = this->path_set(url);
	if ( stat(file_url.c_str(), &buf) ){
		this->error("File " + url + " not exist");
		return false;
	} 

	// Set node variables
	struct tm* clock;
	char strbuf[1024];
	int permission = ((buf.st_mode & S_IRWXU) >> 6)*100 + ((buf.st_mode & S_IRWXG) >> 3)*10 + ((buf.st_mode & S_IRWXO));
	out["permission"] = (long int) permission;
	out["inode"] = (long int) buf.st_ino;
	out["name"] = path_last(file_url);
	out["url"] = path_absolute(file_url);
	out["uid"] = (long int) buf.st_uid;
	out["gid"] = (long int)buf.st_gid;
	out["size_bytes"]  = (long int)buf.st_size;
	out["size_blocks"] = (long int)buf.st_blocks;
	out["block_size"]  = (long int)buf.st_blksize;
	clock = gmtime(&(buf.st_atime));
	sprintf(strbuf,"%d/%d/%d,%d:%d:%d",clock->tm_year+1900,clock->tm_mon+1,clock->tm_mday,clock->tm_hour,clock->tm_min,clock->tm_sec); 
	out["atime"] = strbuf;
	clock = gmtime(&(buf.st_mtime));
	sprintf(strbuf,"%d/%d/%d,%d:%d:%d",clock->tm_year+1900,clock->tm_mon+1,clock->tm_mday,clock->tm_hour,clock->tm_min,clock->tm_sec); 
	out["mtime"] = strbuf;
	clock = gmtime(&(buf.st_ctime));
	sprintf(strbuf,"%d/%d/%d,%d:%d:%d",clock->tm_year+1900,clock->tm_mon+1,clock->tm_mday,clock->tm_hour,clock->tm_min,clock->tm_sec); 
	out["ctime"] = strbuf;
	
	// Get node type
	if (S_ISREG (buf.st_mode)){
		out.classe = this->file_type(file_url);
	} else if (S_ISLNK(buf.st_mode)){
		out.classe = "Link";
	} else if (S_ISDIR (buf.st_mode)){
		out.classe = this->folder_type(file_url);
	}
	return true;
}

bool Filesystem::newfolder(std::string url, mode_t mode){
	this->log("newfolder");
	struct stat st;
	int  status = 0;
	string real_url = this->path_set(url);
	if (stat(real_url.c_str(), &st) != 0){
		if (mkdir(real_url.c_str(), mode) != 0 && errno != EEXIST)
			status = -1;
	} else if (!S_ISDIR(st.st_mode)){
		errno = ENOTDIR;
		status = -1;
	}
	return true;
}

bool Filesystem::newfile(std::string url, mode_t mode){
	this->log("mkfile");
}

bool Filesystem::newlink(std::string to, std::string in){
	this->log("mklink");
}


bool Filesystem::rm(std::string url){
	this->log("rm");
	csystem("rm "+url);
}

bool Filesystem::rmdir(std::string url){
	this->log("rmdir");
	csystem("rmdir "+url);
}

bool Filesystem::rmtree(std::string url){
	this->log("rmtree");
	csystem("rm -rf "+url);
}

bool Filesystem::cp(std::string from, std::string to){
	this->log("cp");
	csystem("cp "+from+" "+to);
}

bool Filesystem::cptree(std::string from, std::string to){
	this->log("cptree");
	//csystem("cp -rf "+url);
}

bool Filesystem::mv(std::string from, std::string to){
	this->log("mv");
	csystem("mv "+from+" "+to);
}

bool Filesystem::rename(std::string  old, std::string novo){
	this->log("rename");
}

bool Filesystem::node_exist (std::string url){
	this->log("node_exist");
	struct stat buffer;
	string real_url = this->path_set(url); 
	return ( stat(real_url.c_str(), &buffer)==0 ); 
}

bool Filesystem::node_isfolder(std::string url){
	this->log("node_isfolder");
	struct stat buffer;
	string real_url = this->path_set(url);
	if ( stat(real_url.c_str(),&buffer) == 0){
		return (S_ISDIR(buffer.st_mode));
	} else
		return false;
}

bool Filesystem::node_isfile  (std::string url){
	this->log("node_isfile");
	struct stat buffer;
	string real_url = this->path_set(url);
	if ( stat(real_url.c_str(),&buffer) == 0){
		return (S_ISREG(buffer.st_mode));
	} else
		return false;
}

bool Filesystem::node_islink  (std::string url){
	struct stat buffer;
	string real_url = this->path_set(url);
	if ( stat(real_url.c_str(),&buffer) == 0){
		return (S_ISLNK(buffer.st_mode));
	} else
		return false;
}

bool Filesystem::node_isblock (std::string url){
	struct stat buffer;
	string real_url = this->path_set(url);
	if ( stat(real_url.c_str(),&buffer) == 0){
		return (S_ISBLK(buffer.st_mode));
	} else
		return false;
}

std::string Filesystem::node_type(std::string url){
	struct stat buffer;
	string real_url = this->path_set(url);
	if ( stat(real_url.c_str(),&buffer) != 0){
		return "";
	}
	if ( S_ISREG(buffer.st_mode) ){
		return file_type(url);
	} else if ( S_ISDIR(buffer.st_mode) ){
		return folder_type(url);
	}
}

bool Filesystem::mount(){
}

bool Filesystem::umount(){
}

std::string Filesystem::last_error(){
	return "["+this->status_func+"]: "+this->status_msg;
}

void Filesystem::error(std::string msg){
	this->status_msg = msg;
}

void Filesystem::log(std::string function){
	this->status_func = function;
	this->status_msg  = "";
}

std::string Filesystem::file_type  (std::string url){
	string ext = url.substr(url.find_last_of(".") + 1);
	int ini = 0;
	int end = sizeof(VETOR)/(2*sizeof(string));
	while (true){
		if ( ini >= end ){
			return "File";
		}
		int meio = (ini + end)/2;
		if ( ext == VETOR[meio][0] ){
			return string("File:")+VETOR[meio][1];
		} else if ( ext > VETOR[meio][0] ){
			ini = meio+1;
		} else
			end = meio;
	}
}

std::string Filesystem::folder_type(std::string url){
	string descfile = path_add(url,".sysobj.ti");
	FILE* fd   = fopen(descfile.c_str(), "r");
	if ( fd ){
		TiObj meta;
		meta.loadFile(descfile);
		fclose(fd);
		return string("Folder:")+meta.classe;
	} else {
		return "Folder";
	}
}

std::string Filesystem::path_set(std::string url){
	if ( url == "" ){
		if ( this->cur_path == "" )
			return ".";
		else
			return this->cur_path;
	} else if ( url[0] == '/' ){
		return path_add(this->root,url);
	} else {
		return path_add(this->cur_path, url);
	}
}

/*-------------------------------------------------------------------------------------*/









/*=====================================================================================


string  TiSys::getCtx(string classe, string _url){
	string url;
	if ( _url == "" ){
		url.resize(4096);
		char* url_buf = (char*) url.c_str();
		getcwd(url_buf, 4096);
	} else if ( _url[0] == '/' ){
		url = _url;
	} else {
		url.resize(4096);
		char* url_buf = (char*) url.c_str();
		getcwd(url_buf, 4096);
		url = path_add(url, _url);
	}

	if ( !TiSys::existNode(url) ){
		return "";
	}
	if(TiSys::isFile(url)) {
		url = path_remove(url);
	}

	TiObj aux;
	while ( url != "" ){
		aux.classe = Folder_getType(url);
		if ( aux.is(classe) )
			return url;
		url = path_remove(url);
	}
	return "";
}


/*-------------------------------------------------------------------------------------*/



/*=====================================================================================









void tiurl_explode(string& out_classe, string& out_name, string ptiurl){
	int pos=-1;
	for (int i=0; i<ptiurl.size(); i++){
		if ( ptiurl[i] == ':' )
			pos = i;
	}
	if ( pos >= 0 ){
		out_classe = ptiurl.substr(0, pos);
		out_name   = ptiurl.substr(pos+1);
	} else {
		out_classe = "";
		out_name   = ptiurl;
	}
}

int search_file(string& out, string ptiurl){
	TiObj folder;
	TiSys::info(folder, out);
	string classe,name; 
	tiurl_explode(classe,name,ptiurl);
	
	if ( ptiurl[0] == '%' ){
		string realpath = folder["url"].str;
		out = TiSys::getCtx(ptiurl.substr(1), realpath);
		if ( out == "" )
			return 1;
		return 0;
	} else {
		if ( classe == "" ){
			for ( int i=0; i<folder.box.size(); i++ ){
				TiObj& node = folder.box[i];
				if ( name == node["name"].str ){
					out += "/"+name;
					return 0;
				}
			}
			return 1;
		} else {
			for ( int i=0; i<folder.box.size(); i++ ){
				TiObj& node = folder.box[i];
				if ( node.is(classe) && name == node["name"].str ){
					out += "/"+name;
					return 0;
				}
			}
			
			string aux;
			for ( int i=0; i<folder.box.size(); i++ ){
				TiObj& node = folder.box[i];
				if ( node.is("Package") ){
					aux = out;
					search_file(aux, ptiurl);
				}
			}
			
			return 1;	
		}
	}
}



string TiSys::getUrl(string tiurl){
	if ( tiurl.size() == 0 )
		return "";
	
	
	// Explode a tiurl
	int i = (tiurl[0]=='@')?1:0;
	char c;
	string buf = "";
	vector<string> v_url;
	for (; i<tiurl.size(); i++){
		c = tiurl[i];
		if ( c == '/' ){
			v_url.push_back(buf);
			buf = "";
		} else
			buf += c;
	}
	if ( buf.size() > 0 )
		v_url.push_back(buf);
	for (i=0;i<v_url.size();i++){
		cout << v_url[i] << endl;
	}
	
	// Processa o tiurl
	if ( v_url.size() == 0 )
		return "";
	
	string base = ".";
	for (i=0; i<v_url.size(); i++){
		int error = search_file(base, v_url[i]);
		if ( error ){
			cout << "ERROR\n";
			return "";
		} else {
			cout << "OK\n";
		}
		
	}
	
	
	return "";
}


/*-------------------------------------------------------------------------------------*/



















