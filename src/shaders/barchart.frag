/*
 * SPDX-FileCopyrightText: 2021 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

// This requires "sdf.frag" which is included through SDFShader.

layout (binding = 0) uniform UniformBuffer
{
    highp mat4 matrix;
    mediump vec2 aspect;
    mediump float opacity;
    highp float radius;
    lowp vec4 backgroundColor;
} uniforms;

layout (location = 0) in PerVertex
{
    highp vec2 uv;
    lowp vec4 foregroundColor;
    highp float value;
} inputs;

layout (location = 0) out mediump vec4 out_color;

void main()
{
    mediump vec4 color = vec4(0.0);

    highp float background = sdf_round(sdf_rectangle(inputs.uv, vec2(1.0, uniforms.aspect.y) - uniforms.radius), uniforms.radius);

    color = sdf_render(background, color, uniforms.backgroundColor);

    highp float y = -uniforms.aspect.y + inputs.uv.y + inputs.value;
    highp float foreground = sdf_rectangle(vec2(inputs.uv.x, y), vec2(1.0, inputs.value) - uniforms.radius);
    foreground = sdf_round(foreground, uniforms.radius);

    color = sdf_render(foreground, color, inputs.foregroundColor);

    out_color = color * uniforms.opacity;
}
