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
        for (Member *member : users)
        {
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
            cout << member->getHouseForOwn()->countDays(request->getStart(), request->getEnd()) << endl;
            this->getMemberByUsername(request->getRequestUsername())->setCreditPoints(this->getMemberByUsername(request->getRequestUsername())->getCreditPoints() - member->getHouseForOwn()->countDays(request->getStart(), request->getEnd()) * member->getHouseForOwn()->getConsumingPoints());

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

int main()
{
    System appSys;

    // Member *mem2 = appSys.registerAccount();

    // appSys.users.push_back(mem2);

    // cout<<mem2->getUsername()<<endl;

    // mem1->setAvailablePeriod("2022/04/27", "2022/05/27");
    // cout<<"a"<<endl;

    // cout<<mem1->getHouseForOwn()->getAvailablePeriodStart()<<endl;
    // cout<<"a"<<endl;

    // mem1->getHouseForOwn()->setConsumingPoints(30);
    // cout<<"a"<<endl;

    // vector<House*> availableHousesForMember = appSys.availableHousesForMember(mem2, "2022/04/27", "2022/04/29", "HCM");
    // for(House *house: availableHousesForMember){
    //     cout<<"Location: "<<house->getLocation()<< " | Description: "<<house->getDescription()<<endl;
    // }

    // appSys.sendRequest(mem2->getUsername(), availableHousesForMember, 0,"2022/04/27", "2022/04/29");

    // // appSys.showRequests(mem1);
    // appSys.acceptRequest(mem1, 0);

    // cout<<mem2->getCreditPoints()<<endl;
    bool check = false;
    string choice;

    while (!check)
    {
        cout << "EEET2482/COSC2082 ASSIGNMENT" << endl
             << "VACATION HOUSE EXCHANGE APPLICATION" << endl
             << "\n"
             << "Instructor: Mr. Linh Tran" << endl
             << "Group: Group 11" << endl
             << "s3891528, Ma Phu Dien" << endl
             << "s3922087, Kieu Cong Tam" << endl
             << "s3915468, Nguyen Xuan Thanh" << endl
             << "s3754105, Nguyen Dang Lam Phuong" << endl
             << "\n"
             << "Use the app as 1. Guest   2. Member   3. Admin" << endl
             << "Enter your choice: " << endl;
        cin >> choice;
        if ((choice.size() > 1) | (choice.size() < 1))
        {
            cout << "Invalid Input! Please enter your choice again" << endl;
            cout << "\n********************************************\n";
        }
        else if (("1" > choice | choice > "3") | (!isdigit(choice[0])))
        {
            cout << "Invalid Input! Please enter your choice again" << endl;
            cout << "\n********************************************\n";
        }
        else
            check = true;
    }

    if (choice == "1")
    {
        check = false;
        while (!check)
        {
            cout << "\nRegist an account? Y/N" << endl;
            cin >> choice;
            if (choice == "Y" | choice == "y")
            {
                Member *mem1 = appSys.registerAccount();
                appSys.users.push_back(mem1);
                check = true;
            }
            else if (choice == "N" | choice == "n")
                check = true;
            if (!check)
                cout << "Invalid Input! Please enter your choice again\n";
        }
    }

    // while (choice != 0)
    // {
    //     cout << "\nThis is your menu:" << endl
    //          << "0. Exit" << endl
    //          << "1. List/Unlist available occupied houses" << endl
    //          << "2. Search for available suitable houses" << endl
    //          << "3. Request to occupy" << endl
    //          << "4. View requests" << endl
    //          << "5. Rate house" << endl
    //          << "6. Rate occupier" << endl
    //          << "7. View information" << endl
    //          << "Enter your choice: " << endl;
    //     cin >> choice;
    //     if ((choice < 0) | (choice > 7) | (!isdigit(choice)))
    //     {
    //         cout << "Invalid Input! Please enter your choice again" << endl;
    //         cout << "\n";
    //         continue;
    //     }
    // }

    return 0;
}
