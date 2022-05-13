#include <iostream>
#include <stdio.h>
#include <vector>

#include "Member.cpp"

using namespace std;
using std::string;

class System
{
public:
    vector<Member *> users;

    // Constructor
    System(){};

    Member *getMemberByUsername(string username)
    {
        Member *res;
        for (Member *member : users){
            if (member->getUsername() == username)
                res = member;
        }
        return res;
    }

    // Get all the houses on the system
    vector<House *> getAllHouses()
    {
        vector<House *> records;
        for (Member *member : this->users)
        {
            records.push_back(member->getHouseForOwn());
        }
        return records;
    };

    // Show list of houses to be used in a the period
    vector<House *> availableHouses(string start, string end)
    {
        vector<House *> allHouses = this->getAllHouses();
        vector<House *> availableRecords;
        for (House *house : allHouses)
            if (house->isFree(start, end))
            {
                availableRecords.push_back(house);
            }
        return availableRecords;
    }

    // Show list of houses for member
    vector<House *> availableHousesForMember(Member *member, string start, string end, string city)
    {
        vector<House *> allHouses = this->getAllHouses();
        vector<House *> availableRecords;
        for (House *house : allHouses)
            if (house->isFree(start, end) && house->getLocation() == city && house->getRequiredMinOccupierRating() <= member->getOccupierRatingScore())
            {
                long totalCost = house->countDays(start, end) * house->getConsumingPoints();
                if (member->getCreditPoints() >= totalCost)
                    availableRecords.push_back(house);
            }
        return availableRecords;
    }

    // Send request to owner of the house
    void sendRequest(string username, vector<House *> availableHouses, int index, string start, string end)
    {
        if (index >= availableHouses.size())
        {
            cout << "Invalid choice !" << endl;
            return;
        }
        House *house = availableHouses[index];

        Member *member = this->getMemberByUsername(username);
        if(member->getHouseForLive()!=NULL) {
            cout<<"You have selected a house before !"<<endl;
            return;
        }
        Request *request = new Request();
        request->setRequestUsername(username);
        request->setStart(start);
        request->setEnd(end);

        vector<Request *> requests = house->getRequests();
        requests.push_back(request);
        house->setRequests(requests);
    }

    // Show all requests for the owner of his house
    void showRequests(Member *member)
    {
        vector<Request *> requests = member->getHouseForOwn()->getRequests();
        if (requests.size() == 0)
        {
            cout << "You don't have any request !" << endl;
            return;
        }
        cout << "All requests for you: " << endl;
        int index = 0;
        for (Request *request : member->getHouseForOwn()->getRequests())
        {
            cout << "Id: " << index << " | User: " << this->getMemberByUsername(request->getRequestUsername())->getUsername() << " | From: " << request->getStart() << " | To: " << request->getEnd() << endl;
        }
    }

    // Delete appropriate request
    void delRequests(vector<Request*> &requests, string start, string end){
		tm tmcStart = stringToTime(start);
		tm tmcEnd = stringToTime(end);
        
        vector<int> delIndex;
        for(int i=0; i<requests.size(); i++){
            Request *request = requests[i];
            if(compareSmallerTime(stringToTime(request->getEnd()), tmcEnd) && compareBiggerTime(stringToTime(request->getStart()), tmcStart) 
            ||  compareSmallerTime(stringToTime(request->getEnd()), tmcEnd) && compareBiggerTime(stringToTime(request->getEnd()), tmcStart)
            || compareSmallerTime(stringToTime(request->getStart()), tmcEnd) && compareBiggerTime(stringToTime(request->getStart()), tmcStart)
            )
                delIndex.push_back(i);
        }

        int sub=0;
        for(int index: delIndex){
            int actIndex = index-sub;
            requests.erase(requests.begin()+actIndex);
            sub++;
        }
    }

    // Owner check the requests for accept
    void acceptRequest(Member *member, int index)
    {
        vector<Request *> allRequests = member->getHouseForOwn()->getRequests();

        if (index >= allRequests.size())
        {
            cout << "Invalid choice !" << endl;
            return;
        }

        Request *request = allRequests[index];
        if (member->getHouseForOwn()->isFree(request->getStart(), request->getEnd()))
        {
            cout << "Successfully accept request from user: " << this->getMemberByUsername(request->getRequestUsername())->getUsername() << " | From: " << request->getStart() << " | To: " << request->getEnd() << endl;
            allRequests.erase(allRequests.begin() + index);

            member->getHouseForOwn()->setStartDate(request->getStart());
            member->getHouseForOwn()->setEndDate(request->getEnd());
            // cout << member->getHouseForOwn()->countDays(request->getStart(), request->getEnd()) << endl;

            // Update credits amount of renter
            this->getMemberByUsername(request->getRequestUsername())->setCreditPoints(this->getMemberByUsername(request->getRequestUsername())->getCreditPoints() - member->getHouseForOwn()->countDays(request->getStart(), request->getEnd()) * member->getHouseForOwn()->getConsumingPoints());
            
            // Update house for live of renter
            this->getMemberByUsername(request->getRequestUsername())->setHouseForLive(member->getHouseForOwn());

            // Update occupy times of renter
            this->getMemberByUsername(request->getRequestUsername())->setOccupyTimes();

            // Update used times of house
            member->getHouseForLive()->setUsedTimes();
            
            // Update partner for owner 
            member->setPartner(this->getMemberByUsername(request->getRequestUsername()));

            // Update credit points of the owner
            member->setCreditPoints(member->getHouseForOwn()->countDays(request->getStart(), request->getEnd()) * member->getHouseForOwn()->getConsumingPoints());

            this->showRequests(member);
        }
        else
            cout << "Your house will not be free on this period ! Please check again" << endl;
    }


    // Registration for non-member
    Member *registerAccount()
    {
        cout << "-----Register account--------\n";
        cout << "Register as 1->Admin  2->Member: ";
        int AdminOrUser;
        cin >> AdminOrUser;
        Member *newMember = new Member();

        if (AdminOrUser == 1)
            newMember->setIsAdmin(true);
        else
            newMember->setIsAdmin(false);

        cout << "Enter username: ";
        string username;
        cin >> username;
        newMember->setUsername(username);

        cout << "Enter password: ";
        string password;
        cin >> password;
        newMember->setPassword(password);

        cout << "Enter fullname: ";
        string fullname;
        cin >> fullname;
        newMember->setFullname(fullname);

        cout << "Enter Phone: ";
        string phone;
        cin >> phone;
        newMember->setPhone(phone);

        cout << "Enter house ? Press 1->Yes  2->No : ";
        int enterHouse;
        cin >> enterHouse;
        if (enterHouse == 1)
        {
            cout << "Enter Location: ";
            string location;
            cin >> location;

            cout << "Enter Description: ";
            string description;
            cin >> description;
            // House house = House(location, description);
            // House house_copy = house;
            House *house = new House(location, description);
            newMember->setHouseForOwn(house);
        }
        // Member *ptMember = new Member();
        // ptMember = &newMember;

        return newMember;

        // // Store account into the system (if the registration is successful)
        // this->users.push_back(newMember);
        // cout << "All accounts in the system: \n";
        // cout<<users.size()<<endl;
        // for (Member eachUser: this->users)
        // cout << "Name = " << eachUser.getUsername() << ", pwd = " << eachUser.getPassword()<<endl;
    }
};

// Convert string to time
// tm stringToTime(string str){
//     const char *cstr = str.c_str();
//     tm tmStr;
//     sscanf(cstr,"%4d/%2d/%2d",&tmStr.tm_year,&tmStr.tm_mon,&tmStr.tm_mday);
//     return tmStr;
// }

// // Compare two time - return true if t1<t2
// bool compareTime(tm t1, tm t2){
//     if(t1.tm_year>t2.tm_year) return false;
// 	else if(t1.tm_year=t2.tm_year){
// 		if(t1.tm_mon>t2.tm_mon) return false;
// 		else if(t1.tm_mon=t2.tm_mon){
// 			if(t1.tm_mday>t2.tm_mday) return false;
// 		}
	}
// 	return true;
// }

bool checkChoice(string choice, string start, string end)
{
    if ((choice.size() > 1) | (choice.size() < 1))
    {
        cout << "Invalid Input! Please enter your choice again" << endl;
        cout << "\n********************************************\n";
        return true;
    }
    else if ((start > choice | choice > end) | (!isdigit(choice[0])))
    {
        cout << "Invalid Input! Please enter your choice again" << endl;
        cout << "\n********************************************\n";
        return true;
    }
    return false;
}

<<<<<<< HEAD
// int main()
// {
//     System appSys;
=======
// void viewInfo(bool isAdmin, House *house)
// {
//     if (this.getIsAdmin)
//     {
//         cout << "Your info: " << endl;
//         cout << "\n";
//         cout << "Username:              " << this.username << endl;
//         cout << "Fullname:              " << this.fullname << endl;
//         cout << "Credit Points:         " << this.creditPoints << endl;
//         cout << "Occupier Rating Score: " << this.occupierRatingScore << endl;

//         cout << "Occupy Times:           " << this.occupyTimes;
//     }
// }

// void viewHouse(){
//     cout << "Houses For Live:       " << endl;

//         for (House *house : this.houseForLive)
//         {
//             cout << "Location: " << house->getLocation() << " | Description: " << house->getDescription() << endl;
//             cout << "Rating Score: " << house->getHouseRatingScrore() << " | Consuming Points: " << house->getConsumingPoints() << endl;
//             cout << endl;
//         }
//         cout << "Owned Houses:           " << endl;

//         for (House *house : this.houseForOwn)
//         {
//             cout << "Location: " << house->getLocation() << " | Description: " << house->getDescription() << endl;
//             cout << "Rating Score: " << house->getHouseRatingScrore() << " | Consuming Points: " << house->getConsumingPoints() << endl;
//             cout << endl;
//         }
// }

void Login(vector<Member *> users)
{
    bool check = false;
    while (!check)
    {
        cout << "\nEnter username: ";
        string username;
        cin >> username;

        cout << "\nEnter password: ";
        string password;
        cin >> password;
        for (int i = 0; i < users.size(); i++)
        {

            if ((users[i]->getUsername() == username) & (users[i]->getPassword() == password))
                check = true;
            else
                cout << "\nWrong username or wrong password. Please enter again!" << endl;
        }
    }
}

int main()
{
    System appSys;
>>>>>>> 6a906815eab39482ee7b0c4567e901f8b39cdbf4

//     // Member *mem2 = appSys.registerAccount();

//     // appSys.users.push_back(mem2);

//     // cout<<mem2->getUsername()<<endl;

//     // mem1->setAvailablePeriod("2022/04/27", "2022/05/27");
//     // cout<<"a"<<endl;

//     // cout<<mem1->getHouseForOwn()->getAvailablePeriodStart()<<endl;
//     // cout<<"a"<<endl;

//     // mem1->getHouseForOwn()->setConsumingPoints(30);
//     // cout<<"a"<<endl;

//     // vector<House*> availableHousesForMember = appSys.availableHousesForMember(mem2, "2022/04/27", "2022/04/29", "HCM");
//     // for(House *house: availableHousesForMember){
//     //     cout<<"Location: "<<house->getLocation()<< " | Description: "<<house->getDescription()<<endl;
//     // }

//     // appSys.sendRequest(mem2->getUsername(), availableHousesForMember, 0,"2022/04/27", "2022/04/29");

//     // // appSys.showRequests(mem1);
//     // appSys.acceptRequest(mem1, 0);

//     // cout<<mem2->getCreditPoints()<<endl;
//     bool check = false;
//     string choice;

//     while (true)
//     {
//         cout << "EEET2482/COSC2082 ASSIGNMENT" << endl
//              << "VACATION HOUSE EXCHANGE APPLICATION" << endl
//              << "\n"
//              << "Instructor: Mr. Linh Tran" << endl
//              << "Group: Group 11" << endl
//              << "s3891528, Ma Phu Dien" << endl
//              << "s3922087, Kieu Cong Tam" << endl
//              << "s3915468, Nguyen Xuan Thanh" << endl
//              << "s3754105, Nguyen Dang Lam Phuong" << endl
//              << "\n"
//              << "Use the app as 1. Guest   2. Member   3. Admin" << endl
//              << "Enter your choice: " << endl;
//         cin >> choice;
//         if (!checkChoice(choice, "1", "3"))
//             break;
//     }

<<<<<<< HEAD
//     if (choice == "1")
//     {
//         check = false;
//         while (true)
//         {
//             cout << "\nRegist an account? Y/N" << endl;
//             cin >> choice;
//             if (choice == "Y" | choice == "y")
//             {
//                 Member *mem1 = appSys.registerAccount();
//                 appSys.users.push_back(mem1);
//                 break;
//                 ;
//             }
//             else if (choice == "N" | choice == "n")
//                 break;
//             cout << "Invalid Input! Please enter your choice again\n";
//         }
//     } else if (choice=="2"){

//     } else {
        
//     }
=======
    if (choice == "1")
    {
        check = false;
        while (true)
        {
            cout << "\nRegist an account? Y/N" << endl;
            cin >> choice;
            if (choice == "Y" | choice == "y")
            {
                Member *mem1 = appSys.registerAccount();
                appSys.users.push_back(mem1);
                Login(appSys.users);
                break;
                ;
            }
            else if (choice == "N" | choice == "n")
                break;
            cout << "Invalid Input! Please enter your choice again\n";
        }
    }
    else
        Login(appSys.users);
>>>>>>> 6a906815eab39482ee7b0c4567e901f8b39cdbf4

//     while (choice != "0")
//     {
//         cout << "\nThis is your menu:" << endl
//              << "0. Exit" << endl
//              << "1. List/Unlist available occupied houses" << endl
//              << "2. Search for available suitable houses" << endl
//              << "3. Request to occupy" << endl
//              << "4. View requests" << endl
//              << "5. Rate house" << endl
//              << "6. Rate occupier" << endl
//              << "7. View information" << endl
//              << "8. View the reviews" << endl
//              << "Enter your choice: " << endl;
//         cin >> choice;
//         checkChoice(choice, "0", "7");
//         if (choice == "1")
//         {
//         }
//     }

//     cout << "\nHave A Nice Day!!!" << endl;

<<<<<<< HEAD
//     return 0;
// }

// int main(){

//     System app = System();

//     Request r1 = Request();
//     r1.setStart("2022/04/20");
//     r1.setEnd("2022/04/21");

//     Request r2 = Request();
//     r2.setStart("2022/04/23");
//     r2.setEnd("2022/04/24");

//     vector<Request*> requests;
//     requests.push_back(&r1);
//     requests.push_back(&r2);

//     app.delRequests(requests, "2022/04/21", "2022/04/29" );

//     for(Request *r: requests) cout<<r->getStart()<<endl;
//     return 0;
// }
=======
    return 0;
}
>>>>>>> 6a906815eab39482ee7b0c4567e901f8b39cdbf4
