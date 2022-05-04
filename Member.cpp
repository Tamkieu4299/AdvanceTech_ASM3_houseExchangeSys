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
	long creditPoints = 500;
	double occupierRatingScore;
	House *houseForLive;
	House *houseForOwn;
	long occupyTimes;
	bool isAdmin;
	vector<Request*> requests; 

public:
	// Constructor
	Member(){
		this->occupyTimes=0;
	};

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

	void setHouseForLive(House* houseForLive){
		this->houseForLive=houseForLive;
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

	void setRequests(vector<Request*> requests){
		this->requests=requests;
	}

	vector<Request*> getRequests(){
		return this->requests;
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

	// Show all requests for the owner of his house
	void showRequests(){
		vector<Request*> requests = this->getRequests();
		if(requests.size()==0) {
			cout<<"You don't have any request !"<<endl;
			return;
		}
		cout<<"All requests for you: "<<endl;
		int index=0;
		for(Request *request: this->getRequests()){
			cout<<"Id: "<<index<<" | User: "<<request->getRequestMember()->getUsername()<<" | From: "<<request->getStart()<<" | To: "<<request->getEnd()<<endl;
		}
	}

	// Owner check the requests for accept
	void acceptRequest(int index){
		vector<Request*> allRequests = this->getRequests();

		if(index>=allRequests.size()){
			cout<<"Invalid choice !"<<endl;
			return;
		}

		Request *request = allRequests[index];
		if(this->getHouseForOwn()->isFree(request->getStart(), request->getEnd())){
			cout<<"Successfully accept request from user: "<<request->getRequestMember()->getUsername()<<" | From: "<<request->getStart()<<" | To: "<<request->getEnd()<<endl;
			allRequests.erase(allRequests.begin()+index);

			this->getHouseForOwn()->setStartDate(request->getStart());
			this->getHouseForOwn()->setEndDate(request->getEnd());

			this->showRequests();
		}
		else cout<<"Your house will not be free on this period ! Please check again"<<endl;
	}
};


