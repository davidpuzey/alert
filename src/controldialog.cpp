#include "inc/controldialog.h"
#include "inc/livescreen.h"

ControlDialog::ControlDialog()
{
    int i;
    int screens = QApplication::desktop()->screenCount();

    /* Window Properties */
    setAttribute(Qt::WA_QuitOnClose, true);

    loadSettings();
    livescreen = new LiveScreen;

    saveButton = new QPushButton("Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveSettings()));

    positionLabel = new QLabel(tr("Position"));
    positionComboBox = new QComboBox;
    positionComboBox->addItem("Top");
    positionComboBox->addItem("Bottom");
    positionComboBox->setCurrentIndex(position);
    livescreen->setPosition(position);
    connect(positionComboBox, SIGNAL(currentIndexChanged(int)), livescreen, SLOT(setPosition(int)));

    screenLabel = new QLabel(tr("Screen"));
    screenComboBox = new QComboBox;
    for (i = 0; i < screens; i++)
    {
        screenComboBox->addItem(QString::number(i+1));
    }
    if (screenNum < 1 || screenNum > screenComboBox->count())
        screenNum = 1;
    screenComboBox->setCurrentIndex(screenNum-1);
    livescreen->changeScreen(screenNum-1);
    connect(screenComboBox, SIGNAL(currentIndexChanged(int)), livescreen, SLOT(changeScreen(int)));

    backgroundcolourPushButton = new QPushButton(tr("Background Colour"));
    backgroundColourDialog = new QColorDialog;
    backgroundColourDialog->setWindowTitle(tr("Select Background Color"));
    backgroundColourDialog->setOption(QColorDialog::ShowAlphaChannel);
    backgroundColourDialog->setCurrentColor(backgroundColour);
    livescreen->setBackgroundColour(backgroundColour);
    connect(backgroundcolourPushButton, SIGNAL(clicked()), backgroundColourDialog, SLOT(show()));
    connect(backgroundColourDialog, SIGNAL(colorSelected(QColor)), livescreen, SLOT(setBackgroundColour(QColor)));

    textcolourPushButton = new QPushButton(tr("Text Colour"));
    textColourDialog = new QColorDialog;
    textColourDialog->setWindowTitle(tr("Select Text Colour"));
    textColourDialog->setCurrentColor(textColour);
    livescreen->setTextColour(textColour);
    connect(textcolourPushButton, SIGNAL(clicked()), textColourDialog, SLOT(show()));
    connect(textColourDialog, SIGNAL(colorSelected(QColor)), livescreen, SLOT(setTextColour(QColor)));

    textfontPushButton = new QPushButton(tr("Text Font"));
    textFontDialog = new QFontDialog;
    textFontDialog->setWindowTitle(tr("Select Text Font"));
    textFontDialog->setCurrentFont(textFont);
    livescreen->setTextFont(textFont);
    connect(textfontPushButton, SIGNAL(clicked()), textFontDialog, SLOT(show()));
    connect(textFontDialog, SIGNAL(fontSelected(QFont)), livescreen, SLOT(setTextFont(QFont)));

	durationLabel = new QLabel(tr("Duration"));
	durationSpinBox = new QSpinBox;
	durationSpinBox->setSuffix(tr(" s"));
	durationSpinBox->setValue(duration);
	livescreen->setAnimationTime(duration);
	connect(durationSpinBox, SIGNAL(valueChanged(int)), livescreen, SLOT(setAnimationTime(int)));

    settingsGroupBox = new QGroupBox(tr("Settings"));
	settingsGroupBox->setCheckable(true);
	if (QFile("settings.ini").exists())
		settingsGroupBox->setChecked(false);
	else
		settingsGroupBox->setChecked(true);
	//connect(settingsGroupBox, SIGNAL(toggled(bool)), settingsGroupBox, SLOT(setVisible(bool)));
    settingsGroupBoxLayout = new QGridLayout;
    settingsGroupBoxLayout->addWidget(positionLabel, 0, 0);
    settingsGroupBoxLayout->addWidget(positionComboBox, 0, 1);
    settingsGroupBoxLayout->addWidget(screenLabel, 0, 2);
    settingsGroupBoxLayout->addWidget(screenComboBox, 0, 3);
    settingsGroupBoxLayout->addWidget(backgroundcolourPushButton, 1, 0);
    settingsGroupBoxLayout->addWidget(textcolourPushButton, 1, 1);
    settingsGroupBoxLayout->addWidget(textfontPushButton, 1, 2);
	settingsGroupBoxLayout->addWidget(durationLabel, 2, 0);
	settingsGroupBoxLayout->addWidget(durationSpinBox, 2, 1);
    settingsGroupBoxLayout->addWidget(saveButton, 3, 0);
    settingsGroupBox->setLayout(settingsGroupBoxLayout);

    messageLabel = new QLabel(tr("Message"));
    messageLineEdit = new QLineEdit;
    messageLineEdit->setText(message);
    livescreen->setMessage(message);
    connect(messageLineEdit, SIGNAL(textChanged(QString)), livescreen, SLOT(setMessage(QString)));
    repeatLabel = new QLabel(tr("Repeat\n(0 means forever)"));
    repeatSpinBox = new QSpinBox;
    repeatSpinBox->setMinimum(0);
    if (repeatNum < 0)
        repeatNum = 1;
    repeatSpinBox->setValue(repeatNum);
    livescreen->setRepeat(repeatNum);
    connect(repeatSpinBox, SIGNAL(valueChanged(int)), livescreen, SLOT(setRepeat(int)));

    contentLayout = new QGridLayout;
    contentLayout->addWidget(messageLabel, 0, 0);
    contentLayout->addWidget(messageLineEdit, 1, 0);
    contentLayout->addWidget(repeatLabel, 0, 1);
    contentLayout->addWidget(repeatSpinBox, 1, 1);

    showhidePushButton = new QPushButton(tr("Show"));
    showhidePushButton->setCheckable(true);
    showhidePushButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(showhidePushButton, SIGNAL(clicked(bool)), this, SLOT(showhideToggle(bool)));
    connect(showhidePushButton, SIGNAL(clicked(bool)), livescreen, SLOT(setVisibleAnimate(bool)));
    connect(livescreen, SIGNAL(scrollEnded()), showhidePushButton, SLOT(click()));

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(settingsGroupBox);
    mainLayout->addLayout(contentLayout);
    mainLayout->addWidget(showhidePushButton);
    setLayout(mainLayout);

    setWindowTitle(tr("Alert"));
}

void ControlDialog::loadSettings()
{
    settings = new QSettings("settings.ini", QSettings::IniFormat);
    backgroundColour = settings->value("background-colour", QColor(Qt::black)).value<QColor>();
    textColour = settings->value("text-colour", QColor(Qt::white)).value<QColor>();
    textFont = settings->value("text-font").value<QFont>();
    position = settings->value("position", 1).toInt();
    screenNum = settings->value("screen", 2).toInt();
	duration = settings->value("duration", 10).toInt();
    message = settings->value("message", "").toString();
    repeatNum = settings->value("repeat", 1).toInt();
}

void ControlDialog::saveSettings()
{
    settings->setValue("background-colour", backgroundColourDialog->currentColor());
    settings->setValue("text-colour", textColourDialog->currentColor());
    settings->setValue("text-font", textFontDialog->currentFont());
    settings->setValue("position", positionComboBox->currentIndex());
    settings->setValue("screen", screenComboBox->currentIndex() + 1);
	settings->setValue("duration", durationSpinBox->value());
    settings->setValue("message", messageLineEdit->text());
    settings->setValue("repeat", repeatSpinBox->value());
}

void ControlDialog::showhideToggle(bool checked)
{
    if (checked)
    {
        showhidePushButton->setText(tr("Hide"));
    }
    else
    {
        showhidePushButton->setText(tr("Show"));
    }
}

void ControlDialog::closeEvent(QCloseEvent *event)
{
    livescreen->hide();
    backgroundColourDialog->hide();
    textColourDialog->hide();
    textFontDialog->hide();
    event->accept();
}
