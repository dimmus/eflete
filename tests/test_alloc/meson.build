src = files([
    'mem_calloc.c',
    'mem_malloc.c',
    'test_alloc.c',
])

exe = executable('test_alloc', src,
  dependencies : test_deps,
  include_directories : test_include_dirs,
  install : false
)

test('Eflete Test Suit - Memory', exe)