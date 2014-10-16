#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/if_link.h>
#include <string.h>

#define NETPKG_MAX 128

typedef struct {
	char name[255];
	char ipv4[32];
	char ipv6[64];
	unsigned int rx_bytes, tx_bytes;
	unsigned int rx_packs, tx_packs;
} Net;

typedef struct {
	int size;
	Net data[NETPKG_MAX];
} NetPkg;

void NetPkg_init(NetPkg* this){
	this->size = 0;
}

Net* NetPkg_add (NetPkg* this, char* name){
	if ( this->size < NETPKG_MAX ){
		int   id = this->size;
		Net* net = &this->data[id]; 
		strcpy (net->name, name);
		strcpy (net->ipv4, "");
		strcpy (net->ipv6, "");
		net->rx_packs = net->tx_packs = 0;
		net->rx_bytes = net->tx_bytes = 0;		 
		this->size += 1;
		return net;
	}
	return NULL;
}

Net* NetPkg_get (NetPkg* this, char* name){
	int i;
	for (i=0; i<this->size; i++){
		if ( strcmp(this->data[i].name,name) == 0 )
			return &this->data[i];
	}
	return NULL;
}



void printAll(){
	static NetPkg netpkg;
	NetPkg_init(&netpkg);

	struct ifaddrs *ifaddr, *ifa;
	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		exit(EXIT_FAILURE);
	}

	Net* net;
	int family, s;
	char host[NI_MAXHOST];
	for (ifa=ifaddr; ifa!=NULL; ifa=ifa->ifa_next) {
		if (ifa->ifa_addr == NULL)
			continue;
		family = ifa->ifa_addr->sa_family;

		net = NetPkg_get(&netpkg, ifa->ifa_name);
		if ( net == NULL ){
			net = NetPkg_add(&netpkg, ifa->ifa_name);
		}



		if (family == AF_INET){ // || family ) {  
			s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
			if (s != 0) {
				printf("getnameinfo() failed: %s\n", gai_strerror(s));
				exit(EXIT_FAILURE);
			}
			strcpy (net->ipv4,host);
		} else if (family == AF_INET6 ){
			s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in6), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
			if (s != 0) {
				printf("getnameinfo() failed: %s\n", gai_strerror(s));
				exit(EXIT_FAILURE);
			}
			strcpy (net->ipv6,host);
		} else if (family == AF_PACKET && ifa->ifa_data != NULL) {
			struct rtnl_link_stats *stats = ifa->ifa_data;
			net->rx_packs = stats->rx_packets;
			net->tx_packs = stats->tx_packets;
			net->rx_bytes = stats->rx_bytes;
			net->tx_bytes = stats->tx_bytes;
		}



	}

	int i;
	for (i=0; i<netpkg.size; i++){
		printf("Device:Network {\n");
		printf("\tname='%s';\n", netpkg.data[i].name);
		printf("\tinet='%s';\n", netpkg.data[i].ipv4);
		printf("\tinet6='%s';\n", netpkg.data[i].ipv6);
		printf("\ttx_bytes=%u;\n", netpkg.data[i].tx_bytes);
		printf("\trx_bytes=%u;\n", netpkg.data[i].rx_bytes);
		printf("\ttx_packs=%u;\n", netpkg.data[i].tx_packs);
		printf("\trx_packs=%u;\n", netpkg.data[i].rx_packs);
		printf("}\n");
	}


	freeifaddrs(ifaddr);
}




int main(int argc, char *argv[]){
	printAll();	
	return 0;
}
