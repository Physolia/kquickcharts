/*
 * SPDX-FileCopyrightText: 2022 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "ShaderDataWriter.h"

ShaderDataWriter::ShaderDataWriter()
{
}

void ShaderDataWriter::addField(std::size_t index, uint32_t offset, uint32_t size)
{
    if (index >= m_fields.size()) {
        m_fields.resize(index + 1);
    }

    m_fields[index] = std::make_pair(offset, size);
}

std::size_t ShaderDataWriter::appendField(uint32_t size)
{
    m_fields.push_back(std::make_pair(m_currentOffset, size));
    m_currentOffset += size;
    return m_fields.size() - 1;
}

void ShaderDataWriter::write(QByteArray* destination, std::size_t index, int data)
{
    writeImpl(destination, index, &data);
}

void ShaderDataWriter::write(QByteArray *destination, std::size_t index, float data)
{
    writeImpl(destination, index, &data);
}

void ShaderDataWriter::write(QByteArray *destination, std::size_t index, const QVector2D &data)
{
    float tmp[2] = { data.x(), data.y() };
    writeImpl(destination, index, tmp);
}

void ShaderDataWriter::write(QByteArray *destination, std::size_t index, const QVector3D &data)
{
    float tmp[3] = { data.x(), data.y(), data.z() };
    writeImpl(destination, index, tmp);
}

void ShaderDataWriter::write(QByteArray *destination, std::size_t index, const QVector4D& data)
{
    float tmp[4] = { data.x(), data.y(), data.z(), data.w() };
    writeImpl(destination, index, tmp);
}

void ShaderDataWriter::write(QByteArray *destination, std::size_t index, const QColor& data)
{
    float tmp[4] = { data.redF(), data.greenF(), data.blueF(), data.alphaF() };
    writeImpl(destination, index, tmp);
}

void ShaderDataWriter::write(QByteArray *destination, std::size_t index, const QMatrix4x4& data)
{
    writeImpl(destination, index, data.constData());
}

void ShaderDataWriter::write(QByteArray* destination, std::size_t index, float *data)
{
    writeImpl(destination, index, data);
}

void ShaderDataWriter::writeImpl(QByteArray *destination, std::size_t index, const void *data)
{
    if (index >= m_fields.size()) {
        return;
    }

    auto info = m_fields.at(index);
    memcpy(destination->data() + info.first, data, info.second);
}
