#include <string>
#include <iostream>
#include <tclap/CmdLine.h>
#include <opencv2/opencv.hpp>
#include <ctime>
#include <filesystem>
#include <sstream>


using namespace TCLAP;
using namespace std;
using namespace cv;


std::vector<std::pair<std::string, std::string> > codec{
    {"WMV1", "wmv"},
    {"WMV2", "wmv"},

    {"DIVX", "mp4"},
    {"XVID ", "mp4"},
    {"MP4V", "mp4"},
    {"H264", "mp4"},

    {"DIVX", "avi"},
    {"XVID", "avi"},
    {"MJPG", "avi"},
    {"IYUV", "avi"},
    {"UYVY", "avi"},
    {"YV12", "avi"}
};

std::string get_timestamp();

bool create_directory(const std::string &folderPath);

int main(int argc, char **argv) {
    try {
        CmdLine cmd("Command description message", ' ', "0.9");

        ValueArg<uint> timeArg("t", "time", "time for video", false, 100, "default is 100");
        cmd.add(timeArg);

        ValueArg cameraIndexArg("i", "cameraIndex", "camera index", false, 0, "starts from 0");
        cmd.add(cameraIndexArg);

        ValueArg widthArg("w", "width", "Resolution Width", false, 640, "default is 640");
        cmd.add(widthArg);

        ValueArg heightArg("u", "height", "Resolution Height", false, 480, "default is 480");
        cmd.add(heightArg);

        SwitchArg simulateWebAtmSwitch("s", "simulate", "Simulate Web-ATM", false);
        cmd.add(simulateWebAtmSwitch);

        cmd.parse(argc, argv);

        int cameraIndex = cameraIndexArg.getValue();
        bool simulateWebAtm = simulateWebAtmSwitch.getValue();
        int height = heightArg.getValue();
        int width = widthArg.getValue();
        uint time = timeArg.getValue();

        // Open the default camera (index 0)
        VideoCapture cap(cameraIndex, CAP_DSHOW);

        // Check if camera opened successfully
        if (!cap.isOpened()) {
            std::cerr << "Error opening camera!" << std::endl;
            return 1;
        }

        if (simulateWebAtm) {
            time = time / 10;
            cap.set(CAP_PROP_FRAME_WIDTH, 320);
            cap.set(CAP_PROP_FRAME_HEIGHT, 240);
        } else {
            cap.set(CAP_PROP_FRAME_WIDTH, width);
            cap.set(CAP_PROP_FRAME_HEIGHT, height);
        }


        const auto timestamp = get_timestamp();
        const auto directory = "./Result_" + timestamp;


        create_directory(directory);

        for (const auto &[fourcc, type]: codec) {
            const auto fourccInt = VideoWriter::fourcc(fourcc[0], fourcc[1], fourcc[2], fourcc[3]);

            std::string filename = directory + "/" + get_timestamp() + '.' + fourcc + '.' + type;

            VideoWriter writer;
            writer = simulateWebAtm
                         ? VideoWriter(filename, fourccInt, int(30 / 6), Size(320, 240))
                         : VideoWriter(filename, fourccInt, 25.0, Size(width, height));

            if (!writer.isOpened()) {
                cerr << "Could not open the output video file for write\n";
                writer.release();
                continue;
            }

            for (int i{0}; i < time; i++) {
                Mat frame;
                cap >> frame;

                if (frame.empty()) {
                    std::cerr << "Can't receive frame (stream end?). Exiting...\n";
                    break;
                }

                writer.write(frame);
                if (simulateWebAtm)
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }

            writer.release();
            std::cout << fourcc << " ." << type << " done\n";
        }
        cap.release();

        return 0;
    } catch (ArgException &e) {
        cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
    }
    catch (...) {
        cerr << "Unkown Error\n";
    }
}


std::string get_timestamp() {
    // Get current time
    const std::time_t now = std::time(nullptr);

    // Convert time to structured format
    const std::tm *timeinfo = std::localtime(&now);

    // Format the timestamp string
    std::stringstream ss;
    ss << std::put_time(timeinfo, "%Y-%m-%d_%H-%M-%S");
    return ss.str();
}

bool create_directory(const std::string &folderPath) {
    try {
        std::filesystem::create_directory(folderPath);
        return true;
    } catch (const std::filesystem::filesystem_error &e) {
        return false;
    }
}
