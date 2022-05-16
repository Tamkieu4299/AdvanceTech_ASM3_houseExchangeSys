#include <iostream>
#include <stdio.h>
#include <vector>

#include "Member.cpp"

using namespace std;
using std::string;

const int MAX_VALID_YR = 9999;
const int MIN_VALID_YR = 1800;

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

        Member *member = this->getMemberByUsername(username);
        if (member->getHouseForLive() != NULL)
        {
            cout << "You have selected a house before !" << endl;
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
    void delRequests(vector<Request *> &requests, string start, string end)
    {
        tm tmcStart = stringToTime(start);
        tm tmcEnd = stringToTime(end);

        vector<int> delIndex;
        for (int i = 0; i < requests.size(); i++)
        {
            Request *request = requests[i];
            if (compareSmallerTime(stringToTime(request->getEnd()), tmcEnd) && compareBiggerTime(stringToTime(request->getStart()), tmcStart) || compareSmallerTime(stringToTime(request->getEnd()), tmcEnd) && compareBiggerTime(stringToTime(request->getEnd()), tmcStart) || compareSmallerTime(stringToTime(request->getStart()), tmcEnd) && compareBiggerTime(stringToTime(request->getStart()), tmcStart))
                delIndex.push_back(i);
        }

        int sub = 0;
        for (int index : delIndex)
        {
            int actIndex = index - sub;
            requests.erase(requests.begin() + actIndex);
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

void houseAvailabilityManage(Member *&mem)
{
    if (mem->getHouseForOwn()->getAvailablePeriodStart() != "" && mem->getHouseForOwn()->getAvailablePeriodEnd() != "")
        cout << "Your house is now available for rented from: " << mem->getHouseForOwn()->getAvailablePeriodStart() << " to: " << mem->getHouseForOwn()->getAvailablePeriodEnd() << endl;
    else
    {
        cout << "Your house is now free on every period" << endl;
    }
    string choice;
    cout << "Do you want to change the period of your house: Y/N" << endl;
    while (choice == "")
    {
        cin >> choice;
        if (choice == "Y" || choice == "y")
        {
            string newStartDate;
            cout << "Please enter a start day: " << endl;
            cin >> newStartDate;

            string newEndDate;
            cout << "Please enter an end day: " << endl;
            cin >> newEndDate;

            mem->setAvailablePeriod(newStartDate, newEndDate);
            cout << "Your house is now available for rented from: " << mem->getHouseForOwn()->getAvailablePeriodStart() << " to: " << mem->getHouseForOwn()->getAvailablePeriodEnd() << endl;
        }
    }
}
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
// }
// 	return true;
// }

// validate user choice
bool checkChoice(string choice, string start, string end)
{
    if ((choice.size() > 1) | (choice.size() < 1))
    {
        cout << "Invalid Input! Please enter your choice again" << endl
             << "\n********************************************\n";
        return true;
    }
    else if ((start > choice | choice > end) | (!isdigit(choice[0])))
    {
        cout << "Invalid Input! Please enter your choice again" << endl
             << "\n********************************************\n";
        return true;
    }
    return false;
}

Member *Login(vector<Member *> users)
{
    Member *mem;
    bool check = false;
    while (!check)
    {
        cout << "\nEnter username: ";
        string username;
        cin >> username;
        cout << endl;

        cout << "\nEnter password: ";
        string password;
        cin >> password;
        cout << endl;
        for (int i = 0; i < users.size(); i++)
        {

            if ((users[i]->getUsername() == username) && (users[i]->getPassword() == password))
            {
                cout << endl;
                check = true;
                mem = users[i];
            }
        }
    }
    if(!check) {
        cout << "\nWrong username or wrong password. Please enter again!" << endl;
        cout << endl;
    }
    return mem;
}

// validate if user enter right date format
bool checkDate(string str)
{
    if (str.size() == 10)
    {
        for (int i = 0; i < str.size(); i++)
        {
            if ((!isdigit(str[i])) & ((i != 4) & (i != 7)))
            {
                cout << "false" << endl;
                return false;
            }
            if ((str[7] != '/') | (str[4] != '/'))
            {
                cout << "false" << endl;
                return false;
            }
        }
    }
    cout << "true" << endl;
    return true;
}

bool isLeap(int year)
{
    // Return true if year
    // is a multiple of 4 and
    // not multiple of 100.
    // OR year is multiple of 400.
    return (((year % 4 == 0) &&
             (year % 100 != 0)) ||
            (year % 400 == 0));
}

// Returns true if given
// year is valid or not.
bool isValidDate(string y, string m, string d)
{
    int day = stoi(d);
    int month = stoi(m);
    int year = stoi(y);

    // If year, month and day
    // are not in given range
    if (year > MAX_VALID_YR ||
        year < MIN_VALID_YR)
        return false;
    if (month < 1 || month > 12)
        return false;
    if (day < 1 || day > 31)
        return false;

    // Handle February month
    // with leap year
    if (month == 2)
    {
        if (isLeap(year))
            return (day <= 29);
        else
            return (day <= 28);
    }

    // Months of April, June,
    // Sept and Nov must have
    // number of days less than
    // or equal to 30.
    if (month == 4 || month == 6 ||
        month == 9 || month == 11)
        return (day <= 30);

    return true;
}

// validate which one is using
void checkRole(string role)
{
    // menu for guest
    if (role == "1")
    {
        cout << "\nThis is your menu:" << endl
             << "0. Exit" << endl
             << "1. View houses" << endl
             << "Enter your choice: " << endl;
    }
    // menu for member
    else if (role == "2")
    {
        cout << "\nThis is your menu:" << endl
             << "0. Exit" << endl
             << "1. List/Unlist available occupied houses" << endl
             << "2. Search for available suitable houses" << endl
             << "3. Rate house" << endl
             << "4. Rate occupier" << endl
             << "5. View requests" << endl
             << "6. View information" << endl
             << "Enter your choice: " << endl;
    }
    // menu for admin
    else
    {
        cout << "\nThis is your menu:" << endl
             << "0. Exit" << endl
             << "1. List/Unlist available occupied houses" << endl
             << "2. Search for available suitable houses" << endl
             << "3. Rate house" << endl
             << "4. Rate occupier" << endl
             << "5. View requests" << endl
             << "6. View information" << endl
             << "7. View others information" << endl
             << "Enter your choice: " << endl;
    }
}

// validate choice by user's menu
void checkMenu(string role, string choice)
{
    if (role == "1")
        checkChoice(choice, "0", "1");
    else if (role == "2")
        checkChoice(choice, "0", "9");
    else
        checkChoice(choice, "0", "10");
}

// check function base on user's choice
void checkFunction(string role, string choice, Member *&mem, System sys)
{
    // use menu as a guest
    if (role == "1")
    {
        if (choice == "1")
        {
            cout << "Houses Details: " << endl;
            cout << endl;
            int count = 0;
            for (House *house : sys.getAllHouses())
            {
                count++;
            
                cout << count << "." << endl
                     << "Location: " << house->getLocation() << "         | Consuming Points: " << house->getConsumingPoints() << endl
                     << "Rating : " << house->getHouseRatingScrore() << " | Used Times: " << house->getUsedTimes() << endl;
            }
        }
    }
    // use menu as a member
    else if (role == "2")
    {
        int intChoice = stoi(choice);
        switch (intChoice)
        {
        // list/unlist house
        case 1:
            houseAvailabilityManage(mem);
            break;
        // rate living house
        case 3:
            if ((mem->getHouseForLive()) == NULL)
            {
                cout << "Invalid house" << endl;
                break;
            }
            cout << "\nPlease rating your living house (from scale -10 to 10): " << endl;
            float score;
            while ((!(cin >> score)) || (!((score >= -10) && (score <= 10)))) // validate input
            {
                cout << "ERROR: a number must be entered " << endl
                     << endl;
                cin.clear();
                cin.ignore(123, '\n');
                cout << "Please rating your living house (from scale -10 to 10): " << endl;
            }

            mem->ratingHouse(score, mem->getHouseForLive());
            cout << "\nThank you for rating!" << endl;
            break;
        // rate occupiers
        case 4:
            if ((mem->getPartner()) == NULL)
            {
                cout << "Invalid occupier" << endl;
                break;
            }
            cout << "\nPlease rating your occupiers (from scale -10 to 10): " << endl;
            // float score;
            while ((!(cin >> score)) || (!((score >= -10) && (score <= 10)))) // validate input
            {
                cout << "ERROR: a number must be entered " << endl
                     << endl;
                cin.clear();
                cin.ignore(123, '\n');
                cout << "Please rating your occupiers (from scale -10 to 10): " << endl;
            }

            mem->ratingOccupier(score, mem->getPartner());
            cout << "\nThank you for rating!" << endl;
            break;
        // view requests
        case 5:
            if (mem->getHouseForOwn()->getRequests().size() == 0)
                cout << "No requests" << endl;
            else
            {
                cout << "\nRequest: " << endl
                     << endl;
                for (Request *req : mem->getHouseForOwn()->getRequests())
                    cout << "Requested Username: " << req->getRequestUsername() << endl
                         << "Start Date - End Date: " << req->getStart() << "-" << req->getEnd() << endl;
            }
            break;
        // view info
        case 6:
            cout << "\nPersonal Info: " << endl
                 << endl
                 << "Fullname: " << mem->getFullname() << endl
                 << "Phone: " << mem->getPhone() << endl
                 << "Credit Points : " << mem->getCreditPoints() << endl
                 << "Occupier Rating : " << mem->getOccupierRatingScore() << endl
                 << "Occupied Times : " << mem->getOccupyTimes() << endl
                 << "Username : " << mem->getUsername() << endl
                 << "Present Partner : " << mem->getPartner() << endl
                 << endl;
            break;
        // search for suitable house
        case 2:
            cout << "\nHouses : " << endl
                 << endl;
            string start, end, city, review;
            while (true) // validate input
            {
                cout << "Enter the start date (YYYY/MM/DD): " << endl;
                cin >> start;
                if (!checkDate(start))
                    cout << "Invalid Input! Renter your date by the given format!" << endl;
                else if (!isValidDate(start.substr(0, 4), start.substr(5, 2), start.substr(8, 2)))
                    cout << "Invalid Date!" << endl;
                else
                    break;
            }

            while (true) // validate input
            {
                cout << "Enter the end date (YYYY/MM/DD): " << endl;
                cin >> end;
                if (!checkDate(end))
                    cout << "Invalid Input! Renter your date by the given format!" << endl;
                else if (!isValidDate(end.substr(0, 4), end.substr(5, 2), end.substr(8, 2)))
                    cout << "Invalid Date!" << endl;
                else
                    break;
            }
            while (true) // validate input
            {
                cout << "Enter the city (HaNoi/ SaiGon/ DaNang): " << endl;
                cin >> city;
                if (city=="HaNoi" && (city != "SaiGon") && (city != "DaNang"))
                    cout << "Invalid City! Enter the city again!" << endl;
                else
                    break;
            }
            while (true) // validate input
            {
                cout << "Do you want to see the reviews also? (Y/N)" << endl;
                cin >> review;
                if ((review != "Y") && (review != "y") && (review != "N") && (review != "n"))
                    cout << "Invalid Input! Enter your choice again!" << endl;
                else
                    break;
            }

            int count = 0;
            if ((review == "Y") || (review == "y"))
            {
                for (House *house : sys.availableHousesForMember(mem, start, end, city)) // print houses' details with review
                {
                    
                    cout << count << "." << endl
                         << "Location: " << house->getLocation() << "  Consuming Points: " << house->getConsumingPoints() << endl
                         << "Rating : " << house->getHouseRatingScrore() << "  Used Times: " << house->getUsedTimes() << endl
                         << "Review : " << endl;
                    for (string cmt : house->getComments())
                        cout << "            " << house->getHouseRatingScrore() << endl;
                    cout << endl;
                    count++;
                }
            }
            else
            {
                for (House *house : sys.availableHousesForMember(mem, start, end, city)) // print houses' details without review
                {
                    
                    cout << count << "." << endl
                         << "Location: " << house->getLocation() << "  Consuming Points: " << house->getConsumingPoints() << endl
                         << "Rating : " << house->getHouseRatingScrore() << "  Used Times: " << house->getUsedTimes() << endl
                         << end;
                    count++;
                }
            }

            while (true) // validate input
            {
                cout << "Do you want to make a request? (Y/N)" << endl;
                cin >> review;
                if ((review != "Y") && (review != "y") && (review != "N") && (review != "n"))
                    cout << "Invalid Input! Enter your choice again!" << endl;
                else
                    break;
            }

            string num_house;
            if ((review != "Y") || (review != "y")) // make a request
            {
                while (true)
                {
                    cout << "Pick a house's number which one you would like to make a request?" << endl;
                    cin >> num_house;
                    if ((num_house.size() > 2) && (!isdigit(num_house[0])) && (!isdigit(num_house[1])))
                        cout << "Invalid Input! Enter your choice again!" << endl;
                    else
                        break;
                }
                int index = stoi(num_house);
                sys.sendRequest(mem->getUsername(), sys.availableHouses(start, end), index, start, end);
            }
            cout << endl;
            break;
        }
    }
    // use menu as an admin
    else if (role == "3")
    {
        int intChoice = stoi(choice);
        switch (intChoice)
        {
        // list/unlist house
        case 1:
            houseAvailabilityManage(mem);
            break;
        // rate living house
        case 3:
            if ((mem->getHouseForLive()) == NULL)
            {
                cout << "Invalid house" << endl;
                break;
            }
            cout << "\nPlease rating your living house (from scale -10 to 10): " << endl;
            float score;
            while ((!(cin >> score)) || (!((score >= -10) && (score <= 10)))) // validate input
            {
                cout << "ERROR: a number must be entered " << endl
                     << endl;
                cin.clear();
                cin.ignore(123, '\n');
                cout << "Please rating your living house (from scale -10 to 10): " << endl;
            }

            mem->ratingHouse(score, mem->getHouseForLive());
            cout << "\nThank you for rating!" << endl;
            break;
        // rate occupiers
        case 4:
            if ((mem->getPartner()) == NULL)
            {
                cout << "Invalid occupier" << endl;
                break;
            }
            cout << "\nPlease rating your occupiers (from scale -10 to 10): " << endl;
            // float score;
            while ((!(cin >> score)) || (!((score >= -10) && (score <= 10)))) // validate input
            {
                cout << "ERROR: a number must be entered " << endl
                     << endl;
                cin.clear();
                cin.ignore(123, '\n');
                cout << "Please rating your occupiers (from scale -10 to 10): " << endl;
            }

            mem->ratingOccupier(score, mem->getPartner());
            cout << "\nThank you for rating!" << endl;
            break;
        // view requests
        case 5:
            if (mem->getHouseForOwn()->getRequests().size() == 0)
                cout << "No requests" << endl;
            else
            {
                cout << "\nRequest: " << endl
                     << endl;
                for (Request *req : mem->getHouseForOwn()->getRequests())
                    cout << "Requested Username: " << req->getRequestUsername() << endl
                         << "Start Date - End Date: " << req->getStart() << "-" << req->getEnd() << endl;
            }
            break;
        // view info
        case 6:
            cout << "\nPersonal Info: " << endl
                 << endl
                 << "Fullname: " << mem->getFullname() << endl
                 << "Phone: " << mem->getPhone() << endl
                 << "Credit Points : " << mem->getCreditPoints() << endl
                 << "Occupier Rating : " << mem->getOccupierRatingScore() << endl
                 << "Occupied Times : " << mem->getOccupyTimes() << endl
                 << "Username : " << mem->getUsername() << endl
                 << "Present Partner : " << mem->getPartner() << endl
                 << endl;
            break;
        // search for suitable house
        case 2:
        {
            cout << "\nHouses : " << endl
                 << endl;
            string start, end, city, review;
            while (true) // validate input
            {
                cout << "Enter the start date (YYYY/MM/DD): " << endl;
                cin >> start;
                if (!checkDate(start))
                    cout << "Invalid Input! Renter your date by the given format!" << endl;
                else if (!isValidDate(start.substr(0, 4), start.substr(5, 2), start.substr(8, 2)))
                    cout << "Invalid Date!" << endl;
                else
                    break;
            }

            while (true) // validate input
            {
                cout << "Enter the end date (YYYY/MM/DD): " << endl;
                cin >> end;
                if (!checkDate(end))
                    cout << "Invalid Input! Renter your date by the given format!" << endl;
                else if (!isValidDate(end.substr(0, 4), end.substr(5, 2), end.substr(8, 2)))
                    cout << "Invalid Date!" << endl;
                else
                    break;
            }
            while (true) // validate input
            {
                cout << "Enter the city (Ha Noi/ Sai Gon/ Da Nang): " << endl;
                cin >> city;
                if ((city != "Ha Noi") | (city != "Sai Gon") | (city != "Da Nang"))
                    cout << "Invalid City! Enter the city again!" << endl;
                else
                    break;
            }
            while (true) // validate input
            {
                cout << "Do you want to see the reviews also? (Y/N)" << endl;
                cin >> review;
                if ((review != "Y") | (review != "y") | (review != "N") | (review != "n"))
                    cout << "Invalid Input! Enter your choice again!" << endl;
                else
                    break;
            }

            int count = 0;
            if ((review == "Y") | (review == "y"))
            {
                for (House *house : sys.availableHousesForMember(mem, start, end, city)) // print houses' details with review
                {
                    count++;
                    cout << count << "." << endl;
                    cout << "Location: " << house->getLocation() << "  Consuming Points: " << house->getConsumingPoints() << endl
                         << "Rating : " << house->getHouseRatingScrore() << "  Used Times: " << house->getUsedTimes() << endl
                         << "Review : " << endl;
                    for (string cmt : house->getComments())
                        cout << "            " << house->getHouseRatingScrore() << endl;
                    cout << endl;
                }
            }
            else
            {
                for (House *house : sys.availableHousesForMember(mem, start, end, city)) // print houses' details without review
                {
                    count++;
                    cout << count << "." << endl;
                    cout << "Location: " << house->getLocation() << "  Consuming Points: " << house->getConsumingPoints() << endl
                         << "Rating : " << house->getHouseRatingScrore() << "  Used Times: " << house->getUsedTimes() << endl
                         << end;
                }
            }

            while (true) // validate input
            {
                cout << "Do you want to make a request? (Y/N)" << endl;
                cin >> review;
                if ((review != "Y") && (review != "y") && (review != "N") && (review != "n"))
                    cout << "Invalid Input! Enter your choice again!" << endl;
                else
                    break;
            }

            string num_house;
            if ((review != "Y") || (review != "y")) // make a request
            {
                while (true)
                {
                    cout << "Pick a house's number which one you would like to make a request?" << endl;
                    cin >> num_house;
                    if ((num_house.size() > 2) | (!isdigit(num_house[0])) | (!isdigit(num_house[1])))
                        cout << "Invalid Input! Enter your choice again!" << endl;
                    else
                        break;
                }
                int index = stoi(num_house);
                sys.sendRequest(mem->getUsername(), sys.availableHouses(start, end), index, start, end);
            }
        }
            cout << endl;
            break;
        // view everyone and houses info
        case 7:
            int count = 0;
            cout << "\nPeople Info: " << endl
                 << endl;

            for (Member *mem : sys.users)
            {
                count++;
                cout << count << "." << endl
                     << "Fullname: " << mem->getFullname() << endl
                     << "Phone: " << mem->getPhone() << endl
                     << "Credit Points : " << mem->getCreditPoints() << endl
                     << "Occupier Rating : " << mem->getOccupierRatingScore() << endl
                     << "Occupied Times : " << mem->getOccupyTimes() << endl
                     << "Username : " << mem->getUsername() << endl
                     << "Present Partner : " << mem->getPartner() << endl
                     << endl;
            }

            cout << "Houses Details: " << endl;
            cout << endl;
            count = 0;
            for (House *house : sys.getAllHouses())
            {
                count++;
                cout << count << "." << endl
                     << "Location: " << house->getLocation() << "         | Consuming Points: " << house->getConsumingPoints() << endl
                     << "Rating : " << house->getHouseRatingScrore() << " | Used Times: " << house->getUsedTimes() << endl
                     << endl;
            }
            break;
        }
    }
}

int main()
{

    // while(true){

    
    System appSys = System();

    Member mem1 = Member("Tam", "123", "Tam Kieu", "0123456");
    Member mem2 = Member("Thanh", "123", "Thanh Nguyen", "0123456");
    Member mem3 = Member("Tamad", "456", "Tam Kieu", "0123456");

    House h1 = House("SaiGon", "hcm");
    h1.setConsumingPoints(30);

    House h2 = House("SaiGon", "hcm");
    h2.setConsumingPoints(10);

    House h3 = House("SaiGon", "hcm");
    h3.setConsumingPoints(20);

    mem1.setHouseForOwn(&h1);
    mem2.setHouseForOwn(&h2);
    mem3.setHouseForOwn(&h3);

    mem2.setIsAdmin(false);
    mem3.setIsAdmin(true);

    appSys.users.push_back(&mem1);
    appSys.users.push_back(&mem2);
    appSys.users.push_back(&mem3);

    // Member *mem2 = appSys.registerAccount();

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

    // cout<<mem2->getCreditPoints()<<endl;
    while(true){

    
    bool check = false;
    string choice, role;
    Member *mem;

    while (true)
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
        cin >> role;
        if (!checkChoice(role, "1", "3")) // validate input
            break;
    }

    if (role == "1")
    {
        check = false;
        while (true)
        {
            cout << "\nRegist an account? Y/N" << endl;
            cin >> choice;
            if (choice == "Y" | choice == "y") // validate input
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
        mem = Login(appSys.users);

    while (choice != "0")
    {
        checkRole(role);
        cin >> choice;
        checkMenu(role, choice);
        checkFunction(role, choice, mem, appSys);
    }

    cout << "\nHave A Nice Day!!!" << endl;
    }
    return 0;
    
}
