#include <iostream>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

void sem(int semId, int n, int m)
{
    struct sembuf op;
    op.sem_op = m;
    op.sem_flg = 0;
    op.sem_num = n;
    semop(semId, &op, 1);
}

int main(int argv, char *argc[])
{
    int n = atoi(argc[1]);
    int minn = atoi(argc[2]);
    int maxn = atoi(argc[3]);

    int memId = shmget(IPC_PRIVATE, sizeof(int) * n, 0600|IPC_CREAT|IPC_EXCL);
    int semId = semget(IPC_PRIVATE, n, 0600 | IPC_CREAT);
    int* arr = (int*)shmat(memId, 0, 0);

    int r = maxn - minn + 1;
    srand(time(NULL));
    for (int i = 0; i < n; i++)
    {
        arr[i] = minn + rand() % r;
    }
    
    cout << "Source: ";
    for (int i = 0; i < n; ++i)
        {
            cout << arr[i] << " ";
        }
    cout << endl;
    for (int i = 0; i < n; ++i)
    {
        sem(semId, i, 1);
    }

    int ch = fork();
    if (ch == 0)
    {
        int *arr = (int*)shmat(memId, 0, 0);

    for (int i = 0; i < n; ++i)
    {
        int mInd = i;
        for (int j = i + 1; j < n; ++j)
        {
            sem(semId, i, -1);
            sem(semId, j, -1);
            if (arr[j] < arr[mInd])
            {
                mInd = j;
                usleep(400000);
            }
            sem(semId, i, 1);
            sem(semId, j, 1);
        }
        if (i != mInd)
        {
            sem(semId, i, -1);
            sem(semId,mInd, -1);
            int temp = arr[i];
            arr[i] = arr[mInd];
            arr[mInd] = temp;
            usleep(400000);
            sem(semId, i, 1);
            sem(semId, mInd, 1);
        }
    }
    }
    else
    {
        int i = 0; 
        int status; 
        do
        {
            cout << "Iter: "<< i << endl;
            for (int j = 0; j < n; ++j)
            {   
                sem(semId, j, -1);
                {
                    cout << arr[j] << " ";
                }
                sem(semId, j, 1);
                usleep(40000);
            }
            cout << endl;
            status = waitpid(ch, NULL, WNOHANG);
            i++;
            usleep(100000);
        } while(!status);

        cout << "Result: ";
        for (int i = 0; i < n; ++i)
        {
            cout << arr[i] << " ";
        }
        cout << endl;

        shmctl(memId, 0, IPC_RMID);
        semctl(semId, 0, IPC_RMID);
    }
}