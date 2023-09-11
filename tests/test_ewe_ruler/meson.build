src = files([
    'test_ewe_ruler.c',
    'ewe_ruler_add.c',
    'ewe_ruler_scale_add.c',
    'ewe_ruler_scale_del.c',
    'ewe_ruler_scale_visible_get.c',
    'ewe_ruler_scale_visible_set.c',
    'ewe_ruler_scale_middle_mark_get.c',
    'ewe_ruler_scale_middle_mark_set.c',
    'ewe_ruler_horizontal_get.c',
    'ewe_ruler_horizontal_set.c',
    'ewe_ruler_step_set.c',
    'ewe_ruler_step_get.c',
    'ewe_ruler_value_step_set.c',
    'ewe_ruler_value_step_get.c',
    'ewe_ruler_zero_offset_set.c',
    'ewe_ruler_zero_offset_get.c',
    'ewe_ruler_format_get.c',
    'ewe_ruler_format_set.c',
    'ewe_ruler_style_get.c',
    'ewe_ruler_style_set.c',
    'ewe_ruler_marker_style_get.c',
    'ewe_ruler_marker_style_set.c',
    'ewe_ruler_marker_absolute_get.c',
    'ewe_ruler_marker_absolute_set.c',
    'ewe_ruler_marker_relative_get.c',
    'ewe_ruler_marker_relative_set.c',
    'ewe_ruler_marker_visible_get.c',
    'ewe_ruler_marker_visible_set.c',
    'ewe_ruler_marker_add.c',
    'ewe_ruler_marker_del.c',
])

exe = executable('test_ewe_ruler', src,
  dependencies : test_deps,
  include_directories : test_include_dirs + 
                        [ include_directories(join_paths('..','..','build/src/lib/ewe_ruler')) ],
  install : false
)

test('Eflete Test Suit - EWE', exe)