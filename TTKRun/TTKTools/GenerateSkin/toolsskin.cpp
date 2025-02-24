#include "toolsskin.h"
#include "ui_toolsskin.h"
#include "musicextractwrapper.h"
#include "musicfileutils.h"
#include "musiccolordialog.h"
#include "musicbackgroundconfigmanager.h"

ToolsSkin::ToolsSkin(QWidget *parent)
    : QWidget(parent),
      m_ui(new Ui::ToolsSkin)
{
    m_ui->setupUi(this);
    setFixedSize(630, 400);

    m_pixmap = QPixmap(1, 1);
    m_pixmap.fill(Qt::white);
    m_ui->pixLabel->setPixmap(m_pixmap.scaled(m_ui->pixLabel->size()));

    connect(m_ui->localButton, SIGNAL(clicked()), SLOT(imageClicked()));
    connect(m_ui->paletteButton, SIGNAL(clicked()), SLOT(paletteClicked()));
    connect(m_ui->readButton, SIGNAL(clicked()), SLOT(readClicked()));
    connect(m_ui->writeButton, SIGNAL(clicked()), SLOT(writeClicked()));
}

ToolsSkin::~ToolsSkin()
{
    delete m_ui;
}

void ToolsSkin::imageClicked()
{
    const QString &path = MusicUtils::File::openFileDialog(this);
    if(path.isEmpty())
    {
        return;
    }

    m_pixmap = QPixmap(path);
    m_ui->pixLabel->setPixmap(m_pixmap.scaled(m_ui->pixLabel->size()));
}

void ToolsSkin::paletteClicked()
{
    const QColor &color = MusicColorDialog::popup(this);
    if(!color.isValid())
    {
        return;
    }

    m_pixmap = QPixmap(1, 1);
    m_pixmap.fill(color);
    m_ui->pixLabel->setPixmap(m_pixmap.scaled(m_ui->pixLabel->size()));
}

void ToolsSkin::readClicked()
{
    const QString &path = MusicUtils::File::openFileDialog(this, "TTKS Files (*.ttks)");
    if(path.isEmpty())
    {
        return;
    }

    MusicBackgroundImage image;
    if(MusicExtractWrapper::outputSkin(&image, path))
    {
        m_pixmap = image.m_pix;
        m_ui->pixLabel->setPixmap(m_pixmap.scaled(m_ui->pixLabel->size()));
        m_ui->nameLineEdit->setText(image.m_item.m_name);
        m_ui->countLineEdit->setText(QString::number(image.m_item.m_useCount));
    }
}

void ToolsSkin::writeClicked()
{
    const QString &path = MusicUtils::File::saveFileDialog(this, "TTKS Files (*.ttks)");
    if(path.isEmpty())
    {
        return;
    }

    MusicBackgroundImage image;
    image.m_pix = m_pixmap;
    image.m_item.m_name = m_ui->nameLineEdit->text().trimmed();
    image.m_item.m_useCount = m_ui->countLineEdit->text().trimmed().toInt();
    TTK_LOGGER_INFO("Save state " << MusicExtractWrapper::inputSkin(&image, path));
}
