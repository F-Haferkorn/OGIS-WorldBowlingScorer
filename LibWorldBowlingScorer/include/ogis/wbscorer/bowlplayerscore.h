#pragma once

#include <QGraphicsEffect>
#include <QList>
#include <QWidget>

#include <ogis/wbscorer/bowlframescore.h>

class BowlPlayerScore : public QWidget
{
  Q_OBJECT
public:
  // all members are linked using the Qt-Parent mechanism
  // and are resolved at deletion

  ~BowlPlayerScore() = default;
  explicit BowlPlayerScore(QWidget* parent);

  int thrownCountOfPins(int pinsThrown);

  int frameId() { return m_currentFrameIndex + 1; }
  void setFrameId(int frameId) { m_currentFrameIndex = frameId - 1; }

  int resultScore();

  BowlFrameScore* frameScore(int index);

  BowlFrameScore* currentFrameScore() { return frameScore(frameId() - 1); }

  BowlFrameScore* previousFrameScore()
  {
    return frameId() > 1 ? frameScore(frameId() - 2) : nullptr;
  }

public slots:

  void clear();
  void activateCurrentFrame(bool enable);
  void markResult(const QColor& color);

private:
  int findfirstTrailingStrikeInARow(int lastStrike);
  void treatRowOfStrikes(BowlFrameScore* currentFrame,
                         int firstStrikeInARow,
                         int lastStrike);

  void updateAllFrameScores();

  QWidget* createResultWidget();
  void activateNextFrameScore();

  const int c_maxItemCount = 10;
  int m_currentFrameIndex = 0;

  QList<BowlFrameScore*> m_frameScores;
  QLabel* m_resultLabel = nullptr;

  int m_regularFrameLineWidth = 1;
  int m_activeFrameLineWidth = 1;

  QGraphicsColorizeEffect* m_resultEffect;
};

Q_DECLARE_TYPEINFO(BowlPlayerScore*, Q_PRIMITIVE_TYPE);
