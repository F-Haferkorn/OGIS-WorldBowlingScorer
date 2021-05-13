#pragma once

#include <QObject>

#include <ogis/wbscorer/bowlscorer.h>

class TestBowlScorer : public BowlScorer {
  Q_OBJECT

public:
  TestBowlScorer() : BowlScorer(1, nullptr) {}
  virtual ~TestBowlScorer() {}

  void throwPins(int noPins);

  bool hasGameEnded() { return m_gameHasEnded; }
  void setGameHasEnded(bool hasEnded) { m_gameHasEnded = hasEnded; }

  int allowedPinCount() { return m_allowedCountOfPins; }
  void setAllowedPinCount(int allowedCountOfPins) {
    m_allowedCountOfPins = allowedCountOfPins;
  }

private slots:
  void initTestCase();
  void cleanupTestCase();

  void test_case1();

private:
  int m_allowedCountOfPins = 10;
  bool m_gameHasEnded = false;
};

QTEST_MAIN(TestBowlScorer)
