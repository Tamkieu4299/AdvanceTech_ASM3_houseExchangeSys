#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <time.h>
#include "Request.cpp"

using namespace std;

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

	double getHouseRatingScrore(){
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

	// void newUsedDays(string start, string end){
	// 	long curUsedDays = this->getUsedDays();
	// 	this->setUsedDays(curUsedDays+countDays(start,end));
	// }

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

	// Check if the house is free in the period
	bool isFree(string start, string end){
	
		const char *availbleStart = this->getAvailablePeriodStart().c_str();
		const char *availbleEnd = this->getAvailablePeriodEnd().c_str();
		tm tmAvailableStart;
		tm tmAvailableEnd;
		sscanf(availbleStart,"%4d/%2d/%2d",&tmAvailableStart.tm_year,&tmAvailableStart.tm_mon,&tmAvailableStart.tm_mday);
		sscanf(availbleEnd,"%4d/%2d/%2d",&tmAvailableEnd.tm_year,&tmAvailableEnd.tm_mon,&tmAvailableEnd.tm_mday);

		const char *startDate = start.c_str();
		const char *endDate = end.c_str();
		tm tmStart;
		tm tmEnd;
		sscanf(startDate,"%4d/%2d/%2d",&tmStart.tm_year,&tmStart.tm_mon,&tmStart.tm_mday);
		sscanf(endDate,"%4d/%2d/%2d",&tmEnd.tm_year,&tmEnd.tm_mon,&tmEnd.tm_mday);

		const char *houseStart = this->startDate.c_str();
		const char *houseEnd = this->endDate.c_str();
		tm tmHouseStart;
		tm tmHouseEnd;
		sscanf(houseStart,"%4d/%2d/%2d",&tmHouseStart.tm_year,&tmHouseStart.tm_mon,&tmHouseStart.tm_mday);
		sscanf(houseEnd,"%4d/%2d/%2d",&tmHouseEnd.tm_year,&tmHouseEnd.tm_mon,&tmHouseEnd.tm_mday);

		//End to HouseStart
		if(tmEnd.tm_year<=tmAvailableStart.tm_year && tmEnd.tm_mon<=tmAvailableStart.tm_mon && tmEnd.tm_mday<=tmAvailableStart.tm_mday 
			|| tmAvailableEnd.tm_year<=tmStart.tm_year && tmAvailableEnd.tm_mon<=tmStart.tm_mon && tmAvailableEnd.tm_mday<=tmStart.tm_mday)
				if(tmEnd.tm_year<=tmHouseStart.tm_year && tmEnd.tm_mon<=tmHouseStart.tm_mon && tmEnd.tm_mday<=tmHouseStart.tm_mday 
					|| tmHouseEnd.tm_year<=tmStart.tm_year && tmHouseEnd.tm_mon<=tmStart.tm_mon && tmHouseEnd.tm_mday<=tmStart.tm_mday)
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
};

// int main(int argc, char const *argv[])
// {
// 	House house = House();
// 	house.setAvailablePeriodStart("2022/04/27");
// 	cout<<house.getAvailablePeriodStart()<<endl;
// 	return 0;
// }



