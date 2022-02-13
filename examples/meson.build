# examples

# Input: xmlxx_own_dep, build_examples
# Output: -

example_programs = [
# [[dir-name], exe-name, [sources], [arguments]]
  [['dom_build'], 'example', ['main.cc'], []],
  [['dom_parse_entities'], 'example', ['main.cc'], []],
  [['dom_parser'], 'example', ['main.cc'], []],
  [['dom_parser_raw'], 'example', ['main.cc'], []],
  [['dom_read_write'], 'example', ['main.cc'],
    ['example.xml', meson.current_build_dir() / 'dom_read_write_example_output.xml']],
  [['dom_update_namespace'], 'example', ['main.cc'], []],
  [['dom_xinclude'], 'example', ['main.cc'], []],
  [['dom_xpath'], 'example', ['main.cc'], []],
  [['dtdvalidation'], 'example', ['main.cc'], []],
  [['import_node'], 'example', ['main.cc'], []],
  [['sax_exception'], 'example', ['main.cc', 'myparser.cc'], []],
  [['sax_parser'], 'example', ['main.cc', 'myparser.cc'], []],
  [['sax_parser_build_dom'], 'example', ['main.cc', 'svgparser.cc',
    'svgdocument.cc', 'svgelement.cc'], []],
  [['sax_parser_entities'], 'example', ['main.cc', 'myparser.cc'], []],
  [['schemavalidation'], 'example', ['main.cc'], []],
  [['textreader'], 'example', ['main.cc'], []],
]

foreach ex : example_programs
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
    build_by_default: build_examples
  )

  if build_examples
    # Some programs can find their input file(s) only if the current directory,
    # when they are executed, is the program's own source directory.
    # To make these program invocations as consistent as possible, and to avoid
    # having to specify parameters for the programs, the programs are executed
    # from their own source directory.
    #
    # dom_read_write shall write its output file in the build directory.
    # It's necessary to specify parameters when the input file and the output
    # file are located in different directories.

    test(ex_name, exe_file,
      workdir: meson.current_source_dir() / dir,
      args: ex[3],
    )
  endif
endforeach
