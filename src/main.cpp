
#include "MainWindow.hpp"
#include "system/Sensors.hpp"
#include <QApplication>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <thread>

using namespace std;

int cli() {
  auto cpuStart = getCpuStats();
  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  auto cpuEnd = getCpuStats();

  auto stats = getDetailedRamStats();
  cout << fixed << setprecision(2);
  for(int i = 0; i < stats.size(); i++) {
        cout << stats[i] << endl;
  }

  cout << "CPU usage: " << cpuStart.percentAgainst(cpuEnd) << '%' << endl;

  return 0;
}

int gui(int argc, char *argv[]) {
  QApplication app(argc, argv);
  MainWindow *window = new MainWindow();
  window->show();
  return app.exec();
}

int main(int argc, char *argv[]) {
  bool headless = false;
  for (int i = 0; i < argc; i++) {
    if (strcmp("--cli", argv[i]) == 0)
      headless = true;
  }

  if (headless)
    return cli();
  else
    return gui(argc, argv);
}
