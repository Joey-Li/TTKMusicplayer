#include "musicplaylist.h"
#include "musictime.h"

MusicPlaylist::MusicPlaylist(QObject *parent)
    : QObject(parent)
{
    MusicTime::initRandom();
    m_currentIndex = -1;
    m_playbackMode = MusicObject::PlayOrder;
}

MusicObject::PlayMode MusicPlaylist::playbackMode() const
{
    return m_playbackMode;
}

void MusicPlaylist::setPlaybackMode(MusicObject::PlayMode mode)
{
    m_playbackMode = mode;
    Q_EMIT playbackModeChanged(m_playbackMode);
}

int MusicPlaylist::mapItemIndex(const MusicPlayItem &item) const
{
    int playIndex = -1;
    for(int i=0; i<m_mediaList.count(); ++i)
    {
        const MusicPlayItem &it = m_mediaList[i];
        if(item == it)
        {
            playIndex = i;
            break;
        }
    }

    return playIndex;
}

int MusicPlaylist::currentIndex() const
{
    return m_currentIndex;
}

MusicPlayItem MusicPlaylist::currentItem() const
{
    if(m_currentIndex == -1 || m_currentIndex >= m_mediaList.count())
    {
        return MusicPlayItem();
    }
    return m_mediaList.isEmpty() ? MusicPlayItem() : m_mediaList[m_currentIndex];
}

QString MusicPlaylist::currentMediaPath() const
{
    const MusicPlayItem &item = currentItem();
    if(item.m_toolIndex == MUSIC_NETWORK_LIST && /*Replace network url path to local path*/
      (item.m_path.startsWith(HTTP_PREFIX) || item.m_path.startsWith(HTTPS_PREFIX)))
    {
        const QString &id = item.m_path.section("#", -1);
        if(id == item.m_path)
        {
            return item.m_path;
        }

        const QString &cachePath = CACHE_DIR_FULL + id;
        return QFile::exists(cachePath) ? cachePath : item.m_path;
    }
    return item.m_path;
}

MusicPlayItems *MusicPlaylist::mediaList()
{
    return &m_mediaList;
}

int MusicPlaylist::mediaCount() const
{
    return m_mediaList.count();
}

bool MusicPlaylist::isEmpty() const
{
    return m_mediaList.isEmpty();
}

void MusicPlaylist::clear()
{
    m_mediaList.clear();
}

int MusicPlaylist::find(int toolIndex, const QString &content, int from)
{
    return m_mediaList.indexOf(MusicPlayItem(toolIndex, content), from);
}

void MusicPlaylist::addMedia(int toolIndex, const QString &content)
{
    m_mediaList.clear();
    m_queueMediaList.clear();
    m_mediaList << MusicPlayItem(toolIndex, content);
}

void MusicPlaylist::addMedia(int toolIndex, const QStringList &items)
{
    m_mediaList.clear();
    m_queueMediaList.clear();
    for(const QString &path : qAsConst(items))
    {
        m_mediaList << MusicPlayItem(toolIndex, path);
    }
}

void MusicPlaylist::addMedia(const MusicPlayItem &item)
{
    m_mediaList.clear();
    m_queueMediaList.clear();
    m_mediaList << item;
}

void MusicPlaylist::addMedia(const MusicPlayItems &items)
{
    m_queueMediaList.clear();
    m_mediaList = items;
}

void MusicPlaylist::appendMedia(int toolIndex, const QString &content)
{
    m_mediaList << MusicPlayItem(toolIndex, content);
}

void MusicPlaylist::appendMedia(int toolIndex, const QStringList &items)
{
    for(const QString &path : qAsConst(items))
    {
        m_mediaList << MusicPlayItem(toolIndex, path);
    }
}

void MusicPlaylist::appendMedia(const MusicPlayItem &item)
{
    m_mediaList << item;
}

void MusicPlaylist::appendMedia(const MusicPlayItems &items)
{
    m_mediaList << items;
}

bool MusicPlaylist::removeMedia(int pos)
{
    if(pos < 0 || pos >= m_mediaList.count())
    {
        return false;
    }

    m_mediaList.removeAt(pos);
    removeQueueList();
    return true;
}

int MusicPlaylist::removeMedia(int toolIndex, const QString &content)
{
    const int index = find(toolIndex, content);
    if(index != -1)
    {
        m_mediaList.removeAt(index);
        removeQueueList();
    }

    return index;
}

MusicPlayItems *MusicPlaylist::queueMediaList()
{
    return &m_queueMediaList;
}

void MusicPlaylist::insertQueueMedia(int toolIndex, const QString &content)
{
    if(m_currentIndex != -1)
    {
        const int index = m_currentIndex + 1;
        (index != m_mediaList.count()) ? m_mediaList.insert(index, MusicPlayItem(toolIndex, content))
                                       : m_mediaList.append(MusicPlayItem(toolIndex, content));
        m_queueMediaList << MusicPlayItem(index + m_queueMediaList.count(), content);
    }
}

void MusicPlaylist::removeQueueList()
{
    m_queueMediaList.clear();
}

void MusicPlaylist::setCurrentIndex(int index)
{
    if(index == DEFAULT_LOW_LEVEL)
    {
        switch(m_playbackMode)
        {
            case MusicObject::PlayOneLoop: break;
            case MusicObject::PlayOrder:
            {
                if(++m_currentIndex >= m_mediaList.count())
                {
                    m_currentIndex = -1;
                }
                break;
            }
            case MusicObject::PlaylistLoop:
            {
                if(++m_currentIndex >= m_mediaList.count())
                {
                    m_currentIndex = 0;
                }
                break;
            }
            case MusicObject::PlayRandom:
            {
                m_currentIndex = MusicTime::random() % m_mediaList.count();
                break;
            }
            case MusicObject::PlayOnce: break;
            default: break;
        }
    }
    else
    {
        m_currentIndex = index;
    }

    if(!m_queueMediaList.isEmpty())
    {
        const MusicPlayItem &item = m_queueMediaList.takeFirst();
        m_currentIndex = item.m_toolIndex;
        if(m_currentIndex < 0 || m_currentIndex >= m_mediaList.count())
        {
            m_currentIndex = -1;
        }
    }

    Q_EMIT currentIndexChanged(m_currentIndex);
}

void MusicPlaylist::setCurrentIndex(int toolIndex, const QString &path)
{
    const int playIndex = mapItemIndex(MusicPlayItem(toolIndex, path));
    setCurrentIndex(playIndex);
}
