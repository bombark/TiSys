#ifndef TISYS_HPP
#define TISYS_HPP

#include <tiobj.hpp>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


std::string path_add(std::string base, std::string add1);
std::string path_add(std::string base, std::string add1, std::string add2);
std::string path_add(std::string base, std::string add1, std::string add2, std::string add3);
std::string path_remove  (std::string url);
std::string path_last    (std::string url);
std::string path_absolute(std::string url);
std::string path_context (std::string classe, std::string _url=".");


void tiurl_explode(TiObj& out, std::string tiurl);
void tiurl_sysobj (TiObj& out, std::string tiurl);

int  csystem(std::string cmd);

class Filesystem {
	std::string root;
	std::string cur_path;
	std::string status_func;
	std::string status_msg;
	int         status_code;

  public:
	Filesystem();
	Filesystem(std::string cur_path, string root="");

	bool listdir    (TiObj& out, std::string url=".");
	bool listdirtree(TiObj& out, std::string url=".");
	bool info       (TiObj& out, std::string url=".");
	//bool select(TiObj& out, string query, string url);
	
	bool newfolder(std::string url, mode_t mode=0755);
	bool newfile  (std::string url, mode_t mode=0644);
	bool newlink(std::string to, std::string in="");

	
	bool rm    (std::string url);
	bool rmdir (std::string url);
	bool rmtree(std::string url);

	bool cp    (std::string from, std::string to);
	bool cptree(std::string from, std::string to);
	bool mv    (std::string from, std::string to);
	bool rename(std::string  old, std::string novo);
	
	bool node_exist   (std::string url);
	bool node_isfolder(std::string url);
	bool node_isfile  (std::string url);	
	bool node_islink  (std::string url);
	bool node_isblock (std::string url);
	
	std::string node_type  (std::string url);
	
	bool mount ();
	bool umount();
	
	std::string last_error();
	
	std::string file_type  (std::string url);
	std::string folder_type(std::string url);

  private:
	void error(std::string msg);
	void log  (std::string function);
	std::string path_set   (std::string url);
};


class Objsystem : public Filesystem {
	bool list    (TiObj& out, std::string tiurl);
	bool list    (TiObj& out, std::string query, std::string tiurl);
	bool listtree(TiObj& out, std::string query, std::string tiurl);
	bool info    (TiObj& out, std::string tiurl);
	
	//bool context (std::string url);
	
	bool mkobj(std::string tiurl, TiObj& data);
};


class System {
	bool listDevice           (TiObj& out);
	bool listDeviceBlock      (TiObj& out);
	bool listDeviceEthernet   (TiObj& out);
	bool listDeviceVideo      (TiObj& out);
	bool listDeviceInputVideo (TiObj& out);
	bool listDeviceAudio      (TiObj& out);
	bool listDeviceInputAudio (TiObj& out);
	
	bool listUser (TiObj& out);
	bool listCmd  (TiObj& out);
	bool listWifi (TiObj& out);
	
	//bool listLib();
};








class TiProcess {
	std::string buf_send;
	std::string buf_recv;
	std::string lasterror;
	FILE *wstream;
	FILE *rstream;
	pid_t id;
	

  public:
	~TiProcess();
	bool start(std::string cmd);
	int  exec (std::string& out, std::string cmd);

	void send   (std::string data);
	void recv   (std::string& out);
	int  recvAll(std::string& out);
	void recvObj();

	void flush();
	int  wait ();
	void finalize();
};


















/*namespace TiSys{
	// Gerenciamento de Path
	std::string path_add(std::string base, std::string add1);
	std::string path_add(std::string base, std::string add1, std::string add2);
	std::string path_add(std::string base, std::string add1, std::string add2, std::string add3);
	std::string path_remove(std::string url);
	std::string path_last(std::string url);
	std::string path_absolute (std::string url);

	int    newFolder(std::string path, mode_t mode=0755);

	bool   existNode(std::string url);
	bool   isFolder(std::string url);
	bool   isFile(std::string url);

	int    info(TiObj& out, std::string filename, std::string base="");
	string getCtx(string classe, string _url="");
	void   getArgs(TiObj& out, int argc, char** argv);

	
	string getUrl(string tiurl);
	

	void listUsers(TiObj& out);
	void listBlockDevice(TiObj& out);


	bool listRecursive( TiObj& out, std::string classe, std::string url, std::string base=".");
}*/



#endif
