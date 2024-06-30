#include "Tester.h"

#include <ctime>
#include <filesystem>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
#include <sstream>
#include <string>

using namespace cv;

std::string Tester::get_timestamp() {
  // Get current time
  const std::time_t now = std::time(nullptr);

  // Convert time to structured format
  const std::tm *timeinfo = std::localtime(&now);

  // Format the timestamp string
  std::stringstream ss;
  ss << std::put_time(timeinfo, "%Y-%m-%d_%H-%M-%S");
  return ss.str();
}

bool Tester::create_directory(const std::string &folderPath) {
  try {
    std::filesystem::create_directory(folderPath);
    return true;
  } catch (const std::filesystem::filesystem_error &e) {
    return false;
  }
}

bool Tester::timeExceeded(
    const std::chrono::high_resolution_clock::time_point startTime,
    const int timeInSeconds) {
  const auto endTime = std::chrono::high_resolution_clock::now();
  const auto duration =
      std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);

  return duration < std::chrono::seconds(timeInSeconds);
}

void Tester::startTest(const ImageData &imageData) {
  testThread = std::async(std::launch::async, [this, imageData]() {
    try {
      const auto timestamp = get_timestamp();
      const auto directory = "./Result_" + timestamp;

      create_directory(directory);

      // auto logger = spdlog::basic_logger_mt(
      //     "CameraTest", directory + "/" + timestamp + ".log");
      // spdlog::set_default_logger(logger);

      // spdlog::info("********** Starting Application **********");

      const auto cameraIndex = imageData.cameraIndex;
      const auto simulateWebAtm = imageData.simulateBTM;
      const auto height = imageData.height;
      const auto width = imageData.width;
      const auto codec = imageData.codec;
      const auto fps = imageData.fps;
      const auto time = imageData.time;

      // spdlog::info("Opening Camera with id: {}", cameraIndex);
      VideoCapture cap(cameraIndex, CAP_DSHOW);

      // Check if camera opened successfully
      if (!cap.isOpened()) {
        // spdlog::critical("Could not open Camera with id: {}", cameraIndex);
        std::cerr << "Error opening camera!" << std::endl;
        throw std::runtime_error("Error opening camera!");
      }

      const auto fourccInt =
          VideoWriter::fourcc(codec[0], codec[1], codec[2], codec[3]);

      std::string filename = directory + "/" + get_timestamp() + '.' + codec +
                             '.' + codecFormat[codec];

      auto writer = VideoWriter(filename, fourccInt, fps, Size(width, height));

      if (!writer.isOpened()) {
        // spdlog::error(
        //     "Could not open the output video file for write. Config: " +
        //     codec + ' ' + codecFormat[codec]);
        std::cerr << "Could not open the output video file for write\n";
        writer.release();
        throw std::runtime_error(
            "ould not open the output video file for write");
      }

      auto startTime = std::chrono::high_resolution_clock::now();

      while (timeExceeded(startTime, time)) {
        Mat frame;
        cap >> frame;

        if (frame.empty()) {
          // spdlog::error(
          //     "Can't receive frame (stream end?). Exiting... . Config: " +
          //     codec);
          std::cerr << "Can't receive frame (stream end?). Exiting...\n";
          break;
        }

        writer.write(frame);
        if (simulateWebAtm)
          std::this_thread::sleep_for(std::chrono::milliseconds(200));
      }

      writer.release();
      std::cout << codec << " done\n";
      // spdlog::info("config: " + codec + " Done.");
      cap.release();
      // spdlog::info("********** Ending Application **********");
    } catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
      emit testFinished();
    }
    emit testFinished();
  });
}
