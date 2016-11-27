#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>

#include<netinet/ip_icmp.h>   //Provides declarations for icmp header
#include<netinet/udp.h>   //Provides declarations for udp header
#include<netinet/tcp.h>   //Provides declarations for tcp header
#include<netinet/ip.h>    //Provides declarations for ip header

#define exit_on_error(expr) do { \
	if (-1 == (expr)) { \
		perror(# expr); \
		exit(1); \
	} \
} while(0)

#define BUF_SZ 2048

char in_buf[BUF_SZ];

void hex_print(char *buf, int len) {
	const int width = 16;
	for (int i = 0; i < len; ++i) {
		printf("%02hhx%c", buf[i], ((i+1) % width == 0) ? '\n' : ' ');
	}

	if ((len % width) != 0) {
		putchar('\n');
	}
	putchar('\n');
}

void ip_print(char *buf){
    unsigned short iphdrlen;
    struct iphdr *iph = (struct iphdr *)buf;
    iphdrlen = iph->ihl * 4;

    struct tcphdr *tcph=(struct tcphdr*)(buf + iphdrlen);

    struct sockaddr_in source,dest;
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;

    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;
    printf("Source IP     : %s\n", inet_ntoa(source.sin_addr));
    printf("Destination IP: %s\n", inet_ntoa(dest.sin_addr));
}

int main(int argc, char *argv[]) {
	int raw_sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	exit_on_error(raw_sock);

	int read_cnt;
	while (0 < (read_cnt = read(raw_sock, in_buf, BUF_SZ))) {

        printf("\n\n\***********************TCP Packet*************************\n\n");
        unsigned short iphdrlen;
        struct iphdr *iph = (struct iphdr *)in_buf;
        iphdrlen = iph->ihl * 4;

        struct tcphdr *tcph = (struct tcphdr*)(in_buf + iphdrlen);

        ip_print(in_buf);
        printf("Data          :\n");
        int tcphlen = (tcph->doff) * 4;
        int iphlen = (iph->ihl) * 4;
        hex_print(in_buf + iphdrlen + tcphlen, read_cnt - tcphlen - iphlen);
	}
}
