#include <iostream>
#include <stdio.h>
#include <cstring>
#include "House.cpp"

using namespace std;

class Member{
private:
	string username;
	string fullname;
	string phone;
	string password;
	long creditPoints;
	double occupierRatingScore;
	House *houseForLive;
	House *houseForOwn;
	long occupyTimes;
	bool isAdmin;

public:
	// Constructor
	Member(){
		this->occupyTimes=0;
		this->occupierRatingScore=0;
		this->creditPoints=500;
	};

	Member(string username, string password, string fullname, string phone){
		this->username=username;
		this->password=password;
		this->fullname=fullname;
		this->phone=phone;
		this->occupyTimes=0;
		this->creditPoints=500;
	}

	// Getter - Setter
	string getUsername(){
		return this->username;
	}

	void setUsername(string username){
		this->username=username;
	}

	string getFullname(){
		return this->fullname;
	}

	void setFullname(string fullname){
		this->fullname=fullname;
	}

	string getPhone() {
		return this->phone;
	}

	void setPhone(string phone){
		this->phone=phone;
	}

	string getPassword(){
		return this->password;
	}

	void setPassword(string password){
		this->password=password;
	}

	long getCreditPoints(){
		return this->creditPoints;
	}

	void setCreditPoints(long creditPoints){
		this->creditPoints=creditPoints;
	}

	double getOccupierRatingScore(){
		return this->occupierRatingScore;
	}

	void setOccupierRatingScore(double occupierRatingScore){
		this->occupierRatingScore=occupierRatingScore;
	}

	House* getHouseForLive(){
		return this->houseForLive;
	}	

	void setHouseForLive(House houseForLive){
		this->houseForLive=&houseForLive;
	}

	House* getHouseForOwn(){
		return this->houseForOwn;
	}	

	void setHouseForOwn(House *houseForOwn){
		this->houseForOwn=houseForOwn;
	}

	long getOccupyTimes(){
		return this->occupyTimes;
	}

	void setOccupyTimes(){
		this->occupyTimes++;
	}

	bool getIsAdmin(){
		return this->isAdmin;
	}

	void setIsAdmin(bool isAdmin){
		this->isAdmin=isAdmin;
	}

	// Rate the House while living
	void ratingHouse(double rating, House house){
		double newRating = (house.getHouseRatingScrore()+rating)/house.getUsedTimes();
		house.setHouseRatingScore(newRating);
	}

	// Rate the Occupier while renting
	void ratingOccupier(double rating, Member member){
		double newRating = (member.getOccupierRatingScore()+rating)/member.getOccupyTimes();
		member.setOccupierRatingScore(newRating);
	}

	// Set the period which the owner wants to rent his house
	void setAvailablePeriod(string start, string end){
		this->getHouseForOwn()->setAvailablePeriodStart(start);
		this->getHouseForOwn()->setAvailablePeriodEnd(end);
	}
};

// int main(int argc, char const *argv[]){
// 	Member m1 = Member("kt", "1233", "123", "123");
// 	House house = House("HCM", "Hcm");
// 	m1.setHouseForOwn(house);
// 	m1.getHouseForOwn()->setAvailablePeriodStart("2022/04/22");
// 	cout<<m1.getHouseForOwn()->getAvailablePeriodStart()<<endl;

// 	return 0;
// }



