#include "TransactionsList.hpp"
#include "HashTable.hpp"
#include "BlockchainTree.hpp"
#include "Wallet.hpp"
#include "Helpers.hpp"
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cerrno>

using namespace std;

int main(int argc, char ** argv)
{
  if(argc != 13)
  {
    cout << "ERROR:: Invalid arguments!\n";
    return 4;
  }
  int argCount = 0;
  int * argPos = new int[6];
  for(int i = 0; i < argc; i++)//get mixed order arguments but all are needed
  {
    if(strcmp(argv[i], "-a") == 0)
    {
      argPos[0] = i+1;
      argCount++;
    }
    if(strcmp(argv[i], "-t") == 0)
    {
      argPos[1] = i+1;
      argCount++;
    }
    if(strcmp(argv[i], "-v") == 0)
    {
      argPos[2] = i+1;
      argCount++;
    }
    if(strcmp(argv[i], "-h1") == 0)
    {
      argPos[3] = i+1;
      argCount++;
    }
    if(strcmp(argv[i], "-h2") == 0)
    {
      argPos[4] = i+1;
      argCount++;
    }
    if(strcmp(argv[i], "-b") == 0)
    {
      argPos[5] = i+1;
      argCount++;
    }
  }
  if(argCount != 6)
  {
    cout << "ERROR:: Invalid arguments!\n";
    delete[] argPos;
    return 4;
  }
  argCount = 0;
  if(isNumber(argv[argPos[2]]) == true)
    argCount++;
  if(isNumber(argv[argPos[3]]) == true)
    argCount++;
  if(isNumber(argv[argPos[4]]) == true)
    argCount++;
  if(isNumber(argv[argPos[5]]) == true)
    argCount++;
  if(argCount != 4)
  {
    cout << "ERROR:: Invalid arguments!\n";
    delete[] argPos;
    return 4;
  }
  int testV = stoi(argv[argPos[2]], nullptr, 10);
  int testB = stoi(argv[argPos[5]], nullptr, 10);
  int testH1 = stoi(argv[argPos[3]], nullptr, 10);
  int testH2 = stoi(argv[argPos[4]], nullptr, 10);
  if(testH2 < 1 || testH1 < 1)
  {
    cout << "ERROR:: HashTables minimum size = 1!\n";
    delete[] argPos;
    return 5;
  }
  if(testV < 2)
  {
    cout << "ERROR:: Bitcoin Price can't be less than 2$!\n";
    delete[] argPos;
    return 5;
  }
  int ptrSz = sizeof(int*);
  int minBkSz = ((2 * sizeof(int)) + (3 * ptrSz));//min requirements
  if(testB < minBkSz)
  {
    cout << "ERROR:: Bucket size must be over " << minBkSz << " bytes!\n";
    delete[] argPos;
    return 5;
  }
  string lineBal;
  string lineTrans;
  string owner;
  int * idArray;
  cout << "Processing Balances file...\n";
  ifstream Bal(argv[argPos[0]]);//open stream
  if(!Bal)
  {
    cerr << "File could not be opened!\n"; //Report error
    cerr << "Error code: " << strerror(errno) << endl; //Get some info as to why
    return 3;
    delete[] argPos;
  }
  int totalWallets = 0;
  while(getline(Bal, lineBal))//line counting and name checking
    totalWallets++;
  Bal.clear();
  Bal.seekg(0, ios::beg);
  //cout << totalWallets << " counted!\n";
  Wallet ** walletArray = new Wallet*[totalWallets];
  string * ownerArray = new string[totalWallets];
  int ownerCount = 0;
  int walletCount = 0;
  int coinVal = stoi(argv[argPos[2]], nullptr, 10);
  List * BTCList = new List();
  while(getline(Bal, lineBal))//update owner wallets with initial status
  {
    int pos = 0;
    int length = lineBal.length();
    int counter = 0;
    while(counter < length)
    {
      if(lineBal[pos] == ' ')
        break;
      else
        pos++;
      counter++;
    }
    owner = lineBal.substr(0,pos);
    ownerArray[ownerCount] = owner;
    ownerCount++;
    //cout << "owner read : " << owner << endl;
    string temp = lineBal.substr(pos);
    //cout << temp << endl;
    int size = 0;
    pos = 0;
    length = temp.length();
    if(length > 0)//owner has at least 1 BTC
    {
      for(int i = 0; i < length; i++)
        if(temp[i] == ' ')
          size++;
      //cout << "Read " << size << " unique btc ids\n";
      idArray = new int[size];
      stringstream ss;
      ss << temp;
      string temp2;
      int found;
      while(!ss.eof())
      {
        ss >> temp2;
        if(stringstream(temp2) >> found)
        {
          idArray[pos] = found;
          BTCList->addNode(found, owner);
          pos++;
          temp2 = "";
        }
      }
      walletArray[walletCount] = new Wallet(size, coinVal, idArray, owner);
      walletCount++;
      delete[] idArray;
    }
    else//owner doesnt have any
    {
      walletArray[walletCount] = new Wallet(0, coinVal, NULL, owner);
      walletCount++;
    }
  }
  //BTCList->print();//print read BTCs
  //cout << endl;
  if(BTCList->searchDuplicate() == true)//search for shared BTC IDs
  {
    Bal.close();
    for(int i = 0; i < totalWallets; i++)
      delete walletArray[i];
    delete[] walletArray;
    delete BTCList;
    delete[] ownerArray;
    delete[] argPos;
    cout << "\nERROR: Found owners with same BTC IDs...\nExiting...";
    return 1;
  }
  for(int i = 0; i < (totalWallets - 1); i++)//search for owners with same name
  {
    for(int j = (i+1); j < totalWallets; j++)
    {
      if(ownerArray[i].compare(ownerArray[j]) == 0)//found same owner
      {
        Bal.close();
        for(int i = 0; i < totalWallets; i++)
          delete walletArray[i];
        delete[] walletArray;
        delete BTCList;
        delete[] argPos;
        cout << "\nERROR: Found 2 owners with name " << ownerArray[i];
        cout << "\nExiting...";
        delete[] ownerArray;
        return 2;
      }
    }
  }
  Bal.close();
  int totalCoins = BTCList->getSize();
  int * coinIDs = new int[totalCoins];
  string * duplOwnerArray = new string[totalCoins];
  BlockchainTree ** rootArray = new BlockchainTree*[totalCoins];
  for(int i = 0; i < totalCoins; i++)//create the tree array for each BTC
  {
    BTCList->getIDs(coinIDs);
    BTCList->getOwners(duplOwnerArray);
    rootArray[i] = new BlockchainTree(duplOwnerArray[i], coinVal, coinIDs[i]);
  }
  delete[] coinIDs;
  delete[] duplOwnerArray;
  cout << "Processing Transactions file...\n";
  ifstream Trans(argv[argPos[1]]);//open stream for tx count and duplic
  if(!Trans)
  {
    cerr << "File could not be opened!\n"; //Report error
    cerr << "Error code: " << strerror(errno) << endl; //Get some info as to why
    delete[] ownerArray;
    delete[] argPos;
    for(int i = 0; i < totalWallets; i++)
      delete walletArray[i];
    delete[] walletArray;
    delete BTCList;
    for(int i = 0; i < totalCoins; i++)
      delete rootArray[i];
    delete[] rootArray;
    return 3;
  }
  int totalTX = 0;
  while(getline(Trans, lineTrans))
    totalTX++;
  Trans.clear();
  Trans.seekg(0, ios::beg);
  string * txIDsArray = new string[totalTX];//array of transaction ids
  int pos = 0;
  int maxID = -1;
  while(getline(Trans, lineTrans))
  {
    int length = lineTrans.length();
    int counter = 0;
    int pos1 = 0;
    while(counter < length)
    {
      if(lineTrans[pos1] == ' ')
        break;
      else
        pos1++;
      counter++;
    }
    txIDsArray[pos] = lineTrans.substr(0,pos1);
    if(stoi(txIDsArray[pos], nullptr, 10) > maxID)//find the biggest ID to handle tx name giving
      maxID = stoi(txIDsArray[pos], nullptr, 10);
    pos++;
  }
  for(int i = 0; i < (totalTX - 1); i++)//search for txs with same ID
  {
    for(int j = (i+1); j < totalTX; j++)
    {
      if(txIDsArray[i].compare(txIDsArray[j]) == 0)//found same IDS & != whiteS
      {
        for(int i = 0; i < totalWallets; i++)
          delete walletArray[i];
        delete[] walletArray;
        cout << "\nERROR: Found 2 Transactions with ID " << txIDsArray[i];
        cout << "\nExiting...";
        delete[] txIDsArray;
        for(int i = 0; i < totalCoins; i++)
          delete rootArray[i];
        delete[] rootArray;
        delete[] ownerArray;
        delete BTCList;
        delete[] argPos;
        return 2;
      }
    }
  }
  int buckSizeCalced = stoi(argv[argPos[5]], nullptr, 10);
  buckSizeCalced -= (2 * sizeof(int));
  buckSizeCalced -= ptrSz;
  buckSizeCalced /= (2 * ptrSz);//recalculated size based on implementation
  HashTable * sendHT = new HashTable(stoi(argv[argPos[3]], nullptr, 10),
                                     buckSizeCalced);
  HashTable * recvHT = new HashTable(stoi(argv[argPos[4]], nullptr, 10),
                                     buckSizeCalced);
  Trans.clear();
  Trans.seekg(0, ios::beg);
  string sender;
  string receiver;
  int amount;
  string temp;
  int txID;
  int day;
  int month;
  int year;
  int hour;
  int min;
  int lastDay = -1;//init with values less than actual timeframes
  int lastMonth = -1;
  int lastYear = -1;
  int lastHour = -1;
  int lastMin = -1;
  bool breakResult;
  while(getline(Trans, lineTrans))//process transactions
  { 
    breakResult = breakDownTx(lineTrans, txID, sender, receiver, amount,
                              day, month, year, hour, min);
    if(breakResult == false)
    {
      cout << "ERROR :: Invalid data!\n";
      cout << "Skipping transaction...\n";
      continue;
    }
    bool resultDate = dateCheck(year, month, day, hour, min, 
                               lastYear, lastMonth, lastDay, lastHour, lastMin);
    if(resultDate == false)
    {
      cout << "ERROR :: Transaction Timeframes are wrong!\n";
      cout << "1st) ";
      cout << lastDay << "-";
      cout << lastMonth << "-";
      cout << lastYear << " ";
      cout << lastHour << ":";
      cout << lastMin << endl;
      cout << "2nd) ";
      cout << day << "-";
      cout << month << "-";
      cout << year << " ";
      cout << hour << ":";
      cout << min << endl;
      cout << "Skipping transaction...\n";
      continue;
    }
    if(sender.compare(receiver) == 0)
    {
      cout << "ERROR :: Receiver must be different from sender!\n";
      cout << "Skipping transaction...\n";
      continue;
    }
    int result;
    int posS = -1;
    int posR = -1;
    for(int i = 0; i < totalWallets; i++)
    {
      result = walletArray[i]->checkBalance(sender, amount);
      if(result == 1)//found and has balance
      {
        posS = i;//hold array position for instant access
        break;
      }
      if(result == 2)//found but not enough balance
      {
        posS = -2;//hold array position for instant access
        break;
      }//else continue looking IDs didn't match
    }
    if(posS == -1)//not found
      cout << "ERROR: Sender ID not found, transaction skipped!\n";
    else if(posS == -2)//not enough balance
    {
      cout << "ERROR: " << sender;
      cout << "'s balance is not enough, transaction skipped!\n";
    }
    else//sender is legit but check if receiver exists firstly
    {
      result = 0;
      for(int i = 0; i < totalWallets; i++)
      {
        result = walletArray[i]->checkRcvr(receiver);
        if(result == 1)
        {
          posR = i;
          break;
        }
      }
      if(result == 0)//receiver not found
        cout << "ERROR: Receiver ID not found, transaction skipped!\n";
      else//transaction is legit on both ends, process it
      {
        lastDay = day;
        lastMonth = month;
        lastYear = year;
        lastHour = hour;
        lastMin = min;
        cout << "Processing transaction...\n";
        int size = walletArray[posS]->getSize();
        int * coinIDs = new int[size];//for coin IDs involved in transaction
        int * coinValues = new int[size];//for amounts taken from each coin
        int emptied = 0;
        for(int i = 0; i < size; i++)//init arrays
        {
          coinIDs[i] = -1;
          coinValues[i] = -1;
        }
        walletArray[posS]->getBTCs(coinIDs, coinValues, amount, emptied);
        int involved = 0;
        for(int i = 0; i < size; i++)
        {
          if(coinIDs[i] == -1)
            break;
          involved++;
          //cout << "Coin involved : " << coinIDs[i] << "\t";
          //cout << "Amount taken : " << coinValues[i] << endl;
        }
        while(emptied != 0)
        {
          walletArray[posS]->delBTCs();
          emptied--;
        }
        walletArray[posR]->addBTCs(coinIDs, coinValues, amount, involved);
        int tempID;
        TreeNode * involvedNode = NULL;
        for(int i = 0; i < size; i++)
        {
          if(coinIDs[i] == -1)//account used coins only
            break;
          for(int j = 0; j < totalCoins; j++)
          {
            tempID = rootArray[j]->getCoinID();
            if(tempID == coinIDs[i])//found the root
            {
              rootArray[j]->root->addNode(sender, receiver, coinValues[i],
                                   coinIDs[i], involvedNode);
              sendHT->findAdd(sender, receiver, txID, amount, day, month, year,
                              hour, min, 1, involvedNode, coinIDs[i]);
              recvHT->findAdd(sender, receiver, txID, amount, day, month, year,
                              hour, min, 2, involvedNode, coinIDs[i]);
              break;
            }
          }
        }
        delete[] coinIDs;
        delete[] coinValues;
      }
    }
  }
  Trans.close();
  cout << "\n\nInitial input files processed successfully...\n";
  //initial structures filled, now wait user input for results
  string input;
  string command;
  while(1)//ASSUMPTION::USER LVL = CHILD
  {
    cout << "\nPlease state your next command...\n";
    cout << "Type '/exit' to end program...\n";
    cout << "Type '/help' to get a list of available commands...\n\n";
    getline(cin, input);
    if(input.compare("/exit") == 0)//bye
      break;
    if(input.compare("/help") == 0)//display commands
    {
      cout << "Available Commands Below\n";
      cout << "----------------------------\n";
      cout << "Command   : /requestTransaction\n";
      cout << "Arguments : senderWalletID receiverWalletID amount date time\n\n";
      cout << "Command   : /requestTransactions\n";
      cout << "Arguments : senderWalletID1 receiverWalletID1 amount1 date1 time1;\n";
      cout << "Arguments : senderWalletID2 receiverWalletID2 amount2 date2 time2;\n";
      cout << "Arguments : .....................................................;\n";
      cout << "Arguments : senderWalletIDN receiverWalletIDN amountN dateN timeN;\n\n";
      cout << "Arguments : inputFile\n\n";
      cout << "Command   : /findEarnings\n";
      cout << "Arguments : walletID [time1][date1][time2][date2]\n";
      cout << "Arguments : walletID [time1][time2]\n";
      cout << "Arguments : walletID [date1][date2]\n\n";
      cout << "Command   : /findPayments\n";
      cout << "Arguments : walletID [time1][date1][time2][date2]\n";
      cout << "Arguments : walletID [time1][time2]\n";
      cout << "Arguments : walletID [date1][date2]\n\n";
      cout << "Command   : /walletStatus\n";
      cout << "Arguments : walletID\n\n";
      cout << "Command   : /bitCoinStatus\n";
      cout << "Arguments : bitCoinID\n\n";
      cout << "Command   : /traceCoin\n";
      cout << "Arguments : bitCoinID\n\n";
      cout << "Command   : /exit\n\n";
    }
    else//lets process more stuff
    {
      int length = input.length();
      int counter = 0;
      int pos = 0;
      while(counter < length)
      {
        if(input[pos] == ' ')
          break;
        else
          pos++;
        counter++;
      }
      command = input.substr(0, pos);//the command to be executed
      input = input.substr(pos);//the rest arguments
      cout << "\nCommand was : " << command << endl;
      cout << "Arguments given :" << input << "\n\n";
      length = input.length();
      if(length == 0)
      {
        cout << "Please give valid arguments to execute command!\n";
        continue;
      }
      if(command.compare("/requestTransaction") == 0)
      {
        txID = ++maxID;//get new id ready
        string txIDstring = to_string(txID) + input;//create the tx string
        breakResult = breakDownTx(txIDstring, txID, sender, receiver, amount,
                              day, month, year, hour, min);
        if(breakResult == false)
        {
          cout << "ERROR :: Invalid data!\n";
          cout << "Skipping transaction...\n";
          continue;
        }
        bool resultDate = dateCheck(year, month, day, hour, min, 
                               lastYear, lastMonth, lastDay, lastHour, lastMin);
        if(resultDate == false)
        {
          cout << "ERROR :: Transaction Timeframes are wrong!\n";
          cout << "1st) ";
          cout << lastDay << "-";
          cout << lastMonth << "-";
          cout << lastYear << " ";
          cout << lastHour << ":";
          cout << lastMin << endl;
          cout << "2nd) ";
          cout << day << "-";
          cout << month << "-";
          cout << year << " ";
          cout << hour << ":";
          cout << min << endl;
          cout << "Skipping transaction...\n";
          maxID--;
          continue;
        }
        if(sender.compare(receiver) == 0)
        {
          cout << "ERROR :: Receiver must be different from sender!\n";
          cout << "Skipping transaction...\n";
          maxID--;
          continue;
        }
        int result;
        int posS = -1;
        int posR = -1;
        for(int i = 0; i < totalWallets; i++)
        {
          result = walletArray[i]->checkBalance(sender, amount);
          if(result == 1)//found and has balance
          {
            posS = i;//hold array position for instant access
            break;
          }
          if(result == 2)//found but not enough balance
          {
            posS = -2;//hold array position for instant access
            break;
          }//else continue looking IDs didn't match
        }
        if(posS == -1)//not found
          cout << "ERROR: Sender ID not found, transaction skipped!\n";
        else if(posS == -2)//not enough balance
        {
          cout << "ERROR: " << sender;
          cout << "'s balance is not enough, transaction skipped!\n";
        }
        else//sender is legit but check if receiver exists firstly
        {
          result = 0;
          for(int i = 0; i < totalWallets; i++)
          {
            result = walletArray[i]->checkRcvr(receiver);
            if(result == 1)
            {
              posR = i;
              break;
            }
          }
          if(result == 0)//receiver not found
            cout << "ERROR: Receiver ID not found, transaction skipped!\n";
          else//transaction is legit on both ends, process it
          {
            lastDay = day;
            lastMonth = month;
            lastYear = year;
            lastHour = hour;
            lastMin = min;
            cout << "Processing transaction...\n";
            int size = walletArray[posS]->getSize();
            int * coinIDs = new int[size];//for coin IDs involved in transaction
            int * coinValues = new int[size];//for amounts taken from each coin
            int emptied = 0;
            for(int i = 0; i < size; i++)//init arrays
            {
              coinIDs[i] = -1;
              coinValues[i] = -1;
            }
            walletArray[posS]->getBTCs(coinIDs, coinValues, amount, emptied);
            int involved = 0;
            for(int i = 0; i < size; i++)
            {
              if(coinIDs[i] == -1)
                break;
              involved++;
              //cout << "Coin involved : " << coinIDs[i] << "\t";
              //cout << "Amount taken : " << coinValues[i] << endl;
            }
            while(emptied != 0)
            {
              walletArray[posS]->delBTCs();
              emptied--;
            }
            walletArray[posR]->addBTCs(coinIDs, coinValues, amount, involved);
            int tempID;
            TreeNode * involvedNode = NULL;
            for(int i = 0; i < size; i++)
            {
              if(coinIDs[i] == -1)//account used coins only
                break;
              for(int j = 0; j < totalCoins; j++)
              {
                tempID = rootArray[j]->getCoinID();
                if(tempID == coinIDs[i])//found the root
                {
                  rootArray[j]->root->addNode(sender, receiver, coinValues[i],
                                       coinIDs[i], involvedNode);
                  sendHT->findAdd(sender, receiver, txID, amount, day, month, year,
                                  hour, min, 1, involvedNode, coinIDs[i]);
                  recvHT->findAdd(sender, receiver, txID, amount, day, month, year,
                                  hour, min, 2, involvedNode, coinIDs[i]);
                  break;
                }
              }
            }
            delete[] coinIDs;
            delete[] coinValues;
          }
        }
      }
      else if(command.compare("/requestTransactions") == 0)//2 cases 1 for file
      {
        //first case file
        string firstArg = input;
        cout << "arg was:" << firstArg << endl;
        length = firstArg.length();
        pos = 0;
        counter = 0;
        for(int i = 0; i < length; i++)
        {
          if(firstArg[i] == ' ')
            counter++;
        }
        if(counter == 1)//file probably
        {
          firstArg = firstArg.substr(1);
          //cout << "File probably\n";
          cout << "Processing Transactions file...\n";
          ifstream TransFile(firstArg);//open stream for tx file
          if(!TransFile)
          {
            cerr << "File could not be opened!\n"; // Report error
            cerr << "Error code: " << strerror(errno) << endl; // Get some info as to why
            continue;
          }
          string transLine;
          while(getline(TransFile, transLine))
          {
            bool semicolRes = false;
            int lineLength = transLine.length();
            //cout << "transLine:" << transLine << endl;
            if(transLine[lineLength-1] == ';')
              semicolRes = true;
            if(semicolRes == false)
            {
              cout << "ERROR :: Invalid data!\n";
              cout << "Skipping transaction...\n";
              continue;
            }
            transLine = transLine.substr(0,lineLength-1);
            //cout << "transLine:" << transLine << endl;
            txID = ++maxID;//get new id ready
            string txIDstring = to_string(txID) + ' ' + transLine;//create the tx string
            breakResult = breakDownTx(txIDstring, txID, sender, receiver, amount,
                              day, month, year, hour, min);
            if(breakResult == false)
            {
              cout << "ERROR :: Invalid data!\n";
              cout << "Skipping transaction...\n";
              maxID--;
              continue;
            }
            bool resultDate = dateCheck(year, month, day, hour, min, 
                              lastYear, lastMonth, lastDay, lastHour, lastMin);
            if(resultDate == false)
            {
              cout << "ERROR :: Transaction Timeframes are wrong!\n";
              cout << "1st) ";
              cout << lastDay << "-";
              cout << lastMonth << "-";
              cout << lastYear << " ";
              cout << lastHour << ":";
              cout << lastMin << endl;
              cout << "2nd) ";
              cout << day << "-";
              cout << month << "-";
              cout << year << " ";
              cout << hour << ":";
              cout << min << endl;
              cout << "Skipping transaction...\n";
              maxID--;
              continue;
            }
            if(sender.compare(receiver) == 0)
            {
              cout << "ERROR :: Receiver must be different from sender!\n";
              cout << "Skipping transaction...\n";
              maxID--;
              continue;
            }
            int result;
            int posS = -1;
            int posR = -1;
            for(int i = 0; i < totalWallets; i++)
            {
              result = walletArray[i]->checkBalance(sender, amount);
              if(result == 1)//found and has balance
              {
                posS = i;//hold array position for instant access
                break;
              }
              if(result == 2)//found but not enough balance
              {
                posS = -2;//hold array position for instant access
                break;
              }//else continue looking IDs didn't match
            }
            if(posS == -1)//not found
              cout << "ERROR: Sender ID not found, transaction skipped!\n";
            else if(posS == -2)//not enough balance
            {
              cout << "ERROR: " << sender;
              cout << "'s balance is not enough, transaction skipped!\n";
            }
            else//sender is legit but check if receiver exists firstly
            {
              result = 0;
              for(int i = 0; i < totalWallets; i++)
              {
                result = walletArray[i]->checkRcvr(receiver);
                if(result == 1)
                {
                  posR = i;
                  break;
                }
              }
              if(result == 0)//receiver not found
                cout << "ERROR: Receiver ID not found, transaction skipped!\n";
              else//transaction is legit on both ends, process it
              {
                lastDay = day;
                lastMonth = month;
                lastYear = year;
                lastHour = hour;
                lastMin = min;
                cout << "Processing transaction...\n";
                int size = walletArray[posS]->getSize();
                int * coinIDs = new int[size];//for coin IDs involved in transaction
                int * coinValues = new int[size];//for amounts taken from each coin
                int emptied = 0;
                for(int i = 0; i < size; i++)//init arrays
                {
                  coinIDs[i] = -1;
                  coinValues[i] = -1;
                }
                walletArray[posS]->getBTCs(coinIDs, coinValues, amount, emptied);
                int involved = 0;
                for(int i = 0; i < size; i++)
                {
                  if(coinIDs[i] == -1)
                    break;
                  involved++;
                  //cout << "Coin involved : " << coinIDs[i] << "\t";
                  //cout << "Amount taken : " << coinValues[i] << endl;
                }
                while(emptied != 0)
                {
                  walletArray[posS]->delBTCs();
                  emptied--;
                }
                walletArray[posR]->addBTCs(coinIDs, coinValues, amount, involved);
                int tempID;
                TreeNode * involvedNode = NULL;
                for(int i = 0; i < size; i++)
                {
                  if(coinIDs[i] == -1)//account used coins only
                    break;
                  for(int j = 0; j < totalCoins; j++)
                  {
                    tempID = rootArray[j]->getCoinID();
                    if(tempID == coinIDs[i])//found the root
                    {
                      rootArray[j]->root->addNode(sender, receiver, coinValues[i],
                                           coinIDs[i], involvedNode);
                      sendHT->findAdd(sender, receiver, txID, amount, day, month, year,
                                      hour, min, 1, involvedNode, coinIDs[i]);
                      recvHT->findAdd(sender, receiver, txID, amount, day, month, year,
                                      hour, min, 2, involvedNode, coinIDs[i]);
                      break;
                    }
                  }
                }
                delete[] coinIDs;
                delete[] coinValues;
              }
            }
          }
          TransFile.close();
        }
        else//set of transactions
        {
          cout << "Processing Transactions stream...\n";
          firstArg = firstArg.substr(1);
          length = firstArg.length();
          cout << "firstArg:" << firstArg << endl;
          bool semicolRes = false;
          if(firstArg[length-1] == ';')
            semicolRes = true;
          if(semicolRes == false)
          {
            cout << "ERROR :: Invalid data!\n";
            cout << "Skipping transaction...\n";
            continue;
          }
          firstArg  = firstArg.substr(0, length-1);
          txID = ++maxID;//get new id ready
          string txIDstring = to_string(txID) + ' ' + firstArg;//create the tx string
          breakResult = breakDownTx(txIDstring, txID, sender, receiver, amount,
                            day, month, year, hour, min);
          if(breakResult == false)
          {
            cout << "ERROR :: Invalid data!\n";
            cout << "Skipping transaction...\n";
            maxID--;
            continue;
          }
          bool resultDate = dateCheck(year, month, day, hour, min, 
                              lastYear, lastMonth, lastDay, lastHour, lastMin);
          if(resultDate == false)
          {
            cout << "ERROR :: Transaction Timeframes are wrong!\n";
            cout << "1st) ";
            cout << lastDay << "-";
            cout << lastMonth << "-";
            cout << lastYear << " ";
            cout << lastHour << ":";
            cout << lastMin << endl;
            cout << "2nd) ";
            cout << day << "-";
            cout << month << "-";
            cout << year << " ";
            cout << hour << ":";
            cout << min << endl;
            cout << "Skipping transaction...\n";
            maxID--;
            continue;
          }
          if(sender.compare(receiver) == 0)
          {
            cout << "ERROR :: Receiver must be different from sender!\n";
            cout << "Skipping transaction...\n";
            maxID--;
            continue;
          }
          int result;
          int posS = -1;
          int posR = -1;
          for(int i = 0; i < totalWallets; i++)
          {
            result = walletArray[i]->checkBalance(sender, amount);
            if(result == 1)//found and has balance
            {
              posS = i;//hold array position for instant access
              break;
            }
            if(result == 2)//found but not enough balance
            {
              posS = -2;//hold array position for instant access
              break;
            }//else continue looking IDs didn't match
          }
          if(posS == -1)//not found
            cout << "ERROR: Sender ID not found, transaction skipped!\n";
          else if(posS == -2)//not enough balance
          {
            cout << "ERROR: " << sender;
            cout << "'s balance is not enough, transaction skipped!\n";
          }
          else//sender is legit but check if receiver exists firstly
          {
            result = 0;
            for(int i = 0; i < totalWallets; i++)
            {
              result = walletArray[i]->checkRcvr(receiver);
              if(result == 1)
              {
                posR = i;
                break;
              }
            }
            if(result == 0)//receiver not found
              cout << "ERROR: Receiver ID not found, transaction skipped!\n";
            else//transaction is legit on both ends, process it
            {
              lastDay = day;
              lastMonth = month;
              lastYear = year;
              lastHour = hour;
              lastMin = min;
              cout << "Processing transaction...\n";
              int size = walletArray[posS]->getSize();
              int * coinIDs = new int[size];//for coin IDs involved in transaction
              int * coinValues = new int[size];//for amounts taken from each coin
              int emptied = 0;
              for(int i = 0; i < size; i++)//init arrays
              {
                coinIDs[i] = -1;
                coinValues[i] = -1;
              }
              walletArray[posS]->getBTCs(coinIDs, coinValues, amount, emptied);
              int involved = 0;
              for(int i = 0; i < size; i++)
              {
                if(coinIDs[i] == -1)
                  break;
                involved++;
                //cout << "Coin involved : " << coinIDs[i] << "\t";
                //cout << "Amount taken : " << coinValues[i] << endl;
              }
              while(emptied != 0)
              {
                walletArray[posS]->delBTCs();
                emptied--;
              }
              walletArray[posR]->addBTCs(coinIDs, coinValues, amount, involved);
              int tempID;
              TreeNode * involvedNode = NULL;
              for(int i = 0; i < size; i++)
              {
                if(coinIDs[i] == -1)//account used coins only
                  break;
                for(int j = 0; j < totalCoins; j++)
                {
                  tempID = rootArray[j]->getCoinID();
                  if(tempID == coinIDs[i])//found the root
                  {
                    rootArray[j]->root->addNode(sender, receiver, coinValues[i],
                                         coinIDs[i], involvedNode);
                    sendHT->findAdd(sender, receiver, txID, amount, day, month, year,
                                    hour, min, 1, involvedNode, coinIDs[i]);
                    recvHT->findAdd(sender, receiver, txID, amount, day, month, year,
                                    hour, min, 2, involvedNode, coinIDs[i]);
                    break;
                  }
                }
              }
              delete[] coinIDs;
              delete[] coinValues;
            }
          }
          pos = 0;
          counter = 0;
          string transLine;
          while(getline(cin, transLine))
          {
            bool semicolRes = false;
            int lineLength = transLine.length();
            //cout << "transLine:" << transLine << endl;
            if(transLine[lineLength-1] == ';')
              semicolRes = true;
            if(semicolRes == false)
            {
              cout << "ERROR :: Invalid data!\n";
              cout << "Skipping transaction...\n";
              break;
            }
            transLine = transLine.substr(0,lineLength-1);
            //cout << "transLine:" << transLine << endl;
            txID = ++maxID;//get new id ready
            string txIDstring = to_string(txID) + ' ' + transLine;//create the tx string
            breakResult = breakDownTx(txIDstring, txID, sender, receiver, amount,
                              day, month, year, hour, min);
            if(breakResult == false)
            {
              cout << "ERROR :: Invalid data!\n";
              cout << "Skipping transaction...\n";
              maxID--;
              break;
            }
            bool resultDate = dateCheck(year, month, day, hour, min, 
                              lastYear, lastMonth, lastDay, lastHour, lastMin);
            if(resultDate == false)
            {
              cout << "ERROR :: Transaction Timeframes are wrong!\n";
              cout << "1st) ";
              cout << lastDay << "-";
              cout << lastMonth << "-";
              cout << lastYear << " ";
              cout << lastHour << ":";
              cout << lastMin << endl;
              cout << "2nd) ";
              cout << day << "-";
              cout << month << "-";
              cout << year << " ";
              cout << hour << ":";
              cout << min << endl;
              cout << "Skipping transaction...\n";
              maxID--;
              break;
            }
            if(sender.compare(receiver) == 0)
            {
              cout << "ERROR :: Receiver must be different from sender!\n";
              cout << "Skipping transaction...\n";
              maxID--;
              break;
            }
            int result;
            int posS = -1;
            int posR = -1;
            for(int i = 0; i < totalWallets; i++)
            {
              result = walletArray[i]->checkBalance(sender, amount);
              if(result == 1)//found and has balance
              {
                posS = i;//hold array position for instant access
                break;
              }
              if(result == 2)//found but not enough balance
              {
                posS = -2;//hold array position for instant access
                break;
              }//else continue looking IDs didn't match
            }
            if(posS == -1)//not found
              cout << "ERROR: Sender ID not found, transaction skipped!\n";
            else if(posS == -2)//not enough balance
            {
              cout << "ERROR: " << sender;
              cout << "'s balance is not enough, transaction skipped!\n";
            }
            else//sender is legit but check if receiver exists firstly
            {
              result = 0;
              for(int i = 0; i < totalWallets; i++)
              {
                result = walletArray[i]->checkRcvr(receiver);
                if(result == 1)
                {
                  posR = i;
                  break;
                }
              }
              if(result == 0)//receiver not found
                cout << "ERROR: Receiver ID not found, transaction skipped!\n";
              else//transaction is legit on both ends, process it
              {
                lastDay = day;
                lastMonth = month;
                lastYear = year;
                lastHour = hour;
                lastMin = min;
                cout << "Processing transaction...\n";
                int size = walletArray[posS]->getSize();
                int * coinIDs = new int[size];//for coin IDs involved in transaction
                int * coinValues = new int[size];//for amounts taken from each coin
                int emptied = 0;
                for(int i = 0; i < size; i++)//init arrays
                {
                  coinIDs[i] = -1;
                  coinValues[i] = -1;
                }
                walletArray[posS]->getBTCs(coinIDs, coinValues, amount, emptied);
                int involved = 0;
                for(int i = 0; i < size; i++)
                {
                  if(coinIDs[i] == -1)
                    break;
                  involved++;
                  //cout << "Coin involved : " << coinIDs[i] << "\t";
                  //cout << "Amount taken : " << coinValues[i] << endl;
                }
                while(emptied != 0)
                {
                  walletArray[posS]->delBTCs();
                  emptied--;
                }
                walletArray[posR]->addBTCs(coinIDs, coinValues, amount, involved);
                int tempID;
                TreeNode * involvedNode = NULL;
                for(int i = 0; i < size; i++)
                {
                  if(coinIDs[i] == -1)//account used coins only
                    break;
                  for(int j = 0; j < totalCoins; j++)
                  {
                    tempID = rootArray[j]->getCoinID();
                    if(tempID == coinIDs[i])//found the root
                    {
                      rootArray[j]->root->addNode(sender, receiver, coinValues[i],
                                           coinIDs[i], involvedNode);
                      sendHT->findAdd(sender, receiver, txID, amount, day, month, year,
                                      hour, min, 1, involvedNode, coinIDs[i]);
                      recvHT->findAdd(sender, receiver, txID, amount, day, month, year,
                                      hour, min, 2, involvedNode, coinIDs[i]);
                      break;
                    }
                  }
                }
                delete[] coinIDs;
                delete[] coinValues;
              }
            }
          }
        }
      }
      else if(command.compare("/findEarnings") == 0)
      {
        input = input.substr(1);//skip the space
        counter = 0;
        int result = 0;
        pos = 0;
        length = input.length();
        string sDate = "N/A";//starting date
        string eDate = "N/A";//end date
        string sTime = "N/A";//starting time
        string eTime = "N/A";//end time
        bool inpResult = false;
        while(counter < length)
        {
          if(input[counter] == ' ')
          {
            result++;
            pos = counter;
          }
          counter++;
        }
        if(result == 0)//get all the history
        {
          bool walResult = false;
          for(int i = 0 ; i < totalWallets; i++)
          {
            if(input.compare(walletArray[i]->getID()) == 0)//found him
            {
              walResult = true;
              break;
            }
          }
          if(walResult == false)
          {
            cout << "ERROR: Wallet owner " << input << " not found!\n";
            continue;
          }
          inpResult = recvHT->findEarnings(input, sDate, eDate, sTime, eTime);
          {
            if(inpResult == false)
            {
              cout << "ERROR:: Invalid Time and Date Format!\n";
              continue;
            }
          }
        }
        else if(result == 1)
        {
          owner = input.substr(0, pos);
          input = input.substr(pos+1);
          length = input.length();
          bool walResult = false;
          for(int i = 0 ; i < totalWallets; i++)
          {
            if(owner.compare(walletArray[i]->getID()) == 0)//found him
            {
              walResult = true;
              break;
            }
          }
          if(walResult == false)
          {
            cout << "ERROR: Wallet owner " << owner << " not found!\n";
            continue;
          }
          cout << "Getting earnings history!\n";
          cout << "For owner : " << owner << endl;
          counter = 0;
          int timeMode = 0;
          int flagDate = 0;
          int flagTime = 0;
          while(counter < length)
          {
            if(input[counter] == '[')
              timeMode++;
            if(input[counter] == ':')
              flagTime++;
            if(input[counter] == '-')
              flagDate++;
            counter++;
          }
          if(timeMode == 2 && ((flagTime == 2) != (flagDate == 4)))//date or time frames
          {
            counter = 0;
            int lbPos = 0;
            int rbPos = 0;
            if(flagDate == 4)//date frame
            {
              while(counter < length)
              {
                if(input[counter] == '[')
                  lbPos = counter;
                if(input[counter] == ']')
                {
                  rbPos = counter;
                  break;
                }
                counter++;
              }
              sDate = input.substr(lbPos, rbPos+1);
              cout << sDate << " : Start date" << endl;
              eDate = input.substr(rbPos+1);
              cout << eDate << " : End date" << endl;
              inpResult = recvHT->findEarnings(owner,sDate,eDate,sTime,eTime);
              {
                if(inpResult == false)
                {
                  cout << "ERROR:: Invalid Time and Date Format!\n";
                  continue;
                }
              }
            }
            else if(flagTime == 2)//time frame
            {
              while(counter < length)
              {
                if(input[counter] == '[')
                  lbPos = counter;
                if(input[counter] == ']')
                {
                  rbPos = counter;
                  break;
                }
                counter++;
              }
              sTime = input.substr(lbPos, rbPos+1);
              cout << sTime << " : Start time" << endl;
              eTime = input.substr(rbPos+1);
              cout << eTime << " : End time" << endl;
              inpResult = recvHT->findEarnings(owner,sDate,eDate,sTime,eTime);
              {
                if(inpResult == false)
                {
                  cout << "ERROR:: Invalid Time and Date Format!\n";
                  continue;
                }
              }
            }
            else
            {
              cout << "ERROR:: Invalid Time and Date Format!\n";
              continue;
            }
          }
          else if(timeMode == 4)//date and time frames
          {
            cout << "date and time frame!\n";
            counter = 0;
            int lbPos = 0;
            int rbPos = 0;
            int lastFlag = 0;
            while(counter < length)
            {
              if(input[counter] == '[')
                lbPos = counter;
              if(input[counter] == ']')
              {
                rbPos = counter;
                break;
              }
              if(input[counter] == ':')
                lastFlag++;
              counter++;
            }
            if(lastFlag != 1)
            {
              cout << "ERROR:: Wrong Timeframes format!\n";
              continue;
            }
            sTime = input.substr(lbPos, rbPos+1);
            cout << sTime << " : Start time" << endl;
            input = input.substr(rbPos+1);
            lbPos = 0;
            rbPos = 0;
            counter = 0;
            lastFlag = 0;
            while(counter < length)
            {
              if(input[counter] == '[')
                lbPos = counter;
              if(input[counter] == ']')
              {
                rbPos = counter;
                break;
              }
              if(input[counter] == '-')
                lastFlag++;
              counter++;
            }
            if(lastFlag != 2)
            {
              cout << "ERROR:: Wrong Timeframes format!\n";
              continue;
            }
            sDate = input.substr(lbPos, rbPos+1);
            cout << sDate << " : Start date" << endl;
            input = input.substr(rbPos+1);
            lbPos = 0;
            rbPos = 0;
            counter = 0;
            lastFlag = 0;
            while(counter < length)
            {
              if(input[counter] == '[')
                lbPos = counter;
              if(input[counter] == ']')
              {
                rbPos = counter;
                break;
              }
              if(input[counter] == ':')
                lastFlag++;
              counter++;
            }
            if(lastFlag != 1)
            {
              cout << "ERROR:: Wrong Timeframes format!\n";
              continue;
            }
            eTime = input.substr(lbPos, rbPos+1);
            cout << eTime << " : End time" << endl;
            input = input.substr(rbPos+1);
            lbPos = 0;
            rbPos = 0;
            counter = 0;
            lastFlag = 0;
            while(counter < length)
            {
              if(input[counter] == '[')
                lbPos = counter;
              if(input[counter] == ']')
              {
                rbPos = counter;
                break;
              }
              if(input[counter] == '-')
                lastFlag++;
              counter++;
            }
            if(lastFlag != 2)
            {
              cout << "ERROR:: Wrong Timeframes format!\n";
              continue;
            }
            eDate = input.substr(lbPos, rbPos+1);
            cout << sDate << " : End date" << endl;
            inpResult = recvHT->findEarnings(owner,sDate,eDate,sTime,eTime);
            {
              if(inpResult == false)
              {
                cout << "ERROR:: Invalid Time and Date Format!\n";
                continue;
              }
            }
          }
          else
          {
            cout << "ERROR:: Wrong Timeframes format!\n";
            continue;
          }
        }
        else//wrong data
        {
          cout << "ERROR:: Invalid data!\n";
          continue;
        }
      }
      else if(command.compare("/findPayments") == 0)
      {
        input = input.substr(1);//skip the space
        counter = 0;
        int result = 0;
        pos = 0;
        length = input.length();
        string sDate = "N/A";//starting date
        string eDate = "N/A";//end date
        string sTime = "N/A";//starting time
        string eTime = "N/A";//end time
        bool inpResult = false;
        while(counter < length)
        {
          if(input[counter] == ' ')
          {
            result++;
            pos = counter;
          }
          counter++;
        }
        if(result == 0)//get all the history
        {
          bool walResult = false;
          for(int i = 0 ; i < totalWallets; i++)
          {
            if(input.compare(walletArray[i]->getID()) == 0)//found him
            {
              walResult = true;
              break;
            }
          }
          if(walResult == false)
          {
            cout << "ERROR: Wallet owner " << input << " not found!\n";
            continue;
          }
          inpResult = sendHT->findPayments(input, sDate, eDate, sTime, eTime);
          {
            if(inpResult == false)
            {
              cout << "ERROR:: Invalid Time and Date Format!\n";
              continue;
            }
          }
        }
        else if(result == 1)
        {
          owner = input.substr(0, pos);
          input = input.substr(pos+1);
          length = input.length();
          bool walResult = false;
          for(int i = 0 ; i < totalWallets; i++)
          {
            if(owner.compare(walletArray[i]->getID()) == 0)//found him
            {
              walResult = true;
              break;
            }
          }
          if(walResult == false)
          {
            cout << "ERROR: Wallet owner " << owner << " not found!\n";
            continue;
          }
          cout << "Getting payments history!\n";
          cout << "For owner : " << owner << endl;
          counter = 0;
          int timeMode = 0;
          int flagDate = 0;
          int flagTime = 0;
          while(counter < length)
          {
            if(input[counter] == '[')
              timeMode++;
            if(input[counter] == ':')
              flagTime++;
            if(input[counter] == '-')
              flagDate++;
            counter++;
          }

          if(timeMode == 2 && ((flagTime == 2) != (flagDate == 4)))//date or time frames
          {
            counter = 0;
            int lbPos = 0;
            int rbPos = 0;
            if(flagDate == 4)//date frame
            {
              while(counter < length)
              {
                if(input[counter] == '[')
                  lbPos = counter;
                if(input[counter] == ']')
                {
                  rbPos = counter;
                  break;
                }
                counter++;
              }
              sDate = input.substr(lbPos, rbPos+1);
              cout << sDate << " : Start date" << endl;
              eDate = input.substr(rbPos+1);
              cout << eDate << " : End date" << endl;
              inpResult = sendHT->findPayments(owner,sDate,eDate,sTime,eTime);
              {
                if(inpResult == false)
                {
                  cout << "ERROR:: Invalid Time and Date Format!\n";
                  continue;
                }
              }
            }
            else if(flagTime == 2)//time frame
            {
              while(counter < length)
              {
                if(input[counter] == '[')
                  lbPos = counter;
                if(input[counter] == ']')
                {
                  rbPos = counter;
                  break;
                }
                counter++;
              }
              sTime = input.substr(lbPos, rbPos+1);
              cout << sTime << " : Start time" << endl;
              eTime = input.substr(rbPos+1);
              cout << eTime << " : End time" << endl;
              inpResult = sendHT->findPayments(owner,sDate,eDate,sTime,eTime);
              {
                if(inpResult == false)
                {
                  cout << "ERROR:: Invalid Time and Date Format!\n";
                  continue;
                }
              }
            }
            else
            {
              cout << "ERROR:: Invalid Time and Date Format!\n";
              continue;
            }
          }
          else if(timeMode == 4)//date and time frames
          {
            cout << "date and time frame!\n";
            counter = 0;
            int lbPos = 0;
            int rbPos = 0;
            int lastFlag = 0;
            while(counter < length)
            {
              if(input[counter] == '[')
                lbPos = counter;
              if(input[counter] == ']')
              {
                rbPos = counter;
                break;
              }
              if(input[counter] == ':')
                lastFlag++;
              counter++;
            }
            if(lastFlag != 1)
            {
              cout << "ERROR:: Wrong Timeframes format!\n";
              continue;
            }
            sTime = input.substr(lbPos, rbPos+1);
            cout << sTime << " : Start time" << endl;
            input = input.substr(rbPos+1);
            lbPos = 0;
            rbPos = 0;
            counter = 0;
            lastFlag = 0;
            while(counter < length)
            {
              if(input[counter] == '[')
                lbPos = counter;
              if(input[counter] == ']')
              {
                rbPos = counter;
                break;
              }
              if(input[counter] == '-')
                lastFlag++;
              counter++;
            }
            if(lastFlag != 2)
            {
              cout << "ERROR:: Wrong Timeframes format!\n";
              continue;
            }
            sDate = input.substr(lbPos, rbPos+1);
            cout << sDate << " : Start date" << endl;
            input = input.substr(rbPos+1);
            lbPos = 0;
            rbPos = 0;
            counter = 0;
            lastFlag = 0;
            while(counter < length)
            {
              if(input[counter] == '[')
                lbPos = counter;
              if(input[counter] == ']')
              {
                rbPos = counter;
                break;
              }
              if(input[counter] == ':')
                lastFlag++;
              counter++;
            }
            if(lastFlag != 1)
            {
              cout << "ERROR:: Wrong Timeframes format!\n";
              continue;
            }
            eTime = input.substr(lbPos, rbPos+1);
            cout << eTime << " : End time" << endl;
            input = input.substr(rbPos+1);
            lbPos = 0;
            rbPos = 0;
            counter = 0;
            lastFlag = 0;
            while(counter < length)
            {
              if(input[counter] == '[')
                lbPos = counter;
              if(input[counter] == ']')
              {
                rbPos = counter;
                break;
              }
              if(input[counter] == '-')
                lastFlag++;
              counter++;
            }
            if(lastFlag != 2)
            {
              cout << "ERROR:: Wrong Timeframes format!\n";
              continue;
            }
            eDate = input.substr(lbPos, rbPos+1);
            cout << sDate << " : End date" << endl;
            inpResult = sendHT->findPayments(owner,sDate,eDate,sTime,eTime);
            {
              if(inpResult == false)
              {
                cout << "ERROR:: Invalid Time and Date Format!\n";
                continue;
              }
            }
          }
          else
          {
            cout << "ERROR:: Wrong Timeframes format!\n";
            continue;
          }
        }
        else//wrong data
        {
          cout << "ERROR:: Invalid data!\n";
          continue;
        }
      }
      else if(command.compare("/walletStatus") == 0)
      {
        string walletID = input.substr(1);//skip the space char
        //cout << "read:" << walletID << endl;
        bool result = false;
        for(int i = 0; i < totalWallets; i++)
        {
          if(walletID.compare(walletArray[i]->getID()) == 0)//found it
          {
            result = true;
            cout << walletID << "'s Wallet Balance : ";
            cout << walletArray[i]->getBalance() << "$\n";
            break;
          }
        }
        if(result == false)
          cout << "ERROR: Wallet owner " << walletID << " not found!\n";
      }
      else if(command.compare("/bitCoinStatus") == 0)
      {
        bool inputResult = isNumber(input.substr(1));
        if(inputResult == false)
        {
          cout << "ERROR: BTC id must be a positive integer!\n";
          continue;
        }
        int BTCid = stoi(input.substr(1), nullptr, 10);
        //cout << "read:" << BTCid << endl;
        bool result = false;
        for(int i = 0; i < totalCoins; i++)
        {
          if(rootArray[i]->getCoinID() == BTCid)//found
          {
            int txsInvolved = 0;
            int unitsUnspent = coinVal;
            result = true;
            txsInvolved = sendHT->searchBTCTxsInv(BTCid);
            //rootArray[i]->root->searchBTCTxsInv(txsInvolved);//faster but lame
            rootArray[i]->root->searchBTCUnspent(unitsUnspent,
                                                 rootArray[i]->root->getID());
            cout << "BTC ID : " << BTCid << endl;
            cout << "Participated in : " << txsInvolved << " transactions\n";
            cout << "Units still unspent : " << unitsUnspent << "$\n";
            break;
          }
        }
        if(result == false)//coin not found
          cout << "ERROR: BTC with id " << BTCid << " not found!\n";
      }
      else if(command.compare("/traceCoin") == 0)
      {
        bool inputResult = isNumber(input.substr(1));
        if(inputResult == false)
        {
          cout << "ERROR: BTC id must be a positive integer!\n";
          continue;
        }
        int BTCid = stoi(input.substr(1), nullptr, 10);
        //cout << "read:" << BTCid << endl;
        bool result = sendHT->traceCoinHistory(BTCid);//choosing one of them
        if(result == false)
          cout << "ERROR: History of BTC with id " << BTCid << " not found!\n";
      }
      else//error data
        cout << "ERROR: Unknown Command given : " << command << endl;
    }   
  }
  delete[] ownerArray;
  delete recvHT;
  delete sendHT;
  for(int i = 0; i < totalWallets; i++)
    delete walletArray[i];
  delete[] walletArray;
  for(int i = 0; i < totalCoins; i++)
      delete rootArray[i];
  delete[] rootArray;
  delete BTCList;
  delete[] txIDsArray;
  delete[] argPos;
  return 0;
}