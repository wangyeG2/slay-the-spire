#include "deckmanager.h"
#include <QDebug>  // 添加这行
#include <algorithm>  // 添加这行
#include <random>     // 添加这行
DeckManager::DeckManager(QObject *parent) : QObject(parent)
{
    initializeDeck();
}

void DeckManager::initializeDeck()
{
    m_drawPile.clear();
    m_discardPile.clear();
    static bool registered = [](){
        Card::registerMetaType();
        return true;
    }();
    Q_UNUSED(registered);

    // 初始卡组配置
    m_drawPile.append(Card("strike1",  "Strike", "造成6点伤害", ATTACK, 6, ":/resources/cards/strike.png"));
    m_drawPile.append(Card("strike2",  "Strike", "造成6点伤害", ATTACK, 6, ":/resources/cards/strike.png"));
    m_drawPile.append(Card("defend1",  "Defend", "获得5点格挡", DEFENSE, 5, ":/resources/cards/defend.png"));
    m_drawPile.append(Card("defend2",  "Defend", "获得5点格挡", DEFENSE, 5, ":/resources/cards/defend.png"));
    m_drawPile.append(Card("autoshield",  "Auto-Shields", "无格挡时获得11点格挡", SPECIAL, 11, ":/resources/cards/auto-shields.png"));
    // 使用 std::shuffle 替代 std::random_shuffle
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(m_drawPile.begin(),  m_drawPile.end(),  g);

    emit deckChanged();
}
QVector<Card> DeckManager::drawCards(int count)
{
    QVector<Card> drawnCards;

    // 如果牌不够且弃牌堆有牌，先重洗
    if (m_drawPile.size()  < count && !m_discardPile.isEmpty())  {
        reshuffleDiscardPile();
        qDebug() << "自动重洗弃牌堆，当前抽牌堆:" << m_drawPile.size();
    }

    // 抽牌
    for (int i = 0; i < count && !m_drawPile.isEmpty();  ++i) {
        drawnCards.append(m_drawPile.takeFirst());
    }

    emit deckChanged();
    return drawnCards;
}
void DeckManager::discardCards(const QVector<Card>& cards)
{
    for (const Card &card : cards) {
        m_discardPile.append(card);
    }
    emit deckChanged();
}

void DeckManager::reshuffleDiscardPile()
{
    // 将弃牌堆的牌放回抽牌堆
    while (!m_discardPile.isEmpty())  {
        m_drawPile.append(m_discardPile.takeFirst());
    }

    // 使用 std::shuffle 替代 std::random_shuffle
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(m_drawPile.begin(),  m_drawPile.end(),  g);

    emit deckChanged();
}
int DeckManager::drawPileCount() const
{
    return m_drawPile.size();
}

int DeckManager::discardPileCount() const
{
    return m_discardPile.size();
}
