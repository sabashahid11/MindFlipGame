#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

struct User {
    string username;
    string password;
    string role;
};

struct Flight {
    string flightNumber;
    string origin;
    string destination;
    string departureTime;
    string arrivalTime;
    int availableSeats;
};

struct CrewMember {
    string username;
    string password;
    string name;
    string position;
    string* assignedFlights;
    int assignedFlightCount;
    string* pastFlights;
    int pastFlightCount;
};

void clearScreen();
void saveUser(const User& user);
void pressEnterToContinue();
User* loadUsers(int& userCount);
void saveFlight(const Flight& flight);
Flight* loadFlights(int& flightCount);
void updateFlightSchedule(const Flight& flight);
void saveCrewMember(const CrewMember& member);
CrewMember* loadCrewMembers(int& crewCount);
void updateCrewMembers(CrewMember* members, int crewCount);
int getTicketPrice(int age);
void addFlightSchedule();
void changeFlightSchedule();
void displayAvailableFlights();
void bookFlight();
void assignCrewToFlight();
void showCurrentSchedule(const string& username);
void showHistoryOfPastFlights(const string& username);
void showAdminMenu();
void showCrewMenu(const string& username);
void showPassengerMenu();
void displayMainMenu();
void addCrewMember();
bool authenticateCrewMember(const string& username, const string& password);
void addAdminUser();
void initSystem();
bool adminSignIn();
void markFlightCompleted(const string& flightNumber);

int main() {
    int choice;
    initSystem();

    while (true) {
        clearScreen();
        cout << "===== Airline Reservation System =====\n";
        cout << "1. Admin\n";
        cout << "2. Crew\n";
        cout << "3. Passenger\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            if (adminSignIn()) {
                showAdminMenu();
            }
            else {
                cout << "Invalid username or password.\n";
                pressEnterToContinue();
            }
            break;
        case 2:
        {
            string username;
            cout << "Enter username: ";
            cin >> username;
            cin.ignore();
            showCrewMenu(username);
            break;
        }
        case 3:
            showPassengerMenu();
            break;
        case 4:
            return 0;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}

void clearScreen() {
    cout << "\033[H\033[J";
}

void pressEnterToContinue() {
    cout << "Press Enter to continue...";
    cin.ignore();
}

bool adminSignIn() {
    string username, password;
    cout << "Enter admin username: ";
    cin >> username;
    cin.ignore();
    cout << "Enter admin password: ";
    cin >> password;
    cin.ignore();

    ifstream file("admin.txt");
    if (file.is_open()) {
        string fileUsername, filePassword;
        file >> fileUsername >> filePassword;
        file.close();
        if (username == fileUsername && password == filePassword) {
            return true;
        }
    }
    return false;
}

void showAdminMenu() {
    int choice;
    while (true) {
        clearScreen();
        cout << "===== Admin Menu =====\n";
        cout << "1. Add Flight Schedule\n";
        cout << "2. Change Flight Schedule\n";
        cout << "3. Assign Crew to Flight\n";
        cout << "4. Add Crew Member\n";
        cout << "5. Mark Flight as Completed\n";
        cout << "6. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            addFlightSchedule();
            break;
        case 2:
            changeFlightSchedule();
            break;
        case 3:
            assignCrewToFlight();
            break;
        case 4:
            addCrewMember();
            break;
        case 5:
        {
            string flightNumber;
            cout << "Enter flight number to mark as completed: ";
            getline(cin, flightNumber);
            markFlightCompleted(flightNumber);
            break;
        }
        case 6:
            return;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }
}

void showCrewMenu(const string& username) {
    string password;
    cout << "Enter password: ";
    cin >> password;
    cin.ignore();

    if (!authenticateCrewMember(username, password)) {
        cout << "Invalid username or password.\n";
        pressEnterToContinue();
        return;
    }

    int choice;
    while (true) {
        clearScreen();
        cout << "===== Crew Menu =====\n";
        cout << "1. Show Current Schedule\n";
        cout << "2. Show History of Past Flights\n";
        cout << "3. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            showCurrentSchedule(username);
            break;
        case 2:
            showHistoryOfPastFlights(username);
            break;
        case 3:
            return;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }
}

void showPassengerMenu() {
    int choice;
    while (true) {
        clearScreen();
        cout << "===== Passenger Menu =====\n";
        cout << "1. Book Flight\n";
        cout << "2. Display Available Flights\n";
        cout << "3. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            bookFlight();
            break;
        case 2:
            displayAvailableFlights();
            break;
        case 3:
            return;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }
}

void addFlightSchedule() {
    Flight newFlight;
    clearScreen();
    cout << "======= Add Flight Schedule =======\n";
    cout << "Enter flight number: ";
    getline(cin, newFlight.flightNumber);
    cout << "Enter origin: ";
    getline(cin, newFlight.origin);
    cout << "Enter destination: ";
    getline(cin, newFlight.destination);
    cout << "Enter departure time: ";
    getline(cin, newFlight.departureTime);
    cout << "Enter arrival time: ";
    getline(cin, newFlight.arrivalTime);
    cout << "Enter available seats: ";
    cin >> newFlight.availableSeats;
    cin.ignore();
    saveFlight(newFlight);
    cout << "Flight schedule added successfully.\n";
    pressEnterToContinue();
}

void changeFlightSchedule() {
    int flightCount;
    Flight* flights = loadFlights(flightCount);
    if (!flights) {
        cout << "No flights available.\n";
        pressEnterToContinue();
        return;
    }

    string flightNumber;
    clearScreen();
    cout << "Enter flight number to change schedule: ";
    getline(cin, flightNumber);

    for (int i = 0; i < flightCount; ++i) {
        if (flights[i].flightNumber == flightNumber) {
            cout << "Enter new origin: ";
            getline(cin, flights[i].origin);
            cout << "Enter new destination: ";
            getline(cin, flights[i].destination);
            cout << "Enter new departure time: ";
            getline(cin, flights[i].departureTime);
            cout << "Enter new arrival time: ";
            getline(cin, flights[i].arrivalTime);
            cout << "Enter new available seats: ";
            cin >> flights[i].availableSeats;
            cin.ignore();
            updateFlightSchedule(flights[i]);
            cout << "Flight schedule updated successfully.\n";
            delete[] flights;
            pressEnterToContinue();
            return;
        }
    }
    cout << "Flight not found.\n";
    delete[] flights;
    pressEnterToContinue();
}

void displayAvailableFlights() {
    int flightCount;
    Flight* flights = loadFlights(flightCount);
    if (!flights) {
        cout << "No flights available.\n";
        pressEnterToContinue();
        return;
    }

    clearScreen();
    cout << "======= Available Flights =======\n";
    for (int i = 0; i < flightCount; ++i) {
        cout << "Flight Number: " << flights[i].flightNumber << endl;
        cout << "Origin: " << flights[i].origin << endl;
        cout << "Destination: " << flights[i].destination << endl;
        cout << "Departure Time: " << flights[i].departureTime << endl;
        cout << "Arrival Time: " << flights[i].arrivalTime << endl;
        cout << "Available Seats: " << flights[i].availableSeats << endl;
        cout << "-------------------------------\n";
    }
    delete[] flights;
    pressEnterToContinue();
}

void bookFlight() {
    int flightCount;
    Flight* flights = loadFlights(flightCount);
    if (!flights) {
        cout << "No flights available.\n";
        pressEnterToContinue();
        return;
    }

    string flightNumber;
    int age;
    clearScreen();
    cout << "Enter flight number to book: ";
    getline(cin, flightNumber);

    for (int i = 0; i < flightCount; ++i) {
        if (flights[i].flightNumber == flightNumber) {
            if (flights[i].availableSeats > 0) {
                cout << "Enter your age: ";
                cin >> age;
                cin.ignore();
                int price = getTicketPrice(age);
                flights[i].availableSeats--;
                updateFlightSchedule(flights[i]);
                cout << "Flight booked successfully. Ticket price: $" << price << endl;
                delete[] flights;
                pressEnterToContinue();
                return;
            }
            else {
                cout << "No available seats.\n";
                delete[] flights;
                pressEnterToContinue();
                return;
            }
        }
    }
    cout << "Flight not found.\n";
    delete[] flights;
    pressEnterToContinue();
}

void assignCrewToFlight() {
    int flightCount;
    int crewCount;
    Flight* flights = loadFlights(flightCount);
    if (!flights) {
        cout << "No flights available.\n";
        pressEnterToContinue();
        return;
    }
    CrewMember* crewMembers = loadCrewMembers(crewCount);
    if (!crewMembers) {
        cout << "No crew members available.\n";
        delete[] flights;
        pressEnterToContinue();
        return;
    }

    string flightNumber, crewUsername;
    clearScreen();
    cout << "Enter flight number to assign crew: ";
    getline(cin, flightNumber);

    for (int i = 0; i < flightCount; ++i) {
        if (flights[i].flightNumber == flightNumber) {
            cout << "Enter crew member username: ";
            getline(cin, crewUsername);
            for (int j = 0; j < crewCount; ++j) {
                if (crewMembers[j].username == crewUsername) {
                    string* newAssignedFlights = new string[crewMembers[j].assignedFlightCount + 1];
                    for (int k = 0; k < crewMembers[j].assignedFlightCount; ++k) {
                        newAssignedFlights[k] = crewMembers[j].assignedFlights[k];
                    }
                    newAssignedFlights[crewMembers[j].assignedFlightCount] = flightNumber;
                    delete[] crewMembers[j].assignedFlights;
                    crewMembers[j].assignedFlights = newAssignedFlights;
                    crewMembers[j].assignedFlightCount++;

                    updateCrewMembers(crewMembers, crewCount);
                    cout << "Crew member assigned to flight successfully.\n";
                    delete[] flights;
                    for (int k = 0; k < crewCount; ++k) {
                        delete[] crewMembers[k].assignedFlights;
                        delete[] crewMembers[k].pastFlights;
                    }
                    delete[] crewMembers;
                    pressEnterToContinue();
                    return;
                }
            }
            cout << "Crew member not found.\n";
            delete[] flights;
            for (int k = 0; k < crewCount; ++k) {
                delete[] crewMembers[k].assignedFlights;
                delete[] crewMembers[k].pastFlights;
            }
            delete[] crewMembers;
            pressEnterToContinue();
            return;
        }
    }
    cout << "Flight not found.\n";
    delete[] flights;
    for (int k = 0; k < crewCount; ++k) {
        delete[] crewMembers[k].assignedFlights;
        delete[] crewMembers[k].pastFlights;
    }
    delete[] crewMembers;
    pressEnterToContinue();
}

void showCurrentSchedule(const string& username) {
    int crewCount;
    CrewMember* crewMembers = loadCrewMembers(crewCount);
    if (!crewMembers) {
        cout << "No crew members available.\n";
        pressEnterToContinue();
        return;
    }

    clearScreen();
    for (int i = 0; i < crewCount; ++i) {
        if (crewMembers[i].username == username) {
            cout << "Current Schedule for " << crewMembers[i].name << ":\n";
            for (int j = 0; j < crewMembers[i].assignedFlightCount; ++j) {
                cout << "Flight Number: " << crewMembers[i].assignedFlights[j] << endl;
            }
            for (int k = 0; k < crewCount; ++k) {
                delete[] crewMembers[k].assignedFlights;
                delete[] crewMembers[k].pastFlights;
            }
            delete[] crewMembers;
            pressEnterToContinue();
            return;
        }
    }
    cout << "Crew member not found.\n";
    for (int k = 0; k < crewCount; ++k) {
        delete[] crewMembers[k].assignedFlights;
        delete[] crewMembers[k].pastFlights;
    }
    delete[] crewMembers;
    pressEnterToContinue();
}

void showHistoryOfPastFlights(const string& username) {
    int crewCount;
    CrewMember* crewMembers = loadCrewMembers(crewCount);
    if (!crewMembers) {
        cout << "No crew members available.\n";
        pressEnterToContinue();
        return;
    }

    clearScreen();
    for (int i = 0; i < crewCount; ++i) {
        if (crewMembers[i].username == username) {
            cout << "History of Past Flights for " << crewMembers[i].name << ":\n";
            for (int j = 0; j < crewMembers[i].pastFlightCount; ++j) {
                cout << "Flight Number: " << crewMembers[i].pastFlights[j] << endl;
            }
            for (int k = 0; k < crewCount; ++k) {
                delete[] crewMembers[k].assignedFlights;
                delete[] crewMembers[k].pastFlights;
            }
            delete[] crewMembers;
            pressEnterToContinue();
            return;
        }
    }
    cout << "Crew member not found.\n";
    for (int k = 0; k < crewCount; ++k) {
        delete[] crewMembers[k].assignedFlights;
        delete[] crewMembers[k].pastFlights;
    }
    delete[] crewMembers;
    pressEnterToContinue();
}

bool authenticateCrewMember(const string& username, const string& password) {
    int crewCount;
    CrewMember* crewMembers = loadCrewMembers(crewCount);
    if (!crewMembers) {
        return false;
    }

    for (int i = 0; i < crewCount; ++i) {
        if (crewMembers[i].username == username && crewMembers[i].password == password) {
            for (int k = 0; k < crewCount; ++k) {
                delete[] crewMembers[k].assignedFlights;
                delete[] crewMembers[k].pastFlights;
            }
            delete[] crewMembers;
            return true;
        }
    }
    for (int k = 0; k < crewCount; ++k) {
        delete[] crewMembers[k].assignedFlights;
        delete[] crewMembers[k].pastFlights;
    }
    delete[] crewMembers;
    return false;
}

void saveUser(const User& user) {
    ofstream file("users.txt", ios::app);
    if (file.is_open()) {
        file << user.username << " " << user.password << " " << user.role << endl;
        file.close();
    }
}

User* loadUsers(int& userCount) {
    ifstream file("users.txt");
    if (file.is_open()) {
        userCount = 0;
        string line;
        while (getline(file, line)) {
            userCount++;
        }
        file.clear();
        file.seekg(0, ios::beg);

        User* users = new User[userCount];
        int index = 0;
        while (getline(file, line)) {
            stringstream ss(line);
            ss >> users[index].username >> users[index].password >> users[index].role;
            index++;
        }
        file.close();
        return users;
    }
    userCount = 0;
    return nullptr;
}

void saveFlight(const Flight& flight) {
    ofstream file("flights.txt", ios::app);
    if (file.is_open()) {
        file << flight.flightNumber << " " << flight.origin << " " << flight.destination << " "
            << flight.departureTime << " " << flight.arrivalTime << " " << flight.availableSeats << endl;
        file.close();
    }
}

Flight* loadFlights(int& flightCount) {
    ifstream file("flights.txt");
    if (file.is_open()) {
        flightCount = 0;
        string line;
        while (getline(file, line)) {
            flightCount++;
        }
        file.clear();
        file.seekg(0, ios::beg);

        Flight* flights = new Flight[flightCount];
        int index = 0;
        while (getline(file, line)) {
            stringstream ss(line);
            ss >> flights[index].flightNumber >> flights[index].origin >> flights[index].destination
                >> flights[index].departureTime >> flights[index].arrivalTime >> flights[index].availableSeats;
            index++;
        }
        file.close();
        return flights;
    }
    flightCount = 0;
    return nullptr;
}

void updateFlightSchedule(const Flight& flight) {
    int flightCount;
    Flight* flights = loadFlights(flightCount);
    if (!flights) {
        return;
    }

    ofstream file("flights.txt");
    if (file.is_open()) {
        for (int i = 0; i < flightCount; ++i) {
            if (flights[i].flightNumber == flight.flightNumber) {
                flights[i] = flight;
            }
            file << flights[i].flightNumber << " " << flights[i].origin << " " << flights[i].destination << " "
                << flights[i].departureTime << " " << flights[i].arrivalTime << " " << flights[i].availableSeats << endl;
        }
        file.close();
    }
    delete[] flights;
}

void saveCrewMember(const CrewMember& member) {
    ofstream file("crew.txt", ios::app);
    if (file.is_open()) {
        file << member.username << " " << member.password << " " << member.name << " " << member.position << " ";
        file << member.assignedFlightCount << " ";
        for (int i = 0; i < member.assignedFlightCount; ++i) {
            file << member.assignedFlights[i] << " ";
        }
        file << member.pastFlightCount << " ";
        for (int i = 0; i < member.pastFlightCount; ++i) {
            file << member.pastFlights[i] << " ";
        }
        file << endl;
        file.close();
    }
}

CrewMember* loadCrewMembers(int& crewCount) {
    ifstream file("crew.txt");
    if (file.is_open()) {
        crewCount = 0;
        string line;
        while (getline(file, line)) {
            crewCount++;
        }
        file.clear();
        file.seekg(0, ios::beg);

        CrewMember* crewMembers = new CrewMember[crewCount];
        int index = 0;
        while (getline(file, line)) {
            stringstream ss(line);
            crewMembers[index].username = "";
            crewMembers[index].password = "";
            crewMembers[index].name = "";
            crewMembers[index].position = "";
            ss >> crewMembers[index].username >> crewMembers[index].password >> crewMembers[index].name >> crewMembers[index].position;
            ss >> crewMembers[index].assignedFlightCount;
            crewMembers[index].assignedFlights = new string[crewMembers[index].assignedFlightCount];
            for (int i = 0; i < crewMembers[index].assignedFlightCount; ++i) {
                ss >> crewMembers[index].assignedFlights[i];
            }
            ss >> crewMembers[index].pastFlightCount;
            crewMembers[index].pastFlights = new string[crewMembers[index].pastFlightCount];
            for (int i = 0; i < crewMembers[index].pastFlightCount; ++i) {
                ss >> crewMembers[index].pastFlights[i];
            }
            index++;
        }
        file.close();
        return crewMembers;
    }
    crewCount = 0;
    return nullptr;
}

void updateCrewMembers(CrewMember* members, int crewCount) {
    ofstream file("crew.txt");
    if (file.is_open()) {
        for (int i = 0; i < crewCount; ++i) {
            file << members[i].username << " " << members[i].password << " " << members[i].name << " " << members[i].position << " ";
            file << members[i].assignedFlightCount << " ";
            for (int j = 0; j < members[i].assignedFlightCount; ++j) {
                file << members[i].assignedFlights[j] << " ";
            }
            file << members[i].pastFlightCount << " ";
            for (int j = 0; j < members[i].pastFlightCount; ++j) {
                file << members[i].pastFlights[j] << " ";
            }
            file << endl;
        }
        file.close();
    }
}

int getTicketPrice(int age) {
    if (age < 2) return 0;
    if (age <= 12) return 100;
    if (age >= 60) return 150;
    return 200;
}

void addCrewMember() {
    CrewMember newMember;
    clearScreen();
    cout << "======= Add Crew Member =======\n";
    cout << "Enter username: ";
    getline(cin, newMember.username);
    cout << "Enter password: ";
    getline(cin, newMember.password);
    cout << "Enter name: ";
    getline(cin, newMember.name);
    cout << "Enter position: ";
    getline(cin, newMember.position);
    newMember.assignedFlights = new string[0];
    newMember.assignedFlightCount = 0;
    newMember.pastFlights = new string[0];
    newMember.pastFlightCount = 0;
    saveCrewMember(newMember);
    cout << "Crew member added successfully.\n";
    pressEnterToContinue();
}

void addAdminUser() {
    ifstream infile("admin.txt");
    if (!infile.good()) {
        ofstream file("admin.txt");
        if (file.is_open()) {
            file << "admin1 password1" << endl;
            file.close();
        }
    }
}

void initSystem() {
    addAdminUser();
}

void markFlightCompleted(const string& flightNumber) {
    int crewCount;
    CrewMember* crewMembers = loadCrewMembers(crewCount);
    if (!crewMembers) {
        cout << "No crew members available.\n";
        pressEnterToContinue();
        return;
    }

    bool flightFound = false;
    for (int i = 0; i < crewCount; ++i) {
        for (int j = 0; j < crewMembers[i].assignedFlightCount; ++j) {
            if (crewMembers[i].assignedFlights[j] == flightNumber) {
                flightFound = true;
                string* newAssignedFlights = new string[crewMembers[i].assignedFlightCount - 1];
                for (int k = 0, l = 0; k < crewMembers[i].assignedFlightCount; ++k) {
                    if (crewMembers[i].assignedFlights[k] != flightNumber) {
                        newAssignedFlights[l++] = crewMembers[i].assignedFlights[k];
                    }
                }
                delete[] crewMembers[i].assignedFlights;
                crewMembers[i].assignedFlights = newAssignedFlights;
                crewMembers[i].assignedFlightCount--;

                string* newPastFlights = new string[crewMembers[i].pastFlightCount + 1];
                for (int k = 0; k < crewMembers[i].pastFlightCount; ++k) {
                    newPastFlights[k] = crewMembers[i].pastFlights[k];
                }
                newPastFlights[crewMembers[i].pastFlightCount] = flightNumber;
                delete[] crewMembers[i].pastFlights;
                crewMembers[i].pastFlights = newPastFlights;
                crewMembers[i].pastFlightCount++;
                break;
            }
        }
    }

    if (flightFound) {
        updateCrewMembers(crewMembers, crewCount);
        cout << "Flight marked as completed successfully.\n";
    }
    else {
        cout << "Flight not found in assigned flights.\n";
    }

    for (int k = 0; k < crewCount; ++k) {
        delete[] crewMembers[k].assignedFlights;
        delete[] crewMembers[k].pastFlights;
    }
    delete[] crewMembers;
    pressEnterToContinue();
}