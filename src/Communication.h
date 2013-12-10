/* 
 * File:   Communication.h
 * Author: kuba
 *
 * Created on 25. listopad 2013, 16:34
 */

#ifndef COMMUNICATION_H
#define	COMMUNICATION_H

#include <mpi.h>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include "globals.h"

using namespace std;

#define MSG_REQUEST_WORK 1000
#define MSG_WORK_REQUEST_DENIED 1001
#define MSG_WORK_REQUEST_ACCEPTED 1002
#define MSG_BEST_RESULT 2000
#define MSG_TERMINATE 3000
#define MSG_PEON 4000

int processId;
int processNumber;
int color = 1;
int peon = 0;

double time() {
    return MPI_Wtime();
}

void barier(){
    MPI_Barrier(MPI_COMM_WORLD);
}

void sendRefuse(int dest){ // odmitnuti poslani prace, protoze proces sam praci nema
    cout<<"Process:"<<processId<<" msg to:"<<dest<<" tag:"<<MSG_WORK_REQUEST_DENIED<<endl;
    MPI_Send(NULL, 0, MPI_INT, dest, MSG_WORK_REQUEST_DENIED, MPI_COMM_WORLD);
    cout<<"Process:"<<processId<<" msg send"<<endl;
}

void sendTerminate(){
    for(int i=0;i<processNumber;i++){
        if(i==processId)continue;
        cout<<"Process:"<<processId<<" msg to:"<<(processId+1)%processNumber<<" tag:"<<MSG_TERMINATE<<endl;
        MPI_Send(NULL,0,MPI_INT, (processId+1)%processNumber, MSG_TERMINATE, MPI_COMM_WORLD);
        cout<<"Process:"<<processId<<" msg send"<<endl; 
    }
}

void sendPeon(){
    color = 1;
    MPI_Send(&peon,1,MPI_INT,(processId+1)%processNumber,MSG_PEON, MPI_COMM_WORLD);
    peon = 0;
}

void recievePeon(int p){
    if(color==-1)peon=-1;
    else peon = p;
    if(processId==0){
        if(peon==1){
            sendTerminate();
            finished = true;
            return;
        }
        peon=1;
    }
}

int * getWork(){
    MPI_Status status;
    int i=1;
    int * buf=new int[nodeCount+1];
    if(peon!=0){
        sendPeon();
    }
    cout<<"Process:"<<processId<<" msg to:"<<(processId+i)%processNumber<<" tag:"<<MSG_REQUEST_WORK<<endl;
    MPI_Send(NULL, 0, MPI_INT, (processId+i)%processNumber, MSG_REQUEST_WORK, MPI_COMM_WORLD);
    cout<<"Process:"<<processId<<" msg send"<<endl;
    while(i<processNumber){  // cekani jestli nekdo odpovi na zadost o praci
        MPI_Recv(buf, nodeCount+1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status );
        // TADY SE TO ZASEKAVA KVULI BLOKUJICIMU CEKANI NA ZPRAVU
        cout<<"Process:"<<processId<<" msg from:"<<status.MPI_SOURCE<<" tag:"<<status.MPI_TAG<<endl<<"Process:"<<processId<<" msg recieved"<<endl;
        switch(status.MPI_TAG){
            case MSG_REQUEST_WORK:{     // nekdo me zada o praci -> zadnou nemam -> odmitnuti poslani prace
                sendRefuse(status.MPI_SOURCE);
            }break;
            case MSG_WORK_REQUEST_DENIED:{      // odmitli mi dat praci
                i++;
                if(i==processNumber){   // obeslal jsem vsechny sousedy a nidko mi neposlal praci
                    cout<<"Process:"<<processId<<" : No one want to share work :-("<<endl;
                    break;
                }
                // zadost nasledujicimu procesoru o praci
                cout<<"Process:"<<processId<<" msg to:"<<(processId+i)%processNumber<<" tag:"<<MSG_REQUEST_WORK<<endl;
                MPI_Send(NULL, 0, MPI_INT, (processId+i)%processNumber, MSG_REQUEST_WORK, MPI_COMM_WORLD);
                cout<<"Process:"<<processId<<" msg send"<<endl;
            }break;
            case MSG_WORK_REQUEST_ACCEPTED:{    // prisla prace
                return buf;
            }break;
            case MSG_TERMINATE:{        // dostal jsem prikaz k ukonceni -> koncim a preposilam zprav sousedovi
                finished=true;
                return NULL;
            }break;
            case MSG_PEON:{
                recievePeon(buf[0]);
                sendPeon();
            }
            case MSG_BEST_RESULT:{      // prisel novy nejlepsi vysledek
                if(dilatation>buf[nodeCount]){
                    dilatation=buf[nodeCount];
                    if(minPermutation==NULL){
                        minPermutation= new int[nodeCount];
                    }
                    memcpy(minPermutation,buf,nodeCount*sizeof(int));
                    cout<<"Process:"<<processId<<" msg to:"<<(processId+1)%processNumber<<" tag:"<<MSG_BEST_RESULT<<endl;
                    MPI_Send(buf,nodeCount+1,MPI_INT,(processId+1)%processNumber,MSG_BEST_RESULT,MPI_COMM_WORLD);
                    cout<<"Process:"<<processId<<" msg send"<<endl;
                }
            }break;
                
        }
       
    }
    finished=true;
    sendTerminate();
    return NULL;
}

void sendWork(int dest){
    int * buf= permutation->wrap();
    if(buf==NULL){
        sendRefuse(dest);
        return;
    }
    cout<<"Process:"<<processId<<" msg to:"<<dest<<" tag:"<<MSG_WORK_REQUEST_ACCEPTED<<endl;
    MPI_Send(buf, buf[0]+3, MPI_INT, dest, MSG_WORK_REQUEST_ACCEPTED, MPI_COMM_WORLD);
        cout<<"Process:"<<processId<<" msg send"<<endl;
    delete[] buf;
}

void sendBest(){
    int * msg = new int[nodeCount+1];
    msg[nodeCount]=dilatation;
    memcpy(msg,minPermutation,nodeCount*sizeof(int));
    cout<<"Process:"<<processId<<" msg to:"<<(processId+1)%processNumber<<" tag:"<<MSG_BEST_RESULT<<endl;
    MPI_Send(msg, nodeCount+1, MPI_INT, (processId+1)%processNumber, MSG_BEST_RESULT, MPI_COMM_WORLD);
    cout<<"Process:"<<processId<<" msg send"<<endl;
    delete[] msg;
}

void checkForWorkRequest(){
    int flag;
    MPI_Status status;
    int * buf;
    while(!finished){
        MPI_Iprobe ( MPI_ANY_SOURCE, MSG_REQUEST_WORK, MPI_COMM_WORLD, &flag, &status );
        if(!flag)break;
        cout<<"Process:"<<processId<<" msg from:"<<status.MPI_SOURCE<<" tag:"<<status.MPI_TAG<<endl;
        MPI_Recv(buf,0,MPI_INT,MSG_REQUEST_WORK,status.MPI_TAG,MPI_COMM_WORLD,&status);
        cout<<"Process:"<<processId<<" msg recieved"<<endl;
        sendWork(status.MPI_SOURCE);
        if(status.MPI_SOURCE<processId)
            color=-1;
    }
}

void checkForBestMsg(){
    int flag;
    MPI_Status status;
    int * msg=new int[nodeCount+1];
    bool improved=false;
    while(!finished){
        MPI_Iprobe ( MPI_ANY_SOURCE, MSG_BEST_RESULT, MPI_COMM_WORLD, &flag, &status );
        if(!flag)break;
        cout<<"Process:"<<processId<<" msg from:"<<status.MPI_SOURCE<<" tag:"<<status.MPI_TAG<<endl;
        MPI_Recv(msg,nodeCount+1,MPI_INT,status.MPI_SOURCE,MSG_BEST_RESULT,MPI_COMM_WORLD,&status);
        cout<<"Process:"<<processId<<" msg recieved"<<endl;
        if(dilatation>msg[nodeCount]){
            dilatation=msg[nodeCount];
            if(minPermutation==NULL){
                minPermutation= new int[nodeCount];
            }
            memcpy(minPermutation,msg,nodeCount*sizeof(int));
            improved=true;
        }
    }
    if(improved){
        memcpy(msg,minPermutation,nodeCount*sizeof(int));
        msg[nodeCount]=dilatation;
            cout<<"Process:"<<processId<<" msg to:"<<(processId+1)%processNumber<<" tag:"<<MSG_BEST_RESULT<<endl;
            MPI_Send(msg,nodeCount+1,MPI_INT,(processId+1)%processNumber,MSG_BEST_RESULT,MPI_COMM_WORLD);
            cout<<"Process:"<<processId<<" msg send"<<endl;
    }
    delete[] msg;
}

void checkForMsg(){
    checkForBestMsg();
    checkForWorkRequest();
    int flag;
    MPI_Status status;
    int * buf=new int[nodeCount+1];     // buffer na prijatou zpravu
    while(!finished){   // pokud nebyla prijata zprava TERMINATE, cti zpravy
        MPI_Iprobe ( MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status );     // testovani jestli  neprisla nova zprava
        if(!flag){     // neprisla zadna zprava
            break;      // konec cteni zprav
        }
        cout<<"Process:"<<processId<<" msg from:"<<status.MPI_SOURCE<<" tag:"<<status.MPI_TAG<<endl;
        MPI_Recv(buf,nodeCount+1,MPI_INT,status.MPI_SOURCE,status.MPI_TAG,MPI_COMM_WORLD,&status);      // blokujici prijem zpravy
        cout<<"Process:"<<processId<<" msg recieved"<<endl;
        switch(status.MPI_TAG){
            case MSG_PEON:
                recievePeon(buf[0]);
            case MSG_TERMINATE:
                finished=true;
                return;
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



#endif	/* COMMUNICATION_H */

