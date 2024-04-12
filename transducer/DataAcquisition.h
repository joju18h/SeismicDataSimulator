#ifndef _DATAACQUISITIONUNIT_H_
#define _DATAACQUISITIONUNIT_H_

//dependencies
#include <errno.h>
#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include "SeismicData.h"


// interuppt handler for shutdown (Ctrl-C)
static void interruptHandler(int signum);

//constants
#define IP_ADDRESS "127.0.0.1"
#define PORT 1153
#define PASSWORD "Leaf"
#define MAX_CLI 6

//data structures

//info for a data center
struct DataCenter{
    char Username[32];
    struct sockaddr_in addr;
};

//packet structure
struct DataPaacket{
    uint16_t packet_number; // First two bytes: packet number
    uint8_t data_length;    // Third byte: length of the data
    uint8_t data[BUF_LEN]; // Remaining bytes: the data
};


//data acquisition unit class
class DataAcquisitionUnit
{
    bool is_running;
    //shared with transducer
    sem_t *sem_id1;
    key_t  ShmKey;
    int    ShmID;
    struct SeismicMemory *ShmPTR;
    struct sigaction action;

    //data acquisition unit specific
    int datacenterno;
    int blockListno;

    //subscribed data centers
    struct DataCenter dataCenters[MAX_CLI];

    //list of blocked data centers(IP addresses)
    std::string blockList[MAX_CLI];

    //history of data centers to prevent brute force attacks as well DDoS attacks
    std::string history[3];

public:
    DataAcquisitionUnit();
    void* readFunc(void*);
    void* writeFunc(void*);
    int run();
    void shutdown();

    static DataAcquisitionUnit* instance;
};

#endif// _DATAACQUISITIONUNIT_H_