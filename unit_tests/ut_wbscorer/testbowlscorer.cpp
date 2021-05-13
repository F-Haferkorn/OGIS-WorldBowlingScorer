#include <QtTest>

#include "testbowlscorer.h"

void TestBowlScorer::initTestCase() {
  m_gameHasEnded = false;
  m_allowedCountOfPins = 10;
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
