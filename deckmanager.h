#ifndef DECKMANAGER_H
#define DECKMANAGER_H

#include <QObject>
#include <QVector>
#include "card.h"

class DeckManager : public QObject
{
    Q_OBJECT

public:
    explicit DeckManager(QObject *parent = nullptr);

    void initializeDeck();
    QVector<Card> drawCards(int count);
    void discardCards(const QVector<Card>& cards);
    void reshuffleDiscardPile();

    int drawPileCount() const;
    int discardPileCount() const;

signals:
    void deckChanged();

private:
    QVector<Card> m_drawPile;
    QVector<Card> m_discardPile;
};

#endif // DECKMANAGER_H
