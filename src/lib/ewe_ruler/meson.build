eo_src = ['ewe_ruler']
eo_gen = []

foreach eo : eo_src
  eo_file = eo + '.eo'
  eo_gen += custom_target('eolian_gen_' + eo_file,
    input : eo_file,
    output : [eo_file + '.h'],
    command : [eolian_gen_exe, '-I', meson.current_source_dir(),
                           '-o', 'i:' + join_paths(meson.current_build_dir(), eo + '.c'),
                           '-o', 'h:' + join_paths(meson.current_build_dir(), eo_file + '.h'),
                           '-o', 'c:' + join_paths(meson.current_build_dir(), eo_file + '.c'),
                           '-o', 'd:' + join_paths(meson.current_build_dir(), eo_file + '.d'),
                           '-gchd', '@INPUT@'])
endforeach

ewe_src += files([
  'ewe_ruler.c'
 ])
