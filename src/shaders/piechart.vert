/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#version 450

layout (constant_id = 0) const int MaxSegments = 100;

layout (std140, binding = 0) uniform UniformBuffer
{
    highp mat4 matrix;
    mediump vec2 aspect;

    mediump float opacity;
    highp float innerRadius;
    highp float outerRadius;

    mediump float fromAngle;
    mediump float toAngle;

    int smoothEnds;

    lowp vec4 backgroundColor;

    int segmentCount;
    highp vec2 segments[MaxSegments];
    lowp vec4 colors[MaxSegments];
} uniforms;

layout (location = 0) in highp vec4 in_vertex;
layout (location = 1) in mediump vec2 in_uv;

layout (location = 0) out mediump vec2 uv;

out gl_PerVertex
{
    highp vec4 gl_Position;
};

void main() {
    uv = (-1.0 + 2.0 * in_uv) * uniforms.aspect;
    uv.y *= -1.0;
    gl_Position = uniforms.matrix * in_vertex;
}
