
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <ogis/wbscorer/bowlframescore.h>

BowlFrameScore::BowlFrameScore(bool isLlastFrame, QWidget* parent)
  : QWidget(parent)
  , c_maxCountOfRolls(isLlastFrame ? 3 : 2)
  , m_isLastFrame(isLlastFrame)
{
  setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

  auto* vLayout = new QVBoxLayout();
  vLayout->setContentsMargins(0, 0, 0, 0);
  {
    auto* hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(0);

    hLayout->addWidget(new QLabel(" ", this));

    for (int index = 0; index < 2; index++) {
      auto label = new QLabel(this);
      label->setText(" ");
      if (index == 1)
        label->setFrameStyle(QFrame::Box | QFrame::Raised);

      m_throwLabels.push_back(label);
      hLayout->addWidget(label);
    }

    if (isLastFrame()) {
      auto label = new QLabel(this);
      label->setText(" ");
      label->setFrameStyle(QFrame::Box | QFrame::Raised);
      m_throwLabels.push_back(label);
      hLayout->addWidget(label);
    }

    vLayout->addLayout(hLayout);

    if (m_throwLabels.size() > 0)
      m_lastThrowLabel = m_throwLabels.last();
    else
      m_lastThrowLabel = nullptr;
  }
  m_scoreLabel = new QLabel(this);
  m_scoreLabel->setText("   ");
  auto font = m_scoreLabel->font();
  font.setItalic(true);
  font.setPointSize(static_cast<int>(font.pointSize() * 1.5));
  m_scoreLabel->setFont(font);
  vLayout->addWidget(m_scoreLabel);

  auto frame = new QFrame(this);

  frame->setLayout(vLayout);
  frame->setFrameStyle(QFrame::Box | QFrame::Raised);

  auto frameLayout = new QHBoxLayout();
  frameLayout->setContentsMargins(0, 0, 0, 0);
  frameLayout->addWidget(frame);

  setLayout(frameLayout);
  setMinimumSize(m_isLastFrame ? 75 : 50, 50);
}

void
BowlFrameScore::clear()
{
  m_scoreLabel->setText("   ");
  for (auto& label : m_throwLabels)
    label->setText(" ");
  m_lastThrowLabel = m_throwLabels.last();

  for (auto& pinValue : m_thrownPinValue)
    pinValue = 0;

  m_previousScore = 0;
  m_score = 0;
  m_extra = 0;

  m_isLastFrame = false;
  m_hasSpare = false;
  m_hasStrike = false;
}

QFrame*
BowlFrameScore::widgetFrame()
{
  auto w = layout()->itemAt(0)->widget();
  return dynamic_cast<QFrame*>(w);
}

QLabel*
BowlFrameScore::throwLabel(int index)
{
  if (0 <= index && index < noThrows())
    return m_throwLabels.at(index);
  else
    return nullptr;
}

// if negative, cumulate over all throws
int
BowlFrameScore::cumulateThrownPins(int maxIndex)
{
  int cummulatedPinCount = 0;
  int index = 0;
  for (auto value : m_thrownPinValue) {
    if (maxIndex >= 0 && index >= maxIndex)
      break;
    cummulatedPinCount += qMax(value, 0);
    index++;
  }
  return cummulatedPinCount;
}

int
BowlFrameScore::thrownCountOfPins(int pinsThrown)
{
  bool allowAnotherThrow = true;

  m_thrownPinValue.push_back(pinsThrown);

  if (noThrows() > c_maxCountOfRolls) {
    throw std::logic_error("exceeded max number of rolls");
  }

  auto valueLabel = throwLabel(noThrows() - 1);
  if (!valueLabel)
    throw std::logic_error("unexpected exceeded range of roll-label");

  m_score = cumulateThrownPins();
  if ((m_isLastFrame && cumulateThrownPins() > 3 * c_counOfAllPins) ||
      ((!m_isLastFrame) && cumulateThrownPins() > c_counOfAllPins)) {
    allowAnotherThrow = false;
    widgetFrame()->setStyleSheet(
      "QWidget{ background-color : rgba( 255, 0, 0, 127);  }");
  } else if ((pinsThrown == c_counOfAllPins) &&
             ( // last Score items allows multiple STRIKES
               ((!m_isLastFrame) &&
                noThrows() == 1) // strike only at first Throw
               || m_isLastFrame)) {
    // rolled a STRIKE!!!
    markStrike(true);
    if (m_isLastFrame) {
      valueLabel->setText("X");
    } else {
      allowAnotherThrow = false;
      m_lastThrowLabel->setText("X");
    }
  } else if (pinsThrown < 0) //   rolled a FAULT!!!
  {
    valueLabel->setText("F");
  } else if (pinsThrown == 0) //   rolled NO PIN!!!
  {
    valueLabel->setText("-");
  } else {
    if (m_score > 0 && m_score % c_counOfAllPins == 0) {
      // rolled a SPARE
      if (m_isLastFrame) // last
      {
        valueLabel->setText("/");
      } else {
        m_lastThrowLabel->setText("/");
      }
      allowAnotherThrow = false;
      markSpare(true);
    } else {
      // rolled  any other value.
      valueLabel->setText(QString("%1").arg(pinsThrown));
    }
  }

  if (m_isLastFrame) // for last frame, the rules are different
  {
    // only allow third throw if  on last frame if has Strike or has Spare
    if (noThrows() >= c_maxCountOfRolls)
      return 0;

    if (noThrows() == 2) {
      if (hasSpare()) {
        return c_counOfAllPins;
      } else if (hasStrike()) {
        return pinsLeftOnTheTable();
      } else
        return 0;
    } else
      return pinsLeftOnTheTable();
  } else if (allowAnotherThrow && noThrows() < c_maxCountOfRolls) {
    // otherwise, if no strike or spare, and not at end of allows rolls
    return pinsLeftOnTheTable();
  } else {
    // otherwise, no more throw allowed
    return 0;
  }
}

void
BowlFrameScore::renderScore()
{
  if (shouldScoreBeRendered())
    m_scoreLabel->setText(QString("%1").arg(totalScore()));
}

bool
BowlFrameScore::shouldScoreBeRendered()
{
  if (isLastFrame() || !m_skipRendering)
    if (isFrameClosed(true))
      return true;
  return false;
}

bool
BowlFrameScore::isFrameClosed(bool useStrike)
{
  auto thwrownPins = noThrows();
  if (isLastFrame()) {
    if (thwrownPins == 3)
      return true;
    else
      return thwrownPins == 2 && !((useStrike && hasStrike()) || hasSpare());
  } else
    return thwrownPins == 2 || (useStrike && hasStrike());
}

int
BowlFrameScore::pinsLeftOnTheTable()
{
  return c_counOfAllPins - (cumulateThrownPins() % c_counOfAllPins);
}
