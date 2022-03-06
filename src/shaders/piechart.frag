/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

// This requires "sdf.frag" which is included through SDFShader.

// The maximum number of segments we can support for a single pie.
// This is based on OpenGL's MAX_FRAGMENT_UNIFORM_COMPONENTS.
// MAX_FRAGMENT_UNIFORM_COMPONENTS is required to be at least 1024.
// Assuming a segment of size 1, each segment needs
// 2 (size of a vec2) * 2 (number of points) + 4 (size of vec4) + 1 (segment size)
// components. We also need to leave some room for the other uniforms.
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

layout (location = 0) in mediump vec2 uv;

layout (location = 0) out lowp vec4 out_color;

const highp vec2 origin = vec2(0.0, 0.0);
const highp float lineSmooth = 0.001;

highp float rounded_segment(highp float from, highp float to, highp float inner, highp float outer, highp float rounding)
{
    return sdf_torus_segment(uv, from + rounding, to - rounding, inner + rounding, outer - rounding) - rounding;
}

void main()
{
    lowp vec4 color = vec4(0.0);

    highp float thickness = (uniforms.outerRadius - uniforms.innerRadius) / 2.0;
    highp float rounding = uniforms.smoothEnds > 0 ? thickness : 0.0;

    // Background first, slightly smaller than the actual pie to avoid antialiasing artifacts.
    highp float background_rounding = (uniforms.toAngle - uniforms.fromAngle) >= 2.0 * pi ? 0.001 : rounding + 0.001;
    highp float background = rounded_segment(uniforms.fromAngle, uniforms.toAngle, uniforms.innerRadius, uniforms.outerRadius, background_rounding);
    color = sdf_render(background, color, uniforms.backgroundColor);

    for (int i = 0; i < uniforms.segmentCount && i < MaxSegments; ++i) {
        highp vec2 segment = uniforms.segments[i];

        highp float segment_sdf = rounded_segment(segment.x, segment.y, uniforms.innerRadius, uniforms.outerRadius, rounding);
        color = sdf_render(segment_sdf, color, uniforms.colors[i]);
    }

    out_color = color * uniforms.opacity;
}
