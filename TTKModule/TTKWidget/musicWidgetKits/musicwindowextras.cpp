#include "musicwindowextras.h"
#include "musicapplication.h"

#include <QStyle>
#if defined Q_OS_WIN && TTK_QT_VERSION_CHECK(5,2,0)
#  include <QtWinExtras/QtWinExtras>
#endif

MusicWindowExtras::MusicWindowExtras(QObject *parent)
    : QObject(parent)
{
#if defined Q_OS_WIN && TTK_QT_VERSION_CHECK(5,2,0)
    m_playToolButton = nullptr;
    m_forwardToolButton = nullptr;
    m_backwardToolButton = nullptr;
    m_thumbnailToolBar = nullptr;
    m_parentClass = TTKStatic_cast(MusicApplication*, parent);

    disableBlurBehindWindow(QtWin::isCompositionEnabled());
    createThumbnailToolBar();
#else
    m_disableBlurBehindWindow = true;
#endif
}

MusicWindowExtras::~MusicWindowExtras()
{
#if defined Q_OS_WIN && TTK_QT_VERSION_CHECK(5,2,0)
    delete m_playToolButton;
    delete m_forwardToolButton;
    delete m_backwardToolButton;
    delete m_thumbnailToolBar;
#endif
}

void MusicWindowExtras::disableBlurBehindWindow(bool enable)
{
    m_disableBlurBehindWindow = enable;
#if defined Q_OS_WIN && TTK_QT_VERSION_CHECK(5,2,0)
    QtWin::enableBlurBehindWindow(m_parentClass);
    QtWin::disableBlurBehindWindow(m_parentClass);
#endif
}

void MusicWindowExtras::setCurrentPlayStatus(bool status) const
{
#if defined Q_OS_WIN && TTK_QT_VERSION_CHECK(5,2,0)
    const QStyle::StandardPixmap pix = status ? QStyle::SP_MediaPause : QStyle::SP_MediaPlay;
    m_playToolButton->setIcon(m_parentClass->style()->standardIcon(pix));
#else
    Q_UNUSED(status);
#endif
}

#if defined Q_OS_WIN && TTK_QT_VERSION_CHECK(5,2,0)
void MusicWindowExtras::createThumbnailToolBar()
{
    m_thumbnailToolBar = new QWinThumbnailToolBar(this);
    m_thumbnailToolBar->setWindow(m_parentClass->windowHandle());

    m_playToolButton = new QWinThumbnailToolButton(m_thumbnailToolBar);
    m_playToolButton->setToolTip(tr("Play"));
    m_playToolButton->setIcon(m_parentClass->style()->standardIcon(QStyle::SP_MediaPlay));
    connect(m_playToolButton, SIGNAL(clicked()), m_parentClass, SLOT(musicStatePlay()));

    m_forwardToolButton = new QWinThumbnailToolButton(m_thumbnailToolBar);
    m_forwardToolButton->setToolTip(tr("Previous"));
    m_forwardToolButton->setIcon(m_parentClass->style()->standardIcon(QStyle::SP_MediaSeekBackward));
    connect(m_forwardToolButton, SIGNAL(clicked()), m_parentClass, SLOT(musicPlayPrevious()));

    m_backwardToolButton = new QWinThumbnailToolButton(m_thumbnailToolBar);
    m_backwardToolButton->setToolTip(tr("Next"));
    m_backwardToolButton->setIcon(m_parentClass->style()->standardIcon(QStyle::SP_MediaSeekForward));
    connect(m_backwardToolButton, SIGNAL(clicked()), m_parentClass, SLOT(musicPlayNext()));

    m_thumbnailToolBar->addButton(m_forwardToolButton);
    m_thumbnailToolBar->addButton(m_playToolButton);
    m_thumbnailToolBar->addButton(m_backwardToolButton);
}
#endif
