/* 
 * File:   Communication.h
 * Author: kuba
 *
 * Created on 25. listopad 2013, 16:34
 */

#ifndef COMMUNICATION_H
#define	COMMUNICATION_H

using namespace std;

#define MSG_REQUEST_WORK 1000
#define MSG_WORK_REQUEST_DENIED 1001
#define MSG_WORK_REQUEST_ACCEPTED 1002
#define MSG_BEST_RESULT 2000
#define MSG_TERMINATE 3000
#define MSG_TERMINATE_TOKEN 3001

#define TERMINATE_TOKEN_WHITE 100 
#define TERMINATE_TOKEN_BLACK 200

int processId = 0;
int processNumber = 1;
int processColor = TERMINATE_TOKEN_WHITE;

double time() {
    return 0;
}

void barier(){ }

void sendRefuse(int dest){ }

void sendTerminate(){ }

int * getWork(){ return NULL; }

void sendWork(int dest){ }

void sendBest(){ }

void recieveBest(int * msg){ }

void sendTerminateToken(int token, int destination) { }

void recieveTerminateToken(int* msg, int source) { }

void checkForMsg(){ }



void initialize(int argc,char **argv){ }

void finalize(){ }



#endif	/* COMMUNICATION_H */

