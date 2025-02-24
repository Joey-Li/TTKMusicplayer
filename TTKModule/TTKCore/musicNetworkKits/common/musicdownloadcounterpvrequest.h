#ifndef MUSICDOWNLOADCOUNTERPVREQUEST_H
#define MUSICDOWNLOADCOUNTERPVREQUEST_H

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

#include "musicabstractnetwork.h"

const QString QUERY_URL  = "S1AwTklDeUNQdG95dkIrNTdzZ1lBR3hMSFFqeTlGTjJjTlo4M3hYaHAyMHV3dTBFZ0x2R1dJVXJvOFN4REphRTFBS3BFeThHUzU3QVRjMHErTEFyZ2Y3N1NPRW0zYXh6anV2VGtUYzNUb3p6NHY0eQ==";
const QString HOST_URL   = "WUwzSXNrdXhZS2owUEpzWTN3V3NOVUNPTkZNdHBMWDY=";
const QString REFER_URL  = "a0RUSDNPMnFabWQwOVhVa2JFSkVtM0JCazVKRTIrbVFJWEpKd2g0Y1NLbz0=";
const QString COOKIE_URL = "cWpxdUI0YmIyNDdOcWp6TWZvMy9hYkdNN25UYVdpdStSUEV6Q3ZpMzNkRjd4WjV0ZmxzYnYxejRiTVJlUG5Wdi9UazlweFJPZGJlbVVpdGltTndId3NxNFUrSHNUcUQ5OENMdENnaUFOcElvTkpVcFVsRTQvS2Z3MndLNTNBb3Q=";
const QString RM_KEYWORD = "S3Z3ZCsxRzJTU2krRCtoeWNjMnhodjRtYk43VVRSYjhXc05vKzM3Titpc3VmcHpuSEU0SDZnPT0=";

/*! @brief The class of get counter pv of player.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicDownloadCounterPVRequest : public MusicAbstractNetwork
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDownloadCounterPVRequest)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownloadCounterPVRequest(QObject *parent = nullptr);

    /*!
     * Start to download counter pv from net.
     */
    void startToDownload();

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override;

};

#endif // MUSICDOWNLOADCOUNTERPVREQUEST_H
