#include <iostream>
#include <fstream>
#include "fifo.h"

using namespace std;

string receive_fifo = "toUser1";
string send_fifo = "fromUser1";
string rec2_fifo = "toUser2";
string send2_fifo = "fromUser2";

int main(){

	Fifo recfifo(receive_fifo);
	Fifo sendfifo(send_fifo);
	Fifo recfifo2(rec2_fifo);
	Fifo sendfifo2(send2_fifo);
	string connect, recMsg, message, update, disconnect;
	string userNumber;

	connect = "$CONNECT*";
	message = "$MESSAGE%";

	cout << "Send: "<< connect << endl;
	sendfifo.openwrite();
	sendfifo.send(connect);

	recfifo.openread();
	recMsg = recfifo.recv();
	cout << "Server sent: "<<recMsg<< endl;
	recfifo.fifoclose();
	sendfifo.fifoclose();

	if (recMsg == "$USER%USER1*"){
		userNumber = "1";
	}
	if (recMsg == "$USER%USER2*"){
		userNumber = "2";
	}

	cout << "Assigned to: User " << userNumber << endl;
	if (userNumber == "1"){
		update = "$UPDATE%USER"+userNumber+"*";
		char choice;
		cout << "update or send message (u/s)" << endl;
		cin >> choice;
		string chat;
		if (choice == 's'){
			while(1){
				sendfifo.openwrite();
				cout << "type message: "<< endl;
				//cin >> chat;
				getline(cin, chat);
				chat = message +"USER"+userNumber+"%"+chat+"*";
				sendfifo.send(chat);
				sendfifo.send(update);
				string chatRec;

				recfifo.openread();
		
				while (chatRec != "$UPTODATE*"){
					chatRec = recfifo.recv();
					if(chatRec != "$UPTODATE*"){
						//chatRec.erase(0,9);
						cout << "chat received: " << chatRec << endl;
					} 
				}
				recfifo.fifoclose();
				sendfifo.fifoclose();
			}
		}
		if (choice == 'u'){
			while(1){
				sendfifo.openwrite();
				//cout << "UPDATE"<< endl;
				sendfifo.send(update);
				//cout << "SENT" << endl;
				recfifo.openread();
				string chatRec;
				while (chatRec != "$UPTODATE*"){
					chatRec = recfifo.recv();
					//cout <<"updating..."<<endl;
					if (chatRec != "$UPTODATE*"){
						//chatRec.erase(0,9);
						cout << "chat received: " << chatRec << endl;
					}
					//if (chatRec == "$UPTODATE*"){
					//	chatRec = "$/MESSAGE*";
					//}
				}
				recfifo.fifoclose();
				sendfifo.fifoclose();
				//cout << "update? (y/n)" << endl;
				//cin >> x;
			}
		}
		sendfifo.fifoclose();
		disconnect = "$UNLOAD%USER"+userNumber+"*";
		char quit;
		cout << "Quit?"<< endl;
		cin >> quit;
		sendfifo.openwrite();
		sendfifo.send(disconnect);
		sendfifo.fifoclose();
	}

if (userNumber == "2"){
	update = "$UPDATE%USER"+userNumber+"*";
		char choice;
		cout << "update or send message (u/s)" << endl;
		cin >> choice;
		string chat;
		if (choice == 's'){
			while(1){
				sendfifo2.openwrite();
				cout << "type message: "<< endl;
				//cin >> chat;
				getline(cin, chat);
				chat = message +"USER"+userNumber+"%"+chat;
				sendfifo2.send(chat);
				sendfifo2.send(update);
				string chatRec;

				recfifo2.openread();
		
				while (chatRec != "$UPTODATE*"){
					chatRec = recfifo2.recv();
					if(chatRec != "$UPTODATE*"){
						//chatRec.erase(0,9);
						cout << "chat received: " << chatRec << endl;
					} 
				}
				recfifo2.fifoclose();
				sendfifo2.fifoclose();
			}
		}
		disconnect = "$UNLOAD%USER"+userNumber+"*";
		if (choice == 'u'){
			int var = 0;
			while(var < 10){
				sendfifo2.openwrite();
				//cout << "UPDATE"<< endl;
				sendfifo2.send(update);
				//cout << "SENT" << endl;
				recfifo2.openread();
				string chatRec;
				while (chatRec != "$UPTODATE*"){
					chatRec = recfifo2.recv();
					if (chatRec != "$UPTODATE*"){
						//chatRec.erase(0,9);
						cout << "chat received: " << chatRec << endl;
					}
					//if (chatRec == "$UPTODATE*"){
					//	chatRec = "$/MESSAGE*";
					//}
				}
				recfifo2.fifoclose();
				sendfifo2.fifoclose();
				//cout << "update? (y/n)" << endl;
				//cin >> x;
				var++;
			}
			sendfifo2.openwrite();
			sendfifo2.send(disconnect);
			sendfifo2.fifoclose();

		}
		//sendfifo2.fifoclose();
		//disconnect = "$UNLOAD%USER"+userNumber+"*";
		char quit;

		//cout << "Quit?"<< endl;
		//cin >> quit;
		//sendfifo2.openwrite();
		//sendfifo2.send(disconnect);
		//cout<<"quit?"<<endl;
		//cin >> quit;
		//sendfifo2.fifoclose();
		//cout<<"quit?"<<endl;
		//cin >> quit;
	}
}
