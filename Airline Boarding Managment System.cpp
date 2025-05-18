#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cctype>

using namespace std;

const int ROWS = 15;
const int COLS = 4;
const int TOTAL_SEATS = ROWS * COLS;

struct Passenger {
    string name;
    bool booked = false;
    string seatCode;
    int mealPrice = 0;
    bool wheelchair = false;
    float extraLuggage = 0.0;
};

Passenger passengers[TOTAL_SEATS];// Converts seat code like "3B" to array index for passenger lookup
//,Returns -1 if input is invalid or out of bounds


int seatCodeToIndex(string code) {
    if (code.length() < 2 || code.length() > 3) return -1;
    stringstream ss(code.substr(0, code.length() - 1));
    int row;
    ss >> row;
    char colChar = toupper(code.back());
    if (row < 1 || row > ROWS) return -1;
    int col = colChar - 'A';// Extract row number and convert last character to column index (A-D → 0-3)
    if (col < 0 || col >= COLS) return -1;
    return (row - 1) * COLS + col;
}

void showSeats() {// Displays the current seat layout: booked or available, with class labels and exit markers
    cout << "\n================== AIRLINE SEAT MAP ==================\n";
    cout << " Legend: [ ] = Available   [X] = Booked\n";
    cout << "         B = Business      E = Economy\n\n";
    cout << "==================== FRONT EXIT ======================\n";
    cout << setw(16) << " " << "  A      B               C      D\n";
    for (int row = 0; row < ROWS; ++row) {
        // Add labels like "Wings Start", "Rear Exit", etc. at specific rows
        if (row == 5) cout << setw(16) << " " << "-- Wings Start --\n";
        if (row == 10) cout << setw(16) << " " << "-- Wings End --\n";
        if (row == 13) cout << "==================== REAR EXIT =======================\n";
        cout << "Row " << setw(2) << row + 1 << " [" << (row < 3 ? "B" : "E") << "] :     ";
        for (int col = 0; col < COLS; ++col) {
            if (col == 2) cout << "         ";
            int index = row * COLS + col;
            cout << "[" << (passengers[index].booked ? 'X' : ' ') << "]    ";
        }
        cout << "\n";
    }
    cout << "=======================================================\n";
}

void bookSeat() {// Books a seat if it's valid and not already taken
    string seatCode, name;
    cout << "Enter seat code to book (e.g. 3B): ";
    cin >> seatCode;
    int index = seatCodeToIndex(seatCode);
    if (index == -1 || passengers[index].booked) {
        cout << "Invalid or already booked seat.\n\n";
        return;
    }
    cin.ignore();
    cout << "Enter passenger name: ";
    getline(cin, name);
    passengers[index].booked = true;// Store passenger info in the appropriate array index
    passengers[index].name = name;
    passengers[index].seatCode = seatCode;
    cout << "Seat " << seatCode << " successfully booked for " << name << ".\n\n";
}

void cancelSeat() {// Cancels a booking and resets passenger data to default

    string seatCode;
    cout << "Enter seat code to cancel (e.g. 3B): ";
    cin >> seatCode;
    int index = seatCodeToIndex(seatCode);
    if (index == -1 || !passengers[index].booked) {
        cout << "Invalid or not booked seat.\n\n";
        return;
    }
    passengers[index] = Passenger(); // // Assigning default-constructed Passenger struct to clear data
    cout << "Seat " << seatCode << " successfully cancelled.\n\n";
}

int chooseMeal() {// Displays meal menu and returns selected meal's price

    int foodChoice;
    cout << "\nSelect Meal Option:\n";
    cout << "1. Chicken Biryani (PKR 2000)\n";
    cout << "2. Beef Steak     (PKR 5000)\n";
    cout << "3. Pasta Alfredo  (PKR 3000)\n";
    cout << "4. BBQ Platter    (PKR 8000)\n";
    cout << "5. Veg Sandwich   (PKR 2500)\n";
    cout << "Enter choice (1-5): ";
    cin >> foodChoice;
    int prices[] = { 2000, 5000, 3000, 8000, 2500 };
    return (foodChoice >= 1 && foodChoice <= 5) ? prices[foodChoice - 1] : 0;
}

bool requestWheelchair() {// Checks if wheelchair assistance is required
    char response;
    cout << "Do you require a wheelchair? (y/n): ";
    cin >> response;
    return tolower(response) == 'y';
}

void addExtraLuggage() { // Adds extra luggage weight (in kg) to passenger's profile

    string seatCode;
    cout << "Enter seat code to add luggage for (e.g. 3B): ";
    cin >> seatCode;
    int index = seatCodeToIndex(seatCode);
    if (index == -1 || !passengers[index].booked) {
        cout << "Invalid or unbooked seat.\n";
        return;
    }
    float weight;
    cout << "Enter extra luggage weight in kg: ";
    cin >> weight;
    passengers[index].extraLuggage = weight;
    cout << "Extra luggage of " << weight << "kg added.\n";
}

void printBoardingPass() {// Generates and displays boarding pass with all selected services and cost breakdown

    string seatCode;
    cout << "Enter seat code to print boarding pass (e.g. 3B): ";
    cin >> seatCode;
    int index = seatCodeToIndex(seatCode);
    if (index == -1 || !passengers[index].booked) {
        cout << "Invalid or unbooked seat.\n";
        return;
    }
    Passenger& p = passengers[index];
    p.mealPrice = chooseMeal();
    p.wheelchair = requestWheelchair();

    stringstream ss(seatCode.substr(0, seatCode.length() - 1));
    int row;
    ss >> row;
    string cls = (row <= 3) ? "Business" : "Economy";
    int basePrice = (row <= 3) ? 40000 : 25000;
    int luggageCost = static_cast<int>(p.extraLuggage * 2500);

    cout << "\n========= BOARDING PASS =========\n";
    cout << "Passenger       : " << p.name << "\n";
    cout << "Seat            : " << p.seatCode << "\n";
    cout << "Class           : " << cls << "\n";
    cout << "Seat Price      : PKR " << basePrice << "\n";
    cout << "Meal Price      : PKR " << p.mealPrice << "\n";
    cout << "Wheelchair      : " << (p.wheelchair ? "Yes" : "No") << "\n";
    cout << "Extra Luggage   : " << p.extraLuggage << " kg (PKR " << luggageCost << ")\n";
    cout << "Luggage Allow   : " << (cls == "Business" ? "3 bags (50kg)" : "2 bags (30kg)") << "\n";
    cout << "Total           : PKR " << (basePrice + p.mealPrice + luggageCost) << "\n";
    cout << "=================================\n\n";
}

int main() {// Main menu loop: lets user choose actions until exit
    int choice;
    do {
        cout << "\n========= Airline Reservation Menu =========\n";
        cout << "1. Show Available Seats\n";
        cout << "2. Book a Seat\n";
        cout << "3. Cancel a Seat\n";
        cout << "4. Print Boarding Pass\n";
        cout << "5. Add Extra Luggage\n";
        cout << "6. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;
        switch (choice) {// User's menu choice controls function execution
        case 1: showSeats(); break;
        case 2: bookSeat(); break;
        case 3: cancelSeat(); break;
        case 4: printBoardingPass(); break;
        case 5: addExtraLuggage(); break;
        case 6: cout << "Exiting system. Goodbye!\n"; break;
        default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 6);
    return 0;
}

