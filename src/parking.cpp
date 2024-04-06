// #include <iostream>
// #include <iomanip>
// #include <fstream>
// #include <ctime>
// #include <limits>

// const int NUM_FLOORS = 3;
// const int SPACES_PER_FLOOR = 10;

// struct ParkingSpace {
//     bool occupied = false;
//     std::time_t reservationTime = 0;
//     std::time_t departureTime = 0;
//     std::string licensePlate = "";
//     std::string carColor = "";
// };

// void displayParkingLot(const ParkingSpace parkingLot[NUM_FLOORS][SPACES_PER_FLOOR]) {
//     std::cout << "Parking Lot Status:" << std::endl;

//     for (int floor = 0; floor < NUM_FLOORS; ++floor) {
//         std::cout << "Floor " << floor + 1 << ": ";
//         for (int space = 0; space < SPACES_PER_FLOOR; ++space) {
//             if (parkingLot[floor][space].occupied) {
//                 std::cout << "[X]"; // [X] represents an occupied space
//             } else {
//                 std::cout << "[ ]"; // [ ] represents an empty space
//             }
//         }
//         std::cout << std::endl;
//     }
// }

// bool reserveParkingSpace(ParkingSpace parkingLot[NUM_FLOORS][SPACES_PER_FLOOR]) {
//     int floor, space;
//     std::string licensePlate, carColor;

//     // Get user input for floor and space
//     std::cout << "Enter the floor number: ";
//     std::cin >> floor;
//     std::cout << "Enter the space number: ";
//     std::cin >> space;

//     // Validate user input
//     if (std::cin.fail() || floor < 1 || floor > NUM_FLOORS || space < 1 || space > SPACES_PER_FLOOR) {
//         std::cin.clear(); // Clear error flag
//         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
//         std::cerr << "Invalid floor or space number. Reservation canceled." << std::endl;
//         return false;
//     }

//     // Adjust indices to start from 0 internally
//     --floor;
//     --space;

//     // Check if the space is already occupied
//     if (parkingLot[floor][space].occupied) {
//         std::cerr << "Parking space is already occupied. Reservation canceled." << std::endl;
//         return false;
//     }

//     // Get license plate number and car color from the user
//     std::cout << "Enter License Plate Number: ";
//     std::cin >> parkingLot[floor][space].licensePlate;
//     std::cout << "Enter Car Color: ";
//     std::cin >> parkingLot[floor][space].carColor;

//     // Reserve the parking space and set the reservation time
//     parkingLot[floor][space].occupied = true;
//     parkingLot[floor][space].reservationTime = std::time(nullptr);
    
//     std::cout << "Parking space reserved successfully." << std::endl;

//     // Save reservation information to a file
//     std::ofstream outFile("reservation_log.txt", std::ios::app);
//     if (outFile.is_open()) {
//         outFile << "Floor: " << floor + 1 << ", Space: " << space + 1 << ", Reservation Time: "
//                 << std::put_time(std::localtime(&parkingLot[floor][space].reservationTime), "%Y-%m-%d %H:%M:%S")
//                 << ", License Plate: " << parkingLot[floor][space].licensePlate
//                 << ", Car Color: " << parkingLot[floor][space].carColor << std::endl;
//         outFile.close();
//     } else {
//         std::cerr << "Error: Unable to open the reservation log file." << std::endl;
//     }

//     return true;
// }

// void writeToFile(const ParkingSpace parkingLot[NUM_FLOORS][SPACES_PER_FLOOR]) {
//     std::ofstream outFile("balans.txt", std::ios::trunc);
//     if (outFile.is_open()) {
//         for (int floor = 0; floor < NUM_FLOORS; ++floor) {
//             for (int space = 0; space < SPACES_PER_FLOOR; ++space) {
//                 if (parkingLot[floor][space].occupied) {
//                     outFile << parkingLot[floor][space].licensePlate << " " << floor + 1 << " " << space + 1 << std::endl;
//                 }
//             }
//         }
//         outFile.close();
//     } else {
//         std::cerr << "Error: Unable to open the reservation log file." << std::endl;
//     }
// }

// int calculateParkingDuration(const ParkingSpace& parkingSpace) {
//     std::time_t currentTime = std::time(nullptr);
//     return static_cast<int>(std::difftime(currentTime, parkingSpace.reservationTime) / 60); // Convert seconds to minutes
// }

// void carLeavesParking(ParkingSpace parkingLot[NUM_FLOORS][SPACES_PER_FLOOR]) {
//     // Display occupied spaces
//     std::cout << "Occupied Parking Spaces:" << std::endl;

//     for (int floor = 0; floor < NUM_FLOORS; ++floor) {
//         for (int space = 0; space < SPACES_PER_FLOOR; ++space) {
//             if (parkingLot[floor][space].occupied) {
//                 std::cout << "Floor " << floor + 1 << ", Space " << space + 1  << ", License Plate " << parkingLot[floor][space].licensePlate << std::endl;
//             }
//         }
//     }

//     // Get user input for the leaving car
//     int floor, space;
//     std::string license;
//     std::cout << "Enter the floor number of the leaving car: ";
//     std::cin >> floor;
//     std::cout << "Enter the space number of the leaving car: ";
//     std::cin >> space; 
//     std::cout << "Enter the license number of the leaving car: ";
//     std::cin >> license;

//     // Adjust indices to start from 0 internally
//     --floor;
//     --space;

//     // Validate user input
//     if (std::cin.fail() || floor < 0 || floor >= NUM_FLOORS || space < 0 || space >= SPACES_PER_FLOOR || parkingLot[floor][space].licensePlate.empty() || license != parkingLot[floor][space].licensePlate) {
//         std::cin.clear(); // Clear error flag
//         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
//         std::cout << "Invalid floor, space number, or license plate. Car departure canceled." << std::endl;
//         return;
//     }

//     // Calculate parking duration
//     int parkingDuration = calculateParkingDuration(parkingLot[floor][space]);

//     // Save departure information to a file
//     std::ofstream outFile("departure_log.txt", std::ios::app);
//     if (outFile.is_open()) {
//         parkingLot[floor][space].departureTime = std::time(nullptr);
//         outFile << "Floor: " << floor + 1 << ", Space: " << space + 1 << ", Departure Time: "
//                 << std::put_time(std::localtime(&parkingLot[floor][space].departureTime), "%Y-%m-%d %H:%M:%S")
//                 << ", Parking Duration: " << parkingDuration << " minutes"
//                 << ", License Plate: " << parkingLot[floor][space].licensePlate
//                 << ", Car Color: " << parkingLot[floor][space].carColor << std::endl;
//         outFile.close();
//     } else {
//         std::cout << "Error: Unable to open the departure log file." << std::endl;
//         return;
//     }

//     // Clear the parking space
//     parkingLot[floor][space].occupied = false;
//     parkingLot[floor][space].reservationTime = 0;
//     parkingLot[floor][space].licensePlate = "";
//     parkingLot[floor][space].carColor = "";

//     std::cout << "Car departure processed successfully." << std::endl;
// }

// void loadReservationsFromFile(ParkingSpace parkingLot[NUM_FLOORS][SPACES_PER_FLOOR]) {
//     std::ifstream inFile("balans.txt");
//     if (inFile.is_open()) {
//         int floor, space;
//         std::string licensePlate;

//         // Read data from the file and add it to the parkingLot array
//         while (inFile >> licensePlate >> floor >> space) {
//             // Adjust indices to start from 0 internally
//             --floor;
//             --space;

//             // Check if the indices are within bounds
//             if (floor >= 0 && floor < NUM_FLOORS && space >= 0 && space < SPACES_PER_FLOOR) {
//                 parkingLot[floor][space].occupied = true;
//                 parkingLot[floor][space].licensePlate = licensePlate;
//             } else {
//                 std::cout << "Warning: Invalid floor or space number in the reservation file." << std::endl;
//             }
//         }

//         inFile.close();
//     } else {
//         std::cout << "Warning: Unable to open the reservation log file for reading." << std::endl;
//     }
// }

// int main() {
//     ParkingSpace parkingLot[NUM_FLOORS][SPACES_PER_FLOOR];

//     // Load reservations from file
//     loadReservationsFromFile(parkingLot);

//     // Display the initial parking lot status
//     displayParkingLot(parkingLot);

//     // Example reservation
//     reserveParkingSpace(parkingLot);
//     writeToFile(parkingLot);

//     // Display the updated parking lot status
//     displayParkingLot(parkingLot);

//     // Example car departure
//     carLeavesParking(parkingLot);
//     writeToFile(parkingLot);

//     // Display the updated parking lot status after departure
//     displayParkingLot(parkingLot);

//     return 0;
// }






