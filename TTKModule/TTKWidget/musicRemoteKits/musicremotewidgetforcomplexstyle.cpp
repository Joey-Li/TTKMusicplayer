#include "musicremotewidgetforcomplexstyle.h"
#include "musicstringutils.h"

MusicRemoteWidgetForComplexStyle::MusicRemoteWidgetForComplexStyle(QWidget *parent)
    : MusicRemoteWidget(parent)
{
    setGeometry(200, 200, 320, 110);
    adjustPosition(this);

    m_iconLabel = new QLabel(this);
    m_songName = new QLabel(this);
    m_songArtist = new QLabel(this);

    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->setContentsMargins(5, 5, 5, 5);
    hbox->addWidget(m_mainWidget);
    setLayout(hbox);

    QWidget *mvWidget = new QWidget(m_mainWidget);
    QHBoxLayout *mhbox = new QHBoxLayout(m_mainWidget);
    mhbox->addWidget(m_iconLabel);
    mhbox->addWidget(mvWidget);
    m_mainWidget->setLayout(mhbox);

    QWidget *toolWidget = new QWidget(mvWidget);
    QHBoxLayout *toolbox = new QHBoxLayout(toolWidget);
    toolbox->setContentsMargins(0, 0, 0, 0);
    toolbox->setSpacing(1);
    toolbox->addWidget(m_preSongButton);
    toolbox->addWidget(m_playButton);
    toolbox->addWidget(m_nextSongButton);
    toolbox->addWidget(m_volumeWidget);
    toolbox->addWidget(m_settingButton);
    toolbox->addWidget(m_showMainWindowButton);
    toolWidget->setLayout(toolbox);

    QVBoxLayout *mvbox = new QVBoxLayout(mvWidget);
    mvbox->setContentsMargins(0, 0, 0, 0);
    mvbox->setSpacing(0);
    mvbox->addWidget(m_songName);
    mvbox->addWidget(m_songArtist);
    mvbox->addWidget(toolWidget);
    mvWidget->setLayout(mvbox);

    m_iconLabel->setFixedSize(80, 80);
    m_songName->setStyleSheet(MusicUIObject::MQSSWidgetStyle01);
    m_songArtist->setStyleSheet(MusicUIObject::MQSSWidgetStyle01);
}

MusicRemoteWidgetForComplexStyle::~MusicRemoteWidgetForComplexStyle()
{
    delete m_iconLabel;
    delete m_songName;
    delete m_songArtist;
}

void MusicRemoteWidgetForComplexStyle::setLabelText(const QString &value)
{
    const bool flag = (MusicUtils::String::stringSplit(value).count() == 1);

    m_songName->setText(MusicUtils::String::songName(value));
    m_songArtist->setText(flag ? "--" : MusicUtils::String::artistName(value));

    if(!showArtPicture(m_songArtist->text()) && !showArtPicture(m_songName->text()))
    {
        m_iconLabel->setPixmap(QPixmap(":/image/lb_default_art").scaled(80, 80));
    }
}

bool MusicRemoteWidgetForComplexStyle::showArtPicture(const QString &name)
{
    const QPixmap originPath(QString(ART_DIR_FULL + name + SKN_FILE));
    if(!originPath.isNull())
    {
        m_iconLabel->setPixmap(originPath.scaled(80, 80));
        return true;
    }
    return false;
}
