#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
using std::string;

#ifndef __helpers__
#define __helpers__

int     countFileLines(const string &filename);                  // returns an integer number of lines in input file <filename>
int     countBiggestLine(const string &filename);                // returns an integer with the biggest size of the strings in input file <filename>
int     requestRandLineIndex(const int & total_lines);           // return a random integer in range [0,<total_lines>]
string  getLine(const string &filename, const int &line_number); // return the <line_number>th string of the input file <filename>
void    run(const string &X, const int &K, const int &N, const int &file_lines, const int &biggestLine);

typedef struct timestamp {    // used to hold time instances
    long int sec;
    long int nsec;
} timestamp;

typedef struct client_request {   // used as a shared memory to store client requests to the server
    int       requested_line;     // an integer between 0 and total file lines of the specific line the client requested
    int       client_id;          // the client id that initiated the request
    timestamp time_req_submit;    // a timestamp that contains the time from client requesting until server responding
} client_request;

typedef struct server_response {  // used as a shared memory to store server responses to the client
    char      response_data[100]; // holds the file line data that the client requested // TODO apoti diavasa den gginetai me pointers i mlkia, anarotiemai an ginetai me char[] kai meta na poume megethos
    int       client_id;          // the client id that initiated the request
    timestamp time_req_submit;    // a timestamp that contains the time from client requesting until server responding
} server_response;

typedef client_request  * req_ptr;
typedef server_response * res_ptr;

#endif //__helpers__
