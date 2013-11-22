

#ifndef COMM_MODUL
#define COMM_MODUL

#include "mpi.h"
#include "main.cpp"
#include "WrappedPermutation.h"
#define MSG_REQUEST_WORK 1000
#define MSG_WORK_REQUEST_DENIED 1001
#define MSG_WORK_REQUEST_ACCEPTED 1002
#define MSG_BEST_RESULT 2000
#define MSG_TERMINATE 3000

int processId;
int processNumber;


WrappedPermutation getWork(){
    MPI_Status status;
    int * buf=new int[nodeCount+1];
    int dest=(processId+1)%processNumber;
    MPI_Send(NULL, 0, MPI_INT, dest, MSG_REQUEST_WORK, MPI_COMM_WORLD);
    while(true){
        MPI_Recv(buf, nodeCount+1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status );
        switch(status.MPI_TAG){
            case MSG_REQUEST_WORK:
                sendRefuse(status.MPI_SOURCE);
                break;
            case MSG_WORK_REQUEST_DENIED:
                dest=(dest+1)%processNumber;
                MPI_Send(NULL, 0, MPI_INT, dest, MSG_REQUEST_WORK, MPI_COMM_WORLD);
                break;
            case MSG_WORK_REQUEST_ACCEPTED:
                int* perm;
                int size, end, pos=0;
                MPI_Unpack(buf,nodeCount+1,&pos,&size,1,MPI_INT,MPI_COMM_WORLD);
                MPI_Unpack(buf,nodeCount+1,&pos,&end,1,MPI_INT,MPI_COMM_WORLD);
                MPI_Unpack(buf,nodeCount+1,&pos,&perm,size,MPI_INT,MPI_COMM_WORLD);
                return new WrappedPermutation(perm,size,end);
                break;
            case MSG_TERMINATE :
                return NULL;
            case MSG_BEST_RESULT :
                if(dilatation>buf[nodeCount]){
                    dilatation=buf[nodeCount];
                    if(minPermutation==NULL){
                        minPermutation= new int[nodeCount];
                    }
                    memcpy(minPermutation,buf,nodeCount*sizeof(int));
                    MPI_Send(buf,nodeCount+1,MPI_INT,(processId+1)%processNumber,MSG_BEST_RESULT,MPI_COMM_WORLD);
                }
                break;
                
        }
    }
}
void sendWork(WrappedPermutation msg, int dest){
    int* buf = new int[msg.endLevel+2];
    int pos=0;
    MPI_Pack(&msg.endLevel,1,MPI_INT,buf,msg.endLevel+2,&pos,MPI_COMM_WORLD);
    MPI_Pack(&msg.endVal,1,MPI_INT,buf,msg.endLevel+2,&pos,MPI_COMM_WORLD);
    MPI_Pack(&msg.start,msg.endLevel,MPI_INT,buf,msg.endLevel+2,&pos,MPI_COMM_WORLD);
    MPI_Send(buf, msg.endLevel+2, MPI_INT, dest, MSG_WORK_REQUEST_ACCEPTED, MPI_COMM_WORLD);
}

void sendRefuse(int dest){
    MPI_Send(NULL, 0, MPI_INT, dest, MSG_WORK_REQUEST_DENIED, MPI_COMM_WORLD);
}

void sendBest(int best, int* perm){
    int * msg = new int[nodeCount+1];
    msg[nodeCount]=best;
    memcpy(msg,best,nodeCount*sizeof(int));
    MPI_Send(msg, nodeCount+1, MPI_INT, (processId+1)%processNumber, MSG_BEST_RESULT, MPI_COMM_WORLD);
}

int recieveBest(int * best, int * perm){
    int * msg = new int[nodeCount+1];
    MPI_Recv(msg, nodeCount+1, MPI_INT, (processId+processNumber+1)%processNumber,MSG_BEST_RESULT, MPI_COMM_WORLD);
    memcpy(best, msg, nodeCount*sizeof(int));
    *perm=msg[nodeCount];
}

void initialize(int argc,int **argv){
    MPI_Init(&argc, &argv );
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);
    MPI_Comm_size(MPI_COMM_WORLD, &processNumber);
}

void finalize(){
    MPI_Finalize();
}

#endif
