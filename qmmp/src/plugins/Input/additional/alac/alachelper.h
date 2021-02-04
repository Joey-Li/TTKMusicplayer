/* =================================================
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2021 Greedysky Studio

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
 ================================================= */

#ifndef ALACHELPER_H
#define ALACHELPER_H

#include <QObject>

extern "C" {
#include "alac_demux.h"
#include "alac_decomp.h"
#include "alac_stream.h"
}

class AlacHelper
{
public:
    explicit AlacHelper(const QString &path);
    ~AlacHelper();

    void close();

    bool initialize();
    qint64 totalTime() const;
    void seek(qint64 time);

    int bitrate() const;
    int sampleRate() const;
    int channels() const;

    int read(unsigned char *data, int size);

private:
    FILE *m_file = nullptr;
    unsigned char m_buffer[4096 * 4] = { 0 };
    unsigned char m_stream_buffer[4096 * 4 * 4] = { 0 };

    int m_decoded_frames = 0;
    int m_output_bytes = 0;
    alac_file *m_alac = nullptr;
    demux_res_t m_demux;
    stream_t *m_stream = nullptr;

    int m_bitrate = 0;
    qint64 m_totalTime = 0;
    QString m_path;

};

#endif