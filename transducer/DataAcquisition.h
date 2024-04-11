#ifndef _DATAACQUISITIONUNIT_H_
#define _DATAACQUISITIONUNIT_H_

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
#include <unistd.h>
#include "SeismicData.h"


static void interruptHandler(int signum);

class DataAcquisitionUnit
{
    bool is_running;
    sem_t *sem_id1;
    key_t  ShmKey;
    int    ShmID;
    struct SeismicMemory *ShmPTR;
    struct sigaction action;

    public:
    DataAcquisitionUnit();
    int readData();
    int writeData();

};

#endif// _DATAACQUISITIONUNIT_H_