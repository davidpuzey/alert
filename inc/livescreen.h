#ifndef LIVESCREEN_H
#define LIVESCREEN_H

#include <QtWidgets>
#include <QWidget>

class LiveScreen : public QWidget
{
    Q_OBJECT


    public:
        LiveScreen();

    public slots:
        void setVisibleAnimate(bool);
        void changeMessageAnimation();
        void changeScreen(int);
        void setPosition(int);
        void setMessage(QString);
        void setRepeat(int);
        void setBackgroundColour(QColor);
        void setTextColour(QColor);
        void setTextFont(QFont);
		void setAnimationTime(int);

    signals:
        void scrollEnded();

    private:
        void updateWindow();
        QPropertyAnimation *showhideAnimation;
        QPropertyAnimation *messageAnimation;
        bool isShowing;
        int screenNum;
        int fontSize;
        int position;
        QWidget *backgroundWidget;
        QLabel *messageLabel;
        QFormLayout *backgroundLayout;
        QHBoxLayout *mainLayout;
};

#endif
