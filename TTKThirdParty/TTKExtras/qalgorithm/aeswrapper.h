#ifndef QAESWRAPPER_H
#define QAESWRAPPER_H

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

#include "ttkglobaldefine.h"

namespace QAlgorithm
{
/*! @brief The class of the aes wrapper.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT Aes
{
public:
    /*!
     * Encrypt des ECB by input.
     */
    QByteArray encryptECB(const QByteArray &in, const QByteArray &key);
    /*!
     * Decrypt des ECB by input.
     */
    QByteArray decryptECB(const QByteArray &in, const QByteArray &key);

    /*!
     * Encrypt aes CBC by input.
     */
    QByteArray encryptCBC(const QByteArray &in, const QByteArray &key, const QByteArray &iv);
    /*!
     * Decrypt aes CBC by input.
     */
    QByteArray decryptCBC(const QByteArray &in, const QByteArray &key, const QByteArray &iv);

};
}

#endif // QAESWRAPPER_H
