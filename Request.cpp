#include <iostream>
#include <stdio.h>
#include "House.cpp"
#include "Member.cpp"

using namespace std;

class Request{
private:
    House *house;
    string start;
    string end;
    Member *requestMember;

public:
    Request(){}

    House* getHouse(){
        return this->house;
    }

    void setHouse(House *house){
        this->house=house;
    }

    Member* getRequestMember(){
        return this->requestMember;
    }

    void setRequestMember(Member *requestMember){
        this->requestMember=requestMember;
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