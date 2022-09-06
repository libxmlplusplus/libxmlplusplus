# tests

# Input: xmlxx_own_dep, build_tests
# Output: -

test_programs = [
# [[dir-name], exe-name, [sources]]
  [['istream_ioparser'], 'test', ['main.cc']],
  [['saxparser_chunk_parsing_inconsistent_state'], 'test', ['main.cc']],
  [['saxparser_parse_double_free'], 'test', ['main.cc']],
  [['saxparser_parse_stream_inconsistent_state'], 'test', ['main.cc']],
]

foreach ex : test_programs
  dir = ''
  foreach dir_part : ex[0]
    dir = dir / dir_part
  endforeach
  ex_name = (dir / ex[1]).underscorify()
  ex_sources = []
  foreach src : ex[2]
    ex_sources += dir / src
  endforeach

  exe_file = executable(ex_name, ex_sources,
    dependencies: xmlxx_own_dep,
    implicit_include_directories: false,
    build_by_default: build_tests
  )

  if build_tests
    test(ex_name, exe_file)
  endif
endforeach
