/*
 * SPDX-FileCopyrightText: 2021 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#version 450

layout (binding = 0) uniform UniformBuffer
{
    highp mat4 matrix;
    mediump vec2 aspect;
    mediump float opacity;
    highp float radius;
    lowp vec4 backgroundColor;
} uniforms;

layout (location = 0) in highp vec4 in_vertex;
layout (location = 1) in highp vec2 in_uv;
layout (location = 2) in mediump vec4 in_color;
layout (location = 3) in highp float in_value;

layout (location = 0) out PerVertex
{
    highp vec2 uv;
    lowp vec4 foregroundColor;
    highp float value;
} outputs;

out gl_PerVertex
{
    highp vec4 gl_Position;
};

void main() {
    outputs.uv = (-1.0 + 2.0 * in_uv) * uniforms.aspect;
    outputs.value = in_value;
    outputs.foregroundColor = in_color;
    gl_Position = uniforms.matrix * in_vertex;
}
