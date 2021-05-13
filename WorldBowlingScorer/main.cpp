#include <ogis/wbscorer/bowlscorermainwindow.h>

#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  BowlScorerMainWindow w;
  w.show();
  return a.exec();
}
