#include "Helpers.hpp"
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/time.h>
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

using namespace std;

static int * argPos;
static List * clientList;
static List1 * fileList;
static const char * mirPath;
static const char * comPath;
static int totalTries = 0;
static int slaveShmId;
static int * slaveShmPtr;
static string tempG;

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
    delete[] argPos;
    delete clientList;
    delete fileList;
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
    delete[] argPos;
    delete clientList;
    delete fileList;
    exit(errno);
  }
  remove(dirName);
}

static string listDir(char * dirName, int & total)
{ //returns a concatenated string consisting of only files inside dirName path
  DIR * d;
  string creation = "";//empty at first
  d = opendir(dirName);
  if(!d)
  {
    cerr << "ERROR:: directory could not be opened!\n";//Report error
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    delete[] argPos;
    delete clientList;
    delete fileList;
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
    if(!(entry->d_type & DT_DIR) && !(entry->d_type & DT_FIFO))//skip dirs and pipes
    {
      total++;
      string temp = dirName;
      creation += temp;
      creation += "/";
      creation += entryName;
      creation += "*"; // used as a delimiter
    }
  }
  //After going through all the entries, close the directory
  if(closedir(d))
  {
    cerr << "ERROR:: directory could not be closed!\n";//Report error
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    delete[] argPos;
    delete clientList;
    delete fileList;
    exit(errno);
  }
  return creation;
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
    delete[] argPos;
    delete clientList;
    delete fileList;
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
    total++;
    if(strcmp(entryName, "..") != 0 && strcmp(entryName, ".") != 0)
    {
      string temp = dirName;
      creation += temp;
      creation += "/";
      creation += entryName;
      creation += "*"; // used as a delimiter
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
          delete[] argPos;
          delete clientList;
          delete fileList;
          if(closedir(d))
          {
            cerr << "ERROR:: directory could not be closed!\n";//Report error
            cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
          }
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
    delete[] argPos;
    delete clientList;
    delete fileList;
    exit(errno);
  }
  return creation;
}

static bool checkFifos(char * dirName, const char * target)
{ //searches for already created pipes @ dirName path
  DIR * d;
  d = opendir(dirName);
  string s2 = target;
  bool result = false;
  if(!d)
  {
    cerr << "ERROR:: directory could not be opened!\n";//Report error
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    delete[] argPos;
    delete clientList;
    delete fileList;
    exit(errno);
  }
  while(1)
  {
    struct dirent * entry;
    char * entryName;
    entry = readdir(d);
    if(!entry)//There are no more entries in this directory so break
      break;
    entryName = entry->d_name;
    string s1 = entryName;
    if(entry->d_type & DT_FIFO)//check only named fifo pipes
    {
      if(s2.compare(s1) == 0)//found it
      {
        result = true;
        break;
      }
    }
  }
  if(closedir(d))
  {
    cerr << "ERROR:: directory could not be closed!\n";//Report error
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    delete[] argPos;
    delete clientList;
    delete fileList;
    exit(errno);
  }
  return result;
}

static void makeDirTree(const char * path, const char * parentPath)
{ //this function creates the directory tree identical to the new client
  //before files are created @ the corresponding paths
  string fullPath = path;
  fullPath = fullPath.substr(2);
  int firstSlashPos = fullPath.find_first_of("/");
  string basePath = fullPath.substr(firstSlashPos+1);
  int secondSlashPos = basePath.find_first_of("/");
  basePath = basePath.substr(secondSlashPos+1);
  int length = basePath.length();
  int counter = 0;
  int totalSlash = 0;
  while(counter < length)//count the subpaths needed
  {
    if(basePath[counter] == '/')
      totalSlash++;
    counter++;
  }
  int created = -1;
  if(totalSlash == 0)//create it
  {
    const char * fullPathC = fullPath.c_str();
    if(mkdir(fullPathC, 0777) == -1)
    {
      if(errno != EEXIST)//skip already created by other calls as not fatal
      {
        cerr << "ERROR:: directory target could not be created!\n";//Report error
        cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
        delete[] argPos;
        delete clientList;
        delete fileList;
        exit(errno);
      }
    }
  }
  else
  {
    string parentPathS = parentPath;
    while(created < totalSlash)
    {
      int nextSlashPos = basePath.find_first_of("/");
      string temp = basePath.substr(0, nextSlashPos);
      basePath = basePath.substr(nextSlashPos+1);
      string toBCreated = parentPathS;
      toBCreated += temp;
      toBCreated += "/";
      const char * tempC = toBCreated.c_str();
      cout << "creating directory:" << tempC << endl;
      if(mkdir(tempC, 0777) == -1)
      {
        if(errno != EEXIST)//skip already created by other calls as not fatal
        {
          cerr << "ERROR:: directory target could not be created!\n";//Report error
          cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
          delete[] argPos;
          delete clientList;
          delete fileList;
          exit(errno);
        }
      }
      parentPathS += temp;
      parentPathS += "/";
      created++;
    }
  }
}

static void signalHandler(int signum)
{ //this function handles the signals SIGQUIT and SIGINT
  cout << "\nInterrupt signal (" << signum << ") received.\n";
  // cleanup and close up stuff here
  delete[] argPos;
  delete clientList;
  delete fileList;
  if(remove(comPath) == -1)//remove the id file
  {
    cerr << "ERROR:: .id file could not be destroyed!\n";//Report error
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
  }
  removeDirTree(mirPath);//remove the mirror directory
  exit(signum);
}

static void signalHandler1(int signum)
{ //this function handles the signal SIGUSR1
  cout << "\nInterrupt signal (" << signum << ") received.\n";
  if(signum == SIGUSR1)
  {
    cout << "Received SIGUSR1, problem encountered during file synch!\n";
    totalTries++;
    cout << "Current totalTries = " << totalTries << endl;
    kill(slaveShmPtr[1], SIGKILL );//kill the other child
    clientList->delNode(tempG);//remove the node for next loop
  }
}

static void signalHandler2(int signum)
{ //this function handles the signal SIGUSR2
  cout << "\nInterrupt signal (" << signum << ") received.\n";
  if(signum == SIGUSR2)
  {
    cout << "Received SIGUSR2 , data transfer finished successfully!\n";
    totalTries = 0;
  }
}

int main(int argc, char ** argv)
{
  if(argc != 13)
  {
    cerr << "ERROR:: Invalid arguments!\n";
    return 1;
  }
  int argCount = 0;
  argPos = new int[6];
  for(int i = 0; i < argc; i++)//get mixed order arguments but all are needed
  {
    if(strcmp(argv[i], "-n") == 0)
    {
      argPos[0] = i+1;
      argCount++;
    }
    if(strcmp(argv[i], "-c") == 0)
    {
      argPos[1] = i+1;
      argCount++;
    }
    if(strcmp(argv[i], "-i") == 0)
    {
      argPos[2] = i+1;
      argCount++;
    }
    if(strcmp(argv[i], "-m") == 0)
    {
      argPos[3] = i+1;
      argCount++;
    }
    if(strcmp(argv[i], "-b") == 0)
    {
      argPos[4] = i+1;
      argCount++;
    }
    if(strcmp(argv[i], "-l") == 0)
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
  if(isNumber(argv[argPos[0]]) == true)
    argCount++;
  if(isNumber(argv[argPos[4]]) == true)
    argCount++;
  if(argCount != 2)
  {
    cerr << "ERROR:: Invalid arguments!\n";
    delete[] argPos;
    return 1;
  }
  int clientID = stoi(argv[argPos[0]], nullptr, 10);

  int bufferSize = stoi(argv[argPos[4]], nullptr, 10);
  char * inputDir = argv[argPos[2]];
  char * commonDir = argv[argPos[1]];
  char * mirrorDir = argv[argPos[3]];
  mirPath = mirrorDir;
  char * logFile = argv[argPos[5]];
  if(bufferSize < 10)
  {
    cerr << "ERROR:: Buffer size must be over 10 bytes!\n";
    delete[] argPos;
    return 1;
  }
  cout << "client id : " << clientID << endl;
  cout << "buffer size : " << bufferSize << endl;
  cout << "input directory : " << inputDir << endl;
  cout << "common directory : " << commonDir << endl;
  cout << "mirror directory : " << mirrorDir << endl;
  cout << "log file : " << logFile << endl;
  DIR * inputDirPtr;
  if((inputDirPtr = opendir(inputDir)) == NULL)
  {
    cerr << "ERROR:: Input directory could not be opened!\n";//Report error
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    delete[] argPos;
    return 2;
  }
  DIR * mirrorDirPtr1;
  if((mirrorDirPtr1 = opendir(mirrorDir)) != NULL)
  {
    cerr << "ERROR:: Mirror directory already exists!\n";//Report error
    delete[] argPos;
    closedir(mirrorDirPtr1);//close the mirror directory
    cerr << "mirror directory closed as it already exists!\n";
    closedir(inputDirPtr);//close the input directory
    cerr << "input directory closed since mirror directory already exists!\n";
    return 2;
  }
  closedir(mirrorDirPtr1);
  // create the common directory (if it doesnt already exist)
  DIR * commonDirPtr;
  if((commonDirPtr = opendir(commonDir)) == NULL)
  {
    if(mkdir(commonDir, 0777) == -1)
    {
      cerr << "ERROR:: common directory could not be created!\n";//Report error
      cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
      closedir(inputDirPtr);//close the input directory
      delete[] argPos;
      return 3;
    }
  }
  // create the mirror directory
  if(mkdir(mirrorDir, 0777) == -1)
  {
    cerr << "ERROR:: mirror directory could not be created!\n";//Report error
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    closedir(inputDirPtr);//close the input directory
    closedir(commonDirPtr);//close the common directory
    delete[] argPos;
    return 3;
  }
  DIR * mirrorDirPtr;
  if((mirrorDirPtr = opendir(commonDir)) == NULL)
  {
    cerr << "ERROR:: Mirror directory could not be opened!\n";//Report error
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    closedir(inputDirPtr);//close the input directory
    closedir(commonDirPtr);//close the common directory
    delete[] argPos;
    return 2;
  }
  string fileNam = commonDir;
  string sclientID = argv[argPos[0]];
  fileNam += "/";
  fileNam += sclientID;
  fileNam += ".id";
  const char * fileName = fileNam.c_str();//convert it to char * to create the file
  comPath = fileName;
  FILE *f = fopen(fileName, "r"); 
  if(f != NULL)
  { // file exists error
    cerr << "ERROR:: File has already been created by another client!\n";
    closedir(inputDirPtr);//close the input directory
    closedir(commonDirPtr);//close the common directory
    closedir(mirrorDirPtr);//close the mirror directory
    if(fclose(f) != 0)
    {
      cerr << "ERROR:: Could not close the file!\n";//Report error
      cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    }
    delete[] argPos;
    return 4;
  }
  int fd;//create the file
  if((fd = open(fileName, O_CREAT | O_RDWR, 0777)) == -1)
  {
    cerr << "ERROR:: File could not be created!\n";//Report error
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    closedir(inputDirPtr);//close the input directory
    closedir(commonDirPtr);//close the common directory
    closedir(mirrorDirPtr);//close the mirror directory
    delete[] argPos;
    return 5;
  }
  string sPid = to_string(getpid());
  const char * cPid = sPid.c_str();
  if((write(fd, cPid, strlen(cPid)) == -1))
  {
    cerr << "ERROR:: Could not write to the file!\n";//Report error
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    closedir(inputDirPtr);//close the input directory
    closedir(commonDirPtr);//close the common directory
    closedir(mirrorDirPtr);//close the mirror directory
    if(close(fd) == -1)
    {
      cerr << "ERROR:: Could not close the file!\n";//Report error
      cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    }
    delete[] argPos;
    return 6;
  }
  if(close(fd) == -1)//close the file
  {
    cerr << "ERROR:: Could not close the file!\n";//Report error
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    closedir(inputDirPtr);//close the input directory
    closedir(commonDirPtr);//close the common directory
    closedir(mirrorDirPtr);//close the mirror directory
    delete[] argPos;
    return 8;
  }
  int fd2;//create the logfile
  if((fd2 = open(logFile, O_CREAT | O_RDWR | O_TRUNC, 0777)) == -1)
  {
    cerr << "ERROR:: File could not be created!\n";//Report error
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    closedir(inputDirPtr);//close the input directory
    closedir(commonDirPtr);//close the common directory
    closedir(mirrorDirPtr);//close the mirror directory
    delete[] argPos;
    return 7;
  }
  if(close(fd2) == -1)//close the file
  {
    cerr << "ERROR:: Could not close the file!\n";//Report error
    cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
    closedir(inputDirPtr);//close the input directory
    closedir(commonDirPtr);//close the common directory
    closedir(mirrorDirPtr);//close the mirror directory
    delete[] argPos;
    return 9;
  }
  clientList = new List(clientID);//create a list with new clients .id files
  clientList->addNode(fileName);
  fileList = new List1(clientID);//create another list to handle leaving clients 
  closedir(inputDirPtr);//close the input directory
  closedir(commonDirPtr);//close the common directory
  closedir(mirrorDirPtr);//close the mirror directory
  // register signal SIGINT,SIGQUIT and signal handler  
  signal(SIGINT, signalHandler);
  signal(SIGQUIT, signalHandler);
  //create shared memory area to distinguish between the 2 processes
  slaveShmId = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0666);
  if(slaveShmId < 0)
  {
      perror("shmget");
      exit(1);
  }
  slaveShmPtr = (int *) shmat(slaveShmId, NULL, 0);
  if((int ) *slaveShmPtr == -1)
  {
      perror("shmat");
      exit(1);
  }
  char pipeBuffer[bufferSize+1]={0};//init to avoid trash
  char packetHeader[1024+1]={0};//init to avoid trash
  clock_t t1; 
  clock_t t2; //for time calculating
  const double ratio = 1./CLOCKS_PER_SEC;
  const double maxSession = 30.0;//max wait time
  while(1)
  { 
    //sleep for 1 second period then check
    sleep(1);//is interrupted by signals
    int totalFiles = 0;
    string strResult = listDir(commonDir, totalFiles);//get the file list
    int length = strResult.length();
    for(int i = 0 ; i < totalFiles; i++)
    {
      int pos = 0;
      int counter = 0;
      while(counter < length)
      {
        if(strResult[pos] == '*')//found delimiter
          break;
        else
          pos++;
        counter++;
      }
      tempG = strResult.substr(0, pos);
      strResult = strResult.substr(pos+1);
      length = strResult.length();
      signal(SIGUSR1, signalHandler1);//register the SIGUSR1 signal anything wrong fork again
      if(clientList->searchFiles(tempG) == false)//file not processed yet
      {
        clientList->addNode(tempG);
        fileList->addNode(tempG);
        string newClientID = clientList->extractID(tempG);
        pid_t retpid = 1;// Force Master initially.
        int sCount = 2;//slave processes to be spawned
        int slaves = 0;//starting by 0
        if(totalTries >= 3)
          break;
        while(sCount-- > 0 && retpid > 0)// Only fork if limit not reached AND is Master
        {// Adjust actual sCount if successful.
          if((retpid = fork()) > 0)
          {
            slaveShmPtr[slaves] = retpid;//add pid in array
            slaves++;
          }
        }
        //slaves' code
        if(retpid == 0)
        {
          if(getpid() == slaveShmPtr[0])//slave 1 receives files
          {
            cout << "Receiver slave : " << getpid() << " initiating mirror construction!\n";
            string ffPName = commonDir;
            int lengthD = ffPName.length();
            ffPName += "/";
            ffPName += "id";
            ffPName += newClientID;
            ffPName += "_to_";
            ffPName += sclientID;
            ffPName += ".fifo";
            const char * fifoPipe = ffPName.c_str();
            string toSearchS = ffPName.substr(lengthD+1);
            const char * fifoSearch = toSearchS.c_str();
            bool fResult = checkFifos(commonDir, fifoSearch);
            if(fResult == false)//have to create it
            {
              cout << fifoPipe << ": created by receiver!\n";
              if(mkfifo(fifoPipe, 0777) == -1)
              {
                cerr << "ERROR:: Receiver Could not create the fifo pipe!\n";//Report error
                cerr << "Error code: " << strerror(errno) << endl;//Get some info
                kill(getppid(), SIGUSR1);
                delete[] argPos;
                delete clientList;
                delete fileList;
                exit(errno);
              }
            }
            else
              cout << "pipe already created!\n";
            int pd;
            if((pd = open(fifoPipe, O_RDONLY)) < 0)
            {
              cerr << "ERROR:: Pipe could not be opened by receiver!\n";//Report error
              cerr << "Error code: " << strerror(errno) << endl;//Get some info
              kill(getppid(), SIGUSR1);
              delete[] argPos;
              delete clientList;
              delete fileList;
              exit(errno);
            }
            else
              cout << fifoPipe << " opened successfully by receiver!\n";
            fd_set fds;
            struct timeval tv;
            tv.tv_sec = 0;//wait forever until handled based on time
            tv.tv_usec = 0;
            int ret = 0;
            t1 = clock();//start timer
            double curSession = 0.0;
            while (ret == 0)
            { 
              //sleep(1);
              FD_ZERO(&fds) ;
              FD_SET(pd, &fds);
              ret = select(pd+1, &fds, (fd_set *)0, (fd_set *)0, &tv);
              t2 = clock();
              curSession = ratio*(long)t2 - ratio*(long)t1;
              //cout << "waited so far : " << curSession << endl;
              if(curSession > maxSession)
              {
                cout << "data did not arrive in time!\n";
                kill(getppid(), SIGUSR1);
                delete[] argPos;
                delete clientList;
                delete fileList;
                close(pd);
                exit(errno);
              }
            }
            
            string baseDir = mirrorDir;
            baseDir += "/";
            baseDir += newClientID;
            baseDir += "/";
            const char * baseDirC = baseDir.c_str();
            if(mkdir(baseDirC, 0777) == -1)//create the parent dir for the client
            {
              if(errno != EEXIST)//skip already created by other calls as not fatal
              {
                cerr << "ERROR:: directory target could not be created!\n";//Report error
                cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
                kill(getppid(), SIGUSR1);
                delete[] argPos;
                delete clientList;
                delete fileList;
                close(pd);
                exit(errno);
              }
            }
            int fd3;
            if((fd3 = open(logFile, O_RDWR | O_APPEND, 0777)) == -1)
            {
              cerr << "ERROR:: LogFile could not be opened!\n";//Report error
              cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
              kill(getppid(), SIGUSR1);
              delete[] argPos;
              delete clientList;
              delete fileList;
              close(pd);
              exit(errno);
            }
            cout << "pipe is now ready!\n";
            fflush(stdout);
            while(1)
            {
              int fileNameSize;
              string fileName;
              int fileDataSize;
              string fileDataCnt = "";
              string toBCreated = mirrorDir;
              toBCreated += "/";
              toBCreated += newClientID;
              toBCreated += "/";
              int nread;
              string headerS;
              nread = read(pd, packetHeader, 1024);
              if(nread > 0)//filenameSize
              {
                headerS = packetHeader;
                int posS = headerS.find_first_of("*");//get file name size
                string fileNameSizeS = headerS.substr(0, posS);
                fileNameSize = stoi(fileNameSizeS, nullptr, 10);
                if(fileNameSize == 0)//was the closing data all files are sent
                {
                  kill(getppid(), SIGUSR2);
                  break;
                }
                headerS = headerS.substr(posS+1);
                posS = headerS.find_first_of("*");//get file name
                fileName = headerS.substr(0, posS);
                toBCreated += fileName;
                headerS = headerS.substr(posS+1);
                posS = headerS.find_first_of("*");//get file data size
                string fileDataSizeS = headerS.substr(0, posS);
                fileDataSize = stoi(fileDataSizeS, nullptr, 10);
              }
              else if(nread == -1)//error happened
              {
                cerr << "read 1 receiver error\n";
                cerr << "ERROR:: Receiver could not read from the pipe!\n";
                cerr << "Error code: " << strerror(errno) << endl;//Get some info
                delete[] argPos;
                delete clientList;
                delete fileList;
                close(pd);
                kill(getppid(), SIGUSR1);
                exit(errno);
              }
              else//finished
                break;
              const char * toBCreatedPath = toBCreated.c_str();
              const char * fileNameC = fileName.c_str();
              if(write(fd3, fileNameC, strlen(fileNameC)) == -1)
              { //write the name of the file containing the relative path
                cerr << "ERROR:: Receiver could not write to the LogFile!\n";
                cerr << "Error code: " << strerror(errno) << endl;//Get some info
                delete[] argPos;
                delete clientList;
                delete fileList;
                close(pd);
                exit(errno);
              }
              if(write(fd3, " ", strlen(" ")) == -1)
              { //write a space
                cerr << "ERROR:: Receiver could not write to the LogFile!\n";
                cerr << "Error code: " << strerror(errno) << endl;//Get some info
                delete[] argPos;
                delete clientList;
                delete fileList;
                close(pd);
                exit(errno);
              }
              string fSize = to_string(fileDataSize);
              fSize += " ";//add a space
              const char * fSizeC = fSize.c_str();
              if(write(fd3, fSizeC, strlen(fSizeC)) == -1)
              { //write the size of the file
                cerr << "ERROR:: Receiver could not write to the LogFile!\n";
                cerr << "Error code: " << strerror(errno) << endl;//Get some info
                delete[] argPos;
                delete clientList;
                delete fileList;
                close(pd);
                exit(errno);
              }
              string newClientIDs = newClientID;
              newClientIDs += "\n";//add a newline after
              const char * newClientIDC = newClientIDs.c_str();
              if(write(fd3, newClientIDC, strlen(newClientIDC)) == -1)
              { //write the sender ID
                cerr << "ERROR:: Receiver could not write to the LogFile!\n";
                cerr << "Error code: " << strerror(errno) << endl;//Get some info
                delete[] argPos;
                delete clientList;
                delete fileList;
                close(pd);
                exit(errno);
              }
              if(fileDataSize > 0)//file
              {
                int bytesToR = fileDataSize;
                cout << "FILE !\n";
                fflush(stdout);
                while(bytesToR > bufferSize)
                {
                  if((nread = read(pd, pipeBuffer, bufferSize)) == -1)
                  {
                    cerr << "ERROR:: Receiver could not read from pipe!\n";//Report error
                    cerr << "Error code: " << strerror(errno) << endl;//Get some info
                    delete[] argPos;
                    delete clientList;
                    delete fileList;
                    close(pd);
                    kill(getppid(), SIGUSR1);
                    exit(errno);
                  }
                  //readjust based on bytes read until all the string is parsed
                  bytesToR -= nread;
                  string temp2 = pipeBuffer;
                  fileDataCnt += temp2;
                }
                if(read(pd, pipeBuffer, bytesToR) == -1)
                {
                  cerr << "ERROR:: Receiver could not read from pipe!\n";//Report error
                  cerr << "Error code: " << strerror(errno) << endl;//Get some info
                  delete[] argPos;
                  delete clientList;
                  delete fileList;
                  close(pd);
                  kill(getppid(), SIGUSR1);
                  exit(errno);
                }
                string temp2 = pipeBuffer;
                fileDataCnt += temp2;
                int lastSlashPos = toBCreated.find_last_of("/");
                string justPath = toBCreated.substr(0, lastSlashPos);
                const char * justPathC = justPath.c_str();
                makeDirTree(justPathC, baseDirC);
                const char * fileDataCntC = fileDataCnt.c_str();
                int nf = open(toBCreatedPath, O_WRONLY | O_CREAT | O_TRUNC, 0777);
                if(nf == -1)
                {
                  cerr << "ERROR:: Receiver could not create the mirror file!\n";//Report error
                  cerr << "Error code: " << strerror(errno) << endl;//Get some info
                  delete[] argPos;
                  delete clientList;
                  delete fileList;
                  close(pd);
                  kill(getppid(), SIGUSR1);
                  exit(errno);
                }
                cout << "creating file: " << toBCreatedPath << endl; 
                if(write(nf, fileDataCntC, fileDataSize) == -1)
                {
                  cerr << "ERROR:: Receiver could not write to the file!\n";//Report error
                  cerr << "Error code: " << strerror(errno) << endl;//Get some info
                  delete[] argPos;
                  delete clientList;
                  delete fileList;
                  close(pd);
                  kill(getppid(), SIGUSR1);
                  exit(errno);
                }
                if(close(nf) == -1)//close the file
                {
                  cerr << "ERROR:: Could not close the file!\n";//Report error
                  cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
                  delete[] argPos;
                  delete clientList;
                  delete fileList;
                  close(pd);
                  exit(errno);
                }
              }
              else//directory
              {
                cout << "DIRECTORY !\n";
                fflush(stdout);
                makeDirTree(toBCreatedPath, baseDirC);
              }
            }
            close(pd);
            if(close(fd3) == -1)//close the file
            {
              cerr << "ERROR:: Could not close the LogFile!\n";//Report error
              cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
              delete[] argPos;
              delete clientList;
              delete fileList;
              exit(errno);
            }
            if(remove(fifoPipe) == -1)
            {
              cerr << "ERROR:: Receiver could not delete the fifo pipe!\n";//Report error
              cerr << "Error code: " << strerror(errno) << endl;//Get some info
            }
          }
          else if(getpid() == slaveShmPtr[1])//slave 2 sends files
          {
            cout << "Sender slave : " << getpid() << " initiating file sync!\n";
            string ffPName = commonDir;
            int lengthD = ffPName.length();
            ffPName += "/";
            ffPName += "id";
            ffPName += sclientID;
            ffPName += "_to_";
            ffPName += newClientID;
            ffPName += ".fifo";
            const char * fifoPipe = ffPName.c_str();
            string toSearchS = ffPName.substr(lengthD+1);
            const char * fifoSearch = toSearchS.c_str();
            bool fResult = checkFifos(commonDir, fifoSearch);
            if(fResult == false)//have to create it
            {
              cout << fifoPipe << ": created by sender!\n";
              if(mkfifo(fifoPipe, 0777) == -1)
              {
                cerr << "ERROR:: Sender Could not create the fifo pipe!\n";//Report error
                cerr << "Error code: " << strerror(errno) << endl;//Get some info
                delete[] argPos;
                delete clientList;
                delete fileList;
                kill(getppid(), SIGUSR1);
                exit(errno);
              }
            }
            else
              cout << "pipe already created!\n";
            int pd;
            if((pd = open(fifoPipe, O_WRONLY)) < 0) //sender
            {
              cerr << "ERROR:: Pipe could not be opened by sender!\n";//Report error
              cerr << "Error code: " << strerror(errno) << endl;//Get some info
              delete[] argPos;
              delete clientList;
              delete fileList;
              kill(getppid(), SIGUSR1);
              exit(errno);
            }
            else
              cout << fifoPipe << " opened successfully by sender!\n";
            int totalFiles2 = 0;
            string inputRecResult = listDirRecurs(inputDir, totalFiles2);
            int length4 = inputRecResult.length();
            for(int i = 0; i < totalFiles2; i++)
            {
              bool resultInRec = false;
              int pos = 0;
              int counter = 0;
              while(counter < length4)
              {
                if(inputRecResult[pos] == '*')//found delimiter
                {
                  resultInRec = true;
                  break;
                }
                else
                  pos++;
                counter++;
              }
              if(resultInRec == false)//we done
                break;
              string temp = inputRecResult.substr(0, pos);
              inputRecResult = inputRecResult.substr(pos+1);
              length4 = inputRecResult.length();
              ifstream in(temp);//open stream
              string line;
              getline(in, line);//all the data of the file is in one big line :P
              const char * fileData = line.c_str();
              int fileSz = line.length();
              if(fileSz > 0)
                fileSz++;//account the newline at the end
              string flSz = to_string(fileSz);
              string inputDirS = inputDir;
              int length5 = inputDirS.length();
              temp = temp.substr(length5+1);//keep just the file path after inputdir
              short inpFileLength = temp.length();
              string sIfl = to_string(inpFileLength);
              string headerS = sIfl;
              headerS += "*";
              headerS += temp;
              headerS += "*";
              headerS += flSz;
              int counterP = headerS.length();
              while(counterP < 1024)//add paddings
              {
                headerS += "*";
                counterP ++;
              }
              const char * headerC = headerS.c_str();//send the header
              if(write(pd, headerC, 1024) == -1)
              {
                cout << "write 1 sender error\n";
                cerr << "ERROR:: Sender could not write to pipe!\n";//Report error
                cerr << "Error code: " << strerror(errno) << endl;//Get some info
                delete[] argPos;
                delete clientList;
                delete fileList;
                close(pd);
                kill(getppid(), SIGUSR1);
                exit(errno);
              }
              if(fileSz > 0)//directories skip this
              {
                if(write(pd, fileData, fileSz) == -1)//send the file data
                {
                  cerr << "ERROR:: Sender could not write to pipe!\n";//Report error
                  cerr << "Error code: " << strerror(errno) << endl;//Get some info
                  delete[] argPos;
                  delete clientList;
                  delete fileList;
                  close(pd);
                  kill(getppid(), SIGUSR1);
                  exit(errno);
                }
              }
            }
            const char * dataEnd = "00*";//send the closing two zeros
            if(write(pd, dataEnd, bufferSize) == -1)
            {
              cerr << "ERROR:: Sender could not write to pipe!\n";//Report error
              cerr << "Error code: " << strerror(errno) << endl;//Get some info
              delete[] argPos;
              delete clientList;
              delete fileList;
              close(pd);
              kill(getppid(), SIGUSR1);
              exit(errno);
            }
            close(pd);
            if(remove(fifoPipe) == -1)
            {
              cerr << "ERROR:: Sender could not delete the fifo pipe!\n";//Report error
              cerr << "Error code: " << strerror(errno) << endl;//Get some info
              fflush(stdout);
            }
          }
        }
        //slaves die
        if(retpid == 0)
        {
          cout << "Slave : " << getpid() << " done!\n" << endl;
          delete[] argPos;
          delete clientList;
          delete fileList;
          exit(0);
        }
        //master's code
        if(retpid != 0)
        {
          signal(SIGUSR2, signalHandler2);//register the SIGUSR2 signal anything wrong fork again
          cout << "Master : " << getpid() << " waiting...\n";
          while(slaves-- > 0)
            wait(NULL);
          cout << "Master : " << getpid() << " both slaves are done synching...\n";
        }
      }
    }
    int totalFiles1 = 0;
    string strResult1 = listDir(commonDir, totalFiles1);//get the file list
    int length1 = strResult1.length();
    fileList->resetFound();
    for(int i = 0; i < totalFiles1; i++)
    {
      int pos = 0;
      int counter = 0;
      while(counter < length1)
      {
        if(strResult1[pos] == '*')//found delimiter
          break;
        else
          pos++;
        counter++;
      }
      string temp1 = strResult1.substr(0, pos);
      strResult1 = strResult1.substr(pos+1);
      length1 = strResult1.length();
      fileList->checkFiles(temp1);
    }
    int deletedFiles = 0;
    string delFiles = fileList->checkDeleted(deletedFiles);
    cout << deletedFiles << " files deleted in this check!\n";
    for(int i = 0; i < deletedFiles; i++)
    {
      int firStar = delFiles.find_first_of("*");
      string target = delFiles.substr(0, firStar);
      delFiles = delFiles.substr(firStar+1);
      fileList->delNode(target);
      pid_t pid = fork();
      if(pid == -1)
      {
        cerr << "ERROR:: forking process!\n";//Report error
        cerr << "Error code: " << strerror(errno) << endl;//Get some cause info
        delete[] argPos;
        delete clientList;
        delete fileList;
        return 7;
      }
      if(pid == 0)//slave to delete the mirror contents
      {
        cout << "Slave : " << getpid();
        cout << " going to clean up the mirror directory!\n";
        int firstSlashPos = target.find_last_of("/");
        target = target.substr(firstSlashPos+1);
        int firstDotPos = target.find_first_of(".");
        target = target.substr(0, firstDotPos);
        string toBremoved = mirrorDir;
        toBremoved += "/";
        toBremoved += target;
        string leftClient = "left_client ";
        leftClient += target;
        leftClient += "\n";
        const char * toBremovedC = toBremoved.c_str();
        removeDirTree(toBremovedC);
        int fd3;
        if((fd3 = open(logFile, O_RDWR | O_APPEND, 0777)) == -1)
        {
          cerr << "ERROR:: LogFile could not be opened!\n";//Report error
          cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
          delete[] argPos;
          delete clientList;
          delete fileList;
          exit(errno);
        }
        const char * leftClientC = leftClient.c_str();
        if(write(fd3, leftClientC, strlen(leftClientC)) == -1)
        { //write the id of the client that left
          cerr << "ERROR:: Receiver could not write to the LogFile!\n";
          cerr << "Error code: " << strerror(errno) << endl;//Get some info
          delete[] argPos;
          delete clientList;
          delete fileList;
          exit(errno);
        }
        if(close(fd3) == -1)//close the file
        {
          cerr << "ERROR:: Could not close the LogFile!\n";//Report error
          cerr << "Error code: " << strerror(errno) << endl;//Get some info as to why
          delete[] argPos;
          delete clientList;
          delete fileList;
          exit(errno);
        }
        delete[] argPos;
        delete clientList;
        delete fileList;
        cout << "Slave : " << getpid() << " done!\n" << endl;
        exit(0);//slave dies
      }
      else//master waiting for slave to finish
      {
        cout << "Master : " << getpid() << " waiting...\n";
        wait(NULL);
        cout << "Master : " << getpid() << " slave is done...\n";
      }
    }
  }
  if(shmdt((void *) slaveShmPtr) == -1)
  {
    perror("shmdt");
    exit(1);
  }
  if(shmctl(slaveShmId, IPC_RMID, NULL) == -1)
  {
    perror("shmctl");
    exit(1);
  }
  delete[] argPos;
  delete clientList;
  delete fileList;
  return 0;
}