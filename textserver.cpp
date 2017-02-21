#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <map>
#include "fifo.h"

using namespace std;

/* Fifo names */
string receive_fifo = "userStatus";
string send_fifo = "userAssignment";
/*
 * 
 */
int main() {
  bool user1Connected = false;
  bool user2Connected = false;
  Fifo recUser(receive_fifo);
  Fifo sendUser(send_fifo);
  string openMessage = "page opened";
  string user1Close = "User1: left";
  string user2Close = "User2: left";
 
  while (1){ //outer loop
	//finding users loop
    while ((user1Connected == false) || (user2Connected ==false)){
        recUser.openread();
        string user1Message = recUser.recv();
        
	if (user1Message == openMessage){ //user 1 connected
            sendUser.openwrite();
            sendUser.send("User1");
            cout << "User 1 assigned"<<endl;
            user1Connected = true;
            sendUser.fifoclose();
            recUser.fifoclose();
            
	    while (user2Connected == false){
                recUser.openread();
                string user2Message = recUser.recv();
                if (user2Message == openMessage){ //user 2 connected
                    sendUser.openwrite();
                    sendUser.send("User2");
                    cout << "User 2 assigned"<<endl;
                    user2Connected = true;
                    sendUser.fifoclose();
                    recUser.fifoclose();
                    //recUser.openread(); 
                }else{
                    user2Message ="";
                    recUser.fifoclose();
                }
            }
        } else {
            user1Message = "";
            recUser.fifoclose();
        }
        
    }
    cout <<"Both Users Assigned" << endl;
    char end = 'n';
    while (end != 'y'){
    	recUser.openread();
    	string userMsg = recUser.recv();
	if (userMsg == user1Close){
	  cout << "User 1 left" << endl;
	  sendUser.openwrite();
	  sendUser.send("User 1 left");
	  user1Connected = false;
	  end = 'y';
	}else if(userMsg == user2Close){
	  cout << "User 2 left" << endl;
	  user2Connected = false;
	  end = 'y';
	  recUser.fifoclose();
	  sendUser.openwrite();
	  sendUser.send("User 2 left");
	}
	if ((user1Connected == false) && (user2Connected == false)){
	//recUser.fifoclose();
	end = 'y';
	
	}
	//recUser.fifoclose();
    }
    recUser.fifoclose();
  }
    return 0;
}

