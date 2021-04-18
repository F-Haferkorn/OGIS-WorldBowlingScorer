#include <QWidget>
#include <QFormLayout>
#include <QString>

#include "bowlscorer.h"


BowlScorer::BowlScorer(int countOfPlayers, QWidget *parent)
  : QWidget(parent)
{
    auto defaultPolicy=QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    defaultPolicy.setHorizontalStretch(50);
    defaultPolicy.setVerticalStretch(50);
    setSizePolicy(defaultPolicy);
    init(countOfPlayers);



}

void
BowlScorer::init(int playercount)
{
    auto defaultPolicy=QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    auto layout = new QFormLayout();
    setLayout(layout);
    setSizePolicy(defaultPolicy);
    for(int index= 0; index<playercount; index++)
    {
        auto name=QString("Player %1").arg(index);
        addPlayer(name);
    }

    m_activePlayer=m_playersScores.first();
    m_selectedPlayerIndex=0;
    m_activePlayer->activateCurrentFrame(true);


}

void
BowlScorer::activateNextPlayer()
{
    // un-mark the active Frame of the last active Player.
    m_activePlayer->activateCurrentFrame(false);

    //move to next player.
    m_selectedPlayerIndex =(m_selectedPlayerIndex + 1) % m_playersScores.size();
    m_activePlayer=m_playersScores.at(m_selectedPlayerIndex);
    // mark the active Frame of th eactive Player
    m_activePlayer->activateCurrentFrame(true);
}
int
BowlScorer::throwPins(int noPins)
{
    int allowedCountOfPins = m_activePlayer?m_activePlayer->thrownCountOfPins(noPins):0;
    if(allowedCountOfPins==0)
    {
        activateNextPlayer();
    }
    // check for end of game
    return allowedCountOfPins;
}


void
BowlScorer::addPlayer(const QString &name)
{
    auto  frameId=
            m_playersScores.size()>0
            ?m_playersScores.last()->frameId()
            : 1;
    m_playersScores.push_back(new BowlPlayerScore(this));

    m_playersScores.last()->setFrameId(frameId);

    auto formLayout = dynamic_cast<QFormLayout*>(layout());
    if(formLayout)
        formLayout ->addRow(name, m_playersScores.last());
}

bool
BowlScorer::hasGameEnded()
{
    return m_activePlayer && m_playersScores.last()->frameId()>10;
}

void
BowlScorer::markWinners()
{
    int maxScore=0;
    for(auto &player: m_playersScores)
        maxScore=qMax(maxScore, player->resultScore());

    for(auto &player: m_playersScores)
        if(maxScore == player->resultScore())
            player->markResult(Qt::red);

}

BowlPlayerScore*
BowlScorer::playerScore(int index)
{
    auto size = m_playersScores.size();
    if(0<=index && index < size) {
        // for negative indices access players from the front
        return m_playersScores.at(index);
    }
    else  if(index <0){
        // for negative indices access players from the rear
        index = size+index;
        if(0 <=index && index<size )
            return m_playersScores.at(index);
    }
    return nullptr;
}
