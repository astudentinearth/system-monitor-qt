
#include "MainWindow.hpp"
#include "system/Sensors.hpp"
#include "util.hpp"
#include <QApplication>
#include <cstdio>
#include <cstring>

using namespace std;

int cli() {
  RamStats ramStats = getRamStats();
  printf("Total physical RAM: %.2f GiB (%llu bytes)\n",
         bytesToGiB(ramStats.totalPhysicalRam), ramStats.totalPhysicalRam);
  printf("Page size: %llu bytes\n", ramStats.pageSize);
  printf("Used memory: %.2f GiB (%llu bytes)\n",
         bytesToGiB(ramStats.getUsedRam()), ramStats.getUsedRam());
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
