#pragma once

#include <cstdint>
#include <string>

inline std::unordered_map<std::string, std::string> codecFormat{
    {"WMV1", "wmv"}, {"WMV2", "wmv"}, {"DIVX", "mp4"}, {"XVID ", "mp4"},
    {"MP4V", "mp4"}, {"H264", "mp4"}, {"DIVX", "avi"}, {"XVID", "avi"},
    {"MJPG", "avi"}, {"IYUV", "avi"}, {"UYVY", "avi"}, {"YV12", "avi"}};

inline std::unordered_map<uint, uint> resulotionTable = {
    {360, 640}, {480, 640}, {600, 800}, {720, 1280}, {768, 1024}};

struct ImageData {
  uint8_t cameraIndex{0};
  uint width{640};
  uint height{480};
  std::string codec{"WMV2"};
  uint fps{5};
  uint16_t time{10};
  bool simulateBTM{true};
};
