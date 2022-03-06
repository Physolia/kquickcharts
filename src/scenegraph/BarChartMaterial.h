/*
 * SPDX-FileCopyrightText: 2021 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef PIECHARTMATERIAL_H
#define PIECHARTMATERIAL_H

#include <QColor>
#include <QSGMaterial>
#include <QSGMaterialShader>

#include "ShaderDataWriter.h"

class BarChartMaterial : public QSGMaterial
{
public:
    BarChartMaterial();
    ~BarChartMaterial();

    QSGMaterialType *type() const override;
    QSGMaterialShader * createShader(QSGRendererInterface::RenderMode) const override;
    int compare(const QSGMaterial *other) const override;

    QVector2D aspect = QVector2D{1.0, 1.0};
    float radius = 0.0;
    QColor backgroundColor = Qt::transparent;
    bool dirty = false;
};

class BarChartWriter : public ShaderDataWriter
{
public:
    BarChartWriter();

    std::size_t Matrix;
    std::size_t Aspect;
    std::size_t Opacity;
    std::size_t Radius;
    std::size_t BackgroundColor;
};

class BarChartShader : public QSGMaterialShader
{
public:
    BarChartShader();

    bool updateUniformData(QSGMaterialShader::RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial) override;

    static BarChartWriter *s_writer;
};

#endif // PIECHARTMATERIAL_H
