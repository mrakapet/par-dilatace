

#ifndef COMM_MODUL
#define COMM_MODUL

#include <mpi.h>
#include "globals.cpp"
#include "WrappedPermutation.h"

using namespace std;

#define MSG_REQUEST_WORK 1000
#define MSG_WORK_REQUEST_DENIED 1001
#define MSG_WORK_REQUEST_ACCEPTED 1002
#define MSG_BEST_RESULT 2000
#define MSG_TERMINATE 3000

int processId;
int processNumber;

void sendRefuse(int dest){
    MPI_Send(NULL, 0, MPI_INT, dest, MSG_WORK_REQUEST_DENIED, MPI_COMM_WORLD);
}

void sendTerminate(){
    MPI_Send(NULL,0,MPI_INT, (processId+1)%processNumber, MSG_TERMINATE, MPI_COMM_WORLD);
}

WrappedPermutation * getWork(){
    MPI_Status status;
    int i=1;
    int * buf=new int[nodeCount+1];
    MPI_Send(NULL, 0, MPI_INT, (processId+i)%processNumber, MSG_REQUEST_WORK, MPI_COMM_WORLD);
    while(i<processNumber){
        MPI_Recv(buf, nodeCount+1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status );
        switch(status.MPI_TAG){
            case MSG_REQUEST_WORK:{
                sendRefuse(status.MPI_SOURCE);
            }break;
            case MSG_WORK_REQUEST_DENIED:{
                MPI_Send(NULL, 0, MPI_INT, (processId+i)%processNumber, MSG_REQUEST_WORK, MPI_COMM_WORLD);
            }break;
            case MSG_WORK_REQUEST_ACCEPTED:{
                int* perm;
                int size, end;
                int position=0;
                MPI_Unpack(buf,nodeCount+1,&position,&size,1,MPI_INT,MPI_COMM_WORLD);
                MPI_Unpack(buf,nodeCount+1,&position,&end,1,MPI_INT,MPI_COMM_WORLD);
                MPI_Unpack(buf,nodeCount+1,&position,&perm,size,MPI_INT,MPI_COMM_WORLD);
                return new WrappedPermutation(perm,size,end);
            }break;
            case MSG_TERMINATE:{
                finished=true;
                sendTerminate();
                return NULL;
            }break;
            case MSG_BEST_RESULT:{
                if(dilatation>buf[nodeCount]){
                    dilatation=buf[nodeCount];
                    if(minPermutation==NULL){
                        minPermutation= new int[nodeCount];
                    }
                    memcpy(minPermutation,buf,nodeCount*sizeof(int));
                    MPI_Send(buf,nodeCount+1,MPI_INT,(processId+1)%processNumber,MSG_BEST_RESULT,MPI_COMM_WORLD);
                }
            }break;
                
        }
    }
    finished=true;
    sendTerminate();
    return NULL;
}

void sendWork(int dest){
    WrappedPermutation *msg = permutation->wrap();
    if(msg==NULL){
        sendRefuse(dest);
        return;
    }
    int* buf = new int[msg->endLevel+2];
    int pos=0;
    MPI_Pack(&msg->endLevel,1,MPI_INT,buf,msg->endLevel+2,&pos,MPI_COMM_WORLD);
    MPI_Pack(&msg->endVal,1,MPI_INT,buf,msg->endLevel+2,&pos,MPI_COMM_WORLD);
    MPI_Pack(&msg->start,msg->endLevel,MPI_INT,buf,msg->endLevel+2,&pos,MPI_COMM_WORLD);
    MPI_Send(buf, msg->endLevel+2, MPI_INT, dest, MSG_WORK_REQUEST_ACCEPTED, MPI_COMM_WORLD);
}

void sendBest(){
    int * msg = new int[nodeCount+1];
    msg[nodeCount]=dilatation;
    memcpy(msg,minPermutation,nodeCount*sizeof(int));
    MPI_Send(msg, nodeCount+1, MPI_INT, (processId+1)%processNumber, MSG_BEST_RESULT, MPI_COMM_WORLD);
}

void recieveBest(){
    int * msg = new int[nodeCount+1];
    MPI_Status status;
    MPI_Recv(msg, nodeCount+1, MPI_INT, (processId+processNumber+1)%processNumber,MSG_BEST_RESULT, MPI_COMM_WORLD, &status);
    if(dilatation>msg[nodeCount]){
        dilatation=msg[nodeCount];
        if(minPermutation==NULL){
            minPermutation= new int[nodeCount];
        }
        memcpy(minPermutation,msg,nodeCount*sizeof(int));
        MPI_Send(msg,nodeCount+1,MPI_INT,(processId+1)%processNumber,MSG_BEST_RESULT,MPI_COMM_WORLD);
    }
    if(dilatation <= lowerLimit){
        sendTerminate();
        finished=true;
    }
}
void checkForMsg(){
    int flag;
    MPI_Status status;
    while(!finished){
        MPI_Iprobe ( MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status );
        if(!flag)break;
        int * buf;
        switch(status.MPI_TAG){
            case MSG_BEST_RESULT:
                recieveBest();
                break;
            case MSG_REQUEST_WORK:
                MPI_Recv(buf,0,MPI_INT,status.MPI_SOURCE,MSG_REQUEST_WORK,MPI_COMM_WORLD,&status);
                sendWork(status.MPI_SOURCE);
                break;
            case MSG_TERMINATE:
                sendTerminate();
                finished=true;
                return;
            default:
                MPI_Recv(buf,0,MPI_INT,status.MPI_SOURCE,status.MPI_TAG,MPI_COMM_WORLD, &status);
        }
    }
    
    
}



void initialize(int argc,char **argv){
    MPI_Init(&argc, &argv );
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);
    MPI_Comm_size(MPI_COMM_WORLD, &processNumber);
}

void finalize(){
    MPI_Finalize();
}

#endif
