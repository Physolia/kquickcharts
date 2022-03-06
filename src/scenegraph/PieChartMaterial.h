/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef PIECHARTMATERIAL_H
#define PIECHARTMATERIAL_H

#include <QColor>
#include <QSGMaterial>
#include <QSGMaterialShader>

#include "ShaderDataWriter.h"

class PieChartMaterial : public QSGMaterial
{
public:
    PieChartMaterial();
    ~PieChartMaterial();

    QSGMaterialType *type() const override;
    QSGMaterialShader *createShader(QSGRendererInterface::RenderMode) const override;

    QVector2D aspect;
    float innerRadius = 0.0f;
    float outerRadius = 0.0f;
    QColor backgroundColor;
    bool smoothEnds = false;
    float fromAngle = 0.0;
    float toAngle = 6.28318; // 2 * pi

    QVector<QVector2D> segments;
    QVector<QVector4D> colors;

    bool dirty = false;
};

class PieChartWriter : public ShaderDataWriter
{
public:
    PieChartWriter();

    std::size_t Matrix;
    std::size_t Aspect;

    std::size_t Opacity;
    std::size_t InnerRadius;
    std::size_t OuterRadius;

    std::size_t FromAngle;
    std::size_t ToAngle;

    std::size_t SmoothEnds;

    std::size_t BackgroundColor;

    std::size_t Segments;
    std::size_t Colors;
    std::size_t SegmentCount;
};

class PieChartShader : public QSGMaterialShader
{
public:
    PieChartShader();
    ~PieChartShader();

    bool updateUniformData(QSGMaterialShader::RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial) override;

private:
    static PieChartWriter *s_writer;
};

#endif // PIECHARTMATERIAL_H
