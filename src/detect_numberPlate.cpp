// //this code need a yolov7-tiny.weights for detect licnse plate

// #include <iostream>
// #include <fstream>
// #include <opencv2/opencv.hpp>
// #include <opencv2/dnn.hpp>
// #include <tesseract/baseapi.h>
// #include <leptonica/allheaders.h>
// #include "util.h" 

// using namespace std;
// using namespace cv;
// using namespace cv::dnn;

// // Function to perform OCR using Tesseract
// string performOCR(const Mat& plate) {
//     // Initialize Tesseract OCR engine
//     tesseract::TessBaseAPI ocr;
//     ocr.Init(nullptr, "eng", tesseract::OEM_LSTM_ONLY); // Use LSTM OCR engine for better accuracy

//     // Convert OpenCV Mat to Leptonica PIX
//     Pix* image = pixCreate(plate.cols, plate.rows, 8);
//     l_uint32* data = reinterpret_cast<l_uint32*>(plate.data);
//     pixSetData(image, data);

//     ocr.SetImage(image);

//     // Perform OCR
//     char* outText = ocr.GetUTF8Text();
//     string result(outText);

//     // Cleanup
//     delete[] outText;
//     pixDestroy(&image);

//     return result;
// }


// int main() {
//     // Load the video
//     VideoCapture cap("../data/video1.mp4");
//     if (!cap.isOpened()) {
//         cerr << "Error while trying to read video. Please check the path again" << endl;
//         return -1;
//     }

//     // Define the output video writer
//     int frame_width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
//     int frame_height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));
//     VideoWriter out("result.mp4", VideoWriter::fourcc('m', 'p', '4', 'v'), 30, Size(frame_width, frame_height));

//     // Load YOLOv7 model
//     String modelConfiguration = "/Users/yahya/Documents/parking-project/yolov7-tiny.cfg";
//     String modelWeights = "/Users/yahya/Documents/parking-project/yolov7-tiny.weights";
//     Net net = readNetFromDarknet(modelConfiguration, modelWeights);
//     if (net.empty()) {
//         cerr << "Failed to read YOLOv7 model" << endl;
//         return -1;
//     }

//     // Open a file to save detected license plate numbers
//     ofstream outputFile("detected_plate_numbers.txt");
//     if (!outputFile.is_open()) {
//         cerr << "Failed to open output file" << endl;
//         return -1;
//     }

//     // Process each frame
//     while (true) {
//         Mat frame;
//         cap >> frame;

//         if (frame.empty()) {
//             break;
//         }
//     net.setPreferableBackend( cv::dnn::DNN_BACKEND_OPENCV );
//      net.setPreferableTarget( cv::dnn::DNN_TARGET_CPU );
//         // Object detection using YOLOv7
//         Mat blob;
//         blobFromImage(frame, blob, 1 / 255.0, Size(416, 416), Scalar(0, 0, 0), true, false);
//         net.setInput(blob);
//         vector<Mat> outs;
//         net.forward(outs, getOutputsNames(net));

//         // Iterate through the detected objects
//         for (const auto& out : outs) {
//             for (int i = 0; i < out.rows; i++) {
//                 float* data = (float*)out.ptr(i);
//                 float confidence = data[5];

//                 // Check if the confidence level is high enough
//                 if (confidence > 0.5) {
//                     int classId = static_cast<int>(data[1]);
//                     if (classId == 0) { // Assuming classId 0 corresponds to license plates
//                         // Extract the bounding box coordinates
//                         int x = static_cast<int>(data[0] * frame.cols);
//                         int y = static_cast<int>(data[1] * frame.rows);
//                         int width = static_cast<int>(data[2] * frame.cols);
//                         int height = static_cast<int>(data[3] * frame.rows);

//                         // Crop the license plate region
//                         Rect plateRegion(x, y, width, height);
//                         Mat plate = frame(plateRegion);

//                         // Perform license plate recognition using Tesseract OCR
//                         string plateNumber = performOCR(plate);

//                         // Save the detected license plate number to the output file
//                         outputFile << plateNumber << endl;

//                         // Draw bounding box and license plate number on the frame
//                         rectangle(frame, plateRegion, Scalar(0, 255, 0), 2);
//                         putText(frame, plateNumber, Point(x, y - 10), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);
//                     }
//                 }
//             }
//         }

//         // Write the frame to the output video
//         out.write(frame);

//         // Display the frame
//         imshow("Frame", frame);

//         // Press 'q' to exit
//         if (waitKey(1) == 'q') {
//             break;
//         }
//     }

//     // Release video capture and video writer
//     cap.release();
//     out.release();

//     // Close the output file
//     outputFile.close();

//     return 0;
// }
