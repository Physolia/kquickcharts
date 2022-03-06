/*
 * SPDX-FileCopyrightText: 2021 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "BarChartMaterial.h"

BarChartMaterial::BarChartMaterial()
{
    setFlag(QSGMaterial::Blending);
}

BarChartMaterial::~BarChartMaterial()
{
}

QSGMaterialType *BarChartMaterial::type() const
{
    static QSGMaterialType type;
    return &type;
}

QSGMaterialShader *BarChartMaterial::createShader(QSGRendererInterface::RenderMode) const
{
    return new BarChartShader{};
}

int BarChartMaterial::compare(const QSGMaterial *other) const
{
    auto material = static_cast<const BarChartMaterial *>(other);

    /* clang-format off */
    if (material->aspect == aspect
        && qFuzzyCompare(material->radius, radius)
        && material->backgroundColor == backgroundColor) { /* clang-format on */
        return 0;
    }

    return QSGMaterial::compare(other);
}

BarChartWriter::BarChartWriter()
{
    Matrix = appendField(sizeof(float) * 16);
    Aspect = appendField(sizeof(float) * 2);
    Opacity = appendField(sizeof(float));
    Radius = appendField(sizeof(float));
    BackgroundColor = appendField(sizeof(float) * 4);
}

BarChartWriter *BarChartShader::s_writer = nullptr;

BarChartShader::BarChartShader()
{
    setShaderFileName(QSGMaterialShader::VertexStage, QStringLiteral(":/org.kde.quickcharts/barchart.vert.qsb"));
    setShaderFileName(QSGMaterialShader::FragmentStage, QStringLiteral(":/org.kde.quickcharts/barchart.frag.qsb"));

    if (!s_writer) {
        s_writer = new BarChartWriter{};
    }
}

bool BarChartShader::updateUniformData(QSGMaterialShader::RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial)
{
    auto changed = false;

    auto data = state.uniformData();

    if (state.isMatrixDirty()) {
        s_writer->write(data, s_writer->Matrix, state.combinedMatrix());
        changed = true;
    }

    if (state.isOpacityDirty()) {
        s_writer->write(data, s_writer->Opacity, state.opacity());
        changed = true;
    }

    auto material = static_cast<BarChartMaterial*>(newMaterial);
    if (oldMaterial != newMaterial || material->dirty) {
        s_writer->write(data, s_writer->Aspect, material->aspect);
        s_writer->write(data, s_writer->Radius, material->radius);
        s_writer->write(data, s_writer->BackgroundColor, material->backgroundColor);

        material->dirty = false;
        changed = true;
    }

    return changed;
}
