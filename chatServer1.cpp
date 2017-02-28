#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "fifo.h"

using namespace std;

/* Fifo names */
string receive_fifo = "fromUser1";
string send_fifo = "toUser1";
string rec2_fifo = "fromUser2";
string send2_fifo = "toUser2";

void assignUser(Fifo rec, Fifo send, bool& user1Con, bool& user2Con, string assign1, string assign2){
	send.openwrite();
	if(user1Con == false){
		send.send(assign1);
		send.fifoclose();
		cout <<"User 1 assigned" << endl;
		user1Con = true;
	}else{
		send.send(assign2);
		send.fifoclose();
		cout <<"User 2 assigned" << endl;
		user2Con = true;
	}

		 
}

void processMessages(Fifo rec, Fifo send, string update, string message, string close, 
					 bool& connect, vector<string>& log, vector<string>& usr1Log, vector<string>& usr2Log, char& end){

	string upToDate = "$UPTODATE*";
	rec.openread();
	rec.openread();
	string userMsg = rec.recv();
	cout << userMsg << endl;
	if(userMsg == close){ //if user disconnects
		cout << "User Sent:" << userMsg << endl;
		connect = false;
		//clears message logs
		log.clear();
		usr1Log.clear();
		usr2Log.clear();
		end = 'y';
	}

	//if user sends a message//
	if(userMsg.find(message) != string::npos){
		log.push_back(userMsg); //adds message to message log
	}

	//if user sends an update request//
	if(userMsg == update){
		cout << "Updating User" << endl;
		if (log.size() > usr1Log.size()){ //checks if new messages have arrived
			int dif = (log.size() - usr1Log.size());
			int index = (log.size() -1);
			dif--;
			for (dif; dif >= 0; dif--){ //sends new messages
				string x = log[index - dif];
				cout << "Text sent:"<< x << endl;
				send.openwrite();
				send.send(x);
				usr1Log.push_back(x);
			}
			send.send(upToDate); //message indicates no more new messages
		}else{
			send.openwrite();
			send.send(upToDate); //sends if no new messages
		}
	}
}


int main() {

	bool user1Connect = false; // whether user 1 is connected or not
	bool user2Connect = false; // whether user 2 is connected or not
	Fifo recUser(receive_fifo);
	Fifo sendUser(send_fifo);
	Fifo recUser2(rec2_fifo);
	Fifo sendUser2(send2_fifo);

	string openMessage = "$CONNECT*"; //message sent when connection starts
	string user1Assign = "$USER%USER1*"; //assigns connector to user 1
	string user2Assign = "$USER%USER2*"; //assigns connector to user 2
	string user1Close = "$UNLOAD%USER1*"; //message received when user1 leaves
	string user2Close = "$UNLOAD%USER2*"; //message received when user2 leaves
	string user1Update = "$UPDATE%USER1*"; //request for user1 message update
	string user2Update = "$UPDATE%USER2*"; //request for user2 message update
	string user1Message = "$MESSAGE%USER1%"; //flag to indicate a message from user 1 for text chat
	string user2Message = "$MESSAGE%USER2%"; //flag to induicate a message from user 2 for text chat
	//string endMsg = "*"; //indicates end of messages
	string upToDate = "$UPTODATE*"; //sent when there is nothing to update
	string userMissingMsg = "$WAIT*";  //change to $WAIT* ////////////////////////////// 
		//message sent to update user if other user left

	string userMsg; //Message received from user
	vector<string> msgLog; //log of all messages for current users
	vector<string> sentMsg1; //messages sent to user 1
	vector<string> sentMsg2; //messages sent to user 2

	recUser.openread(); //opens receiving fifo


	while (1){ //keeps server running
		
		//Find Users//
		cout << "finding users" << endl;
		while ((user1Connect == false) || (user2Connect == false)){
			recUser.openread();
			userMsg = recUser.recv();

			//assigns users//
			if (userMsg == openMessage){
				assignUser(recUser, sendUser, user1Connect, user2Connect, user1Assign, user2Assign );
			}

			//updates user if no other user is found//
			if (userMsg == user1Update || userMsg == user2Update){
				sendUser.openwrite();
				sendUser.send(userMissingMsg);
				sendUser.fifoclose();
			}
			
			//resets variables if user disconnects during 'find users'
			if (userMsg == user1Close){
				user1Connect = false;
			}
			if (userMsg == user2Close){
				user2Connect = false;
			}
			recUser.fifoclose();
		}

		sendUser.fifoclose();
		recUser.fifoclose();

		//after both users connect
		cout <<"Both users connected" << endl;
		char end = 'n'; //used to return to 'find users' loop
		while (end != 'y'){
			cout << "USER1_TURN"<< endl;
			processMessages(recUser, sendUser, user1Update, user1Message, user1Close, user1Connect, msgLog, sentMsg1, sentMsg2, end);

			if(end == 'y'){
				break;
			}
			cout << "USER2_TURN"<< endl;
			processMessages(recUser2, sendUser2, user2Update, user2Message, user2Close, user2Connect, msgLog, sentMsg2, sentMsg1, end);
			cout << "end =" << end << endl;
			// recUser2.openread();
			// recUser2.openread();
			// userMsg = recUser2.recv();
			// //looks for 'disconnect' messages//
			// if(userMsg == user2Close){ //checks if user 2 leaves
			// 	cout << "user 2 left" << endl;
			// 	user2Connect = false;
			// 	msgLog.clear();
			// 	sentMsg1.clear();
			// 	sentMsg2.clear();
			// 	end = 'y';
			// 	break;
			// }

			// //Receives messages//
			// if(userMsg.find(user2Message) != string::npos){
			// 	msgLog.push_back(userMsg);
			// }
			// if(userMsg == user2Update){
			// 	cout << "Updating user 2" << endl;
			// 	sendUser2.openwrite();
			// 	if (msgLog.size() > sentMsg2.size()){
			// 		int dif = (msgLog.size() - sentMsg2.size());
			// 		int index = (msgLog.size() - 1);
			// 		dif--;
			// 		for (dif; dif >= 0; dif--){
			// 			string x = msgLog[index - dif];
			// 			cout <<"Text sent:"<< x << endl;
			// 			sendUser2.send(x);
			// 			sentMsg2.push_back(x);
			// 		}
			// 		sendUser2.send(upToDate);
			// 	}else{
			// 		sendUser2.send(upToDate);
			// 	}

		}
		sendUser.fifoclose();
		recUser.fifoclose();
		sendUser2.fifoclose();
		recUser2.fifoclose();
	}
} //main
