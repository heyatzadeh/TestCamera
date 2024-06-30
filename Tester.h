#pragma once

#include <future>

#include <QObject.h>

#include "ImageData.h"

class Tester final : public QObject {
  Q_OBJECT

public:
  void startTest(const ImageData &imageData);

signals:
  void testFinished();

private:
  static std::string get_timestamp();
  static bool create_directory(const std::string &folderPath);
  static bool
  timeExceeded(std::chrono::high_resolution_clock::time_point startTime,
               int timeInSeconds);

private:
  std::future<void> testThread;
};
