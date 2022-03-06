/*
 * SPDX-FileCopyrightText: 2022 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef SHADERDATAWRITER_H
#define SHADERDATAWRITER_H

#include <vector>

#include <QByteArray>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>
#include <QColor>

/**
 * This is a helper class for writing values to a uniform buffer in custom
 * QSGMaterialShader classes. This primarily exists because Qt dropped the ball
 * and doesn't provide any API for this.
 */
class ShaderDataWriter
{
public:
    ShaderDataWriter();

    void addField(std::size_t index, uint32_t offset, uint32_t size);
    std::size_t appendField(uint32_t size);

    void write(QByteArray *destination, std::size_t index, int data);
    void write(QByteArray *destination, std::size_t index, float data);
    void write(QByteArray *destination, std::size_t index, const QVector2D &data);
    void write(QByteArray *destination, std::size_t index, const QVector3D &data);
    void write(QByteArray *destination, std::size_t index, const QVector4D &data);
    void write(QByteArray *destination, std::size_t index, const QColor &data);
    void write(QByteArray *destination, std::size_t index, const QMatrix4x4 &data);
    void write(QByteArray *destination, std::size_t index, float *data);

private:
    void writeImpl(QByteArray *destination, std::size_t index, const void *data);

    QByteArray *m_destination;
    std::vector<std::pair<uint32_t, uint32_t>> m_fields;
    uint32_t m_currentOffset = 0;
};

#endif // SHADERDATAWRITER_H
