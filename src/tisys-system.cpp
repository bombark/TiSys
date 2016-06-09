#include "tisys.hpp"

using namespace std;


/*=====================================================================================*/

TiObj System::listDevice() {
}
	
TiObj System::listDeviceBlock() {
	TiObj raw, out;
	FILE* fd;
	long int buf_int;
	char buf_char[1024];
	//TiSys::info(raw, "/sys/class/block");

	for (int i=0; i<raw.size(); i++){
		TiObj block;
		string block_url = raw.box(i).atStr("url");
		fd = fopen(path_add(block_url,"partition").c_str(), "r");
		if ( fd ){
			buf_int = 0;
			fscanf(fd, "%s", buf_char);
			if ( buf_char )
				block.set ("class","Device:Block:Partition");
			else
				block.set ("class","Device:Block");
			fclose(fd);
		} else
			block.set ("class","Device:Block");

		block["name"] = path_last(block_url);
		// Get size of block
		buf_int = 0;
		fd = fopen(path_add(block_url,"size").c_str(), "r");
		if ( fd ){
			fscanf(fd, "%ld", &buf_int);
			fclose(fd);
		}
		block["size"] = (long int) buf_int;
		// Get number of device
		buf_char[0] = '\0';
		fd = fopen(path_add(block_url,"dev").c_str(), "r");
		if ( fd ){
			fscanf(fd, "%s", buf_char);
			fclose(fd);
		}
		block["dev"] = buf_char;
		// get if removable
		buf_int = 0;
		fd = fopen(path_add(block_url,"removable").c_str(), "r");
		if ( fd ){
			fscanf(fd, "%ld", &buf_int);
			fclose(fd);
		}
		block["is_removable"] = buf_int;
		// Get if only read
		buf_int = 0;
		fd = fopen(path_add(block_url,"ro").c_str(), "r");
		if ( fd ){
			fscanf(fd, "%ld", &buf_int);
			fclose(fd);
		}
		block["is_readonly"] = buf_int;

		out.box() += block;
	}

	cout << out;
}

TiObj System::listDeviceEthernet(){
}

TiObj System::listDeviceVideo(){
}

TiObj System::listDeviceInputVideo(){
}

TiObj System::listDeviceAudio(){
}

TiObj System::listDeviceInputAudio(){
}

TiObj System::listUser(){
	TiObj out;
	FILE* fd = fopen("/etc/passwd", "r");
	if ( !fd ){
		return out;//false;
	}
	char c, token[1024];
	int cursor=0, state=0;
	TiObj cur;
	cur.set ("class","User");

	char fields[7][10] = {"name","isshadow","uid","gid","info","home","command"};
	while ( fscanf(fd, "%c", &c) != EOF ){
		if ( c == ':' ){
			token[cursor] = '\0';
			cursor = 0;
			if ( state == 0 ){
				if ( strcmp(token, "root") == 0 ){
					cur.set ("class","User:Root");
				}
				cur.set(fields[state], token);
			} else if ( state == 1 ){	
				if ( strcmp(token, "x") == 0 ){
					cur.set (fields[state],"y");
				} else
					cur.set (fields[state],"n");
			} else if ( state == 2 || state ==3 ){
				cur.set(fields[state], atoi(token));
			} else
				cur.set(fields[state], token);
			state += 1;
			if ( state > 7 ){
				return out;//false;
			}
		} else if ( c == '\n' ){
			if ( state < 7 ){
				token[cursor] = '\0';
				cur.set(fields[state], token);
			}
			state = cursor = 0;
			if ( cur.atInt("uid") >= 500 && cur.atInt("uid") < 65534 ){ 
				cur.set (fields[state],"User:Login");
			} else
				cur.set (fields[state],"User");
			out.box() += cur;
			cur.create();
		} else {
			token[cursor++] = c;
			if (cursor >= 1024){
				return out;//false;
			}
		}
	}
	return out;//true;
}

TiObj System::listCmd(){
}

TiObj System::listWifi(){
}

/*-------------------------------------------------------------------------------------*/

