#include "card.h"

QDataStream &operator<<(QDataStream &out, const Card &card) {
    out << card.id  << card.name  << card.description
        << static_cast<int>(card.type)  << card.value  << card.image;
    return out;
}

QDataStream &operator>>(QDataStream &in, Card &card) {
    int type;
    in >> card.id  >> card.name  >> card.description
        >> type >> card.value  >> card.image;
    card.type  = static_cast<CardType>(type);
    return in;
}
