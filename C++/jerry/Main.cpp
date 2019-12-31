#include <cstdlib>
#include "List.hpp"
using namespace std;

int main (int argc, char** argv)

{
  List * myList = new List();
  delete myList;
  cout << "Please give 5 rows x 5 integers each!\n"
  counter = 0;
  int number = 1;
  int row = 1;
  int col = 1;
  while(counter < 26)
  {
    if(counter % 5 == 0 || counter == 1)
    {
      myList->addHeadNode(row);
      row++;
      for(int i = 0; i < 5; i++)
      {
        cin >> number;
        cout << "I read just " << number << endl;
        if(number != 0)
        {
          myList->head->addNode(col, number);
        }
        col++;
        counter++;
      }
    }
  }

  return 0;

}