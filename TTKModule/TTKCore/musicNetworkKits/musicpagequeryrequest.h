#ifndef MUSICPAGEQUERYREQUEST_H
#define MUSICPAGEQUERYREQUEST_H

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

/*! @brief The class of abstract download page query request.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicPageQueryRequest : public MusicAbstractNetwork
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicPageQueryRequest)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicPageQueryRequest(QObject *parent = nullptr);

    /*!
     * Start to search data from name and type bt page.
     */
    virtual void startToPage(int offset);

    /*!
     * Return the each page max size.
     */
    inline int pageSize() const { return m_pageSize; }
    /*!
     * Return the total number.
     */
    inline int totalSize() const { return m_totalSize; }
    /*!
     * Return the page index number.
     */
    inline int pageIndex() const { return m_pageIndex; }

protected:
    int m_pageSize, m_totalSize, m_pageIndex;

};

#endif // MUSICPAGEQUERYREQUEST_H
