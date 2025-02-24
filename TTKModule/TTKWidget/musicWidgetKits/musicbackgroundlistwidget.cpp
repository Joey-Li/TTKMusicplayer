#include "musicbackgroundlistwidget.h"
#include "musicextractwrapper.h"
#include "musictoastlabel.h"
#include "musicwidgetutils.h"

#include <QMouseEvent>

#define ITEM_COUNT      4

MusicBackgroundListItem::MusicBackgroundListItem(QWidget *parent)
    : QLabel(parent)
{
    setFixedSize(137, 100);
    setCursor(Qt::PointingHandCursor);

    m_printMask = false;
    m_closeMask = false;
    m_isSelected = false;
    m_closeSet = false;
    m_showNameMask = true;
    m_selectedMask = true;
}

void MusicBackgroundListItem::updatePixImage()
{
    if(!m_path.isEmpty())
    {
        MusicBackgroundImage image;
        if(MusicExtractWrapper::outputSkin(&image, m_path))
        {
            updatePixImage(image);
        }
    }
}

void MusicBackgroundListItem::updatePixImage(const MusicBackgroundImage &image)
{
    m_imageInfo = image.m_item;
    setPixmap(image.m_pix.scaled(size()));
}

bool MusicBackgroundListItem::contains(const MusicSkinConfigItem &item) const
{
    if(item.isValid() && m_imageInfo.isValid())
    {
        return item.m_name == m_imageInfo.m_name;
    }
    return false;
}

void MusicBackgroundListItem::setSelect(bool s)
{
    m_isSelected = s;
    update();
}

void MusicBackgroundListItem::setSelectEnabled(bool s)
{
    m_selectedMask = s;
    update();
}

void MusicBackgroundListItem::setCloseEnabled(bool s)
{
    m_closeSet = s;
    update();
}

void MusicBackgroundListItem::setShowNameEnabled(bool s)
{
    m_showNameMask = s;
    update();
}

void MusicBackgroundListItem::mousePressEvent(QMouseEvent *event)
{
    QLabel::mousePressEvent(event);
    if(m_closeSet && QRect(width() - 16 - 6, 6, 16, 16).contains(event->pos()))
    {
        Q_EMIT closeClicked(this);
    }
    else
    {
        Q_EMIT itemClicked(this);
    }
}

void MusicBackgroundListItem::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);
    m_printMask = false;
    m_closeMask = false;
    update();
}

#if TTK_QT_VERSION_CHECK(6,0,0)
void MusicBackgroundListItem::enterEvent(QEnterEvent *event)
#else
void MusicBackgroundListItem::enterEvent(QEvent *event)
#endif
{
    QLabel::enterEvent(event);
    m_printMask = true;
    m_closeMask = true;
    update();
}

void MusicBackgroundListItem::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);

    if(m_isSelected && m_selectedMask)
    {
        QPainter painter(this);
        painter.drawPixmap(width() - 17, height() - 17, 17, 17, QPixmap(":/tiny/lb_selected"));
    }

    if(m_printMask)
    {
        QPainter painter(this);
        painter.setBrush(QColor(0, 0, 0, 155));
        painter.drawRect(rect());

        QFont f = painter.font();
        f.setPixelSize(13);
        painter.setFont(f);

        painter.setPen(Qt::white);
        if(m_showNameMask)
        {
            painter.drawText((width() - MusicUtils::Widget::fontTextWidth(painter.font(), m_name)) / 2, 32, m_name);
        }

        QString v = QString::number(m_imageInfo.m_useCount);
        painter.drawText((width() - MusicUtils::Widget::fontTextWidth(painter.font(), v)) / 2, 50, v);
                v = m_imageInfo.m_name;
        painter.drawText((width() - MusicUtils::Widget::fontTextWidth(painter.font(), v)) / 2, 68, v);
    }

    if(m_closeSet && m_closeMask)
    {
        QPainter painter(this);
        painter.drawPixmap(width() - 16 - 6, 6, 16, 16, QPixmap(":/functions/btn_close_hover"));
    }
}


MusicBackgroundListWidget::MusicBackgroundListWidget(QWidget *parent)
    : QWidget(parent)
{
    m_type = First;
    m_gridLayout = new QGridLayout(this);
    m_gridLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_gridLayout->setContentsMargins(7, 0, 7, 0);
    setLayout(m_gridLayout);
    m_currentItem = nullptr;
}

MusicBackgroundListWidget::~MusicBackgroundListWidget()
{
    clearAllItems();
}

void MusicBackgroundListWidget::setCurrentItemName(const QString &name)
{
    for(MusicBackgroundListItem *item : qAsConst(m_items))
    {
        if(item->fileName() == name)
        {
            item->setSelect(true);
            m_currentItem = item;
            break;
        }
    }
}

void MusicBackgroundListWidget::clearSelectState()
{
    for(MusicBackgroundListItem *item : qAsConst(m_items))
    {
        item->setSelect(false);
    }
}

void MusicBackgroundListWidget::clearAllItems()
{
    qDeleteAll(m_items);
    m_items.clear();
    m_currentItem = nullptr;
}

void MusicBackgroundListWidget::createItem(const QString &name, const QString &path, bool state)
{
    MusicBackgroundListItem *item = new MusicBackgroundListItem(this);
    item->setCloseEnabled(state);
    item->setFileName(name);
    item->setFilePath(path);
    item->updatePixImage();

    connect(item, SIGNAL(itemClicked(MusicBackgroundListItem*)), SLOT(itemHasClicked(MusicBackgroundListItem*)));
    connect(item, SIGNAL(closeClicked(MusicBackgroundListItem*)), SLOT(itemCloseClicked(MusicBackgroundListItem*)));
    m_gridLayout->addWidget(item, m_items.count() / ITEM_COUNT, m_items.count() % ITEM_COUNT, Qt::AlignLeft | Qt::AlignTop);
    m_items << item;
}

void MusicBackgroundListWidget::createItem(const QString &icon, bool state)
{
    MusicBackgroundListItem *item = new MusicBackgroundListItem(this);
    item->setCloseEnabled(state);
    item->setPixmap(QPixmap(icon).scaled(item->size()));

    connect(item, SIGNAL(itemClicked(MusicBackgroundListItem*)), SLOT(itemHasClicked(MusicBackgroundListItem*)));
    connect(item, SIGNAL(closeClicked(MusicBackgroundListItem*)), SLOT(itemCloseClicked(MusicBackgroundListItem*)));
    m_gridLayout->addWidget(item, m_items.count() / ITEM_COUNT, m_items.count() % ITEM_COUNT, Qt::AlignLeft | Qt::AlignTop);
    m_items << item;
}

void MusicBackgroundListWidget::updateItem(const MusicBackgroundImage &image, const QString &path)
{
    for(MusicBackgroundListItem *item : qAsConst(m_items))
    {
        if(item->fileName().isEmpty())
        {
            item->setShowNameEnabled(false);
            item->setSelectEnabled(false);
            item->setFileName(path);
            item->updatePixImage(image);
            break;
        }
    }
}

bool MusicBackgroundListWidget::contains(const QString &name) const
{
    for(MusicBackgroundListItem *item : qAsConst(m_items))
    {
        if(item->fileName() == name)
        {
            return true;
        }
    }

    return false;
}

bool MusicBackgroundListWidget::contains(const MusicBackgroundImage &image) const
{
    for(MusicBackgroundListItem *item : qAsConst(m_items))
    {
        if(item->contains(image.m_item))
        {
            return true;
        }
    }

    return false;
}

int MusicBackgroundListWidget::find(MusicBackgroundListItem *item) const
{
    for(int i=0; i<m_items.count(); ++i)
    {
        if(m_items[i] == item)
        {
            return i;
        }
    }

    return -1;
}

MusicBackgroundListItem* MusicBackgroundListWidget::find(const QString &name) const
{
    for(MusicBackgroundListItem *item : qAsConst(m_items))
    {
        if(item->fileName() == name)
        {
            return item;
        }
    }

    return nullptr;
}

MusicBackgroundListItem* MusicBackgroundListWidget::find(const MusicBackgroundImage &image) const
{
    for(MusicBackgroundListItem *item : qAsConst(m_items))
    {
        if(item->contains(image.m_item))
        {
            return item;
        }
    }

    return nullptr;
}

void MusicBackgroundListWidget::updateLastedItem()
{
    if(!m_items.isEmpty())
    {
        itemHasClicked(m_items.last());
    }
}

void MusicBackgroundListWidget::itemCloseClicked(MusicBackgroundListItem *item)
{
    if(m_items.count() == 1)
    {
        MusicToastLabel::popup(tr("Last one item can not be deleted!"));
        return;
    }

    m_gridLayout->removeWidget(item);
    const int index = find(item);
    const int cIndex = find(m_currentItem);
    QFile::remove(item->filePath());
    m_items.takeAt(index)->deleteLater();

    if(index == cIndex)
    {
        m_currentItem = nullptr;
        if(!m_items.isEmpty())
        {
            itemHasClicked(m_items[index == 0 ? 0 : index - 1]);
        }
    }

    for(int i=index; i<m_items.count(); ++i)
    {
        m_gridLayout->addWidget(m_items[i], i / ITEM_COUNT, i % ITEM_COUNT, Qt::AlignLeft | Qt::AlignTop);
    }
}

void MusicBackgroundListWidget::itemHasClicked(MusicBackgroundListItem *item)
{
    if(m_currentItem)
    {
        m_currentItem->setSelect(false);
    }

    m_currentItem = item;
    m_currentItem->setSelect(true);
    Q_EMIT itemClicked(m_type, item->fileName());
}
