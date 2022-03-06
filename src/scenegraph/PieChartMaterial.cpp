/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "PieChartMaterial.h"

const int MaxSegments = 100;

PieChartMaterial::PieChartMaterial()
{
    setFlag(QSGMaterial::Blending);
}

PieChartMaterial::~PieChartMaterial()
{
}

QSGMaterialType *PieChartMaterial::type() const
{
    static QSGMaterialType type;
    return &type;
}

QSGMaterialShader * PieChartMaterial::createShader(QSGRendererInterface::RenderMode) const
{
    return new PieChartShader();
}

PieChartWriter::PieChartWriter()
{
    Matrix = appendField(sizeof(float) * 16);
    Aspect = appendField(sizeof(float) * 2);

    Opacity = appendField(sizeof(float));
    InnerRadius = appendField(sizeof(float));
    OuterRadius = appendField(sizeof(float));

    FromAngle = appendField(sizeof(float));
    ToAngle = appendField(sizeof(float));

    SmoothEnds = appendField(sizeof(int));

    BackgroundColor = appendField(sizeof(float) * 4);

    SegmentCount = appendField(sizeof(int));

    Segments = appendField(sizeof(float) * 2 * MaxSegments);
    Colors = appendField(sizeof(float) * 4 * MaxSegments);
}

PieChartWriter *PieChartShader::s_writer = nullptr;

PieChartShader::PieChartShader()
{
    setShaderFileName(QSGMaterialShader::VertexStage, QStringLiteral(":/org.kde.quickcharts/piechart.vert.qsb"));
    setShaderFileName(QSGMaterialShader::FragmentStage, QStringLiteral(":/org.kde.quickcharts/piechart.frag.qsb"));

    if (!s_writer) {
        s_writer = new PieChartWriter;
    }
}

PieChartShader::~PieChartShader()
{
}

bool PieChartShader::updateUniformData(QSGMaterialShader::RenderState& state, QSGMaterial* newMaterial, QSGMaterial* oldMaterial)
{
    bool changed = false;

    auto data = state.uniformData();

    if (state.isMatrixDirty()) {
        s_writer->write(data, s_writer->Matrix, state.combinedMatrix());
        changed = true;
    }

    if (state.isOpacityDirty()) {
        s_writer->write(data, s_writer->Opacity, state.opacity());
        changed = true;
    }

    auto material = static_cast<PieChartMaterial*>(newMaterial);
    if (oldMaterial != newMaterial || material->dirty) {
        qDebug() << "buffer size" << data->size();

        s_writer->write(data, s_writer->Aspect, material->aspect);

        s_writer->write(data, s_writer->InnerRadius, material->innerRadius);
        s_writer->write(data, s_writer->OuterRadius, material->outerRadius);

        s_writer->write(data, s_writer->FromAngle, material->fromAngle);
        s_writer->write(data, s_writer->ToAngle, material->toAngle);

        s_writer->write(data, s_writer->SmoothEnds, material->smoothEnds ? 1 : 0);

        s_writer->write(data, s_writer->BackgroundColor, material->backgroundColor);

        std::size_t segmentCount = material->segments.size();
        s_writer->write(data, s_writer->SegmentCount, int(segmentCount));

        std::vector<float> segments(segmentCount);
        for (const auto &segment : std::as_const(material->segments)) {
            qDebug() << "segment" << segment;
            segments.push_back(segment.x());
            segments.push_back(segment.y());
        }
        s_writer->write(data, s_writer->Segments, segments.data());

        std::vector<float> colors(segmentCount);
        for (const auto &color : std::as_const(material->colors)) {
            qDebug() << "color" << color;
            colors.push_back(color.x());
            qDebug() << colors.at(colors.size() - 1);
            colors.push_back(color.y());
            qDebug() << colors.at(colors.size() - 1);
            colors.push_back(color.z());
            qDebug() << colors.at(colors.size() - 1);
            colors.push_back(color.w());
            qDebug() << colors.at(colors.size() - 1);
        }
        s_writer->write(data, s_writer->Colors, colors.data());

        material->dirty = false;
        changed = true;
    }

    return changed;
}
