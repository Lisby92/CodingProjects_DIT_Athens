#include "helpers.hpp"
using std::cout;
using std::string;
using std::ifstream;

int countFileLines(const string &filename) {
    int numberOfLines = 0;
    string line;
    ifstream file(filename);
    while (getline(file, line))
        numberOfLines++;
    return numberOfLines;
}

int countBiggestLine(const string &filename) {
    int biggestLine = -1;
    int curSize;
    string line;
    ifstream file(filename);
    while (getline(file, line)) {
        curSize = line.length();
        if (curSize > biggestLine)
            biggestLine = curSize;
    }
    return biggestLine;
}

string getLine(const string &filename, const int &line_number) {
    ifstream file(filename);
    string str;
    int counter = 0;
    while (getline(file, str)) {
        if(counter == line_number)
            return str;
        counter++;
    }
    return NULL;
}

int requestRandLineIndex(const int &total_lines) {
    return rand() % total_lines;
}