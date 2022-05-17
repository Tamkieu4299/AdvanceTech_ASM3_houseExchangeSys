#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <time.h>
#include "Request.cpp"

using namespace std;

tm stringToTime(string str){
    const char *cstr = str.c_str();
    tm tmStr;
    sscanf(cstr,"%4d/%2d/%2d",&tmStr.tm_year,&tmStr.tm_mon,&tmStr.tm_mday);
    return tmStr;
}
bool compareSmallerTime(tm t1, tm t2){
    if(t1.tm_year>t2.tm_year) return false;
	else if(t1.tm_year=t2.tm_year){
		if(t1.tm_mon>t2.tm_mon) return false;
		else if(t1.tm_mon=t2.tm_mon){
			if(t1.tm_mday>t2.tm_mday) return false;
		}
	}
	return true;
}
bool compareBiggerTime(tm t1, tm t2){
    if(t1.tm_year<t2.tm_year) return false;
	else if(t1.tm_year=t2.tm_year){
		if(t1.tm_mon<t2.tm_mon) return false;
		else if(t1.tm_mon=t2.tm_mon){
			if(t1.tm_mday<t2.tm_mday) return false;
		}
	}
	return true;
}
class House {
private:
	string location;
	string descripton;
	double houseRatingScrore;
	string startDate;
	string endDate;
	string availablePeriodStart;
	string availablePeriodEnd;
	double consumingPoints; // per day
	double requiredMinOccupierRating;
	long usedTimes;
	long usedDays;
	vector<string> comments;
	vector<Request*> requests; 

public:
	House(){};

	House(string location, string descripton){
		this->location=location;
		this->descripton=descripton;
		this->usedTimes=0;
		this->requiredMinOccupierRating=0;
		this->houseRatingScrore=0;
	}

	string getLocation(){
		return this->location;
	}

	void setLocation(string location){
		this->location=location;
	}

	string getDescription(){
		return this->descripton;
	}

	void setDescription(string descripton){
		this->descripton=descripton;
	}

	double getHouseRatingScore(){
		return this->houseRatingScrore;
	}

	void setHouseRatingScore(double houseRatingScore){
		this->houseRatingScrore=houseRatingScore;
	}

	string getStartDate(){
		return this->startDate;
	}

	void setStartDate(string startDate){
		this->startDate=startDate;
	}

	string getEndDate(){
		return this->endDate;
	}

	void setEndDate(string endDate){
		this->endDate=endDate;
	}

	void setAvailablePeriodStart(string availablePeriodStart){
		this->availablePeriodStart=availablePeriodStart;
	}

	string getAvailablePeriodEnd(){
		return this->availablePeriodEnd;
	}

	void setAvailablePeriodEnd(string availablePeriodEnd){
		this->availablePeriodEnd=availablePeriodEnd;
	}

	string getAvailablePeriodStart(){
		return this->availablePeriodStart;
	}

	double getConsumingPoints(){
		return this->consumingPoints;
	}

	// void newConsumingPoints(){
	// 	this->setConsumingPoints(this->getUsedTimes()/this->getUsedDays());
	// }

	void setConsumingPoints(double consumingPoints){
		this->consumingPoints=consumingPoints;
	}

	long getUsedTimes(){
		return this->usedTimes;
	}

	void setUsedTimes(){
		this->usedTimes++;
	}

	double getRequiredMinOccupierRating(){
		return this->requiredMinOccupierRating;
	}

	void setRequiredMinOccupierRating(double requiredMinOccupier){
		this->requiredMinOccupierRating=requiredMinOccupier;
	}

	void setUsedDays(long usedDays){
		this->usedDays=usedDays;
	}

	long getUsedDays(){
		return this->usedDays;
	}

	void setComments(vector<string> comments){
		this->comments=comments;
	}

	vector<string> getComments(){
		return this->comments;
	}

	void setRequests(vector<Request*> requests){
		this->requests=requests;
	}

	vector<Request*> getRequests(){
		return this->requests;
	}

	bool isFree(string start, string end){
		if(this->getAvailablePeriodStart()=="" && this->getAvailablePeriodEnd()=="" && this->getUsedTimes()==0) 
			return true;

		tm tmAvaiStart = stringToTime(this->getAvailablePeriodStart());
		tm tmAvaiEnd = stringToTime(this->getAvailablePeriodEnd());

		tm tmStart = stringToTime(start);
		tm tmEnd = stringToTime(end);

		tm tmHouseStart = stringToTime(this->getStartDate());
		tm tmHouseEnd = stringToTime(this->getEndDate());

		if(compareSmallerTime(tmEnd, tmAvaiEnd) && compareBiggerTime(tmStart, tmAvaiStart))
			if(compareSmallerTime(tmEnd, tmHouseStart) || compareBiggerTime(tmStart, tmHouseEnd))
				return true;
		return false;
	}

	// Count the days between the period for calculating consuming points in a period
	long countDays(string start, string end){
		stringstream ss(start + "-" + end);
		int year, month, day;
		char hyphen;
	
		// Parse the first date into seconds
		ss >> year >> hyphen >> month >> hyphen >> day;
		struct tm starttm = { 0, 0, 0, day, month - 1, year - 1900 };
		time_t ttStart = mktime(&starttm);
	
		// Parse the second date into seconds
		ss >> hyphen >> year >> hyphen >> month >> hyphen >> day;
		struct tm endtm = { 0, 0, 0, day, month - 1, year - 1900 };
		time_t ttEnd = mktime(&endtm);
		
		// Find out the difference and divide it by 86400 to get the number of days
		return abs(ttEnd - ttStart) / 86400;
	}

	friend class Member;
	
};
// int main(int argc, char const *argv[])
// {
// 	House house = House();
	
	
// 	cout<<house.countDays("2022/05/18", "2022/05/19")<<endl;
	
// 	return 0;
// }



