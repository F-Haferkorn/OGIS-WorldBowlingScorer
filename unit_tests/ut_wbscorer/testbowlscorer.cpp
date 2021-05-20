#include <loop>

#include <QtTest>

#include "testbowlscorer.h"
#include <QObject>

#include <ogis/wbscorer/bowlscorer.h>

class TestBowlScorer : public BowlScorer {
  Q_OBJECT

public:
  TestBowlScorer() : BowlScorer(1, nullptr) {}
  ~TestBowlScorer() override;

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

  void test_case_10_strikes__resultScoreOf_240();
  void test_case_12_strikes__resultScoreOf_300();

private:
  BowlPlayerScore *scorer = nullptr;
  int m_allowedCountOfPins = 10;
  bool m_gameHasEnded = false;
};

TestBowlScorer::~TestBowlScorer() {}
void TestBowlScorer::initTestCase() {
  m_gameHasEnded = false;
  m_allowedCountOfPins = 10;
  scorer = BowlScorer::playerScore(0);
  QVERIFY(scorer != nullptr);
}

void TestBowlScorer::cleanupTestCase() {}

void TestBowlScorer::throwPins(int noPins) {
  if (hasGameEnded()) {
    return;
  }
  if (noPins >= allowedPinCount()) {
    return;
  }
  auto allowedCountOfPins = BowlScorer::throwPins(noPins);
  if (hasGameEnded()) {
    setGameHasEnded(true);
  } else if (allowedCountOfPins == 0) {
    setAllowedPinCount(10);
  } else {
    setAllowedPinCount(allowedCountOfPins);
  }
}

void TestBowlScorer::test_case_10_strikes__resultScoreOf_240() {
  // for (int i = 0; i < 12; i++)
  //   BowlScorer::throwPins(10);
  initTestCase();
  loop(10) BowlScorer::throwPins(10);
  QVERIFY2(scorer->resultScore() == 240,
           "10 strikes in a row MUST give a total score of 240!");
}

void TestBowlScorer::test_case_12_strikes__resultScoreOf_300() {
  /// for (int i = 0; i < 12; i++)
    ///  BowlScorer::throwPins(10);
  ///
  initTestCase();
  loop(10) BowlScorer::throwPins(10);
  QVERIFY2(scorer->resultScore() == 300,
           "12 strikes in a row MUST give a total score of 300!");
}
QTEST_MAIN(TestBowlScorer)
#include "testbowlscorer.moc"
