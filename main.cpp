#include <string>
#include <iostream>
#include <algorithm>
#include <tclap/CmdLine.h>
#include <opencv2/opencv.hpp>


using namespace TCLAP;
using namespace std;
using namespace cv;


int main(int argc, char **argv) {
    // Open the default camera (index 0)
    VideoCapture cap(0, CAP_DSHOW);

    // Check if camera opened successfully
    if (!cap.isOpened()) {
        std::cerr << "Error opening camera!" << std::endl;
        return 1;
    }

    // Create a window to display the camera feed
    namedWindow("Camera", WINDOW_AUTOSIZE);

    while (true) {
        // Capture frame-by-frame
        Mat frame;
        cap >> frame;

        // Check if frame is read correctly
        if (frame.empty()) {
            std::cerr << "Can't receive frame (stream end?). Exiting..." << std::endl;
            break;
        }

        // Display the resulting frame
        imshow("Camera", frame);

        // Wait for 'q' key to close the window
        if (waitKey(1) == 'q') {
            break;
        }
    }

    // Release capture and close window
    cap.release();
    destroyAllWindows();

    return 0;
}
