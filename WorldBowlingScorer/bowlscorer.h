#pragma once

#include <QWidget>

#include "bowlplayerscore.h"

class BowlScorer
    : public QWidget
{
    Q_OBJECT

public:
    // all menbers are linked using the Qt-Parent mechanism
    // and are resolved at deletion
    ~BowlScorer()=default;
    explicit BowlScorer(int countOfPlayers, QWidget *parent);

    int noPlayers() { return m_playersScores.size(); }
    BowlPlayerScore* playerScore(int index);

    bool hasGameEnded();

public slots:

    void activateNextPlayer();
    int  throwPins(int noPins);
    void markWinners();
    void addPlayer(const QString &name);

private:

    void init(int playercount);

    QList<BowlPlayerScore*> m_playersScores;
    BowlPlayerScore* m_activePlayer=nullptr;
    int m_selectedPlayerIndex=0;
};

Q_DECLARE_TYPEINFO(BowlScorer*, Q_PRIMITIVE_TYPE);
