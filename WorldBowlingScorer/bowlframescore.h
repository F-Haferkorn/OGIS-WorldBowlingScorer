#pragma once

#include <QLabel>
#include <QSize>
#include <QWidget>
#include <Qlist>

class BowlFrameScore : public QWidget
{
  Q_OBJECT
public:
  // all members are linked using the Qt-Parent mechanism
  // and are resolved at deletion
  // except UI

  ~BowlFrameScore() = default;
  explicit BowlFrameScore(bool isLlastFrame, QWidget* parent);

  QFrame* widgetFrame();

  void clear();

  int thrownCountOfPins(int pinsThrown);
  int pinsLeftOnTheTable();

  bool hasStrike() { return m_hasStrike; }
  bool hasSpare() { return m_hasSpare; }
  bool shouldScoreBeRendered();

  void setExtra(int extra) { m_extra = extra; }

  int noThrows() { return m_thrownPinValue.size(); }
  int score() { return m_score + m_extra; }
  int totalScore()
  {
    /// do NOT forward own score frame's score is not be rendered.
    /// and will not appear on total score and on result score of class
    /// BowlPlayerScore
    return m_previousScore + (shouldScoreBeRendered() ? score() : 0);
  }
  void setPreviousScore(int previousScore) { m_previousScore = previousScore; }

  int cumulateThrownPins(int maxIndex = -1);
  bool isLastFrame() { return m_isLastFrame; }

  void renderScore();

  void setSkipRendering(bool skipIt) { m_skipRendering = skipIt; }

  bool isFrameClosed(bool useStrike);

private: /// Methods
  void markStrike(bool hasStrike) { m_hasStrike = hasStrike; }
  void markSpare(bool hasSpare) { m_hasSpare = hasSpare; }
  QLabel* throwLabel(int index);

private: // CONSTANTS
  const int c_maxCountOfRolls;
  const int c_counOfAllPins = 10;

private: /// VARIABLES
  QList<int> m_thrownPinValue;
  QLabel* m_scoreLabel = nullptr;

  QList<QLabel*> m_throwLabels;
  QLabel* m_lastThrowLabel = nullptr;

  const int c_strikeBonus = 10;

  int m_previousScore = 0;
  int m_score = 0;
  int m_extra = 0;

  bool m_isLastFrame = false;
  bool m_hasSpare = false;
  bool m_hasStrike = false;

  bool m_skipRendering = false;
};

Q_DECLARE_TYPEINFO(BowlFrameScore*, Q_PRIMITIVE_TYPE);
