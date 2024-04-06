



// #include <iostream>
// #include <fstream>
// #include <string>
// #include <vector>
// #include <opencv2/opencv.hpp>
// #include <opencv2/dnn.hpp>
//  #include "util.h"
// using namespace std;
// using namespace cv;
// using namespace cv::dnn;


// int main( int argc, char* argv[] )
// {




//     // // Open Video Capture
//     cv::VideoCapture capture = cv::VideoCapture("/Users/yahyagoorakani/Documents/parking-project/data/video1.mp4");
//     if( !capture.isOpened() ){
//         return -1;
//     }

//     // Read Class Name List and Color Table
//     const std::string list = "/Users/yahyagoorakani/Documents/parking-project/coco.names";
//     const std::vector<std::string> classes = readClassNameList( list );
//     const std::vector<cv::Scalar> colors = getClassColors( classes.size() );

//     // Read Darknet
//     const std::string model   = "/Users/yahyagoorakani/Documents/parking-project/yolov7-tiny.weights";
//     const std::string config  = "/Users/yahyagoorakani/Documents/parking-project/yolov7-tiny.cfg";
//     cv::dnn::Net net = cv::dnn::readNetFromDarknet( config, model );
//     if( net.empty() ){
//         return -1;
//     }

//     // Set backend and target for DNN processing
//      net.setPreferableBackend( cv::dnn::DNN_BACKEND_OPENCV );
//      net.setPreferableTarget( cv::dnn::DNN_TARGET_CPU );

//     while (true) {
//         // Read Frame
//         Mat frame;
//         capture >> frame;
//         if (frame.empty()) {
//            cv::waitKey(1);
//             break;
//         }
//         if (frame.channels() == 3) {
//           cv::cvtColor( frame, frame, cv::COLOR_BGRA2BGR );
//         }

//          cv::Mat blob = cv::dnn::blobFromImage( frame, 1 / 255.f, cv::Size( 416, 416 ), cv::Scalar(), true, false );

//         // Set Input Blob
//         net.setInput(blob);

//         // Run Forward Network
//        std::vector<cv::Mat> detections;
//        net.forward( detections, getOutputsNames( net ) );

//         // Draw Region
//         std::vector<int32_t> class_ids; std::vector<float> confidences; std::vector<cv::Rect> rectangles;
//         for( cv::Mat& detection : detections ){
//             for( int32_t i = 0; i < detection.rows; i++ ){

//                 cv::Mat region = detection.row( i );

//                 // Retrieve Max Confidence and Class Index
//                 cv::Mat scores = region.colRange( 5, detection.cols );
//                 cv::Point class_id;
//                 double confidence;
//                 //cv::minMaxLoc(nullptr, &confidence, nullptr, &class_id);
//                 cv::minMaxLoc( scores, 0, &confidence, 0, &class_id );
               
//                 // Check Confidence
//                 constexpr float threshold = 0.2;
//                 if (threshold > confidence) {
//                     continue;
//                 }

             

//                 // // Retrieve Object Position
//                 const int32_t x_center = static_cast<int32_t>( region.at<float>( 0 ) * frame.cols );
//                 const int32_t y_center = static_cast<int32_t>( region.at<float>( 1 ) * frame.rows );
//                 const int32_t width    = static_cast<int32_t>( region.at<float>( 2 ) * frame.cols );
//                 const int32_t height   = static_cast<int32_t>( region.at<float>( 3 ) * frame.rows );
                
                
//                 const cv::Rect rectangle  = cv::Rect( x_center - ( width / 2 ), y_center - ( height / 2 ), width, height );
//                 cv::putText(frame, classes[class_id.x] , cv::Point(x_center - ( width / 2 ), y_center - ( height / 2 )), cv::FONT_HERSHEY_PLAIN, 3, cv::Scalar(0, 0, 255), 2);
                
                
//                 // Add Class ID, Confidence, Rectangle
//                 class_ids.push_back(class_id.x);
//                 confidences.push_back(confidence);
//                 rectangles.push_back(rectangle);
//             }
//         }

//         // Remove Overlap Rectangles using Non-Maximum Suppression
//         constexpr float confidence_threshold = 0.4; // Confidence
//         constexpr float nms_threshold = 0.2; // IoU (Intersection over Union)
//         std::vector<int32_t> indices;
//         cv::dnn::NMSBoxes( rectangles, confidences, confidence_threshold, nms_threshold, indices );

//        //Draw Rectangle
//         for( const int32_t& index : indices ){
//             const cv::Rect rectangle = rectangles[index];
//             const cv::Scalar color = colors[class_ids[index]];
//             constexpr int32_t thickness = 3;
//             cv::rectangle( frame, rectangle, color, thickness );
//         }

  


//         // Show Image
//         imshow("Object Detection", frame);

//         const int key = waitKey(1);
//         if (key == 'q') {
//             break;
//         }
//     }

//     destroyAllWindows();

//     return 0;
// }






#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include "util.h"

using namespace std;
using namespace cv;
using namespace cv::dnn;

int main(int argc, char* argv[])
{
    cv::VideoCapture capture = cv::VideoCapture("/Users/yahyagoorakani/Documents/parking-project/data/video2.mp4");
    if (!capture.isOpened()) {
        return -1;
    }

    const std::string list = "/Users/yahyagoorakani/Documents/parking-project/coco.names";
    const std::vector<std::string> classes = readClassNameList(list);
    const std::vector<cv::Scalar> colors = getClassColors(classes.size());

    const std::string model = "/Users/yahyagoorakani/Documents/parking-project/yolov7-tiny.weights";
    const std::string config = "/Users/yahyagoorakani/Documents/parking-project/yolov7-tiny.cfg";
    cv::dnn::Net net = cv::dnn::readNetFromDarknet(config, model);
    if (net.empty()) {
        return -1;
    }

    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

    std::vector<cv::Rect> car_rectangles; // Vector to store rectangles where cars are detected

    
Mat frame;
while (true) {
    capture >> frame;
    if (frame.empty()) {
        cv::waitKey(1);
        break;
    }
    if (frame.channels() == 3) {
        cv::cvtColor(frame, frame, cv::COLOR_BGRA2BGR);
    }

    cv::Mat blob = cv::dnn::blobFromImage(frame, 1 / 255.f, cv::Size(416, 416), cv::Scalar(), true, false);
    net.setInput(blob);

    std::vector<cv::Mat> detections;
    net.forward(detections, getOutputsNames(net));

    std::vector<int32_t> class_ids;
    std::vector<float> confidences;
    std::vector<cv::Rect> rectangles;
    std::vector<cv::Rect> car_rectangles; // Vector to store rectangles where cars are detected

    for (cv::Mat& detection : detections) {
        for (int32_t i = 0; i < detection.rows; i++) {
            cv::Mat region = detection.row(i);
            cv::Point class_id;
            double confidence;
            cv::minMaxLoc(region.colRange(5, detection.cols), 0, &confidence, 0, &class_id);

            if (confidence > 0.2) {
                const int32_t x_center = static_cast<int32_t>(region.at<float>(0) * frame.cols);
                const int32_t y_center = static_cast<int32_t>(region.at<float>(1) * frame.rows);
                const int32_t width = static_cast<int32_t>(region.at<float>(2) * frame.cols);
                const int32_t height = static_cast<int32_t>(region.at<float>(3) * frame.rows);
                const cv::Rect rectangle = cv::Rect(x_center - (width / 2), y_center - (height / 2), width, height);

                if (classes[class_id.x] == "car") { // Check if detected object is a car
                    car_rectangles.push_back(rectangle); // Store the rectangle
                }
            }
        }
    }
    static int car_count = 0; // Static variable to keep track of the total number of saved cars
for (size_t i = 0; i < car_rectangles.size(); ++i) {
    cv::Mat car_image = frame(car_rectangles[i]);
    std::string filename = "car_" + std::to_string(car_count) + ".jpg";
    cv::imwrite(filename, car_image);
    car_count++; // Increment the car count
}


    // for (const auto& rect : car_rectangles) {
    //     cv::rectangle(frame, rect, cv::Scalar(0, 255, 0), 2);
    // }

    imshow("Object Detection", frame);

  // Saving detected cars as separate images
for (size_t i = 0; i < car_rectangles.size(); ++i) {
    cv::Mat car_image = frame(car_rectangles[i]);
    std::string filename = "car_" + std::to_string(i) + ".jpg";
    cv::imwrite(filename, car_image);
}


    const int key = waitKey(1);
    if (key == 'q') {
        break;
    }
}

    destroyAllWindows();

    return 0;
}

