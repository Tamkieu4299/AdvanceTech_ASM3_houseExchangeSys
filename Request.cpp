#include <iostream>
#include <stdio.h>

using namespace std;

class Request{
private:
    string start;
    string end;
    string requestUsername;

public:
    Request(){}

    string getRequestUsername(){
        return this->requestUsername;
    }

    void setRequestUsername(string requestUsername){
        this->requestUsername=requestUsername;
    }

    string getStart(){
        return this->start;
    }

    void setStart(string start){
        this->start=start;
    }

    string getEnd(){
        return this->end;
    }

    void setEnd(string end){
        this->end=end;
    }

};