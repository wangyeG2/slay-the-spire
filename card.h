#ifndef CARD_H
#define CARD_H

#include <QString>
#include <QPixmap>
#include <QDataStream>
#include <QMetaType>

enum CardType {
    ATTACK,
    DEFENSE,
    SPECIAL
};
enum EnemyIntent {
    ATTACKING,    // 准备攻击
    DEFENDING,    // 准备防御
    BUFFING       // 准备增益
};

struct EnemyStatus {
    int maxHp;
    int currentHp;
    EnemyIntent intent;  // 敌人当前意图
    int intentValue;     // 意图数值（如攻击伤害值）
    int block = 0;
};
struct Card {
    QString id;
    QString name;
    QString description;
    CardType type;
    int value;
    QPixmap image;

    // 默认构造函数（必须提供）
    Card() : id(""), name(""), description(""), type(ATTACK), value(0), image() {}

    // 带参数的构造函数
    Card(const QString& id, const QString& name, const QString& desc,
         CardType type, int val, const QString& imgPath = "")
        : id(id), name(name), description(desc), type(type), value(val) {
        if (!imgPath.isEmpty())  {
            image.load(imgPath);
        }
    }

    // 注册元类型
    static void registerMetaType() {
        qRegisterMetaType<Card>("Card");
    }
};

// 序列化操作符
QDataStream &operator<<(QDataStream &out, const Card &card);
QDataStream &operator>>(QDataStream &in, Card &card);

// 声明元类型
Q_DECLARE_METATYPE(Card)

#endif // CARD_H
