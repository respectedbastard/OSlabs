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

void game(int listenfd, int cl1, int cl2)
{
        int number = 11;
	srand(time(0));
	int count = number + rand() % number;
	while (1)
	{
		int check;
		for (int i = count; i > 0; i--)
		{
			if (i % 2 == 0)
			{
				check = 0;
				check = htons(check);
				write(cl1, &check, sizeof(int));
				read(cl1, &check, sizeof(int));
				check = ntohs(check);
				if (check == 1)
				{
					number--;
					if (number <= 0)
					{
						check = 1;
						check = htons(check);
                                        	write(cl1, &check, sizeof(int));
                                        	check = 2;
                                        	check = htons(check);
                                        	write(cl2, &check, sizeof(int));
						break;
					}
				}
				else if (check == 2)
				{
					number = number - 2;
					if (number <= 0)
                                        {
                                                check = 1;
                                                check = htons(check);
                                                write(cl1, &check, sizeof(int));
                                                check = 2;
                                                check = htons(check);
                                                write(cl2, &check, sizeof(int));
                                                break;
                                        }
				}
				else 
				{
					number = number - 3;
					if (number <= 0)
                                        {
                                                check = 1;
                                                check = htons(check);
                                                write(cl1, &check, sizeof(int));
                                                check = 2;
                                                check = htons(check);
                                                write(cl2, &check, sizeof(int));
                                                break;
                                        }
				}
			}
			else 
			{
				check = 0;
                                check = htons(check);
                                write(cl2, &check, sizeof(int));
                                read(cl2, &check, sizeof(int));
                                check = ntohs(check);
                                if (check == 1)
                                {
                                        number--;
                                        if (number <= 0)
                                        {
                                                check = 1;
                                                check = htons(check);
                                                write(cl2, &check, sizeof(int));
                                                check = 2;
                                                check = htons(check);
                                                write(cl1, &check, sizeof(int));
                                                break;
                                        }
                                }
                                else if (check == 2)
                                {
                                        number = number - 2;
                                        if (number <= 0)
                                        {
                                                check = 1;
                                                check = htons(check);
                                                write(cl2, &check, sizeof(int));
                                                check = 2;
                                                check = htons(check);
                                                write(cl1, &check, sizeof(int));
                                                break;
                                        }
                                }
				else
                                {
                                        number = number - 3;
                                        if (number <= 0)
                                        {
                                                check = 1;
                                                check = htons(check);
                                                write(cl2, &check, sizeof(int));
                                                check = 2;
                                                check = htons(check);
                                                write(cl1, &check, sizeof(int));
                                                break;
                                        }
                                }
			}
		}
	}
}

int main(int argc, char *argv[])
{
	int listenfd = 0;
	struct sockaddr_in serveraddr;
	char buf[2049];
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serveraddr, '0', sizeof(serveraddr));
	memset(buf, '0', sizeof(buf));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(atoi(argv[1]));
	bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	listen(listenfd, 10);
	int cl1 = accept(listenfd, (struct sockaddr *)NULL, NULL);
	int cl2 = accept(listenfd, (struct sockaddr *)NULL, NULL); 
	game(listenfd, cl1, cl2);
}