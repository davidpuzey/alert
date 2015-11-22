#ifndef CONTROLDIALOG_H
#define CONTROLDIALOG_H

#include <QtWidgets>
#include <QWidget>
#include <QSettings>

#include "inc/livescreen.h"

class ControlDialog : public QWidget
{
    Q_OBJECT

    public:
        ControlDialog();

    private:
        void loadSettings();
        void closeEvent(QCloseEvent *);

        QSettings *settings;
        QColor backgroundColour;
        QColor textColour;
        QFont textFont;
        int position;
        int screenNum;
		int duration;
        QString message;
        int repeatNum;

        LiveScreen *livescreen;
        QGroupBox *settingsGroupBox;
        QPushButton *saveButton;
        QLabel *positionLabel;
        QComboBox *positionComboBox;
        QLabel *screenLabel;
        QComboBox *screenComboBox;
        QPushButton *backgroundcolourPushButton;
        QColorDialog *backgroundColourDialog;
        QPushButton *textcolourPushButton;
        QColorDialog *textColourDialog;
        QPushButton *textfontPushButton;
        QFontDialog *textFontDialog;
		QLabel *durationLabel;
		QSpinBox *durationSpinBox;

        QGridLayout *settingsGroupBoxLayout;

        QLabel *messageLabel;
        QLabel *repeatLabel;
        QLineEdit *messageLineEdit;
        QSpinBox *repeatSpinBox;
        QGridLayout *contentLayout;
        QPushButton *showhidePushButton;
        QVBoxLayout *mainLayout;

    private slots:
        void saveSettings();
        void showhideToggle(bool);
};

#endif
