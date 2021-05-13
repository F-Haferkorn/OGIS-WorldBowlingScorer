#pragma once

#include <QList>
#include <QMainWindow>
#include <QShowEvent>
#include <QToolBar>

#include <ogis/wbscorer/bowlscorer.h>

class BowlScorerMainWindow : public QMainWindow {
  Q_OBJECT

public:
  // all members are linked using the Qt-Parent mechanism
  // and are resolved at deletion
  // except
  ~BowlScorerMainWindow();
  BowlScorerMainWindow(QWidget *parent = nullptr);

  void throwPins(int noPins);

public slots:

  void throwFault();
  void throw0();
  void throw1();
  void throw2();
  void throw3();
  void throw4();
  void throw5();
  void throw6();
  void throw7();
  void throw8();
  void throw9();
  void throw10();

  void gameEnds();
  void restartGame();
  void resetGame();
  void addPlayer();

private:
  void respawnBowlScorer(int countOfPlayers);

  void enableToolBar(int allowedCountOfPins);
  QToolBar *initToolBar();
  int c_initialPlayersCount = 1;

  BowlScorer *m_bowlScorer = nullptr;

  QToolBar *m_toolBar = nullptr;
  QAction *m_restartAction = nullptr;
  QAction *m_addPlayerAction = nullptr;

  QSizePolicy m_sizePolicy =
      QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
};
