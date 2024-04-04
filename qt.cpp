#include <QApplication>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <fstream>
#include <ctime>
#include <opencv2/opencv.hpp>
#include "util.h"
#include <cstdio> // Include the necessary header for std::remove
const int NUM_FLOORS = 3;
const int SPACES_PER_FLOOR = 10;


struct ParkingSpace {
    bool occupied = false;
    std::time_t reservationTime = 0;
    std::time_t departureTime = 0;
    std::string licensePlate = "";
    std::string carColor = "";
    std::string file_name = "";

};

class ParkingReservationWindow : public QWidget {
    Q_OBJECT
public:
    ParkingReservationWindow(QWidget *parent = nullptr) : QWidget(parent) {
        setupUI();
        loadBalansFromFile();
        displayParkingLot();
    }

private slots:



// void reserveParking() {
//     int floor = floorInput->text().toInt();
//     int space = spaceInput->text().toInt();
//     QString licensePlate = licensePlateInput->text();
//     QString carColor = carColorInput->text();

//     if (!validateInput(floor, space)) return;
//     if (parkingLot[floor - 1][space - 1].occupied) {
//         QMessageBox::warning(this, "Parking Space Occupied", "Parking space is already occupied.");
//         return;
//     }

//     // Assuming you have access to the detected car image in this slot
//     cv::Mat detectedCarImage; // This should be the detected car image
//     // Call reserveSpace function with detected car image
//     reserveSpace(floor, space, licensePlate, carColor, detectedCarImage);
//     writeBalansToFile();
//     displayParkingLot();
// }




    void reserveParking() {
        int floor = floorInput->text().toInt();
        int space = spaceInput->text().toInt();
        QString licensePlate = licensePlateInput->text();
        QString carColor = carColorInput->text();

        if (!validateInput(floor, space)) return;
        if (parkingLot[floor - 1][space - 1].occupied) {
            QMessageBox::warning(this, "Parking Space Occupied", "Parking space is already occupied.");
            return;
        }

        reserveSpace(floor, space, licensePlate, carColor);
        writeBalansToFile();
        displayParkingLot();
    }

    void carLeavesParking() {
        int floor = floorInput->text().toInt();
        int space = spaceInput->text().toInt();
        QString licensePlate = licensePlateInput->text();

        if (!validateInput(floor, space)) return;
        if (!parkingLot[floor - 1][space - 1].occupied) {
            QMessageBox::warning(this, "Parking Space Empty", "Parking space is not occupied.");
            return;
        }

        leaveParking(floor, space, licensePlate);
        writeDepartureToFile(floor, space, licensePlate);
        displayParkingLot();
        writeBalansToFile();
        writeBillToFile(floor, space, licensePlate);
    }

    // Define the calculateParkingDuration function
    int calculateParkingDuration(const ParkingSpace& parkingSpace) {
        std::time_t currentTime = std::time(nullptr);
        return static_cast<int>(std::difftime(currentTime, parkingSpace.reservationTime) / 60); // Convert seconds to minutes
    }

private:
    QLineEdit *floorInput;
    QLineEdit *spaceInput;
    QLineEdit *licensePlateInput;
    QLineEdit *carColorInput;
    ParkingSpace parkingLot[NUM_FLOORS][SPACES_PER_FLOOR];
    QGridLayout *parkingLayout;
    std::string data_address = "/Users/yahyagoorakani/Documents/parking-project/data/cars/";

    void setupUI() {
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        parkingLayout = new QGridLayout();
        mainLayout->addLayout(parkingLayout);

        QGridLayout *inputLayout = new QGridLayout();
        mainLayout->addLayout(inputLayout);

        inputLayout->addWidget(new QLabel("Floor Number:"), 0, 0);
        floorInput = new QLineEdit(this);
        inputLayout->addWidget(floorInput, 0, 1);

        inputLayout->addWidget(new QLabel("Space Number:"), 1, 0);
        spaceInput = new QLineEdit(this);
        inputLayout->addWidget(spaceInput, 1, 1);

        inputLayout->addWidget(new QLabel("License Plate:"), 2, 0);
        licensePlateInput = new QLineEdit(this);
        inputLayout->addWidget(licensePlateInput, 2, 1);

        inputLayout->addWidget(new QLabel("Car Color:"), 3, 0);
        carColorInput = new QLineEdit(this);
        inputLayout->addWidget(carColorInput, 3, 1);

        QPushButton *reserveButton = new QPushButton("Reserve", this);
        inputLayout->addWidget(reserveButton, 4, 0);
        connect(reserveButton, &QPushButton::clicked, this, &ParkingReservationWindow::reserveParking);

        QPushButton *leaveButton = new QPushButton("Car Leaves Parking", this);
        inputLayout->addWidget(leaveButton, 4, 1);
        connect(leaveButton, &QPushButton::clicked, this, &ParkingReservationWindow::carLeavesParking);
    }

    bool validateInput(int floor, int space) {
        if (floor < 1 || floor > NUM_FLOORS || space < 1 || space > SPACES_PER_FLOOR) {
            QMessageBox::warning(this, "Invalid Input", "Invalid floor or space number.");
            return false;
        }
        return true;
    }

    void loadBalansFromFile() {
        std::ifstream inFile("balans.txt");
        if (inFile.is_open()) {
            int floor, space;
            std::string licensePlate;
            std::string file_name;
            while (inFile >> licensePlate >> floor >> space >> file_name) {
                parkingLot[floor - 1][space - 1].occupied = true;
                parkingLot[floor - 1][space - 1].licensePlate = licensePlate;
                parkingLot[floor - 1][space - 1].file_name = file_name;
            }
            inFile.close();
        }
    }





// void reserveSpace(int floor, int space, const QString &licensePlate, const QString &carColor, const cv::Mat &carImage) {
//     parkingLot[floor - 1][space - 1].occupied = true;
//     parkingLot[floor - 1][space - 1].reservationTime = std::time(nullptr);
//     parkingLot[floor - 1][space - 1].licensePlate = licensePlate.toStdString();
//     parkingLot[floor - 1][space - 1].carColor = carColor.toStdString();
//     std::string file_name{std::to_string(floor - 1) +"_" + std::to_string(space - 1) + ".png"};
//     parkingLot[floor - 1][space - 1].file_name = file_name;

//     // Write reservation details to reservation.txt
//     std::ofstream outFile("reservation.txt", std::ios::app);
//     if (outFile.is_open()) {
//         outFile << "Floor: " << floor << ", Space: " << space << ", License Plate: " << licensePlate.toStdString()
//                 << ", Car Color: " << carColor.toStdString() << ", Reservation Time: "
//                 << std::put_time(std::localtime(&parkingLot[floor - 1][space - 1].reservationTime), "%Y-%m-%d %H:%M:%S")
//                 << ", Image Name: " << file_name << std::endl;
//         outFile.close();
//     } else {
//         QMessageBox::warning(this, "Error", "Unable to open the reservation file.");
//     }

//     // Save the detected car image

//     cv::Mat out_frame;
// if (!carImage.empty()) {
//     cv::resize(carImage, out_frame, cv::Size(256, 256));
//     cv::imwrite(data_address + file_name, out_frame);
// } else {
//     // Handle the case where the input image is empty
//     std::cerr << "Error: Input image is empty or null." << std::endl;
// }

//     // Display the parking lot
//     displayParkingLot();
//  }





    void reserveSpace(int floor, int space, const QString &licensePlate, const QString &carColor) {
        parkingLot[floor - 1][space - 1].occupied = true;
        parkingLot[floor - 1][space - 1].reservationTime = std::time(nullptr);
        parkingLot[floor - 1][space - 1].licensePlate = licensePlate.toStdString();
        parkingLot[floor - 1][space - 1].carColor = carColor.toStdString();
        std::string file_name{std::to_string(floor - 1) +"_" + std::to_string(space - 1) + ".png"};
        parkingLot[floor - 1][space - 1].file_name = file_name;

        // Write reservation details to reservation.txt
        std::ofstream outFile("reservation.txt", std::ios::app);
        if (outFile.is_open()) {
            outFile << "Floor: " << floor << ", Space: " << space << ", License Plate: " << licensePlate.toStdString()
                    << ", Car Color: " << carColor.toStdString() << ", Reservation Time: "
                    << std::put_time(std::localtime(&parkingLot[floor - 1][space - 1].reservationTime), "%Y-%m-%d %H:%M:%S")
                    << ", Image Name: " << file_name << std::endl;
            outFile.close();
        } else {
            QMessageBox::warning(this, "Error", "Unable to open the reservation file.");
        }

        //Capture image and save
        cv::VideoCapture capture = cv::VideoCapture("/Users/yahyagoorakani/Documents/parking-project/data/video1.mp4");
        if (!capture.isOpened()) {
            std::cerr << "Couldnt open the camera!\n";
        }
        cv::Mat frame;
        for (int i{}; i < 10; i++) {
            capture >> frame;
        }

        
        capture >> frame;
        cv::Mat out_frame;
        cv::resize(frame, out_frame, cv::Size(256, 256));
        cv::imwrite(data_address + file_name, out_frame);
    }

    void leaveParking(int floor, int space, const QString &licensePlate) {
        if (!parkingLot[floor - 1][space - 1].occupied) {
            QMessageBox::warning(this, "Parking Space Empty", "Parking space is not occupied.");
            return;
        }

        if (parkingLot[floor - 1][space - 1].licensePlate != licensePlate.toStdString()) {
            QMessageBox::warning(this, "Invalid License Plate", "The entered license plate does not match the parked car.");
            return;
        }

        parkingLot[floor - 1][space - 1].occupied = false;
        parkingLot[floor - 1][space - 1].departureTime = std::time(nullptr);

        deleteImageFile(floor, space); // Call the function to delete the image file
    }

    void deleteImageFile(int floor, int space) {
        std::string filepath = data_address + parkingLot[floor - 1][space - 1].file_name;

        if (std::remove(filepath.c_str()) != 0) {
            perror("Error deleting file");
        }
        else {
            printf("File successfully deleted: %s\n", filepath.c_str());
        }
    }
void writeDepartureToFile(int floor, int space, const QString &licensePlate) {
    // Write departure details to Departure.txt
    std::ofstream outFile("Departure.txt", std::ios::app);
    if (outFile.is_open()) {
        outFile << "Floor: " << floor << ", Space: " << space << ", License Plate: " << licensePlate.toStdString()
                << ", Departure Time: " << std::put_time(std::localtime(&parkingLot[floor - 1][space - 1].departureTime), "%Y-%m-%d %H:%M:%S")
                << ", Image Name: " << parkingLot[floor - 1][space - 1].file_name << std::endl;
        outFile.close();
    } else {
        QMessageBox::warning(this, "Error", "Unable to open the Departure file.");
    }
}

void writeBillToFile(int floor, int space, const QString &licensePlate) {
    // Get the current time
    std::time_t currentTime = std::time(nullptr);

    // Open the reservation file to find the arrival time
    std::ifstream inFile("reservation.txt");
    std::string arrivalTime;
    if (inFile.is_open()) {
        std::string line;
        while (std::getline(inFile, line)) {
            if (line.find("License Plate: " + licensePlate.toStdString()) != std::string::npos) {
                arrivalTime = line.substr(line.find("Reservation Time: ") + 18, 19);
                break;
            }
        }
        inFile.close();
    }

    // Calculate parking duration
    std::tm departureTM = *std::localtime(&currentTime);
    std::istringstream arrivalStream(arrivalTime);
    std::tm arrivalTM = {};
    arrivalStream >> std::get_time(&arrivalTM, "%Y-%m-%d %H:%M:%S");
    std::time_t arrival = std::mktime(&arrivalTM);
    int parkingDuration = static_cast<int>(std::difftime(currentTime, arrival) / 60);

    // Open the bill.txt file to append departure information
    std::ofstream outFile("bill.txt", std::ios::app);
    if (outFile.is_open()) {
        // Write bill details to bill.txt
        outFile << "License Plate: " << licensePlate.toStdString() << std::endl;
        outFile << "Floor: " << floor << std::endl;
        outFile << "Space: " << space << std::endl;
        outFile << "Arrival Time: " << arrivalTime << std::endl;
        outFile << "Departure Time: " << std::put_time(&departureTM, "%Y-%m-%d %H:%M:%S") << std::endl;
        outFile << "Parking Duration: " << parkingDuration << " minutes" << std::endl;
        outFile << "Image Name: " << parkingLot[floor - 1][space - 1].file_name << std::endl;
        outFile << "--------------------------------------" << std::endl;

        // Close the file
        outFile.close();
    } else {
        QMessageBox::warning(this, "Error", "Unable to open the bill file.");
    }
}


    void writeBalansToFile() {
        std::ofstream outFile("balans.txt", std::ios::trunc);
        if (outFile.is_open()) {
            for (int floor = 0; floor < NUM_FLOORS; ++floor) {
                for (int space = 0; space < SPACES_PER_FLOOR; ++space) {
                    if (parkingLot[floor][space].occupied) {
                        outFile << parkingLot[floor][space].licensePlate << " " <<  floor + 1 << " " << space + 1 << " " << parkingLot[floor][space].file_name <<std::endl;
                    }
                }
            }
            outFile.close();
        } else {
            QMessageBox::warning(this, "Error", "Unable to open the reservation log file.");
        }
    }

    void displayParkingLot() {
        clearParkingLayout();

        QPixmap carPixmap;

        for (int floor = 0; floor < NUM_FLOORS; ++floor) {
            for (int space = 0; space < SPACES_PER_FLOOR; ++space) {
                QLabel *label = new QLabel();
                label->setAlignment(Qt::AlignCenter);
                label->setFixedSize(124, 140);

                if (parkingLot[floor][space].occupied) {
                    label->setText(QString::fromStdString(parkingLot[floor][space].licensePlate));
                    std::string file_address{data_address+std::to_string(floor) + "_" + std::to_string(space) + ".png"};
                    if (carPixmap.load(QString::fromStdString(file_address))) {
                        if (carPixmap.isNull()) {
                            qDebug() << "Error loading car photo.";
                        }
                        label->setPixmap(carPixmap);
                    }
                } else {
                    label->setText("Empty");
                }
                parkingLayout->addWidget(label, floor, space);
            }
        }
    }

    void clearParkingLayout() {
        QLayoutItem *child;
        while ((child = parkingLayout->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ParkingReservationWindow window;
    window.setWindowTitle("Parking Reservation System");
    window.show();
    return app.exec();
}

#include "qt.moc"
