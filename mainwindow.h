#pragma once

#include <QWidget>

#include <future>

class Tester;
class QPushButton;
class QCheckBox;
class QComboBox;
class QVBoxLayout;
class QBoxLayout;

inline QStringList resulotions = {"360p", "480p", "600p", "720p", "786p"};

inline QStringList codec = {"WMV1", "WMV2", "DIVX", "XVID", "MP4V", "H264",
                            "DIVX", "XVID", "MJPG", "IYUV", "UYVY", "YV12"};

class MainWindow final : public QWidget {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);

private:
  static uint8_t cameraNumber();
  void initConnections();

  static void toggleWidgets(QBoxLayout *layout, bool setDisable = true);

private:
  QPushButton *m_start;
  QCheckBox *m_simulateBTM;
  QComboBox *m_cameraIndex;
  QComboBox *m_resulotion;
  QComboBox *m_codec;
  QVBoxLayout *m_layout;

  std::shared_ptr<Tester> m_tester;
};
