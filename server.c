#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ctype.h>

#define BUF_SIZE 1024
const char *vLengthCode = "85";
const char *dVowelCode = "170";
const char *vowels ="aeiouAEIOU";
int count;


int main(int argc, char* argv[]) {
    unsigned char buf[BUF_SIZE];
    struct sockaddr_in self, other;
    int len = sizeof(struct sockaddr_in);
    int n, s, port;

    if (argc < 2) {
	fprintf(stderr, "Usage: %s <port>\n", argv[0]);
	return 1;
    }

    /* initialize socket */
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
	perror("socket");
	return 1;
    }

    /* bind to server port */
    port = atoi(argv[1]);
    memset((char *) &self, 0, sizeof(struct sockaddr_in));
    self.sin_family = AF_INET;
    self.sin_port = htons(port);
    self.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(s, (struct sockaddr *) &self, sizeof(self)) == -1) {
	perror("bind");
	return 1;
    }

    while ((n = recvfrom(s, buf, BUF_SIZE, 0, (struct sockaddr *) &other, &len)) != -1) 
    {
	printf("Received from %s:%d: ", 
		inet_ntoa(other.sin_addr), 
		ntohs(other.sin_port)); 
	fflush(stdout);
	write(1, buf, n);
	write(1, "\n", 1); 
  unsigned short messageLength = (buf[0] << 8) + buf[1];
  unsigned short requestNum = (buf[2] << 8) + buf[3];
  unsigned char operation = buf[4];
  unsigned char string[BUF_SIZE];
  int q = 0;
  int p;
  for (p = 5; buf[p] != '\0'; p++)
   {
     string[q] = buf[p];
     q++;
   }

      if (operation == 85)
       {   
          count = 0;
          int i = 0;
          n = 6;
          messageLength = 6;
          while(string[i] != '\0') 
          {
                if(string[i] == 'a' || string[i] == 'e'
                   || string[i] == 'i' || string[i] == 'o' || string[i] =='u'
                   || string [i] == 'A' || string[i] == 'E' || string[i] =='I'
                   || string [i] == 'O' || string[i] == 'U') 
                {
                count++;
                }
                i++;
          }   
          unsigned char countChar = (char)count;
          buf[5] = '0'+((unsigned char)count);
          buf[4] = '0'+(unsigned char)(count >> 8);
          buf[1] = '0'+((unsigned char)messageLength);
          buf[0] = '0'+(unsigned char)(messageLength >> 8);
          printf("%c\n", buf[0]);
          printf("%c\n", buf[1]);
          printf("%c\n", buf[4]);
          printf("%c\n", buf[5]);
       }
       
       else if (operation == 170)
       {    
           // printf("Hey");
           // fflush(stdout);
            int k = 0;
            char noVow[100];
            int j = 0;
            while(string[j] != '\0') 
            {
               if(!string[j] == 'a' || !string[j] == 'e'
                   || !string[j] == 'i' || !string[j] == 'o' || !string[j] =='u'
                   || !string [j] == 'A' || !string[j] == 'E' || !string[j] =='I'
                   || !string [j] == 'O' || !string[j] == 'U')  
                {
                noVow[k] = string[j];
                k++;
                }
                j++;
            }
            int r = 0;
            int t = 5;
            while (noVow[r] != '\0')
            {
              buf[t] = noVow[r];
              r++;
              t++;
            }
            
       }
  
	/* echo back to client */
	sendto(s, buf, n, 0, (struct sockaddr *) &other, len);
    }
    memset(buf, 0, sizeof buf);
    close(s);
    return 0;
}