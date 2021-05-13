#include <QColor>
#include <QFont>
#include <QFrame>
#include <QHBoxLayout>

#include "bowlplayerscore.h"

BowlPlayerScore::BowlPlayerScore(QWidget* parent)
  : QWidget(parent)
{

  // auto sizePolicy=QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  auto sizePolicy = QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  setSizePolicy(sizePolicy);

  QHBoxLayout* layout = new QHBoxLayout();

  int index = 0;
  for (; index < c_maxItemCount - 1; index++) {
    m_frameScores.push_back(new BowlFrameScore(false, this));
  }
  m_frameScores.push_back(new BowlFrameScore(true, this));

  layout->setContentsMargins(0, 0, 0, 0);
  for (auto itemptr : m_frameScores)
    layout->addWidget(itemptr);

  auto frameWidth = m_frameScores.first()->width();
  auto frameHeight = m_frameScores.first()->height();

  auto resultFrame = createResultWidget();
  resultFrame->setSizePolicy(sizePolicy);
  resultFrame->setMinimumSize(frameWidth, frameHeight);
  resultFrame->setMaximumSize(frameWidth, frameHeight);

  layout->addWidget(resultFrame);
  setLayout(layout);
  auto scoreFrame = m_frameScores.first()->widgetFrame();

  if (scoreFrame) {
    m_regularFrameLineWidth = scoreFrame->lineWidth();
  }
  m_regularFrameLineWidth = qMax(1, m_regularFrameLineWidth);
  m_activeFrameLineWidth = 3 * m_regularFrameLineWidth;
}

void
BowlPlayerScore::clear()
{
  for (auto* item : m_frameScores)
    item->clear();
  m_resultEffect->setEnabled(false);
  setFrameId(1);
}

QWidget*
BowlPlayerScore::createResultWidget()
{ // add QLabel with result score-frame....
  m_resultLabel = new QLabel(this);
  m_resultLabel->setText("        ");
  QFont font = m_resultLabel->font();
  font.setPixelSize(30);
  font.setBold(true);
  m_resultLabel->setFont(font);
  m_resultLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

  auto resultLayout = new QHBoxLayout();
  resultLayout->setContentsMargins(0, 0, 0, 0);
  resultLayout->addWidget(m_resultLabel);

  auto frame = new QFrame(this);

  frame->setLayout(resultLayout);
  frame->setFrameStyle(QFrame::Box | QFrame::Raised);
  frame->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  m_resultEffect = new QGraphicsColorizeEffect();
  m_resultEffect->setEnabled(false);
  m_resultLabel->setGraphicsEffect(m_resultEffect);

  return frame;
}
int
BowlPlayerScore::thrownCountOfPins(int pinsThrown)
{

  int allowedCountOfPins = 0;

  if (auto item = currentFrameScore(); item) {
    allowedCountOfPins = item->thrownCountOfPins(pinsThrown);

    if (item->hasStrike() && !item->isFrameClosed(false))
      item->setSkipRendering(true);

    if (item->hasSpare() && !item->isLastFrame())
      item->setSkipRendering(true);

    if (auto prevItem = previousFrameScore();
        prevItem && prevItem->hasStrike()) {
      auto lastStrikeIndex = frameId() - 2;
      auto firstStrikeInARow = findfirstTrailingStrikeInARow(lastStrikeIndex);

      treatRowOfStrikes(item, firstStrikeInARow, lastStrikeIndex);
    } else if (auto* previousItem = previousFrameScore();
               previousItem && previousItem->hasSpare()) {
      previousItem->setExtra(item->cumulateThrownPins(1));
      if (item->noThrows() >= 1)
        previousItem->setSkipRendering(false);
    } else {
      item->setPreviousScore(0);
    }
    updateAllFrameScores();
  }

  if (allowedCountOfPins == 0) {
    activateNextFrameScore();
  }

  return allowedCountOfPins;
}

BowlFrameScore*
BowlPlayerScore::frameScore(int index)
{
  if (0 <= index && index < m_frameScores.size()) {
    return m_frameScores.at(index);
  } else {
    return nullptr;
  }
}

void
BowlPlayerScore::activateNextFrameScore()
{

  activateCurrentFrame(false);
  m_currentFrameIndex++;
  activateCurrentFrame(true);
}

void
BowlPlayerScore::updateAllFrameScores()
{
  // update all: render and forward previous score of all
  auto prevScore = 0;
  for (int index = 0; index < m_frameScores.size(); index++) {
    auto _prevItem = frameScore(index - 1);
    if (_prevItem) {
      prevScore = _prevItem->totalScore();
    } else
      prevScore = 0;

    auto _item2 = frameScore(index);
    _item2->setPreviousScore(prevScore);
    _item2->renderScore();
  }

  m_resultLabel->setText(QString("%1").arg(resultScore()));
}

int
BowlPlayerScore::resultScore()
{

  return m_frameScores.last()->totalScore();
}

int
BowlPlayerScore::findfirstTrailingStrikeInARow(int lastStrike)
{
  // iterate backwards over items that hasStrike()
  int firstStrikeInARow = 0;

  for (int index = lastStrike; index >= 0; index--) {
    if (frameScore(index)) {
      if (!frameScore(index)->hasStrike()) {
        firstStrikeInARow = index + 1;
        break;
      }
    }
  }
  return firstStrikeInARow;
}

void
BowlPlayerScore::treatRowOfStrikes(BowlFrameScore* currentFrame,
                                   int firstStrikeInARow,
                                   int lastStrike)
{
  for (int index = lastStrike; index >= firstStrikeInARow; index--) {
    auto _item2 = frameScore(index);
    if (_item2 && _item2->hasStrike()) {
      if (lastStrike - index == 0) // Single Strike
      {
        auto skipIt = !currentFrame->isFrameClosed(false);
        _item2->setSkipRendering(skipIt);
        /// for strike evaluation never use possible 3rd throw in a frame (for
        /// last frame) add all available without 3rd throw of last frame
        _item2->setExtra(currentFrame->cumulateThrownPins(2));
      } else if (lastStrike - index == 1) // Double Strike
      {
        _item2->setExtra(10 + currentFrame->cumulateThrownPins(1));
        _item2->setSkipRendering(false);
      } else {
        _item2->setSkipRendering(false);
        _item2->setExtra(20); // Multiple Strike
      }
    } else
      break;
  }
}

void
BowlPlayerScore::activateCurrentFrame(bool enable)
{
  auto* scoreFrame =
    currentFrameScore() ? currentFrameScore()->widgetFrame() : nullptr;

  if (scoreFrame) {
    if (enable) {
      scoreFrame->setLineWidth(m_activeFrameLineWidth);
    } else {
      scoreFrame->setLineWidth(m_regularFrameLineWidth);
    }
  }
}

void
BowlPlayerScore::markResult(const QColor& color)
{
  m_resultEffect->setColor(color);
  m_resultEffect->setEnabled(true);
}
