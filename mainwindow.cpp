#include "mainwindow.h"

#include "ImageData.h"

#include <QCameraDevice>
#include <QCheckBox>
#include <QComboBox>
#include <QMediaDevices>
#include <QPushButton>
#include <QVBoxLayout>

#include "Tester.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), m_tester(std::make_shared<Tester>()) {
  setWindowTitle("Camera Test");
  setGeometry(100, 100, 200, 100);

  m_start = new QPushButton("Start", this);
  m_simulateBTM = new QCheckBox("Simulate BTM", this);
  m_cameraIndex = new QComboBox(this);
  m_resulotion = new QComboBox(this);
  m_codec = new QComboBox(this);
  m_layout = new QVBoxLayout(this);

  m_resulotion->addItems(resulotions);

  m_codec->addItems(codec);

  const auto availableCamera = cameraNumber();
  for (auto i{0}; i < availableCamera; i++) {
    m_cameraIndex->addItem(QString("Camera %1").arg(i + 1));
  }

  m_layout->addWidget(m_cameraIndex);
  m_layout->addWidget(m_simulateBTM);
  m_layout->addWidget(m_resulotion);
  m_layout->addWidget(m_codec);
  m_layout->addWidget(m_start);
  setLayout(m_layout);

  initConnections();
}

uint8_t MainWindow::cameraNumber() {
  return QMediaDevices::videoInputs().count();
}

void MainWindow::initConnections() {
  connect(m_simulateBTM, &QCheckBox::stateChanged, this,
          [this](const int isChecked) {
            m_resulotion->setDisabled(isChecked);
            m_codec->setDisabled(isChecked);
          });

  connect(m_start, &QPushButton::clicked, this, [this]() {
    toggleWidgets(m_layout, true);

    auto imageData = ImageData();
    imageData.simulateBTM = m_simulateBTM->isChecked();
    imageData.cameraIndex = m_cameraIndex->currentIndex();

    if (!imageData.simulateBTM) {
      imageData.height = m_resulotion->currentText().split("p")[0].toInt();
      imageData.width = resulotionTable[imageData.height];
      imageData.codec = m_codec->currentText().toStdString();
      imageData.fps = 30;
    }
    imageData.time = 10; // TODO

    m_tester->startTest(imageData);
  });

  connect(m_tester.get(), &Tester::testFinished, this,
          [this]() { toggleWidgets(m_layout, false); });
}

void MainWindow::toggleWidgets(QBoxLayout *layout, const bool setDisable) {
  if (layout == nullptr)
    return;

  for (int i = 0; i < layout->count(); ++i) {

    if (QLayoutItem *item = layout->itemAt(i); item) {
      if (QWidget *widget = item->widget(); widget) {
        widget->setEnabled(!setDisable);
      }
    }
  }
}
