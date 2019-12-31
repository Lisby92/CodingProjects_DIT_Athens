#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <time.h>
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstdlib>
#include <csignal>
#include "HelpersClient.hpp"

using namespace std;

/* THREAD SPECIFIC */
typedef struct pthreadStruct
{
  int hostPort;
  string hostIP;
} pthreadStruct;

/* Thread routine to handle CyBuffer Items */
void * pthreadRoutine(void * arg);

static pthread_mutex_t clientListMux;
static pthread_mutex_t versionListMux;
static pthread_mutex_t cyBufferMux;
static pthread_cond_t nonEmptyCond;
static pthread_cond_t nonFullCond;
static pthread_t * threadArray = NULL;
static pthreadStruct * pthreadArgs = NULL;
static int workerThreads;
static int sockFd = 0;
static int sockFd1 = 0;
static int sockFd2 = 0;
static int sockFd5 = 0;
static List * clientList = NULL;
static VersionList * clientVersList = NULL;
static VersionList * threadVersList = NULL;
static const char * IPbuffer;
static int portNum;
static int bufferSize;
static CyBuffer * cbp = NULL;
static int serverPort = 0;
static char * serverIP;
static struct sockaddr_in server;

static void sendLOG_OFF()
{
  string logOffMsgS = "LOG_OFF";
  logOffMsgS += " ";
  logOffMsgS += IPbuffer;
  logOffMsgS += " ";
  logOffMsgS += to_string(portNum);
  cout << "Sending LOG_OFF msg to server...\n";
  if((sockFd5 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    cerr << "Error code: " << strerror(errno) << endl;//Get some info
    if(clientList != NULL)
      delete clientList;
    if(cbp != NULL)
      delete cbp;
    delete[] threadArray;
    delete pthreadArgs;
    delete clientVersList;
    delete threadVersList;
    logOffMsgS.~string();//free xplicitly
    exit(errno);
  }
  if(connect(sockFd5, (struct sockaddr *)&server, sizeof(server)) < 0)
  {
    cerr << "Error code: " << strerror(errno) << endl;//Get some info
    if(clientList != NULL)
      delete clientList;
    if(cbp != NULL)
      delete cbp;
    delete[] threadArray;
    delete pthreadArgs;
    delete clientVersList;
    delete threadVersList;
    logOffMsgS.~string();//free xplicitly
    exit(errno);
  }
  const char * logOffMsg = logOffMsgS.c_str();
  if(write(sockFd5, logOffMsg, strlen(logOffMsg)) == -1)
  {
    cerr << "send 2 client error\n";
    cerr << " could not write LOG_OFF msg to the socket!\n";
    cerr << "Error code: " << strerror(errno) << endl;//Get some info
    if(clientList != NULL)
      delete clientList;
    if(cbp != NULL)
      delete cbp;
    delete[] threadArray;
    delete pthreadArgs;
    delete clientVersList;
    delete threadVersList;
    logOffMsgS.~string();//free xplicitly
    exit(errno);
  }
  else
  {
    if(clientList != NULL)
      delete clientList;
    if(cbp != NULL)
      delete cbp;
    delete[] threadArray;
    delete pthreadArgs;
    delete clientVersList;
    delete threadVersList;
  }
  logOffMsgS.~string();//free xplicitly
}

static void signalHandler(int signum)//thelei socket kai connect edo
{ //this function handles the signals SIGQUIT and SIGINT
  cout << "\nInterrupt signal (" << signum << ") received.\n";
  // cleanup and close up stuff here
  string logOffMsgS = "LOG_OFF";
  logOffMsgS += " ";
  logOffMsgS += IPbuffer;
  logOffMsgS += " ";
  logOffMsgS += to_string(portNum);
  cout << "Sending LOG_OFF msg to server...\n";
  if((sockFd5 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    cerr << "Error code: " << strerror(errno) << endl;//Get some info
    if(clientList != NULL)
      delete clientList;
    if(cbp != NULL)
      delete cbp;
    delete[] threadArray;
    delete pthreadArgs;
    delete clientVersList;
    delete threadVersList;
    logOffMsgS.~string();//free xplicitly
    exit(errno);
  }
  if(connect(sockFd5, (struct sockaddr *)&server, sizeof(server)) < 0)
  {
    cerr << "Error code: " << strerror(errno) << endl;//Get some info
    if(clientList != NULL)
      delete clientList;
    if(cbp != NULL)
      delete cbp;
    delete[] threadArray;
    delete pthreadArgs;
    delete clientVersList;
    delete threadVersList;
    logOffMsgS.~string();//free xplicitly
    exit(errno);
  }
  const char * logOffMsg = logOffMsgS.c_str();
  if(write(sockFd5, logOffMsg, strlen(logOffMsg)) == -1)
  {
    cerr << "send 2 client error\n";
    cerr << " could not write LOG_OFF msg to the socket!\n";
    cerr << "Error code: " << strerror(errno) << endl;//Get some info
    if(clientList != NULL)
      delete clientList;
    if(cbp != NULL)
      delete cbp;
    delete[] threadArray;
    delete pthreadArgs;
    delete clientVersList;
    delete threadVersList;
    logOffMsgS.~string();//free xplicitly
    exit(errno);
  }
  else
  {
    if(clientList != NULL)
      delete clientList;
    if(cbp != NULL)
      delete cbp;
    delete[] threadArray;
    delete pthreadArgs;
    delete clientVersList;
    delete threadVersList;
  }
  logOffMsgS.~string();//free xplicitly
  exit(signum);
}

static string listDirRecurs(char * dirName, int & total)
{ //returns a concatenated string consisting of directories and files
  DIR * d;
  string creation = "";//empty at first
  d = opendir(dirName);
  if(!d)
  {
    cerr << "ERROR:: directory could not be opened!\n";//Report error
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    delete clientList;
    delete cbp;
    delete[] threadArray;
    delete pthreadArgs;
    delete clientVersList;
    delete threadVersList;
    sendLOG_OFF();
    exit(errno);
  }
  while(1)//get subsequent entries
  {
    struct dirent * entry;
    char * entryName;
    entry = readdir(d);
    if(!entry)//There are no more entries in this directory so break
      break;
    entryName = entry->d_name;
    if(strcmp(entryName, "..") != 0 && strcmp(entryName, ".") != 0)
    {
      string temp = dirName;
      creation += temp;
      creation += "/";
      creation += entryName;
      creation += "*"; // used as a delimiter
      total++;
    }
    if(entry->d_type & DT_DIR)//traverse directories
    {
      if(strcmp(entryName, "..") != 0 && strcmp(entryName, ".") != 0)
      {
        int pathLength;
        char path[PATH_MAX];
        pathLength = snprintf(path, PATH_MAX,"%s/%s", dirName, entryName);
        if(pathLength >= PATH_MAX)
        {
          cerr << "ERROR:: Path has got really long!\n";//Report error
          cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
          delete clientList;
          delete cbp;
          delete[] threadArray;
          delete pthreadArgs;
          delete clientVersList;
          delete threadVersList;
          if(closedir(d))
          {
            cerr << "ERROR:: directory could not be closed!\n";//Report error
            cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
          }
          sendLOG_OFF();
          exit(errno);
        }
        //recurse with the new path
        creation += listDirRecurs(path, total);
      }
    }
  }
  //After going through all the entries, close the directory
  if(closedir(d))
  {
    cerr << "ERROR:: directory could not be closed!\n";//Report error
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    delete clientList;
    delete cbp;
    delete[] threadArray;
    delete pthreadArgs;
    delete clientVersList;
    delete threadVersList;
    sendLOG_OFF();
    exit(errno);
  }
  return creation;
}

static void makeDirTree(const char * path, const char * parentPath)
{ //this function creates the directory tree identical to the new client
  //before files are created @ the corresponding paths
  string fullPath = path;
  int length = fullPath.length();
  int counter = 0;
  int totalSlash = 0;
  while(counter < length)//count the subpaths needed
  {
    if(fullPath[counter] == '/')
      totalSlash++;
    counter++;
  }
  int created = -1;
  if(totalSlash == 0)//create it
  {
    string totalPath = parentPath;
    totalPath += "/";
    totalPath += fullPath;
    const char * fullPathC = totalPath.c_str();
    if(mkdir(fullPathC, 0777) == -1)
    {
      if(errno != EEXIST)//skip already created by other calls as not fatal
      {
        cerr << "ERROR:: directory target could not be created!\n";//Report error
        cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
        delete clientList;
        delete cbp;
        delete[] threadArray;
        delete pthreadArgs;
        delete clientVersList;
        delete threadVersList;
        sendLOG_OFF();
        exit(errno);
      }
    }
  }
  else
  {
    string parentPathS = parentPath;
    parentPathS += "/";
    while(created < totalSlash)
    {
      int nextSlashPos = fullPath.find_first_of("/");
      string temp = fullPath.substr(0, nextSlashPos);
      fullPath = fullPath.substr(nextSlashPos+1);
      string toBCreated = parentPathS;
      toBCreated += temp;
      toBCreated += "/";
      const char * tempC = toBCreated.c_str();
      cout << pthread_self() << " thread creating directory:" << tempC << endl;
      if(mkdir(tempC, 0777) == -1)
      {
        if(errno != EEXIST)//skip already created by other calls as not fatal
        {
          cerr << "ERROR:: directory target could not be created!\n";//Report error
          cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
          delete clientList;
          delete cbp;
          delete[] threadArray;
          delete pthreadArgs;
          delete clientVersList;
          delete threadVersList;
          sendLOG_OFF();
          exit(errno);
        }
      }
      parentPathS += temp;
      parentPathS += "/";
      created++;
    }
  }
}

static void removeDirTree(const char * dirName)
{ //recursive traversal of directory tree and deletion of items inside it
  DIR *dir;
  struct dirent *entry;
  char path[PATH_MAX];
  dir = opendir(dirName);
  if(dir == NULL)
  {
    cerr << "ERROR:: directory could not be opened!\n";//Report error
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    delete clientList;
    delete cbp;
    delete[] threadArray;
    delete pthreadArgs;
    delete clientVersList;
    delete threadVersList;
    sendLOG_OFF();
    exit(errno);
  }
  while((entry = readdir(dir)) != NULL)
  {
    if(strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
    {
      snprintf(path, (size_t) PATH_MAX, "%s/%s", dirName, entry->d_name);
      if(entry->d_type == DT_DIR) 
      {
        removeDirTree(path);
      }
      remove(path);
    }
  }
  if(closedir(dir))
  {
    cerr << "ERROR:: directory could not be closed!\n";//Report error
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    delete clientList;
    delete cbp;
    delete[] threadArray;
    delete pthreadArgs;
    delete clientVersList;
    delete threadVersList;
    sendLOG_OFF();
    exit(errno);
  }
  remove(dirName);
}

/* PROGRAM MAIN FUNCTION STARTS HERE */
int main(int argc, char ** argv)
{
  signal(SIGINT, signalHandler);
  signal(SIGQUIT, signalHandler);
  if(argc != 13)
  {
    cerr << "ERROR:: Invalid arguments!\n";
    return 1;
  }
  int argCount = 0;
  int * argPos = new int[6];
  for(int i = 0; i < argc; i++)//get mixed order arguments but all are needed
  {
    if(strcmp(argv[i], "-d") == 0)
    {
      argPos[0] = i+1;
      argCount++;
    }
    if(strcmp(argv[i], "-p") == 0)
    {
      argPos[1] = i+1;
      argCount++;
    }
    if(strcmp(argv[i], "-w") == 0)
    {
      argPos[2] = i+1;
      argCount++;
    }
    if(strcmp(argv[i], "-b") == 0)
    {
      argPos[3] = i+1;
      argCount++;
    }
    if(strcmp(argv[i], "-sp") == 0)
    {
      argPos[4] = i+1;
      argCount++;
    }
    if(strcmp(argv[i], "-sip") == 0)
    {
      argPos[5] = i+1;
      argCount++;
    }
  }
  if(argCount != 6)
  {
    cerr << "ERROR:: Invalid arguments!\n";
    delete[] argPos;
    return 1;
  }
  argCount = 0;
  if(isNumber(argv[argPos[1]]) == true)
    argCount++;
  if(isNumber(argv[argPos[2]]) == true)
    argCount++;
  if(isNumber(argv[argPos[3]]) == true)
    argCount++;
  if(isNumber(argv[argPos[4]]) == true)
    argCount++;
  if(argCount != 4)
  {
    cerr << "ERROR:: Invalid arguments!\n";
    delete[] argPos;
    return 1;
  }
  //cout << "Arguments where correct!\n";
  char * dirName = argv[argPos[0]];
  portNum = stoi(argv[argPos[1]], nullptr, 10);
  workerThreads = stoi(argv[argPos[2]], nullptr, 10);
  bufferSize = stoi(argv[argPos[3]], nullptr, 10);
  serverPort = stoi(argv[argPos[4]], nullptr, 10);
  serverIP = argv[argPos[5]];
  delete[] argPos;
  clientVersList = new VersionList();
  int totalFiles2 = 0;
  string fileList = listDirRecurs(dirName, totalFiles2);
  cout << "total local files found in local folder: " << totalFiles2 << endl;
  for(int i = 0; i < totalFiles2-1; i++)
  {
    int pos = fileList.find_first_of("*");
    string temp = fileList.substr(0, pos);
    fileList = fileList.substr(pos+1);
    int pos2 = temp.find_first_of("/");
    temp = temp.substr(pos2+1);
    pos2 = temp.find_first_of("/");
    temp = temp.substr(pos2+1);
    cout << "file:<" << temp << ">\n";
    string localFilePathS = dirName;
    localFilePathS += "/";
    localFilePathS += temp;
    const char * localFilePath = localFilePathS.c_str();
    ifstream in(localFilePath);//open stream
    string line;
    string result;
    while(getline(in, line))
    {
      result += line;
      result += "\n";
    }
    clientVersList->addNode(temp, hashFunction(result, temp));
    in.close();
  }
  fileList = fileList.substr(0,fileList.length()-1);
  int pos2 = fileList.find_first_of("/");
  fileList = fileList.substr(pos2+1);
  pos2 = fileList.find_first_of("/");
  fileList = fileList.substr(pos2+1);
  cout << "file:<" << fileList << ">\n";
  string localFilePathS = dirName;
  localFilePathS += "/";
  localFilePathS += fileList;
  const char * localFilePath = localFilePathS.c_str();
  ifstream in(localFilePath);//open stream
  string line;
  string result;
  while(getline(in, line))
  {
    result += line;
    result += "\n";
  }
  clientVersList->addNode(fileList, hashFunction(result, fileList));
  localFilePathS.~string();
  fileList.~string();
  result.~string();
  line.~string();
  in.close();
  if((sockFd1 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    cerr << "Error code: " << strerror(errno) << endl;//Get some info
    delete clientVersList;
    exit(errno);
  }
  server.sin_family = AF_INET;
  server.sin_port = htons(serverPort); 
  if(inet_pton(AF_INET, serverIP, &server.sin_addr)<=0)
  {
    cerr << "Error code: " << strerror(errno) << endl;//Get some info
    delete clientVersList;
    exit(errno);
  }
  if(connect(sockFd1, (struct sockaddr *)&server, sizeof(server)) < 0)
  {
    cerr << "Error code: " << strerror(errno) << endl;//Get some info
    delete clientVersList;
    exit(errno);
  }
  //send the LOG ON message
  char hostbuffer[256+1] = ""; 
  char getClientsBuffer[1024+1] = "";
  struct hostent *host_entry;
  // To retrieve hostname 
  gethostname(hostbuffer, sizeof(hostbuffer)); 
  // To retrieve host information 
  host_entry = gethostbyname(hostbuffer);  
  // To convert an Internet network  address into ASCII string 
  IPbuffer = inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]));
  printf("Hostname: %s\n", hostbuffer); 
  printf("Host IP(decimal dot notation):%s\n", IPbuffer);
  //create and send the log_on msg
  string logONmsg = "LOG_ON ";
  logONmsg += IPbuffer;
  logONmsg += " ";
  logONmsg += to_string(portNum);
  //cout << "LOG_ON_message_created =" << logONmsg << endl;
  const char * logONmsgC = logONmsg.c_str();
  if(write(sockFd1, logONmsgC, strlen(logONmsgC)) == -1)
  {
    cerr << "could not write to the socket!\n";
    cerr << "Error code: " << strerror(errno) << endl;//Get some info
    delete clientVersList;
    exit(errno);
  }
  //create and sent the get_clients msg
  if((sockFd2 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    cerr << "Error code: " << strerror(errno) << endl;//Get some info
    delete clientVersList;
    exit(errno);
  }
  if(connect(sockFd2, (struct sockaddr *)&server, sizeof(server)) < 0)
  {
    cerr << "Error code: " << strerror(errno) << endl;//Get some info
    delete clientVersList;
    exit(errno);
  }
  logONmsg.~string();//free explicitly to avoid leaks after signal
  string getClientMsgS = "GET_CLIENTS ";
  getClientMsgS += IPbuffer;
  getClientMsgS += " ";
  getClientMsgS += to_string(portNum);
  cout << "Sending message to server : " << getClientMsgS << endl;
  const char * getClientMsg = getClientMsgS.c_str();
  if(write(sockFd2, getClientMsg, strlen(getClientMsg)) == -1)
  {
    cerr << "could not write to the socket!\n";
    cerr << "Error code: " << strerror(errno) << endl;//Get some info
    delete clientVersList;
    sendLOG_OFF();
    exit(errno);
  }
  //read the list received or special A if client is the first to log on
  int nread = read(sockFd2, getClientsBuffer, 1024);
  clientList = new List();
  cbp = new CyBuffer(bufferSize);//create an empty cyclic buffer
  if(nread > 0)
  {
    string receivedList = getClientsBuffer;
    if(receivedList.compare("A") == 0)
      cout << "0 clients connected so far\n";
    else
    {
      string receivedListH = receivedList.substr(0, 11);
      if(receivedListH.compare("CLIENT_LIST") != 0)
      {
        cerr << "message format was wrong!\n";
        delete clientList;
        delete clientVersList;
        delete cbp;
        sendLOG_OFF();
        exit(123);
      }
      else
      {
        receivedList = receivedList.substr(12);
        int spacePos = receivedList.find_first_of(" ");
        string totalCl = receivedList.substr(0, spacePos);
        receivedList = receivedList.substr(spacePos+1);
        int totalClients = stoi(totalCl, nullptr, 10);
        for(int i = 0; i < totalClients; i++)
        {
          spacePos = receivedList.find_first_of(" ");
          string extractedIP = receivedList.substr(0, spacePos);
          receivedList = receivedList.substr(spacePos+2);
          spacePos = receivedList.find_first_of(" ");
          string extractedPort = receivedList.substr(0, spacePos);
          receivedList = receivedList.substr(spacePos+1);
          clientList->addNode(extractedIP, stoi(extractedPort, nullptr, 10));
          bool result = cbp->addItem("", -2, extractedIP, stoi(extractedPort, nullptr, 10));
          if(result == true)
            cout << "added item successfully in 1st add 641 line\n";
          else
            cout << "buffer was full in 1st add 641 line\n";
        }
      }
    }
  }
  else if(nread == -1)
  {
    cerr << "could not read from the socket!\n";
    cerr << "Error code: " << strerror(errno) << endl;//Get some info
    delete clientList;
    delete clientVersList;
    delete cbp;
    sendLOG_OFF();
    exit(errno);
  }
  getClientMsgS.~string();//free explicitly to avoid leaks after signal
  //now work as a peer to peer server
  struct sockaddr_in p2pServer;
  socklen_t clientlen = sizeof(struct sockaddr);
  struct sockaddr * serverPtr = (struct sockaddr *) & p2pServer;
  if((sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    delete clientList;
    delete clientVersList;
    delete cbp;
    sendLOG_OFF();
    exit(errno);
  }
  p2pServer.sin_family = AF_INET ;//Internet domain
  p2pServer.sin_addr.s_addr = htonl(INADDR_ANY);//accept any IP address
  p2pServer.sin_port = htons(portNum);//The given port
  int enable = 1;
  if (setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    cerr << "setsockopt(SO_REUSEADDR) failed : " << strerror(errno) << endl;
  //experiments....... remove ta pano kai des ti paiuzei
  if(bind(sockFd, serverPtr, sizeof(p2pServer)) < 0)//Bind socket to address
  {
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    delete clientList;
    delete clientVersList;
    delete cbp;
    sendLOG_OFF();
    exit(errno);
  }
  if(listen(sockFd, 1000) < 0)
  {
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    delete clientList;
    delete clientVersList;
    delete cbp;
    sendLOG_OFF();
    exit(errno);
  }
  //initialise threads and mutexes
  if(pthread_mutex_init(&clientListMux, NULL) != 0) //initialise client list mutex
  { 
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    delete clientList;
    delete clientVersList;
    delete cbp;
    delete[] threadArray;
    sendLOG_OFF();
    exit(errno); 
  }
  if(pthread_mutex_init(&cyBufferMux, NULL) != 0) //initialise CyBuffer mutex
  { 
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    delete clientList;
    delete cbp;
    delete clientVersList;
    delete[] threadArray;
    sendLOG_OFF();
    exit(errno);
  }
  if(pthread_cond_init(&nonEmptyCond, NULL) != 0) //initialise CyBuffer cond
  { 
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    delete clientList;
    delete cbp;
    delete[] threadArray;
    delete clientVersList;
    sendLOG_OFF();
    exit(errno);
  }
  if(pthread_cond_init(&nonFullCond, NULL) != 0) //initialise CyBuffer cond
  { 
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    delete clientList;
    delete cbp;
    delete[] threadArray;
    delete clientVersList;
    sendLOG_OFF();
    exit(errno);
  }
  if(pthread_mutex_init(&versionListMux, NULL) != 0) //initialise client list mutex
  { 
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    delete clientList;
    delete cbp;
    delete[] threadArray;
    delete clientVersList;
    sendLOG_OFF();
    exit(errno);
  }
  pthread_attr_t pthread_attr;
  threadArray = new pthread_t[workerThreads];//array of working threads
  /* Initialise pthread attribute to create detached threads. */
  if(pthread_attr_init(&pthread_attr) != 0)
  {
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    delete clientList;
    delete cbp;
    delete[] threadArray;
    delete clientVersList;
    sendLOG_OFF();
    exit(errno);
  }
  if(pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_DETACHED) != 0)
  {
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    delete clientList;
    delete cbp;
    delete[] threadArray;
    delete clientVersList;
    sendLOG_OFF();
    exit(errno);
  }
   /* Initialise pthread arguments */
  threadVersList = new VersionList();
  pthreadArgs = new pthreadStruct;
  pthreadArgs->hostIP = IPbuffer;
  pthreadArgs->hostPort = portNum;
  for(int i = 0; i < workerThreads; i++)//create the threads
  {
    if(pthread_create(&threadArray[i], &pthread_attr,
       pthreadRoutine, (void *)pthreadArgs) != 0)
    {
      cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
      delete clientList;
      delete cbp;
      delete[] threadArray;
      delete pthreadArgs;
      delete clientVersList;
      delete threadVersList;
      sendLOG_OFF();
      exit(errno);
    }
    //pthread_detach(threadArray[i]);
  }
  if(pthread_attr_destroy(&pthread_attr) != 0)
  {
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    delete clientList;
    delete cbp;
    delete[] threadArray;
    delete clientVersList;
    delete threadVersList;
    sendLOG_OFF();
    exit(errno);
  }
  fd_set active_fd_set, read_fd_set;
  /* Initialize the set of active sockets. */
  FD_ZERO(&active_fd_set);
  FD_SET(sockFd, &active_fd_set);
  //work as a peer to peer server
  while(1)
  {
    /* Block until input arrives on one or more active sockets. */
    cout << "waiting for P2P remote client connections...\n";
    read_fd_set = active_fd_set;
    if(select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0)
    {
      perror("select");
      delete clientList;
      delete cbp;
      delete[] threadArray;
      delete pthreadArgs;
      delete clientVersList;
      delete threadVersList;
      sendLOG_OFF();
      exit(EXIT_FAILURE);
    }
    /* Service all the sockets with input pending. */
    for(int i = 0; i < FD_SETSIZE; ++i)
    {
      if(FD_ISSET(i, &read_fd_set))
      {
        if(i == sockFd)
        {
          /* Connection request on original socket. */
          int newFd;
          if((newFd = accept(sockFd, serverPtr, &clientlen)) < 0)
          {
            cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
            delete clientList;
            delete cbp;
            delete[] threadArray;
            delete pthreadArgs;
            delete clientVersList;
            delete threadVersList;
            sendLOG_OFF();
            exit(errno);
          }
          FD_SET(newFd, &active_fd_set);
          cout << "accepted a new incoming connection...\n";
        }
        else
        {
          cout << "communicating through an already open connection...\n";
          /* Data arriving on an already-connected socket. */
          char msgBuffer[256+1] = "";//init
          nread = read(i, msgBuffer, 256);
          if(nread > 0)//LOG ON msg received successfully
          {
            string msgRcvd = msgBuffer;
            if(msgRcvd.length() > 12)
            {
              string msgHeader1 = msgRcvd.substr(0, 8);//USER_ON 
              string msgHeader2 = msgRcvd.substr(0, 13);//GET_FILE_LIST
              string msgHeader3 = msgRcvd.substr(0, 9);//GET_FILE
              string msgHeader4 = msgRcvd.substr(0, 9);//USER_OFF
              if(msgHeader1.compare("USER_ON ") == 0)//prepei na vazeis kai ston buffer
              {
                cout << "USER_ON msg received from server...\n";
                msgRcvd = msgRcvd.substr(8);
                int pos = msgRcvd.find_first_of(" ");
                string clientIP = msgRcvd.substr(0,pos);
                cout << "remote client IP: " << clientIP << endl;
                msgRcvd = msgRcvd.substr(pos+1);
                cout << "remote client port: " << msgRcvd << endl;
                int clientPort;
                if(isNumber(msgRcvd))
                  clientPort = stoi(msgRcvd, nullptr, 10);
                else
                {
                  cerr << "remote client port is invalid!\n";
                  close(i);
                  FD_CLR(i, &active_fd_set);
                  sleep(1);
                }
                if(pthread_mutex_lock(&clientListMux) != 0)//lock
                {
                  cerr << "Mutex lock failed\n";//Get some info as to why
                  delete clientList;
                  delete cbp;
                  delete[] threadArray;
                  delete pthreadArgs;
                  delete clientVersList;
                  delete threadVersList;
                  sendLOG_OFF();
                  exit(errno);
                }
                clientList->addNode(clientIP, clientPort);//CS
                if(pthread_mutex_unlock(&clientListMux) != 0)//unlock
                {
                  cerr << "Mutex unlock failed\n";//Get some info as to why
                  delete clientList;
                  delete cbp;
                  delete[] threadArray;
                  delete pthreadArgs;
                  delete clientVersList;
                  delete threadVersList;
                  sendLOG_OFF();
                  exit(errno);
                }
                pthread_mutex_lock(&cyBufferMux);
                while(cbp->getCurItems() >= bufferSize)
                {
                  //cout << "found CyBuffer full!\n";
                  pthread_cond_wait(&nonFullCond, &cyBufferMux) ;
                }
                bool result = cbp->addItem("", -2, clientIP, clientPort);
                if(result == true)
                  cout << "added item successfully in 2nd add 883 line\n";
                else
                  cout << "buffer was full in 2nd add 883 line\n";
                pthread_mutex_unlock(&cyBufferMux);
                pthread_cond_signal(&nonEmptyCond);
                close(i);
                FD_CLR(i, &active_fd_set);
              }
              else if(msgHeader2.compare("GET_FILE_LIST") == 0)
              {
                cout << "GET_FILE_LIST msg received from remote P2P client...\n";
                string msgFileList = "FILE_LIST";
                msgFileList += " ";
                int totalFiles1 = 0;
                string fileListRecv = listDirRecurs(dirName, totalFiles1);
                cout << "found locally total files/dirs = " << totalFiles1 << endl;
                msgFileList += to_string(totalFiles1);
                msgFileList += " ";
                for(int i = 0; i < totalFiles1-1; i++)
                {
                  int pos = fileListRecv.find_first_of("*");
                  string temp = fileListRecv.substr(0, pos);
                  fileListRecv = fileListRecv.substr(pos+1);
                  msgFileList += temp;
                  msgFileList += " ";
                  msgFileList += to_string(-1);
                  msgFileList += " ";
                }
                msgFileList += fileListRecv.substr(0, fileListRecv.length()-1);
                msgFileList += " ";
                msgFileList += to_string(-1);
                const char * msgFileListC = msgFileList.c_str();
                //cout << "fileList:<" << msgFileListC << ">\n";
                if(write(i, msgFileListC, strlen(msgFileListC)) == -1)
                {
                  cerr << "could not write to the socket!\n";
                  cerr << "Error code: " << strerror(errno) << endl;//Get some info
                  close(i);
                  FD_CLR(i, &active_fd_set);
                  sleep(1);
                }
                close(i);
                FD_CLR(i, &active_fd_set);
              }
              else if(msgHeader3.compare("GET_FILE ") == 0)
              {
                cout << "GET_FILE msg received from remote P2P client...\n";
                msgRcvd = msgRcvd.substr(9);
                int pos = msgRcvd.find_first_of(" ");
                string filePath = msgRcvd.substr(0,pos);
                cout << "filepath extracted: " << filePath << endl;
                msgRcvd = msgRcvd.substr(pos+1);
                int fileVers = stoi(msgRcvd, nullptr, 10);
                cout << "remote P2P client's version extracted: " << fileVers << "\n";
                string pathToLook = dirName;
                pathToLook += "/";
                pathToLook += filePath;
                const char * filePathC = pathToLook.c_str();
                int fileFd = open(filePathC, O_RDONLY, 0777);
                if(fileFd == -1)//file not found
                {
                  const char * replyMsg = "FILE_NOT_FOUND";
                  cout << replyMsg << endl;
                  if(write(i, replyMsg, strlen(replyMsg)) == -1)
                  {
                    cerr << "could not write to the socket!\n";
                    cerr << "Error code: " << strerror(errno) << endl;//Get some info
                    close(i);
                    FD_CLR(i, &active_fd_set);
                    sleep(1);
                  }
                }
                else//file exists check version
                {
                  close(fileFd);
                  int localVersion = clientVersList->getVersion(filePath);
                  if(localVersion == fileVers)//file up to date
                  {
                    const char * replyMsg = "FILE_UP_TO_DATE";
                    cout << replyMsg << endl;
                    if(write(i, replyMsg, strlen(replyMsg)) == -1)
                    {
                      cerr << "could not write to the socket!\n";
                      cerr << "Error code: " << strerror(errno) << endl;//Get some info
                      close(i);
                      FD_CLR(i, &active_fd_set);
                      sleep(1);
                    }
                  }
                  else//file is outdated
                  {
                    cout << "file is outdated, sending it to remote P2P client!\n";
                    ifstream in(filePathC);//open stream
                    string line;
                    string result;
                    while(getline(in, line))
                    {
                      result += line;
                      result += "\n";
                    }
                    in.close();
                    int fileSize = result.length();
                    string replyMsg = "FILESIZE";
                    replyMsg += " ";
                    replyMsg += to_string(localVersion);
                    replyMsg += " ";
                    replyMsg += to_string(fileSize);
                    const char * replyMsgC = replyMsg.c_str();
                    cout << "sending header ...\n";
                    if(write(i, replyMsgC, strlen(replyMsgC)) == -1)
                    {
                      cerr << "could not write to the socket!\n";
                      cerr << "Error code: " << strerror(errno) << endl;//Get some info
                      close(i);
                      FD_CLR(i, &active_fd_set);
                      sleep(1);
                    }
                    const char * fileData = result.c_str();
                    cout << "sending data...\n";
                    if(write(i, fileData, strlen(fileData)) == -1)
                    {
                      cerr << "could not write to the socket!\n";
                      cerr << "Error code: " << strerror(errno) << endl;//Get some info
                      close(i);
                      FD_CLR(i, &active_fd_set);
                      sleep(1);
                    }
                  }
                }
                close(i);
                FD_CLR(i, &active_fd_set);
              }
              else if(msgHeader4.compare("USER_OFF ") == 0)
              {
                cout << "USER_OFF msg received from server...\n";
                msgRcvd = msgRcvd.substr(9);
                int pos = msgRcvd.find_first_of(" ");
                string clientIP = msgRcvd.substr(0,pos+1);
                cout << "remote P2P client's IP: " << clientIP << endl;
                msgRcvd = msgRcvd.substr(pos+2);
                cout << "remote P2P client's port: " << msgRcvd << endl;
                int clientPort;
                if(isNumber(msgRcvd))
                  clientPort = stoi(msgRcvd, nullptr, 10);
                else
                {
                  cerr << "Client port number not an integer!\n";
                  close(i);
                  FD_CLR(i, &active_fd_set);
                  sleep(1);
                }
                if(pthread_mutex_lock(&clientListMux) != 0)//lock
                {
                  cerr << "Mutex lock failed\n";//Get some info as to why
                  delete clientList;
                  delete cbp;
                  delete[] threadArray;
                  delete pthreadArgs;
                  delete clientVersList;
                  delete threadVersList;
                  sendLOG_OFF();
                  exit(errno);
                }//edo prin to delete.
                if(clientList->findClient(clientIP, clientPort) == true)
                  clientList->delNode(clientIP, clientPort);
                if(pthread_mutex_unlock(&clientListMux) != 0)//unlock
                {
                  cerr << "Mutex unlock failed\n";//Get some info as to why
                  delete clientList;
                  delete cbp;
                  delete[] threadArray;
                  delete pthreadArgs;
                  delete clientVersList;
                  delete threadVersList;
                  sendLOG_OFF();
                  exit(errno);
                }
                close(i);
                FD_CLR(i, &active_fd_set);
              }
              else
              {
                cout << "Error invalid message format!\n";
                close(i);
                FD_CLR(i, &active_fd_set);
                sleep(1);
              }
            }
            else
            {
              cout << "Error invalid message format!\n";
              close(i);
              FD_CLR(i, &active_fd_set);
              sleep(1);
            }
          }
          else if(nread == -1)
          {
            cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
            close(i);
            FD_CLR(i, &active_fd_set);
            sleep(1);
          }
          //if done close it and remove from fd_set
          close(i);
          FD_CLR(i, &active_fd_set);
        }
      }
    }
  }
  //actually not supposed to get here...
  delete cbp;
  delete clientList;
  delete[] threadArray;
  delete pthreadArgs;
  delete clientVersList;
  delete threadVersList;
  if(pthread_mutex_destroy(&clientListMux) != 0)
    cerr << "Mutex destruction failed\n";
  if(pthread_mutex_destroy(&versionListMux) != 0)
    cerr << "Mutex destruction failed\n";
  if(pthread_mutex_destroy(&cyBufferMux) != 0)
    cerr << "Mutex destruction failed\n";
  if(pthread_cond_destroy(&nonEmptyCond) != 0)
    cerr << "Condition variable destruction failed\n";
  if(pthread_cond_destroy(&nonFullCond) != 0)
    cerr << "Condition variable destruction failed\n";
  return 0;
}

void *pthreadRoutine(void *arg)
{
  pthreadStruct *pthread_arg = (pthreadStruct *)arg;
  int hostPort = pthread_arg->hostPort;
  string hostIP = pthread_arg->hostIP;
  string remotePath = "./";
  remotePath += hostIP;
  remotePath += "_";
  remotePath += to_string(hostPort);
  const char * remotePathC1 = remotePath.c_str();
  if(mkdir(remotePathC1, 0777) == -1)
  {
    if(errno != EEXIST)//skip already created by other calls as not fatal
    {
      cerr << "ERROR:: remote client directory could not be created!\n";//Report error
      cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
      delete clientList;
      delete cbp;
      delete[] threadArray;
      delete pthreadArgs;
      delete clientVersList;
      delete threadVersList;
      sendLOG_OFF();
      exit(errno);
    }
  }
  remotePath.~string();
  while(1)
  {
    //cout << "consumer thread inside loop: " << pthread_self() << endl;
    Item * temp;
    pthread_mutex_lock(&cyBufferMux);
    while(cbp->getCurItems() <= 0)
    {
      cout << pthread_self() << " thread found buffer empty...\n";
      pthread_cond_wait(&nonEmptyCond ,&cyBufferMux);
    }
    temp = cbp->getItem();//get item and data then remove it
    string itemPathname = temp->getPath();
    int itemVersion = temp->getVersion();
    string itemAddrIP = temp->getIP();
    const char * itemAddrIPc = itemAddrIP.c_str();
    int itemPortNum = temp->getPort();
    cbp->removeItem();//returns bool
    pthread_mutex_unlock(&cyBufferMux);
    pthread_cond_signal(&nonFullCond);
    if(itemPathname.compare("") == 0 && itemVersion == -2)
    {
      if(pthread_mutex_lock(&clientListMux) != 0)//lock
      {
        cerr << "Mutex lock failed\n";//Get some info as to why
        delete clientList;
        delete cbp;
        delete[] threadArray;
        delete pthreadArgs;
        delete clientVersList;
        delete threadVersList;
        sendLOG_OFF();
        exit(errno);
      }
      bool result = clientList->findClient(itemAddrIP, itemPortNum);
      if(pthread_mutex_unlock(&clientListMux) != 0)//unlock
      {
        cerr << "Mutex unlock failed\n";//Get some info as to why
        delete clientList;
        delete cbp;
        delete[] threadArray;
        delete pthreadArgs;
        delete clientVersList;
        delete threadVersList;
        sendLOG_OFF();
        exit(errno);
      }
      cout << pthread_self() << " thread found client tuple item...\n";
      if(result == true)
      {
        cout << pthread_self() << " thread connecting to P2P remote client\n"; 
        struct sockaddr_in remoteClient;
        int remoteSockFd = 0;
        if((remoteSockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
          cerr << "socket function 753\n";
          cerr << "Error code: " << strerror(errno) << endl;//Get some info
          continue;
        }
        remoteClient.sin_family = AF_INET;
        remoteClient.sin_port = htons(itemPortNum); 
        int convert = inet_pton(AF_INET, itemAddrIPc, &remoteClient.sin_addr);
        if(convert == -1)
        {
          cerr << "inet_pton function 765, no af support\n";
          cerr << "Error code: " << strerror(errno) << endl;//Get some info
          continue;
        }
        else if(convert == 0)
        {
          cerr << "inet_pton function 765, address string is not valid!\n";
          continue;
        }
        while(1)
        {
          if(connect(remoteSockFd, (struct sockaddr *)&remoteClient,
           sizeof(remoteClient)) < 0)
          {
            cout << pthread_self() << " thread client not yet ready...\n";
            usleep(200000);
          }
          else
            break;
        }
        
        const char * rmtMsg = "GET_FILE_LIST";
        cout << pthread_self() << " thread sending GET_FILE_LIST msg...\n";
        string remotePath = "./";
        remotePath += hostIP;
        remotePath += "_";
        remotePath += to_string(hostPort);
        remotePath += "/";
        remotePath += itemAddrIP;
        remotePath += "_";
        remotePath += to_string(itemPortNum);
        const char * remotePathC = remotePath.c_str();
        if(mkdir(remotePathC, 0777) == -1)
        {
          if(errno != EEXIST)//skip already created by other calls as not fatal
          {
            cerr << "ERROR:: remote client directory could not be created!\n";//Report error
            cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
            delete clientList;
            delete cbp;
            delete[] threadArray;
            delete pthreadArgs;
            delete clientVersList;
            delete threadVersList;
            sendLOG_OFF();
            exit(errno);
          }
        }
        if(write(remoteSockFd, rmtMsg, strlen(rmtMsg)) == -1)
        {
          cerr << "Write function 791\n";
          cerr << "Error code: " << strerror(errno) << endl;//Get some info
          continue;
        }
        
        string fileListS = "";
        while(1)
        {
          char fileListBuffer[1024+1] = "";
          if(read(remoteSockFd, fileListBuffer, 1024) == 0)
            break;
          fileListS += fileListBuffer;
        }
        int pos = fileListS.find_first_of(" ");
        fileListS = fileListS.substr(pos+1);
        pos = fileListS.find_first_of(" ");
        string totalS = fileListS.substr(0, pos);
        int totalFiles1 = stoi(totalS, nullptr, 10);
        fileListS = fileListS.substr(pos+1);
        for(int i = 0; i < totalFiles1-1; i++)
        {
          int pos = fileListS.find_first_of(" ");
          string tempS = fileListS.substr(0, pos+1);
          fileListS = fileListS.substr(pos+1);
          pos = fileListS.find_first_of(" ");
          string tempVs = fileListS.substr(0, pos);
          int tempVers = stoi(tempVs, nullptr, 10);
          fileListS = fileListS.substr(pos+1);
          int pos2 = tempS.find_first_of("/");
          tempS = tempS.substr(pos2+1);
          pos2 = tempS.find_first_of("/");
          tempS = tempS.substr(pos2+1);
          tempS = tempS.substr(0,tempS.length()-1);
          pthread_mutex_lock(&cyBufferMux);
          while(cbp->getCurItems() >= bufferSize)
          {
            cout << pthread_self() << " thread found buffer full...\n";
            pthread_cond_wait(&nonFullCond, &cyBufferMux) ;
          }
          bool result = cbp->addItem(tempS, tempVers, itemAddrIP, itemPortNum);
          if(result == true)
          {
            cout << "added item successfully in 3rd add 1252 line\n";
            if(pthread_mutex_lock(&versionListMux) != 0)//lock
            {
              cerr << "Mutex lock failed\n";//Get some info as to why
              delete clientList;
              delete cbp;
              delete[] threadArray;
              delete pthreadArgs;
              delete clientVersList;
              delete threadVersList;
              sendLOG_OFF();
              exit(errno);
            }
            threadVersList->addNode(tempS, tempVers);
            if(pthread_mutex_unlock(&versionListMux) != 0)//unlock
            {
              cerr << "Mutex unlock failed\n";//Get some info as to why
              delete clientList;
              delete cbp;
              delete[] threadArray;
              delete pthreadArgs;
              delete clientVersList;
              delete threadVersList;
              sendLOG_OFF();
              exit(errno);
            }
          }
          else
            cout << "buffer was full in 3rd add 1252 line\n";
          pthread_mutex_unlock(&cyBufferMux);
          pthread_cond_signal(&nonEmptyCond);
        }
        pos = fileListS.find_first_of(" ");
        string tempS = fileListS.substr(0, pos+1);
        int pos2 = tempS.find_first_of("/");
        tempS = tempS.substr(pos2+1);
        pos2 = tempS.find_first_of("/");
        tempS = tempS.substr(pos2+1);
        tempS = tempS.substr(0,tempS.length()-1);
        fileListS = fileListS.substr(pos+1);
        int tempVers = stoi(fileListS, nullptr, 10);
        pthread_mutex_lock(&cyBufferMux);
        while(cbp->getCurItems() >= bufferSize)
        {
          cout << pthread_self() << " thread found buffer full...\n";
          pthread_cond_wait(&nonFullCond, &cyBufferMux) ;
        }
        bool result = cbp->addItem(tempS, tempVers, itemAddrIP, itemPortNum);//add the final item
        if(result == true)
            cout << "added item successfully in 4th add 1292 line\n";
          else
            cout << "buffer was full in 4th add 1292 line\n";
        if(pthread_mutex_lock(&versionListMux) != 0)//lock
        {
          cerr << "Mutex lock failed\n";//Get some info as to why
          delete clientList;
          delete cbp;
          delete[] threadArray;
          delete pthreadArgs;
          delete clientVersList;
          delete threadVersList;
          sendLOG_OFF();
          exit(errno);
        }
        threadVersList->addNode(tempS, tempVers);
        if(pthread_mutex_unlock(&versionListMux) != 0)//unlock
        {
          cerr << "Mutex unlock failed\n";//Get some info as to why
          delete clientList;
          delete cbp;
          delete[] threadArray;
          delete pthreadArgs;
          delete clientVersList;
          delete threadVersList;
          sendLOG_OFF();
          exit(errno);
        }
        // create the remote client directory
        pthread_mutex_unlock(&cyBufferMux);
        pthread_cond_signal(&nonEmptyCond);
      }
      else
      {
        cout << pthread_self() << " thread can't reach remote P2P client...\n";
      }
    }
    else
    {
      cout << pthread_self() << " thread found file tuple item...\n";
      if(pthread_mutex_lock(&clientListMux) != 0)//lock
      {
        cerr << "Mutex lock failed\n";//Get some info as to why
        delete clientList;
        delete cbp;
        delete[] threadArray;
        delete pthreadArgs;
        delete clientVersList;
        delete threadVersList;
        sendLOG_OFF();
        exit(errno);
      }
      bool result = clientList->findClient(itemAddrIP, itemPortNum);
      if(pthread_mutex_unlock(&clientListMux) != 0)//unlock
      {
        cerr << "Mutex unlock failed\n";//Get some info as to why
        delete clientList;
        delete cbp;
        delete[] threadArray;
        delete pthreadArgs;
        delete clientVersList;
        delete threadVersList;
        sendLOG_OFF();
        exit(errno);
      }
      if(result == true)
      {
        cout << pthread_self() << " thread checking if file exists locally...\n";
        string remotePath = "./";
        remotePath += hostIP;
        remotePath += "_";
        remotePath += to_string(hostPort);
        remotePath += "/";
        remotePath += itemAddrIP;
        remotePath += "_";
        remotePath += to_string(itemPortNum);
        remotePath += "/";
        remotePath += itemPathname;
        cout << pthread_self() << " thread searching for item : ";
        cout << remotePath << endl;
        const char * targetPath = remotePath.c_str();
        int fileFd = open(targetPath, O_RDONLY, 0777);
        if(fileFd == -1)
        {
          cout << pthread_self() << " thread item not found...\n";
          cout << pthread_self() << " thread going to connect to remote client...\n";
          struct sockaddr_in remoteClient;
          int remoteSockFd = 0;
          if((remoteSockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
          {
            cerr << "socket function 753\n";
            cerr << "Error code: " << strerror(errno) << endl;//Get some info
            continue;
          }
          remoteClient.sin_family = AF_INET;
          remoteClient.sin_port = htons(itemPortNum); 
          int convert = inet_pton(AF_INET, itemAddrIPc, &remoteClient.sin_addr);
          if(convert == -1)
          {
            cerr << "inet_pton function 765, no af support\n";
            cerr << "Error code: " << strerror(errno) << endl;//Get some info
            continue;
          }
          else if(convert == 0)
          {
            cerr << "inet_pton function 765, address string is not valid!\n";
            continue;
          }
          while(1)
          {
            if(connect(remoteSockFd, (struct sockaddr *)&remoteClient,
             sizeof(remoteClient)) < 0)
            {
              cout << pthread_self() << " thread client not yet ready...\n";
              usleep(200000);
            }
            else
              break;
          }
          string rmtMsgS = "GET_FILE";
          rmtMsgS += " ";
          rmtMsgS += itemPathname;
          rmtMsgS += " ";
          rmtMsgS += to_string(-1);
          cout << pthread_self() << " thread sending GET_FILE msg\n";
          const char * rmtMsg = rmtMsgS.c_str();
          if(write(remoteSockFd, rmtMsg, strlen(rmtMsg)) == -1)
          {
            cerr << "Write function 791\n";
            cerr << "Error code: " << strerror(errno) << endl;//Get some info
            continue;
          }
          char headerBuffer[256+1] = "";
          if(read(remoteSockFd, headerBuffer, 256) == -1)
          {
            cerr << "read function 802\n";
            cerr << "Error code: " << strerror(errno) << endl;//Get some info
            continue;
          }
          string headerBufferS = headerBuffer;
          if(headerBufferS.compare("FILE_NOT_FOUND") == 0)//file not found
            cout << pthread_self() << " thread requested file wasn't found...\n";
          else if(headerBufferS.compare("FILE_UP_TO_DATE") == 0)//file is updated
             cout << pthread_self() << " thread requested file is up to date...\n";
          else//file is out of date, receive it from client
          {
            cout << pthread_self() << " thread requested file doesnt exist...\n";
            cout << pthread_self() << " thread acquiring it...\n";
            int pos = headerBufferS.find_first_of(" ");
            headerBufferS = headerBufferS.substr(pos+1);
            pos = headerBufferS.find_first_of(" ");
            string newVersionS = headerBufferS.substr(0, pos);
            int newVersion = stoi(newVersionS, nullptr, 10);
            headerBufferS = headerBufferS.substr(pos+1);
            int newSize = stoi(headerBufferS, nullptr, 10);
            cout << pthread_self() << " thread receiving file data...\n";
            if(newSize == 0)//its a directory
            {
              string parentPath = "./";
              parentPath += hostIP;
              parentPath += "_";
              parentPath += to_string(hostPort);
              parentPath += "/";
              parentPath += itemAddrIP;
              parentPath += "_";
              parentPath += to_string(itemPortNum);
              const char * itemPath = itemPathname.c_str();
              const char * parentPathC = parentPath.c_str();
              cout << pthread_self() << " thread creating directory tree...\n";
              makeDirTree(itemPath, parentPathC);
            }
            else//its a file
            {
              string parentPath = "./";
              parentPath += hostIP;
              parentPath += "_";
              parentPath += to_string(hostPort);
              parentPath += "/";
              parentPath += itemAddrIP;
              parentPath += "_";
              parentPath += to_string(itemPortNum);
              int pos = itemPathname.find_last_of("/");
              string justPath = itemPathname.substr(0, pos);
              const char * justPathC = justPath.c_str();
              const char * parentPathC = parentPath.c_str();
              cout << pthread_self() << " thread creating directory tree...\n";
              if(pos != -1)
                makeDirTree(justPathC, parentPathC);
              int bytesToR = newSize;
              char fileBuffer[1024+1] = "";
              int nread;
              string fileDataCnt = "";
              while(bytesToR > 1024)
              {
                if((nread = read(remoteSockFd, fileBuffer, bufferSize)) == -1)
                {
                  cerr << "ERROR:: Receiver could not read from socket!\n";//Report error
                  cerr << "Error code: " << strerror(errno) << endl;//Get some info
                  continue;
                }
                //readjust based on bytes read until all the string is parsed
                bytesToR -= nread;
                string temp2 = fileBuffer;
                fileDataCnt += temp2;
              }
              if(read(remoteSockFd, fileBuffer, bytesToR) == -1)
              {
                cerr << "ERROR:: Receiver could not read from socket!\n";//Report error
                cerr << "Error code: " << strerror(errno) << endl;//Get some info
                continue;
              }
              string temp2 = fileBuffer;
              fileDataCnt += temp2;
              const char * fileDataCntC = fileDataCnt.c_str();
              cout << "file below:\n\n" << fileDataCntC << "\n\n";
              //cout << "opening file to create:<" << targetPath << ">\n";
              int nf = open(targetPath, O_WRONLY | O_CREAT | O_TRUNC, 0777);
              if(nf == -1)
              {
                cerr << "ERROR:: Could not open the new file!\n";//Report error
                cerr << "Error code: " << strerror(errno) << endl;//Get some info
                continue;
              }
              cout << pthread_self() << " thread creating file: ";
              cout << targetPath << endl; 
              if(write(nf, fileDataCntC, newSize) == -1)
              {
                cerr << "ERROR::Could not write to the file!\n";//Report error
                cerr << "Error code: " << strerror(errno) << endl;//Get some info
                continue;
              }
              if(close(nf) == -1)//close the file
              {
                cerr << "ERROR:: Could not close the file!\n";//Report error
                cerr << "Error code: " << strerror(errno) << endl;//Get some info
                continue;
              }
            }
            if(pthread_mutex_lock(&versionListMux) != 0)//lock
            {
              cerr << "Mutex lock failed\n";//Get some info as to why
              delete clientList;
              delete cbp;
              delete[] threadArray;
              delete pthreadArgs;
              delete clientVersList;
              delete threadVersList;
              sendLOG_OFF();
              exit(errno);
            }
            threadVersList->addNode(itemPathname, newVersion);
            if(pthread_mutex_unlock(&versionListMux) != 0)//unlock
            {
              cerr << "Mutex unlock failed\n";//Get some info as to why
              delete clientList;
              delete cbp;
              delete[] threadArray;
              delete pthreadArgs;
              delete clientVersList;
              delete threadVersList;
              sendLOG_OFF();
              exit(errno);
            }
          }
        }
        else
        {
          cout << pthread_self() << " thread item exists in client folder...\n";
          close(fileFd);
          if(pthread_mutex_lock(&versionListMux) != 0)//lock
          {
            cerr << "Mutex lock failed\n";//Get some info as to why
            delete clientList;
            delete cbp;
            delete[] threadArray;
            delete pthreadArgs;
            delete clientVersList;
            delete threadVersList;
            sendLOG_OFF();
            exit(errno);
          }
          int localVersion = threadVersList->getVersion(itemPathname);
          if(pthread_mutex_unlock(&versionListMux) != 0)//unlock
          {
            cerr << "Mutex unlock failed\n";//Get some info as to why
            delete clientList;
            delete cbp;
            delete[] threadArray;
            delete pthreadArgs;
            delete clientVersList;
            delete threadVersList;
            sendLOG_OFF();
            exit(errno);
          }
          if(localVersion == itemVersion)//item is up to date
            cout << pthread_self() << " thread item is up to date...\n";
          else
          {
            cout << pthread_self() << " thread item is outdated...\n";
            cout << pthread_self() << " thread going to remove old path\n";
            removeDirTree(targetPath);
            cout << pthread_self() << " thread connecting to remote P2P client...\n";
            struct sockaddr_in remoteClient;
            int remoteSockFd = 0;
            if((remoteSockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            {
              cerr << "socket function 753\n";
              cerr << "Error code: " << strerror(errno) << endl;//Get some info
              continue;
            }
            remoteClient.sin_family = AF_INET;
            remoteClient.sin_port = htons(itemPortNum); 
            int convert = inet_pton(AF_INET, itemAddrIPc, &remoteClient.sin_addr);
            if(convert == -1)
            {
              cerr << "inet_pton function 765, no af support\n";
              cerr << "Error code: " << strerror(errno) << endl;//Get some info
              continue;
            }
            else if(convert == 0)
            {
              cerr << "inet_pton function 765, address string is not valid!\n";
              continue;
            }
            while(1)
            {
              if(connect(remoteSockFd, (struct sockaddr *)&remoteClient,
               sizeof(remoteClient)) < 0)
              {
                cout << pthread_self() << " thread client not yet ready...\n";
                usleep(200000);
              }
              else
                break;
            }
            string rmtMsgS = "GET_FILE";
            rmtMsgS += " ";
            rmtMsgS += itemPathname;
            rmtMsgS += " ";
            rmtMsgS += to_string(-1);
            cout << pthread_self() << " thread sending GET_FILE msg...\n";
            const char * rmtMsg = rmtMsgS.c_str();
            if(write(remoteSockFd, rmtMsg, strlen(rmtMsg)) == -1)
            {
              cerr << "Write function 791\n";
              cerr << "Error code: " << strerror(errno) << endl;//Get some info
              continue;
            }
            char headerBuffer[256+1] = "";
            if(read(remoteSockFd, headerBuffer, 256) == -1)
            {
              cerr << "read function 802\n";
              cerr << "Error code: " << strerror(errno) << endl;//Get some info
              continue;
            }
            string headerBufferS = headerBuffer;
            if(headerBufferS.compare("FILE_NOT_FOUND") == 0)//file not found
              cout << pthread_self() << " thread requested file wasn't found...\n";
            else if(headerBufferS.compare("FILE_UP_TO_DATE") == 0)//file is updated
               cout << pthread_self() << " thread requested file is up to date...\n";
            else//file is out of date, receive it from client
            {
              int pos = headerBufferS.find_first_of(" ");
              headerBufferS = headerBufferS.substr(pos+1);
              pos = headerBufferS.find_first_of(" ");
              string newVersionS = headerBufferS.substr(0, pos);
              int newVersion = stoi(newVersionS, nullptr, 10);
              headerBufferS = headerBufferS.substr(pos+1);
              int newSize = stoi(headerBufferS, nullptr, 10);
              cout << pthread_self() << " thread receiving file data...\n";
              if(newSize == 0)//its a directory
              {
                string parentPath = "./";
                parentPath += hostIP;
                parentPath += "_";
                parentPath += to_string(hostPort);
                parentPath += "/";
                parentPath += itemAddrIP;
                parentPath += "_";
                parentPath += to_string(itemPortNum);
                const char * itemPath = itemPathname.c_str();
                const char * parentPathC = parentPath.c_str();
                cout << pthread_self() << " thread creating directory tree...\n";
                makeDirTree(itemPath, parentPathC);
              }
              else//its a file
              {
                string parentPath = "./";
                parentPath += hostIP;
                parentPath += "_";
                parentPath += to_string(hostPort);
                parentPath += "/";
                parentPath += itemAddrIP;
                parentPath += "_";
                parentPath += to_string(itemPortNum);
                int pos = itemPathname.find_last_of("/");
                string justPath = itemPathname.substr(0, pos);
                const char * justPathC = justPath.c_str();
                const char * parentPathC = parentPath.c_str();
                cout << pthread_self() << " thread creating directory tree...\n";
                if(pos != -1)
                  makeDirTree(justPathC, parentPathC);
                int bytesToR = newSize;
                char fileBuffer[1024+1] = "";
                int nread;
                string fileDataCnt;
                while(bytesToR > 1024)
                {
                  if((nread = read(remoteSockFd, fileBuffer, bufferSize)) == -1)
                  {
                    cerr << "ERROR:: Receiver could not read from socket!\n";//Report error
                    cerr << "Error code: " << strerror(errno) << endl;//Get some info
                    continue;
                  }
                  //readjust based on bytes read until all the string is parsed
                  bytesToR -= nread;
                  string temp2 = fileBuffer;
                  fileDataCnt += temp2;
                }
                if(read(remoteSockFd, fileBuffer, bytesToR) == -1)
                {
                  cerr << "ERROR:: Receiver could not read from socket!\n";//Report error
                  cerr << "Error code: " << strerror(errno) << endl;//Get some info
                  continue;
                }
                string temp2 = fileBuffer;
                fileDataCnt += temp2;
                const char * fileDataCntC = fileDataCnt.c_str();
                //cout << "opening file to create:<" << targetPath << ">\n";
                int nf = open(targetPath, O_WRONLY | O_CREAT | O_TRUNC, 0777);
                if(nf == -1)
                {
                  cerr << "ERROR:: Could not open the new file!\n";//Report error
                  cerr << "Error code: " << strerror(errno) << endl;//Get some info
                  continue;
                }
                cout << pthread_self() << " thread creating file: ";
                cout << targetPath << endl; 
                if(write(nf, fileDataCntC, newSize) == -1)
                {
                  cerr << "ERROR::Could not write to the file!\n";//Report error
                  cerr << "Error code: " << strerror(errno) << endl;//Get some info
                  continue;
                }
                if(close(nf) == -1)//close the file
                {
                  cerr << "ERROR:: Could not close the file!\n";//Report error
                  cerr << "Error code: " << strerror(errno) << endl;//Get some info
                  continue;
                }
              }
              if(pthread_mutex_lock(&versionListMux) != 0)//lock
              {
                cerr << "Mutex lock failed\n";//Get some info as to why
                delete clientList;
                delete cbp;
                delete[] threadArray;
                delete pthreadArgs;
                delete clientVersList;
                delete threadVersList;
                sendLOG_OFF();
                exit(errno);
              }
              threadVersList->addNode(itemPathname, newVersion);
              if(pthread_mutex_unlock(&versionListMux) != 0)//unlock
              {
                cerr << "Mutex unlock failed\n";//Get some info as to why
                delete clientList;
                delete cbp;
                delete[] threadArray;
                delete pthreadArgs;
                delete clientVersList;
                delete threadVersList;
                sendLOG_OFF();
                exit(errno);
              }
            }
          }
        }
      }
      else
      {
        cout << pthread_self() << " thread can no longer reach ";
        cout << "remote P2P client...\n";
      }
    }
  }
  pthread_exit(NULL);
}