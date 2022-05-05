#include <iostream>
#include <stdio.h>
#include <vector>

#include "Member.cpp"

using namespace std;

class System{
public:
    vector<Member> users;

    // Constructor
    System(){};

    Member getMemberByUsername(string username){
        Member res;
        for(Member member: users){
            if(member.getUsername()==username) res=member;
        }
        return res;
    }

    // Get all the houses on the system
    vector<House*> getAllHouses(){
        vector<House*> records;
        for(Member member: users){
            records.push_back(member.getHouseForOwn());
        }
        return records;
    };

    // Show list of houses to be used in a the period
	vector<House> availableHouses(string start, string end){
        vector<House*> allHouses = this->getAllHouses();
		vector<House> availableRecords;
		for(House *house: allHouses)
			if(house->isFree(start,end)) {
				availableRecords.push_back(*house);
			}
        return availableRecords;
	}

    // Show list of houses for member
    vector<House*> availableHousesForMember(Member *member, string start, string end, string city){
        vector<House*> allHouses = this->getAllHouses();
        vector<House*> availableRecords;
        for(House *house: allHouses)
            if(house->isFree(start,end) && house->getLocation()==city && house->getRequiredMinOccupierRating()<=member->getOccupierRatingScore()){
                long totalCost = house->countDays(start, end)*house->getConsumingPoints();
                if(member->getCreditPoints()>=totalCost) availableRecords.push_back(house); 
            }
        return availableRecords;
    }

    // Send request to owner of the house
	void sendRequest(string username, vector<House*> availableHouses, int index, string start, string end){
		if(index>=availableHouses.size()){
			cout<<"Invalid choice !"<<endl;
			return;
		}
		House *house = availableHouses[index];
		
        Request request = Request();
        request.setRequestUsername(username);
        request.setStart(start);
        request.setEnd(end);

        vector<Request*> requests = house->getRequests();
        requests.push_back(&request);
        house->setRequests(requests);
	} 

    // Show all requests for the owner of his house
	void showRequests(Member member){
		vector<Request*> requests = member.getHouseForOwn()->getRequests();
		if(requests.size()==0) {
			cout<<"You don't have any request !"<<endl;
			return;
		}
		cout<<"All requests for you: "<<endl;
		int index=0;
		for(Request *request: member.getHouseForOwn()->getRequests()){
			cout<<"Id: "<<index<<" | User: "<<this->getMemberByUsername(request->getRequestUsername()).getUsername()<<" | From: "<<request->getStart()<<" | To: "<<request->getEnd()<<endl;
		}
	}

    // Owner check the requests for accept
	void acceptRequest(Member member, int index){
		vector<Request*> allRequests = member.getHouseForOwn()->getRequests();

		if(index>=allRequests.size()){
			cout<<"Invalid choice !"<<endl;
			return;
		}

		Request *request = allRequests[index];
		if(member.getHouseForOwn()->isFree(request->getStart(), request->getEnd())){
			cout<<"Successfully accept request from user: "<<this->getMemberByUsername(request->getRequestUsername()).getUsername()<<" | From: "<<request->getStart()<<" | To: "<<request->getEnd()<<endl;
			allRequests.erase(allRequests.begin()+index);

			member.getHouseForOwn()->setStartDate(request->getStart());
			member.getHouseForOwn()->setEndDate(request->getEnd());
			this->getMemberByUsername(request->getRequestUsername()).setCreditPoints(this->getMemberByUsername(request->getRequestUsername()).getCreditPoints() - member.getHouseForOwn()->countDays(request->getStart(), request->getEnd())*member.getHouseForOwn()->getConsumingPoints());

			this->showRequests(member);
		}
		else cout<<"Your house will not be free on this period ! Please check again"<<endl;
	}

    // Registration for non-member
    void registerAccount(){
        cout << "-----Register account--------\n";
        cout << "Register as 1->Admin  2->Member: ";
        int AdminOrUser;
        cin >> AdminOrUser;
        Member newMember = Member();

        if(AdminOrUser==1) newMember.setIsAdmin(true);
        else newMember.setIsAdmin(false);

        cout << "Enter username: ";
        string username;
        cin >> username;
        newMember.setUsername(username);
        
        cout << "Enter password: ";
        string password;
        cin >> password;
        newMember.setPassword(password);

        cout << "Enter fullname: ";
        string fullname;
        cin >>  fullname;
        newMember.setFullname(fullname);

        cout << "Enter Phone: ";
        string phone;
        cin >> phone;
        newMember.setPhone(phone);

        cout << "Enter house ? Press 1->Yes  2->No : ";
        int enterHouse;
        cin >> enterHouse;
        if(enterHouse==1){
            cout << "Enter Location: ";
            string location;
            cin >> location;

            cout << "Enter Description: ";
            string description;
            cin >> description;

            newMember.setHouseForOwn((House(location, description)));
        }

        // Store account into the system (if the registration is successful)
        this->users.push_back(newMember);
        cout << "All accounts in the system: \n";
        for (Member eachUser: this->users)
            cout << "Name = " << eachUser.getUsername() << ", pwd = " << eachUser.getPassword()<<endl;
    }
};

int main(){
    System appSys;

    appSys.registerAccount();
    appSys.registerAccount();
    

    Member mem1 = appSys.users[0];
    Member mem2 = appSys.users[1];

    mem1.setAvailablePeriod("2022/04/27", "2022/05/27");
    mem1.getHouseForOwn()->setConsumingPoints(30);
    mem1.getHouseForOwn()->setLocation("HCM");

    vector<House*> availableHousesForMember = appSys.availableHousesForMember(&mem2, "2022/04/27", "2022/04/29", "HCM");
    for(House *house: availableHousesForMember){
        cout<<"Location: "<<house->getLocation()<< " | Description: "<<house->getDescription()<<endl;
    }

    appSys.sendRequest(mem2.getUsername(), availableHousesForMember, 0,"2022/04/27", "2022/04/29");

    appSys.showRequests(mem1);
    appSys.acceptRequest(mem1, 0);

    cout<<mem2.getCreditPoints()<<endl;

    
    return 0;
}

