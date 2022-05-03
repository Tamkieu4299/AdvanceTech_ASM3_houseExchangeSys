#include <iostream>
#include <stdio.h>
#include <vector>

#include "House.cpp"
#include "Member.cpp"

using namespace std;

class System{
public:
    vector<Member> users;

    // Constructor
    System(){};

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
};

int main(){
    System appSys;

    // Non-member do registration
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

        newMember.setHouseForOwn(&House(location, description));
    }

    //Store account into the system (if the registration is successful)
    appSys.users.push_back(newMember);
    cout << "All accounts in the system: \n";
    // for (Member eachUser: appSys.users)
    //     cout << "Name = " << eachUser.getUsername() << ", pwd = " << eachUser.getPassword();
    
    return 0;
}

