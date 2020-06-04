#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <math.h>
#include <stdlib.h>

#include "fft.h"
#include "inlines.h"
#include "outerrayswave.h"

OuterRaysWave::OuterRaysWave (QWidget *parent)
    : Visual(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, false);
    setAttribute(Qt::WA_QuitOnClose, false);

    setWindowTitle(tr("Outer RaysWave Widget"));
}

OuterRaysWave::~OuterRaysWave()
{

}

void OuterRaysWave::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    draw(&painter);
}

void OuterRaysWave::process(float *left, float *)
{
    static fft_state *state = nullptr;
    if(!state)
    {
        state = fft_init();
    }

    const int rows = height();
    const int cols = width();

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;

        if(m_intern_vis_data)
        {
            delete[] m_intern_vis_data;
        }

        m_intern_vis_data = new int[m_cols]{0};
    }

    const int step = (QMMP_VISUAL_NODE_SIZE << 8) / m_cols;
    int pos = 0;

    for(int i = 0; i < m_cols; ++i)
    {
        pos += step;
        m_intern_vis_data[i] = int(left[pos >> 8] * m_rows / 2);
        m_intern_vis_data[i] = qBound(-m_rows / 2, m_intern_vis_data[i], m_rows / 2);
    }
}

void OuterRaysWave::draw(QPainter *p)
{
    p->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    p->setPen(QPen(Qt::white, 1));

    const float maxed = takeMaxRange();

    for(int i = 0; i < m_cols; ++i)
    {
        if((i + 1) >= m_cols)
        {
            break;
        }

        int pFront = m_rows / 2 - m_intern_vis_data[i] * maxed;
        int pEnd = m_rows / 2 - m_intern_vis_data[i + 1] * maxed;

        if(pFront > pEnd)
        {
            qSwap(pFront, pEnd);
        }

        p->drawLine(i, pFront, i + 1, pEnd);
    }
}