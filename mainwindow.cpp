#include "mainwindow.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    m_characterStatus{75, 75},
    m_enemyStatus{44, 44, ATTACKING, 10},
    m_block(0),
    m_enemyIntentLabel(nullptr),    // 显式初始化
    m_enemyIntentValueLabel(nullptr) // 显式初始化
{
    // 设置窗口最小大小
    setMinimumSize(800, 600);
    // 在构造函数中确保所有指针都已初始化

    // 创建中央部件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 加载empty1图片
    m_emptyPixmap = QPixmap(":/resources/empty1.png");
    if(m_emptyPixmap.isNull())  {
        qDebug() << "Failed to load empty1 image";
    }

    // 使用网格布局以便精确定位
    QGridLayout *mainLayout = new QGridLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 创建背景标签
    m_backgroundLabel = new QLabel(this);
    m_backgroundLabel->setAlignment(Qt::AlignCenter);
    m_backgroundPixmap = QPixmap(":/resources/scene.png");
    if(m_backgroundPixmap.isNull())  {
        qDebug() << "Failed to load background image";
    }
    m_backgroundLabel->setPixmap(m_backgroundPixmap);
    m_backgroundLabel->setScaledContents(false);
    mainLayout->addWidget(m_backgroundLabel, 0, 0, 1, 1);

    // 创建角色容器
    QWidget *characterContainer = new QWidget(this);
    QVBoxLayout *characterLayout = new QVBoxLayout(characterContainer);
    characterLayout->setContentsMargins(0, 0, 0, 0);
    characterLayout->setSpacing(5);

    // 创建empty图片容器
    QWidget *emptyContainer = new QWidget(this);
    QHBoxLayout *emptyLayout = new QHBoxLayout(emptyContainer);
    emptyLayout->setContentsMargins(0, 0, 0, 0);
    emptyLayout->setSpacing(20);

    // 创建三个empty标签
    m_emptyLeftLabel = new QLabel(this);
    m_emptyCenterLabel = new QLabel(this);
    m_emptyRightLabel = new QLabel(this);

    // 初始化empty标签
    for (QLabel* label : {m_emptyLeftLabel, m_emptyCenterLabel, m_emptyRightLabel}) {
        label->setPixmap(m_emptyPixmap);
        label->setScaledContents(true);
        label->setAlignment(Qt::AlignCenter);
        label->hide();
        emptyLayout->addWidget(label);
    }

    // 创建角色标签和血条
    m_characterLabel = new QLabel(this);
    m_characterPixmap = QPixmap(":/resources/character.png");
    if(m_characterPixmap.isNull())  {
        qDebug() << "Failed to load character image";
    }
    m_characterLabel->setPixmap(m_characterPixmap);
    m_characterLabel->setScaledContents(true);
    m_characterLabel->setAlignment(Qt::AlignCenter);
    m_characterLabel->hide();

    m_characterHpBar = new QProgressBar(this);
    m_characterHpBar->setRange(0, m_characterStatus.maxHp);
    m_characterHpBar->setValue(m_characterStatus.currentHp);
    m_characterHpBar->setTextVisible(true);
    m_characterHpBar->setFormat("HP: %v/%m");
    m_characterHpBar->setStyleSheet("QProgressBar {"
                                    "border: 2px solid grey;"
                                    "border-radius: 5px;"
                                    "text-align: center;"
                                    "}"
                                    "QProgressBar::chunk {"
                                    "background-color: #FF0000;"
                                    "width: 10px;"
                                    "}");
    m_characterHpBar->hide();

    // 将组件添加到角色布局
    characterLayout->addWidget(emptyContainer);
    characterLayout->addWidget(m_characterLabel);
    characterLayout->addWidget(m_characterHpBar);
    characterLayout->addStretch();

    // 创建敌人容器
    QWidget *enemyContainer = new QWidget(this);
    QVBoxLayout *enemyLayout = new QVBoxLayout(enemyContainer);
    enemyLayout->setContentsMargins(0, 0, 0, 0);
    enemyLayout->setSpacing(5);
    m_enemyIntentLabel = new QLabel("意图: 攻击", this);
    m_enemyIntentValueLabel = new QLabel("伤害: 10", this);

    // 设置对象父子关系
    m_enemyIntentLabel->setParent(this);
    m_enemyIntentValueLabel->setParent(this);
    // 添加到敌人容器布局中

    // 创建敌人标签和血条
    m_enemyLabel = new QLabel(this);
    m_enemyPixmap = QPixmap(":/resources/enemy.png");
    if(m_enemyPixmap.isNull())  {
        qDebug() << "Failed to load enemy image";
    }
    m_enemyLabel->setPixmap(m_enemyPixmap);
    m_enemyLabel->setScaledContents(true);
    m_enemyLabel->setAlignment(Qt::AlignCenter);
    m_enemyLabel->hide();

    m_enemyHpBar = new QProgressBar(this);
    m_enemyHpBar->setRange(0, m_enemyStatus.maxHp);
    m_enemyHpBar->setValue(m_enemyStatus.currentHp);
    m_enemyHpBar->setTextVisible(true);
    m_enemyHpBar->setFormat("HP: %v/%m");
    m_enemyHpBar->setStyleSheet("QProgressBar {"
                                "border: 2px solid grey;"
                                "border-radius: 5px;"
                                "text-align: center;"
                                "}"
                                "QProgressBar::chunk {"
                                "background-color: #FF0000;"
                                "width: 10px;"
                                "}");
    m_enemyHpBar->hide();
    enemyLayout->addWidget(m_enemyLabel);
    enemyLayout->addWidget(m_enemyHpBar);
    enemyLayout->addStretch();
    m_enemyIntentLabel = new QLabel(this);
    m_enemyIntentValueLabel = new QLabel(this);
    updateEnemyIntent(); // 设置初始文本
    if (enemyLayout) {
        enemyLayout->addWidget(m_enemyIntentLabel);
        enemyLayout->addWidget(m_enemyIntentValueLabel);
    }
    enemyLayout->addWidget(m_enemyLabel);
    enemyLayout->addWidget(m_enemyHpBar);
    // 创建按钮
    m_button = new QPushButton("Go to Login", this);
    m_button->setFixedSize(150, 50);

    // 创建结束回合按钮
    m_endTurnButton = new QPushButton("结束回合", this);
    m_endTurnButton->setFixedSize(150, 50);
    m_endTurnButton->hide();

    // 创建手牌列表
    m_handList = new QListWidget(this);
    m_handList->setViewMode(QListWidget::IconMode);
    m_handList->setIconSize(QSize(80, 120));
    m_handList->setSpacing(10);
    m_handList->hide();

    // 创建牌堆信息标签
    m_drawPileLabel = new QLabel("抽牌堆: 0", this);
    m_discardPileLabel = new QLabel("弃牌堆: 0", this);
    m_blockLabel = new QLabel("格挡: 0", this);

    // 使用覆盖布局放置所有组件
    QWidget *overlayWidget = new QWidget(this);
    QVBoxLayout *overlayLayout = new QVBoxLayout(overlayWidget);
    overlayLayout->setContentsMargins(0, 0, 0, 0);
    overlayLayout->addStretch();

    QHBoxLayout *middleLayout = new QHBoxLayout();
    middleLayout->addWidget(characterContainer, 0, Qt::AlignVCenter | Qt::AlignLeft);
    middleLayout->addStretch();
    middleLayout->addWidget(m_button, 0, Qt::AlignCenter);
    middleLayout->addWidget(m_endTurnButton, 0, Qt::AlignCenter);
    middleLayout->addStretch();
    middleLayout->addWidget(enemyContainer, 0, Qt::AlignVCenter | Qt::AlignRight);

    overlayLayout->addLayout(middleLayout);

    // 添加牌堆信息和手牌列表
    QHBoxLayout *infoLayout = new QHBoxLayout();
    infoLayout->addWidget(m_drawPileLabel);
    infoLayout->addWidget(m_blockLabel);
    infoLayout->addStretch();
    infoLayout->addWidget(m_discardPileLabel);

    overlayLayout->addLayout(infoLayout);
    overlayLayout->addWidget(m_handList);
    overlayLayout->addStretch();

    mainLayout->addWidget(overlayWidget, 0, 0, 1, 1);

    // 连接信号和槽
    connect(m_button, &QPushButton::clicked, this, &MainWindow::showLoginWindow);
    connect(m_endTurnButton, &QPushButton::clicked, this, &MainWindow::endTurn);
    connect(m_handList, &QListWidget::itemClicked, this, &MainWindow::playCard);

    // 初始化登录窗口
    m_loginWindow = new LoginWindow(this);
    connect(m_loginWindow, &LoginWindow::loginSuccess, this, &MainWindow::handleLoginSuccess);

    // 初始化牌堆管理器
    m_deckManager = new DeckManager(this);
    connect(m_deckManager, &DeckManager::deckChanged, this, &MainWindow::updateDeckInfo);
}

MainWindow::~MainWindow()
{
    delete m_loginWindow;
    delete m_deckManager;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    updateImageSizes();
}

void MainWindow::updateImageSizes()
{
    QSize windowSize = size();
    QSize bgSize = m_backgroundPixmap.size();

    // 背景图片处理
    qreal bgScale = qMax(qreal(windowSize.width())  / bgSize.width(),
                         qreal(windowSize.height())  / bgSize.height());

    QSize scaledBgSize = bgSize * bgScale;
    QPixmap scaledBgPixmap = m_backgroundPixmap.scaled(scaledBgSize,
                                                       Qt::KeepAspectRatio,
                                                       Qt::SmoothTransformation);

    QPixmap finalBgPixmap(windowSize);
    finalBgPixmap.fill(Qt::black);

    QPainter painter(&finalBgPixmap);
    painter.drawPixmap((windowSize.width()  - scaledBgSize.width())  / 2,
                       (windowSize.height()  - scaledBgSize.height())  / 2,
                       scaledBgPixmap);

    m_backgroundLabel->setPixmap(finalBgPixmap);
    m_backgroundLabel->setFixedSize(windowSize);

    // 角色和empty图片处理
    int characterHeight = windowSize.height()  / 3;

    // 设置empty图片大小
    QSize emptySize = m_emptyPixmap.size();
    qreal emptyScale = qreal(characterHeight/3) / emptySize.height();
    QSize scaledEmptySize = emptySize * emptyScale;

    for (QLabel* label : {m_emptyLeftLabel, m_emptyCenterLabel, m_emptyRightLabel}) {
        label->setPixmap(m_emptyPixmap.scaled(scaledEmptySize,
                                              Qt::KeepAspectRatio,
                                              Qt::SmoothTransformation));
        label->setFixedSize(scaledEmptySize);
    }

    // 设置角色图片大小
    QSize characterSize = m_characterPixmap.size();
    qreal characterScale = qreal(characterHeight) / characterSize.height();
    QSize scaledCharacterSize = characterSize * characterScale;
    m_characterLabel->setPixmap(m_characterPixmap.scaled(scaledCharacterSize,
                                                         Qt::KeepAspectRatio,
                                                         Qt::SmoothTransformation));
    m_characterLabel->setFixedSize(scaledCharacterSize);
    m_characterHpBar->setFixedWidth(scaledCharacterSize.width());

    // 设置敌人图片大小
    QSize enemySize = m_enemyPixmap.size();
    qreal enemyScale = qreal(characterHeight) / enemySize.height();
    QSize scaledEnemySize = enemySize * enemyScale;
    m_enemyLabel->setPixmap(m_enemyPixmap.scaled(scaledEnemySize,
                                                 Qt::KeepAspectRatio,
                                                 Qt::SmoothTransformation));
    m_enemyLabel->setFixedSize(scaledEnemySize);
    m_enemyHpBar->setFixedWidth(scaledEnemySize.width());
}

void MainWindow::setupHealthBars()
{
    m_characterHpBar->setValue(m_characterStatus.currentHp);
    m_enemyHpBar->setValue(m_enemyStatus.currentHp);
}

void MainWindow::showLoginWindow()
{
    this->hide();
    m_loginWindow->show();
}

void MainWindow::handleLoginSuccess()
{
    m_loginWindow->hide();
    this->show();
    m_button->hide();
    m_characterLabel->show();
    m_enemyLabel->show();
    m_characterHpBar->show();
    m_enemyHpBar->show();
    m_emptyLeftLabel->show();
    m_emptyCenterLabel->show();
    m_emptyRightLabel->show();
    m_endTurnButton->show();
    m_handList->show();

    updateImageSizes();
    setupHealthBars();

    // 初始化牌组并抽起始手牌
    m_deckManager->initializeDeck();
    drawInitialCards();
}

void MainWindow::drawInitialCards()
{
    m_currentHand = m_deckManager->drawCards(5);
    updateHandList();
    updateDeckInfo();
}
void MainWindow::updateHandList()
{
    qDebug() << "更新手牌列表，当前手牌数:" << m_currentHand.size();
    m_handList->clear();
    for (const Card &card : m_currentHand) {
        QListWidgetItem *item = new QListWidgetItem(QIcon(card.image),  card.name);
        item->setData(Qt::UserRole, QVariant::fromValue(card));
        item->setToolTip(card.description);
        m_handList->addItem(item);
    }
}
void MainWindow::endTurn()
{
    // 1. 敌人行动
    applyEnemyAction();

    // 2. 清除玩家格挡
    m_block = 0;

    // 3. 弃牌（添加空检查）
    if (!m_currentHand.isEmpty())  {
        m_deckManager->discardCards(m_currentHand);
        m_currentHand.clear();
        m_handList->clear();
    }

    // 4. 更新敌人意图
    updateEnemyIntent();

    // 5. 抽新牌（关键修改点）
    if (m_deckManager->drawPileCount() + m_deckManager->discardPileCount() > 0) {
        m_currentHand = m_deckManager->drawCards(5);
        updateHandList();
        qDebug() << "抽牌完成，当前手牌数:" << m_currentHand.size()
                 << "抽牌堆剩余:" << m_deckManager->drawPileCount()
                 << "弃牌堆:" << m_deckManager->discardPileCount();
    } else {
        qWarning() << "牌库已空！";
    }

    // 6. 更新UI
    updateStatusDisplay();
    updateDeckInfo();
}
void MainWindow::playCard(QListWidgetItem* item)
{
    static bool registered = [](){
        Card::registerMetaType();
        return true;
    }();
    Q_UNUSED(registered);

    // 获取卡牌数据
    QVariant variant = item->data(Qt::UserRole);
    if (!variant.canConvert<Card>())  {
        qWarning() << "Cannot convert variant to Card";
        return;
    }

    Card card = variant.value<Card>();
    switch(card.type)  {
    case ATTACK:
        m_enemyStatus.currentHp  = qMax(0, m_enemyStatus.currentHp  - card.value);
        break;

    case DEFENSE:
        m_block += card.value;
        break;

    case SPECIAL: // Auto-Shields
        if (m_block == 0) {
            m_block = card.value;
        }
        break;
    }

    // 从手牌中移除
    for (int i = 0; i < m_currentHand.size();  ++i) {
        if (m_currentHand[i].id == card.id)  {
            m_currentHand.remove(i);
            break;
        }
    }

    // 放入弃牌堆
    m_deckManager->discardCards({card});

    // 更新UI
    updateHandList();
    updateStatusDisplay();
    updateDeckInfo();

    // 检查敌人是否死亡
    if (m_enemyStatus.currentHp  <= 0) {
        QMessageBox::information(this, "胜利", "你击败了敌人！");
    }
}
void MainWindow::applyEnemyAction()
{
    int damage = 0; // 在switch外部声明变量

    switch(m_enemyStatus.intent)  {
    case ATTACKING:
        damage = qMax(0, m_enemyStatus.intentValue  - m_block);
        m_characterStatus.currentHp  -= damage;

        // 添加伤害日志（可选）
        qDebug() << "敌人造成伤害:" << damage
                 << "(原始:" << m_enemyStatus.intentValue
                 << ", 格挡抵消:" << m_block << ")";

        if (m_characterStatus.currentHp  <= 0) {
            QMessageBox::information(this, "游戏结束", "你被敌人击败了！");
            // 可以添加游戏结束处理逻辑
        }
        break;

    case DEFENDING:
        // 示例：敌人增加格挡
        // m_enemyStatus.block  += m_enemyStatus.intentValue;
        qDebug() << "敌人选择了防御";
        break;

    case BUFFING:
        // 示例：敌人强化下次攻击
        // m_enemyStatus.intentValue  += 5;
        qDebug() << "敌人正在强化自己";
        break;
    }

    updateStatusDisplay();
}
void MainWindow::updateDeckInfo()
{
    m_drawPileLabel->setText(QString("抽牌堆: %1").arg(m_deckManager->drawPileCount()));
    m_discardPileLabel->setText(QString("弃牌堆: %1").arg(m_deckManager->discardPileCount()));
}
void MainWindow::updateEnemyIntent()
{
    // 简单逻辑：固定每回合攻击10点
    m_enemyStatus.intent  = ATTACKING;
    m_enemyStatus.intentValue  = 10;

    // 更新UI显示
    QString intentText;
    switch(m_enemyStatus.intent)  {
    case ATTACKING: intentText = "攻击"; break;
    case DEFENDING: intentText = "防御"; break;
    case BUFFING: intentText = "强化"; break;
    }

    m_enemyIntentLabel->setText("意图: " + intentText);
    m_enemyIntentValueLabel->setText("数值: " + QString::number(m_enemyStatus.intentValue));
}
void MainWindow::updateStatusDisplay()
{
    m_characterHpBar->setValue(m_characterStatus.currentHp);
    m_enemyHpBar->setValue(m_enemyStatus.currentHp);
    m_blockLabel->setText(QString("格挡: %1").arg(m_block));
    if (m_characterStatus.currentHp  < 0) m_characterStatus.currentHp  = 0;
    if (m_enemyStatus.currentHp  < 0) m_enemyStatus.currentHp  = 0;
}
