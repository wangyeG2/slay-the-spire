#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QResizeEvent>
#include <QPixmap>
#include <QPainter>
#include <QProgressBar>
#include <QListWidget>
#include "loginwindow.h"
#include "deckmanager.h"
#include "card.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    struct CharacterStatus {
        int maxHp;
        int currentHp;
    };

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void showLoginWindow();
    void handleLoginSuccess();
    void drawInitialCards();
    void endTurn();
    void playCard(QListWidgetItem* item);
    void updateDeckInfo();
private:
    void updateImageSizes();
    void setupHealthBars();
    void updateHandList();
    void updateStatusDisplay();

    QPushButton *m_button;
    QPushButton *m_endTurnButton;
    QLabel *m_backgroundLabel;
    QLabel *m_characterLabel;
    QLabel *m_enemyLabel;
    QLabel *m_emptyLeftLabel;
    QLabel *m_emptyCenterLabel;
    QLabel *m_emptyRightLabel;
    QLabel *m_drawPileLabel;
    QLabel *m_discardPileLabel;
    QLabel *m_blockLabel;
    QProgressBar *m_characterHpBar;
    QProgressBar *m_enemyHpBar;
    QListWidget *m_handList;
    LoginWindow *m_loginWindow;
    DeckManager *m_deckManager;
    QPixmap m_backgroundPixmap;
    QPixmap m_characterPixmap;
    QPixmap m_enemyPixmap;
    QPixmap m_emptyPixmap;

    CharacterStatus m_characterStatus;
    EnemyStatus m_enemyStatus;
    QLabel *m_enemyIntentLabel;
    QLabel *m_enemyIntentValueLabel;
    void updateEnemyIntent();
    void applyEnemyAction();
    int m_block;
    QVector<Card> m_currentHand;
};

#endif // MAINWINDOW_H
