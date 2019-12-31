#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstdlib>
#include <csignal>
#include "HelpersServer.hpp"

using namespace std;

static List * clientList;
static int sockFd, connFd;
static void signalHandler(int signum)
{ //this function handles the signals SIGQUIT and SIGINT
  cout << "\nInterrupt signal (" << signum << ") received.\n";
  // cleanup and close up stuff here
  if(clientList != NULL)
    delete clientList;
  shutdown(sockFd, SHUT_RDWR);
  close(connFd);
  exit(signum);
}

int main(int argc, char ** argv)
{
  signal(SIGINT, signalHandler);
  signal(SIGQUIT, signalHandler);
  if(argc != 3)
  {
    cerr << "ERROR:: Invalid arguments!\n";
    return 1;
  }
  //string number = argv[2];
  if(strcmp(argv[1], "-p") != 0)
  {
    cerr << "ERROR:: Invalid arguments!\n";
    return 1;
  }
  if(isNumber(argv[2]))
    cout << "Initial arguments where correct!\n";
  else
  {
    cerr << "ERROR:: Invalid arguments!\n";
    return 1;
  }
  int nread, nwrite; 
  int portNum = stoi(argv[2], nullptr, 10);
  cout << "Server listening to port : " << portNum << endl;
  struct sockaddr_in server, client;
  socklen_t clientlen = sizeof(struct sockaddr);//init
  struct sockaddr * serverPtr = (struct sockaddr *) & server;
  struct sockaddr * clientPtr = (struct sockaddr *) & client;
  struct hostent * rem;
  if((sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    exit(errno);
  }
  server.sin_family = AF_INET ;//Internet domain
  server.sin_addr.s_addr = htonl(INADDR_ANY);//accept any IP address
  server.sin_port = htons(portNum);//The given port
  int enable = 1;
  if (setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    perror("setsockopt(SO_REUSEADDR) failed");
  if(bind(sockFd, serverPtr, sizeof(server)) < 0)//Bind socket to address
  {
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    exit(errno);
  }
  listen(sockFd, 100);
  clientList = new List();
  while(1)
  {
    char msgBuffer[256+1] = "";//init
    cout << "Server waiting for connections...\n";
    if((connFd = accept(sockFd, clientPtr, &clientlen)) < 0)
    {
      cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
      delete clientList;
      exit(errno);
    }
    else
      cout << "Server accepted connection...\n";
    /* Find client ’s address */
    if((rem = gethostbyaddr(( char *) &client.sin_addr.s_addr, sizeof(client.sin_addr.s_addr), client.sin_family)) == NULL)
    {
      cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
      delete clientList;
      exit(errno);
    }
    printf (" Accepted connection from %s\n" ,rem->h_name);
    const char * IPclient = inet_ntoa(*((struct in_addr*)rem->h_addr_list[0]));
    cout << "translated to : " << IPclient << endl;
    nread = read(connFd, msgBuffer, 256);
    if(nread > 0)//LOG ON msg received successfully
    {
      string msgRcvd = msgBuffer;
      if(msgRcvd.length() > 12)
      {
        string msgHeader1 = msgRcvd.substr(0, 7);//log_on
        string msgHeader2 = msgRcvd.substr(0, 8);//log_off
        string msgHeader3 = msgRcvd.substr(0, 12);//get_clients
        if(msgHeader1.compare("LOG_ON ") == 0)
        {
          string msgToSend = "USER_ON";
          msgToSend += " ";
          cout << "header received:" << msgHeader1 << endl;
          msgRcvd = msgRcvd.substr(7);
          msgToSend += msgRcvd;
          const char * msgToSendC = msgToSend.c_str();
          //cout << "remaining message :" << msgRcvd << endl;
          int pos = msgRcvd.find_first_of(" ");
          string clientIP = msgRcvd.substr(0,pos+1);
          cout << "client IP:" << clientIP << endl;
          msgRcvd = msgRcvd.substr(pos+1);
          cout << "port:" << msgRcvd << endl;
          int clientPort;
          if(isNumber(msgRcvd))
            clientPort = stoi(msgRcvd, nullptr, 10);
          else
          {
            cerr << "Client port number not an integer!\n";
            shutdown(connFd, SHUT_RDWR);
            close(connFd);
            sleep(1);
            continue;
          }
          //send the USER_ON msg if other clients exist
          string clientListAvail = clientList->getClients2(clientIP, clientPort);
          int totalClients = clientList->getSize();
          if(clientListAvail.compare("") != 0)//clients exist
          {
            cout << "Sending USER_ON msg to all the clients\n";
            clientListAvail = clientListAvail.substr(12);
            cout << "list is:\n" << clientListAvail << endl;
            int * socketsArray = new int[totalClients];
            struct sockaddr_in newClient;
            for(int i = 0 ; i < totalClients; i++)//send to every remaining client
            {
              int spacePos = clientListAvail.find_first_of(" ");
              string extractedIP = clientListAvail.substr(0, spacePos);
              //cout << "IP extracted:" << extractedIP << endl;
              clientListAvail = clientListAvail.substr(spacePos+2);
              spacePos = clientListAvail.find_first_of(" ");
              string extractedPort = clientListAvail.substr(0, spacePos);
              //cout << "port extracted:" << extractedPort << endl;
              int extractedPortClient = stoi(extractedPort, nullptr, 10);
              clientListAvail = clientListAvail.substr(spacePos+1);
              if((socketsArray[i] = socket(AF_INET, SOCK_STREAM, 0)) < 0)
              {
                cerr << "Error code: " << strerror(errno) << endl;//Get some info
                exit(errno);
              }
              newClient.sin_family = AF_INET;
              newClient.sin_port = htons(extractedPortClient);
              const char * extractedIPc = extractedIP.c_str();
              if(inet_pton(AF_INET, extractedIPc, &newClient.sin_addr)<=0)
              {
                cerr << "Error code: " << strerror(errno) << endl;//Get some info
                exit(errno);
              }
              if(connect(socketsArray[i], (struct sockaddr *)&newClient, sizeof(newClient)) < 0)
              {
                cerr << "Error code: " << strerror(errno) << endl;//Get some info
                exit(errno);
              }
              nwrite = write(socketsArray[i], msgToSendC, strlen(msgToSendC));
              if(nwrite == -1)
              {
                cerr << "could not write to the socket!\n";
                cerr << "Error code: " << strerror(errno) << endl;//Get some info
                continue;
              }
            }
            delete[] socketsArray;
          }
          if(clientList->addNode(clientIP, clientPort) == false)
          {
            cerr << "log on client's IP and Port are already listed!\n";
            shutdown(connFd, SHUT_RDWR);
            close(connFd);
            sleep(1);
            continue;
          }
        }
        else if(msgHeader2.compare("LOG_OFF ") == 0)
        {
          cout << "header received:" << msgHeader2 << endl;
          cout << "server will now handle loging off!\n";
          string msgOff = "USER_OFF";
          msgOff += " ";
          msgRcvd = msgRcvd.substr(8);
          //cout << "remaining message :" << msgRcvd << endl;
          int pos = msgRcvd.find_first_of(" ");
          string clientIP = msgRcvd.substr(0,pos+1);
          msgOff += clientIP;
          msgOff += " ";
          cout << "client IP:" << clientIP << endl;
          msgRcvd = msgRcvd.substr(pos+1);
          msgOff += msgRcvd;
          const char * msgOffc = msgOff.c_str();
          cout << "port:" << msgRcvd << endl;
          int clientPort;
          if(isNumber(msgRcvd))
            clientPort = stoi(msgRcvd, nullptr, 10);
          else
          {
            cerr << "Client port number not an integer!\n";
            shutdown(connFd, SHUT_RDWR);
            close(connFd);
            sleep(1);
            continue;
          }
          bool found = clientList->findClient(clientIP, clientPort);
          if(found == false)
          {
            cout << "Client not listed , query aborted!\n";
            shutdown(connFd, SHUT_RDWR);
            close(connFd);
            sleep(1);
            continue;
          }
          clientList->delNode(clientIP, clientPort);
          string clientListAvail = clientList->getClients2(clientIP, clientPort);
          int totalClients = clientList->getSize();
          if(clientListAvail.compare("") == 0)//no other clients on to send
          {
            shutdown(connFd, SHUT_RDWR);
            close(connFd);
            sleep(1);
            continue;
          }
          else//send the USER_OFF msg to the other clients
          {
            cout << "Sending LOG_OFF msg to all the clients\n";
            clientListAvail = clientListAvail.substr(12);
            cout << "list is:\n" << clientListAvail << endl;
            int * socketsArray = new int[totalClients];
            struct sockaddr_in leftClient;
            for(int i = 0 ; i < totalClients; i++)//send to every remaining client
            {
              int spacePos = clientListAvail.find_first_of(" ");
              string extractedIP = clientListAvail.substr(0, spacePos);
              //cout << "IP extracted:" << extractedIP << endl;
              clientListAvail = clientListAvail.substr(spacePos+2);
              spacePos = clientListAvail.find_first_of(" ");
              string extractedPort = clientListAvail.substr(0, spacePos);
              //cout << "port extracted:" << extractedPort << endl;
              int extractedPortClient = stoi(extractedPort, nullptr, 10);
              clientListAvail = clientListAvail.substr(spacePos+1);
              if((socketsArray[i] = socket(AF_INET, SOCK_STREAM, 0)) < 0)
              {
                cerr << "Error code: " << strerror(errno) << endl;//Get some info
                exit(errno);
              }
              leftClient.sin_family = AF_INET;
              leftClient.sin_port = htons(extractedPortClient);
              const char * extractedIPc = extractedIP.c_str();
              if(inet_pton(AF_INET, extractedIPc, &leftClient.sin_addr)<=0)
              {
                cerr << "Error code: " << strerror(errno) << endl;//Get some info
                exit(errno);
              }
              if(connect(socketsArray[i], (struct sockaddr *)&leftClient, sizeof(leftClient)) < 0)
              {
                cerr << "Error code: " << strerror(errno) << endl;//Get some info
                exit(errno);
              }
              nwrite = write(socketsArray[i], msgOffc, strlen(msgOffc));
              if(nwrite == -1)
              {
                cerr << "could not write to the socket!\n";
                cerr << "Error code: " << strerror(errno) << endl;//Get some info
                continue;
              }
            }
            delete[] socketsArray;
          }
        }
        else if(msgHeader3.compare("GET_CLIENTS ") == 0)
        {
          cout << "header received:" << msgHeader3 << endl;
          msgRcvd = msgRcvd.substr(12);
          //cout << "remaining message :" << msgRcvd << endl;
          int pos = msgRcvd.find_first_of(" ");
          string clientIP = msgRcvd.substr(0,pos+1);
          cout << "client IP:" << clientIP << endl;
          msgRcvd = msgRcvd.substr(pos+1);
          cout << "port:" << msgRcvd << endl;
          int clientPort;
          if(isNumber(msgRcvd))
            clientPort = stoi(msgRcvd, nullptr, 10);
          else
          {
            cerr << "Client port number not an integer!\n";
            shutdown(connFd, SHUT_RDWR);
            close(connFd);
            sleep(1);
            continue;
          }
          bool found = clientList->findClient(clientIP, clientPort);
          if(found == false)//client has not sent log on msg firstly
          {
            cout << "Client not listed , query aborted!\n";
            shutdown(connFd, SHUT_RDWR);
            close(connFd);
            sleep(1);
            continue;
          }
          string clientListAvail = clientList->getClients(clientIP, clientPort);
          if(clientListAvail.compare("") == 0)//no other clients on
          {
            const char * clientAloneMsg = "A";
            nwrite = write(connFd, clientAloneMsg, strlen(clientAloneMsg));
            if(nwrite == -1)
            {
              cerr << "write 1 receiver error\n";
              cerr << "could not read from the socket!\n";
              cerr << "Error code: " << strerror(errno) << endl;//Get some info
              shutdown(connFd, SHUT_RDWR);
              close(connFd);
              sleep(1);
              continue;
            }
          }
          else//send the list
          {
            const char * clientListAvailC = clientListAvail.c_str();
            nwrite = write(connFd, clientListAvailC, strlen(clientListAvailC));
            if(nwrite == -1)
            {
              cerr << "write 1 receiver error\n";
              cerr << "could not read from the socket!\n";
              cerr << "Error code: " << strerror(errno) << endl;//Get some info
              shutdown(connFd, SHUT_RDWR);
              close(connFd);
              sleep(1);
              continue;
            }
          }
        }
        else
        {
          cout << "Error invalid message format!\n";
          shutdown(connFd, SHUT_RDWR);
          close(connFd);
          sleep(1);
          continue;
        }
      }
      else
      {
        cout << "Error invalid message format!\n";
        shutdown(connFd, SHUT_RDWR);
        close(connFd);
        sleep(1);
        continue;
      }
    }
    else if(nread == -1)
    {
      cerr << "write 1 receiver error\n";
      cerr << "could not read from the socket!\n";
      cerr << "Error code: " << strerror(errno) << endl;//Get some info
      shutdown(connFd, SHUT_RDWR);
      close(connFd);
      sleep(1);
      continue;
    }
    shutdown(connFd, SHUT_RDWR);
    close(connFd);
  }
  delete clientList;
  return 0;
}