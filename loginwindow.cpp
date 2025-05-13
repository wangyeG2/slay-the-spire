#include "loginwindow.h"
#include <QDebug>

LoginWindow::LoginWindow(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Login");
    setFixedSize(300, 200);

    QVBoxLayout *layout = new QVBoxLayout(this);

    // 用户名输入框
    m_usernameEdit = new QLineEdit(this);
    m_usernameEdit->setPlaceholderText("Username");
    layout->addWidget(m_usernameEdit);

    // 密码输入框
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setPlaceholderText("Password");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(m_passwordEdit);

    // 登录按钮
    m_loginButton = new QPushButton("Login", this);
    layout->addWidget(m_loginButton);

    // 状态标签
    m_statusLabel = new QLabel(this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_statusLabel);

    // 连接按钮点击信号到槽函数
    connect(m_loginButton, &QPushButton::clicked, this, &LoginWindow::attemptLogin);
}

LoginWindow::~LoginWindow()
{
}

void LoginWindow::attemptLogin()
{
    QString username = m_usernameEdit->text();
    QString password = m_passwordEdit->text();

    // 简单的登录验证
    if (username == "admin" && password == "123456") {
        m_statusLabel->setText("Login successful!");
        emit loginSuccess();
    } else {
        m_statusLabel->setText("Invalid username or password");
    }
}
