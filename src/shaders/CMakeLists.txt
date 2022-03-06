
file(GLOB _vertex_shaders CONFIGURE_DEPENDS "*.vert")
file(GLOB _fragment_shaders CONFIGURE_DEPENDS "*.frag")

set(_generated_shaders "")
set(_shader_outputs "")

foreach(_shader ${_vertex_shaders})
    get_filename_component(_shader_filename ${_shader} NAME)
    list(APPEND _generated_shaders ${_shader})
    list(APPEND _shader_outputs "${_shader_filename}.qsb")
endforeach()

file(READ sdf.glsl _sdf_contents)

foreach(_shader ${_fragment_shaders})
    get_filename_component(_shader_filename ${_shader} NAME)
    set(_shader_output "${CMAKE_CURRENT_BINARY_DIR}/${_shader_filename}")

    file(READ ${_shader} _shader_contents)

    file(WRITE "${_shader_output}" "${_sdf_contents}")
    file(APPEND "${_shader_output}" "${_shader_contents}")
    list(APPEND _generated_shaders "${_shader_output}")
    list(APPEND _shader_outputs "${_shader_filename}.qsb")
endforeach()

qt_add_shaders(QuickChartsStatic "QuickChartsShaders"
    BATCHABLE
    GLSL "300es,330"
    PREFIX "/org.kde.quickcharts"
    FILES ${_generated_shaders}
    OUTPUTS ${_shader_outputs}
)
