#include <iostream>
#include "DataAcquisition.h"

using namespace std;

pthread_t tid_read;
pthread_t tid_write;

pthread_mutex_t lock;

DataAcquisitionUnit* DataAcquisitionUnit::instance=nullptr;

DataAcquisitionUnit::DataAcquisitionUnit(){
    is_running=false;
    ShmPTR=nullptr;
    sem_id1=nullptr;
    instance=this;
    datacenterno=0;
    blockListno=0;

}

void DataAcquisitionUnit::shutdown(){
    cout<<"DataAcquisitionUnit::shutdown:"<<endl;
    is_running=false;
}

static void interruptHandler(int signum)
{
    switch(signum) {
        case SIGINT:
            DataAcquisitionUnit::instance->shutdown();
            break;
    }
}

int DataAcquisitionUnit::run(){
    //register signal handler
    action.sa_handler = interruptHandler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(SIGINT, &action, NULL);

    //create shared memory
    ShmKey = ftok(MEMNAME, 65);
    ShmID = shmget(ShmKey, sizeof(struct SeismicMemory), IPC_CREAT | 0666);

    if(ShmID < 0){
        perror("shmget");
        return 1;
    }

    //attach shared memory
    ShmPTR = (struct SeismicMemory*)shmat(ShmID, NULL, 0);
    if((ShmPTR) == (void*)-1){
        perror("shmat");
        return 1;
    }

    //setting up semaphores
    sem_id1 = sem_open(SEMNAME, O_CREAT, 0666, 0);
    if(sem_id1 == SEM_FAILED){
        perror("sem_open");
        return 1;
    }

    //socket setup
    int sockfd;
    struct sockaddr_in servaddr;
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1){
        perror("socket creation failed");
        return 1;
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    servaddr.sin_port = htons(PORT);

    //bind the socket
    if(bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        perror("bind failed");
        return 1;
    }

    //NON-BLOCKING
    if((fcntl(sockfd, F_SETFL, O_NONBLOCK)) < 0){
        perror("fcntl");
        return 1;
    }

}