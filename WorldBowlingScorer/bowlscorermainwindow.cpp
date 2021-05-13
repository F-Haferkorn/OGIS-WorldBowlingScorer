#include <QWidget>
#include <QString>
#include <QToolBar>

#include "bowlscorermainwindow.h"
#include "./ui_bowlscorermainwindow.h"

#include "bowlframescore.h"

BowlScorerMainWindow::~BowlScorerMainWindow()
{
    delete ui;
}
BowlScorerMainWindow::BowlScorerMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BowlScorerMainWindow)
{
    ui->setupUi(this);

    setSizePolicy(m_sizePolicy);

    setWindowTitle("World-Bowling    SCORER");

    addToolBar(initToolBar());
    respawnBowlScorer(c_initialPlayersCount);

    return;
}

void
BowlScorerMainWindow::respawnBowlScorer(int countOfPlayers)
{
    auto policy = sizePolicy();
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setUpdatesEnabled(false);

    if(m_bowlScorer)
    {
        //m_bowlScorer->close();
        delete m_bowlScorer;
    }
    m_bowlScorer = new BowlScorer(countOfPlayers, this);

    setCentralWidget(m_bowlScorer);
    setSizePolicy(m_sizePolicy);
    setUpdatesEnabled(true);
    setSizePolicy(policy);
    update();

}




QToolBar*
BowlScorerMainWindow::initToolBar()
{
    m_toolBar = new QToolBar(this);
    m_toolBar ->addAction("reset", this, &BowlScorerMainWindow::resetGame)->setToolTip("reset the game");
    m_toolBar ->addSeparator();
    m_toolBar ->addAction("restart", this, &BowlScorerMainWindow::restartGame)->setToolTip("restart the game");
    m_toolBar ->addAction("player++", this, &BowlScorerMainWindow::addPlayer)->setToolTip("add a player");
    m_addPlayerAction = m_toolBar->actions().last();

    m_toolBar ->addSeparator();
    m_toolBar ->addAction("Fault", this, &BowlScorerMainWindow::throwFault)->setToolTip("throw FAULT");
    m_toolBar ->addAction("0", this, &BowlScorerMainWindow::throw0)->setToolTip("throw NO pin");
    m_toolBar ->addAction("1", this, &BowlScorerMainWindow::throw1)->setToolTip("throw 1 pin");
    m_toolBar ->addAction("2", this, &BowlScorerMainWindow::throw2)->setToolTip("throw 2 pins");
    m_toolBar ->addAction("3", this, &BowlScorerMainWindow::throw3)->setToolTip("throw 4 pins");
    m_toolBar ->addAction("4", this, &BowlScorerMainWindow::throw4)->setToolTip("throw 4 pins");
    m_toolBar ->addAction("5", this, &BowlScorerMainWindow::throw5)->setToolTip("throw 5 pins");
    m_toolBar ->addAction("6", this, &BowlScorerMainWindow::throw6)->setToolTip("throw 6 pins");
    m_toolBar ->addAction("7", this, &BowlScorerMainWindow::throw7)->setToolTip("throw 7 pins");
    m_toolBar ->addAction("8", this, &BowlScorerMainWindow::throw8)->setToolTip("throw 8 pins");
    m_toolBar ->addAction("9", this, &BowlScorerMainWindow::throw9)->setToolTip("throw 9 pins");
    m_toolBar ->addAction("10",this, &BowlScorerMainWindow::throw10)->setToolTip("throw 10 pins (a STRIKE)");

    m_toolBar->show();
    enableToolBar(10);
    return m_toolBar;
}

void BowlScorerMainWindow::enableToolBar(int allowedCountOfPins)
{
    auto enable = [] (QAction *action, int index, int allowedCountOfPins){
        action->setEnabled(allowedCountOfPins>=index);
        action->setVisible(allowedCountOfPins>=index);
    };

    auto actions = m_toolBar->actions();

    int startIndex=5;
    auto action = m_toolBar->actions().at(startIndex);

    startIndex++;
    enable(action, 0, allowedCountOfPins);   // Fault
    for(int index =0; index < actions.size()-startIndex; index++ )          // values 0..10 of add-action
    {
        enable( actions.at(index+startIndex), index, allowedCountOfPins);
    }
}
void BowlScorerMainWindow::throwPins(int noPins)
{
    auto allowedCountOfPins =  m_bowlScorer->throwPins(noPins);
    if (m_bowlScorer->hasGameEnded())
    {
        gameEnds();
    }
    else if(allowedCountOfPins ==0)
    {
        enableToolBar(10);
    }
    else
    {
        enableToolBar(allowedCountOfPins);
    }
}
void BowlScorerMainWindow::throwFault(){ throwPins( -1); }
void BowlScorerMainWindow::throw0(){ throwPins( 0); }
void BowlScorerMainWindow::throw1(){ throwPins( 1); }
void BowlScorerMainWindow::throw2(){ throwPins( 2); }
void BowlScorerMainWindow::throw3(){ throwPins( 3); }
void BowlScorerMainWindow::throw4(){ throwPins( 4); }
void BowlScorerMainWindow::throw5(){ throwPins( 5); }
void BowlScorerMainWindow::throw6(){ throwPins( 6); }
void BowlScorerMainWindow::throw7(){ throwPins( 7); }
void BowlScorerMainWindow::throw8(){ throwPins( 8); }
void BowlScorerMainWindow::throw9(){ throwPins( 9); }
void BowlScorerMainWindow::throw10(){ throwPins( 10); }

void BowlScorerMainWindow::restartGame()
{
    auto countOfPlayers=m_bowlScorer->noPlayers();
    respawnBowlScorer(countOfPlayers);
    enableToolBar(10);
    m_addPlayerAction->setVisible(true);
}

void
BowlScorerMainWindow::resetGame()
{
    respawnBowlScorer(c_initialPlayersCount);
    enableToolBar(10);
    centralWidget()->adjustSize();
    adjustSize();
    m_addPlayerAction->setVisible(true);
}

void
BowlScorerMainWindow::gameEnds()
{
    enableToolBar(-1);
    m_bowlScorer->markWinners();
    m_addPlayerAction->setVisible(false);
}

void
BowlScorerMainWindow::addPlayer()
{
    auto name = QString("Player %1").arg(m_bowlScorer->noPlayers()+1);
    m_bowlScorer->addPlayer(name );
    m_addPlayerAction->setVisible(true);
}

