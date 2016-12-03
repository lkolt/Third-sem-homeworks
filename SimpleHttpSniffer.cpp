#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>

#include<netinet/tcp.h>   //Provides declarations for tcp header
#include<netinet/ip.h>    //Provides declarations for ip header

#define exit_on_error(expr) do { \
	if (-1 == (expr)) { \
		perror(# expr); \
		exit(1); \
	} \
} while(0)

FILE *file;
#define BUF_SZ 4096

int iter = 0;
char in_buf[BUF_SZ];

void ascii_print(char *buf, int sz) {
	const int width = 32;
	for (int i = 0; i < sz; ++i) {
		fprintf(file, "%c", buf[i]);
	}

	if ((sz % width) != 0) {
        fprintf(file, "\n");
    }
    fprintf(file, "\n");
}

void print_ip_header(char* buf, int sz){
    struct iphdr *iph = (struct iphdr *)buf;
    struct sockaddr_in source,dest;
    int iphdrlen =iph->ihl*4;

    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;

    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;

    fprintf(file,"\n");
    fprintf(file,"IP Header:\n");
    fprintf(file,"*IP Version        : %d\n", (unsigned int)iph->version);
    fprintf(file,"*IP Header Length  : %d Bytes\n", ((unsigned int)(iph->ihl))*4);
    fprintf(file,"*TTL               : %d\n", (unsigned int)iph->ttl);
    fprintf(file,"*IP Total Length   : %d Bytes\n", ntohs(iph->tot_len));
    fprintf(file,"*Protocol          : %d\n", (unsigned int)iph->protocol);
    fprintf(file,"*Checksum          : %d\n", ntohs(iph->check));
    fprintf(file,"*Source IP         : %s\n", inet_ntoa(source.sin_addr));
    fprintf(file,"*Destination IP    : %s\n", inet_ntoa(dest.sin_addr));
}

void print_packet(char* buf, int sz){
    struct iphdr *iph = (struct iphdr *)buf;
    int iphdrlen = iph->ihl*4;

    struct tcphdr *tcph=(struct tcphdr*)(buf + iphdrlen);

    fprintf(file,"\n\n***********************TCP Packet #%d*************************\n", iter);

    print_ip_header(buf, sz);

    fprintf(file, "\n");
    fprintf(file, "TCP Header\n");
    fprintf(file, "*Source Port          : %u\n", ntohs(tcph->source));
    fprintf(file, "*Destination Port     : %u\n", ntohs(tcph->dest));;
    fprintf(file, "*Header Length        : %d BYTES\n", (unsigned int)tcph->doff * 4);
    fprintf(file, "*Checksum             : %d\n", ntohs(tcph->check));
    fprintf(file, "\n");
    fprintf(file, "                        DATA Dump                         ");
    fprintf(file, "\n");

    ascii_print(buf + iphdrlen + tcph->doff * 4 , (sz - tcph->doff * 4 - iph->ihl * 4));

    fprintf(file,"\n###########################################################");
}

void process_packet(char* buf, int sz)
{
    struct iphdr *iph = (struct iphdr*)buf;
    if (iph->protocol == 6){ // tcp
        print_packet(buf , sz);
    } else {
        printf("Cant recognize protocol of the packet\n");
    }
}


int main(int argc, char *argv[]) {
	file = fopen("log2.txt", "w");

    int raw_sock = socket(AF_INET , SOCK_RAW , IPPROTO_TCP);
    exit_on_error(raw_sock);

	int read_cnt;
	while (0 < (read_cnt = read(raw_sock, in_buf, BUF_SZ))) {
        printf("Get packet #%d\n", ++iter);
        process_packet(in_buf, read_cnt);
        fflush(stdout);
	}
}
