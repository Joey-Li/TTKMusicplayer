#ifndef MUSICSOURCEUPDATEWIDGET_H
#define MUSICSOURCEUPDATEWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2022 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include "musicabstractmovedialog.h"
#include "musicabstractmovewidget.h"

#define REPORT_BUG_URL      "V0pTNkY2K1pRdVdzVDBrWFZGWnJiWHhVcHNTd2xlSE1FZUlQbytlN0tDK3ZUSVRqQ1ViZTFxQlpCVkR4akVhSk44cnMrVDhzWU5jPQ=="
#define RELEASE_DATA_URL    "OGk2YzFYT2NubTYxYkVDZG9aczB5NlNoeHVwRDZDeEttcEMxSGhScUVWcGRJNXdBK3IzZ0p2aWpiVnF4VjV3aEtHNlNERkhmN2pNPQ=="

namespace Ui {
class MusicSourceUpdateWidget;
}


/*! @brief The class of the application upgrade notify widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSourceUpdateNotifyWidget : public MusicAbstractMoveSingleWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSourceUpdateNotifyWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSourceUpdateNotifyWidget(QWidget *parent = nullptr);
    ~MusicSourceUpdateNotifyWidget();

    /*!
     * Start update.
     */
    void start();

public Q_SLOTS:
    /*!
     * Update source clicked.
     */
    void updateSourceClicked();
    /*!
     * Download data from kuwo net finished.
     */
    void downLoadFinished(const QVariant &bytes);

protected:
    QLabel *m_textLabel;

};


/*! @brief The class of the application upgrade widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSourceUpdateWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSourceUpdateWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSourceUpdateWidget(QWidget *parent = nullptr);
    ~MusicSourceUpdateWidget();

    /*!
     * Start update.
     */
    void start();

public Q_SLOTS:
    /*!
     * Upgrade button clicked.
     */
    void upgradeButtonClicked();
    /*!
     * Download data from kuwo net finished.
     */
    void downLoadFinished(const QVariant &bytes);
    /*!
     * Override exec function.
     */
    virtual int exec();

protected:
    Ui::MusicSourceUpdateWidget *m_ui;

};

#endif // MUSICSOURCEUPDATEWIDGET_H
