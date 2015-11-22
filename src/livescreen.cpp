#include <QApplication>

#include "inc/livescreen.h"

LiveScreen::LiveScreen()
{
    isShowing = false;
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::SplashScreen);
    setAttribute(Qt::WA_TranslucentBackground, true);

    backgroundWidget = new QWidget;
    backgroundWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    backgroundWidget->setAutoFillBackground(true);

    messageLabel = new QLabel;
    messageLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    messageLabel->setMargin(5);

    backgroundLayout = new QFormLayout;
    backgroundLayout->addWidget(messageLabel);
    backgroundWidget->setLayout(backgroundLayout);

    mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(backgroundWidget);
    setLayout(mainLayout);

    showhideAnimation = new QPropertyAnimation();
    showhideAnimation->setTargetObject(this);
    showhideAnimation->setPropertyName("geometry");
    showhideAnimation->setDuration(700);

    messageAnimation = new QPropertyAnimation();
    messageAnimation->setTargetObject(messageLabel);
    messageAnimation->setPropertyName("pos");

    connect(showhideAnimation, SIGNAL(finished()), this, SLOT(changeMessageAnimation()));
    connect(messageAnimation, SIGNAL(finished()), this, SIGNAL(scrollEnded()));
}

void LiveScreen::setVisibleAnimate(bool visible)
{
    if (visible)
    {
        qDebug() << "pos1 = " << messageLabel->pos();
        show();
        qDebug() << "pos2 = " << messageLabel->pos();
        messageLabel->move(this->width(),0);
        qDebug() << "posm = " << messageLabel->pos();
        updateWindow();
        qDebug() << "pos3 = " << messageLabel->pos();
        showhideAnimation->setDirection(QAbstractAnimation::Forward);
        isShowing = true;
    }
    else
    {
        showhideAnimation->setDirection(QAbstractAnimation::Backward);
        isShowing = false;
    }

    showhideAnimation->start();
}

void LiveScreen::changeMessageAnimation()
{
    if (isShowing)
    {
        messageAnimation->start();
    }
    else
    {
        messageAnimation->stop();
        messageLabel->move(this->width(),0);
        qDebug() << "pos = " << messageLabel->pos();
        hide();
    }
}

void LiveScreen::changeScreen(int screen_num)
{
    screenNum = screen_num;
    updateWindow();
}

void LiveScreen::setPosition(int pos)
{
    position = pos;
    updateWindow();
}

void LiveScreen::updateWindow()
{
    int x, y1, y2, width, height;
    QRect screen = QApplication::desktop()->screenGeometry(screenNum);
    QFontMetrics fontmetrics(messageLabel->font());

    width = screen.width();
    height = backgroundWidget->sizeHint().height();

    resize(width, height);

    x = screen.x();
    switch (position)
    {
        case 0:  /* Top */
            y1 = screen.y() - this->height();
            y2 = screen.y();
            break;
        case 1:  /* Bottom */
        default:
            y1 = screen.y() + screen.height();
            y2 = screen.y() + (screen.height() - this->sizeHint().height());
            break;
    }
    move(QPoint(x, (isShowing) ? y2 : y1));

    int messageY = (this->height()/2)-(messageLabel->sizeHint().height()/2);
    showhideAnimation->setStartValue(QRect(x, y1, width, height));
    showhideAnimation->setEndValue(QRect(x, y2, width, height));
    messageAnimation->setStartValue(QPoint(this->width(), messageY));
    messageAnimation->setEndValue(QPoint(-messageLabel->sizeHint().width(), messageY));
}

void LiveScreen::setMessage(QString message)
{
    messageLabel->setText(message);
    updateWindow();
}

void LiveScreen::setRepeat(int repeat)
{
    if (repeat == 0)
        repeat = -1;
    messageAnimation->setLoopCount(repeat);
}

void LiveScreen::setBackgroundColour(QColor colour)
{
    QPalette palette(backgroundWidget->palette());
    palette.setColor(backgroundWidget->backgroundRole(), colour);
    backgroundWidget->setPalette(palette);
}

void LiveScreen::setTextColour(QColor colour)
{
    QPalette palette(messageLabel->palette());
    palette.setColor(QPalette::WindowText, colour);
    messageLabel->setPalette(palette);
}

void LiveScreen::setTextFont(QFont font)
{
    messageLabel->setFont(font);
    updateWindow();
}

void LiveScreen::setAnimationTime(int time)
{
    messageAnimation->setDuration(time * 1000);
}