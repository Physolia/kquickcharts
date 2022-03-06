/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#version 450

#line 11

layout (constant_id = 0) const int MaxPointCount = 18;

layout (std140, binding = 0) uniform UniformBuffer
{
    highp mat4 matrix;
    lowp float opacity;
    highp float lineWidth;
    highp float smoothing;
    highp float aspect;
} uniforms;

layout (location = 0) in highp vec4 in_vertex;
layout (location = 1) in highp vec2 in_uv;

layout (location = 2) in mediump vec4 in_lineColor;
layout (location = 3) in mediump vec4 in_fillColor;
layout (location = 4) in mediump vec2 in_bounds;

// layout (location = 5) in highp int in_count;

// layout (location = 5) in highp vec2 in_points[MaxPointCount];

// Input points. Since OpenGL 2.1/GLSL 1.10 does not support array vertex
// attributes, we have to manually declare a number of attributes. In addition,
// while we use vec2 as point data format, we can store either a vec2 or a vec4
// as a vertex attribute. Therefore, we define these as vec4 and manually unpack
// to make use of as much storage as possible.
// in mediump vec4 in_points_0;
// in mediump vec4 in_points_1;
// in mediump vec4 in_points_2;
// in mediump vec4 in_points_3;
// in mediump vec4 in_points_4;
// in mediump vec4 in_points_5;
// in mediump vec4 in_points_6;
// in mediump vec4 in_points_7;
// in mediump vec4 in_points_8;

layout (location = 0) out Outputs
{
    highp vec2 uv;

    highp vec2 bounds;

    mediump vec4 lineColor;
    mediump vec4 fillColor;

    highp int pointCount;
    highp vec2 points[MaxPointCount];
} outputs;

out gl_PerVertex
{
    highp vec4 gl_Position;
};

// out mediump vec2 uv;
// out mediump vec2 points[MAXIMUM_POINT_COUNT];
// out highp float pointCount;
// out mediump vec2 bounds;
// out mediump vec4 lineColor;
// out mediump vec4 fillColor;

void main() {
    outputs.uv = in_uv;
    outputs.uv.y = (1.0 + -1.0 * outputs.uv.y) * uniforms.aspect;

//     for (int i = 0; i < MaxPointCount; ++i) {
//         outputs.points[i] = in_points[i];
//     }

//     points[0] = in_points_0.xy;
//     points[1] = in_points_0.zw;
//     points[2] = in_points_1.xy;
//     points[3] = in_points_1.zw;
//     points[4] = in_points_2.xy;
//     points[5] = in_points_2.zw;
//     points[6] = in_points_3.xy;
//     points[7] = in_points_3.zw;
//     points[8] = in_points_4.xy;
//     points[9] = in_points_4.zw;
//     points[10] = in_points_5.xy;
//     points[11] = in_points_5.zw;
//     points[12] = in_points_6.xy;
//     points[13] = in_points_6.zw;
//     points[14] = in_points_7.xy;
//     points[15] = in_points_7.zw;
//     points[16] = in_points_8.xy;
//     points[17] = in_points_8.zw;

//     outputs.points = in_points;
//     outputs.pointCount = in_count;

    outputs.bounds = in_bounds;

    outputs.lineColor = in_lineColor;
    outputs.fillColor = in_fillColor;

    gl_Position = uniforms.matrix * in_vertex;
}
