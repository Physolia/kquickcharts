/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

// This requires "sdf.frag" which is included through SDFShader.

#line 10

layout (constant_id = 0) const int MaxPointCount = 18;

layout (std140, binding = 0) uniform UniformBuffer
{
    highp mat4 matrix;
    lowp float opacity;
    highp float lineWidth;
    highp float smoothing;
    highp float aspect;
} uniforms;

layout (location = 0) in Inputs
{
    highp vec2 uv;

    highp vec2 bounds;

    mediump vec4 lineColor;
    mediump vec4 fillColor;

    highp int pointCount;
    highp vec2 points[MaxPointCount];
} inputs;

layout (location = 0) out mediump vec4 out_color;

// uniform lowp float opacity; // inherited opacity of this item
// uniform lowp float lineWidth;
// uniform lowp float smoothing;

// #define MAXIMUM_POINT_COUNT 18

// #ifdef LEGACY_STAGE_INOUT
// varying mediump vec2 uv;
// varying mediump vec2 points[18];
// varying highp float pointCount;
// varying mediump vec2 bounds;
// varying mediump vec4 lineColor;
// varying mediump vec4 fillColor;
// #define out_color gl_FragColor
// #else
// in mediump vec2 uv;
// in mediump vec2 points[MAXIMUM_POINT_COUNT];
// in highp float pointCount;
// in mediump vec2 bounds;
// in mediump vec4 lineColor;
// in mediump vec4 fillColor;
// out lowp vec4 out_color;
// #endif

// ES2 does not support array function arguments. So instead we need to
// reference the uniform array directly. So this copies the implementation of
// sdf_polygon from sdf.glsl, changing it to refer to the points array directly.
// For simplicity, we use the same function also for other APIs.
// lowp float sdf_polygon(in lowp vec2 point, in int count)
// {
//     lowp float d = dot(point - points[0], point - points[0]);
//     lowp float s = 1.0;
//     for (int i = 0, j = count - 1; i < count && i < MAXIMUM_POINT_COUNT; j = i, i++)
//     {
//         lowp vec2 e = points[j] - points[i];
//         lowp vec2 w = point - points[i];
//         lowp float h = clamp( dot(w, e) / dot(e, e), 0.0, 1.0 );
//         lowp vec2 b = w - e * h;
//         d = min(d, dot(b, b));
//
//         bvec3 c = bvec3(point.y >= points[i].y, point.y < points[j].y, e.x * w.y > e.y * w.x);
//         if(all(c) || all(not(c))) s *= -1.0;
//     }
//     return s * sqrt(d);
// }

void main()
{
    highp vec2 point = inputs.uv;

    mediump vec4 color = vec4(0.0, 0.0, 0.0, 0.0);

    highp float bounds_range = max(0.01, uniforms.lineWidth);

    // bounds.y contains the line segment's maximum value. If we are a bit above
    // that, we will never render anything, so just discard the pixel.
    if (point.y > inputs.bounds.y + bounds_range) {
        discard;
    }

    // bounds.x contains the line segment's minimum value. If we are a bit below
    // that, we know we will always be inside the polygon described by points.
    // So just return a pixel with fillColor.
    if (point.y < inputs.bounds.x - bounds_range) {
        out_color = inputs.fillColor * uniforms.opacity;
        return;
    }

    highp vec2 points[SDF_POLYGON_MAX_POINT_COUNT];
    for (int i = 0; i < inputs.pointCount; ++i) {
        points[i] = inputs.points[i];
    }

    highp float polygon = sdf_polygon(point, points, inputs.pointCount);

    color = sdf_render(polygon, color, inputs.fillColor);

    if (uniforms.lineWidth > 0.0) {
        color = mix(color, inputs.lineColor, 1.0 - smoothstep(-uniforms.smoothing, uniforms.smoothing, sdf_annular(polygon, uniforms.lineWidth)));
    }

    out_color = color * uniforms.opacity;
}
