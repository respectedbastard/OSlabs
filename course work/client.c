#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

void game(int sockfd)
{
	while (1)
        {	
		int check;
                read(sockfd, &check, sizeof(int)); 
                check = ntohs(check);
                if (check ==  0)
		{
			printf("Enter number (1 - one match 2 - two matches 3 - three matches: \n");
                        do 
			{
			scanf("%d", &check);
			} while(check != 1 && check != 2 && check != 3);	
                        check = htons(check);
                        write(sockfd, &check, sizeof(int)); 
		}
		if (check == 1)
		{
                        printf("You win\n");
                        break;
                }
                if (check == 2)
		{
                        printf("The opponent won\n");
			break;
                }
        }
}

int main(int argc, char *argv[])
{
	int sockfd = 0, n = 0;
	char buf[2049];
	struct sockaddr_in servaddr;
	memset(buf, '0', sizeof(buf));
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n socket creation failed \n");
		return 1;
	}
	memset(&servaddr, '0', sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr); 
	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		printf("\nConnect failed.\n");
		return 1;
	}
	printf("Connection sucsess\n");
	printf("Wait ...\n");
	game(sockfd);
}