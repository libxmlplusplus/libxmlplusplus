# libxml++

# Input: xmlxx_build_dep, xmlxx_pcname, xmlxx_libversion, xmlxx_api_version,
#        install_includedir, xmlxx_rc, xmlxx_libname
# Output: source_h_files, xmlxx_dep

# There are no built source files in libxml++-4.0.

source_h_files = []
source_cc_files = []

xmlxx_base_h_cc_files = [
  'attribute',
  'attributedeclaration',
  'attributenode',
  'document',
  'dtd',
  'keepblanks',
  'noncopyable',
  'relaxngschema',
  'schemabase',
  'xsdschema',
]

xmlxx_subdir_h_cc_files = [
# [ dir-name, [files]]
  ['exceptions', [
    'exception',
    'parse_error',
    'validity_error',
    'internal_error',
    'wrapped_exception',
  ]],
  ['io', [
    'istreamparserinputbuffer',
    'outputbuffer',
    'ostreamoutputbuffer',
    'parserinputbuffer',
  ]],
  ['nodes', [
    'cdatanode',
    'commentnode',
    'contentnode',
    'element',
    'entitydeclaration',
    'entityreference',
    'node',
    'processinginstructionnode',
    'textnode',
    'xincludeend',
    'xincludestart',
  ]],
  ['parsers', [
    'parser',
    'saxparser',
    'domparser',
    'textreader',
  ]],
  ['validators', [
    'dtdvalidator',
    'relaxngvalidator',
    'schemavalidatorbase',
    'validator',
    'xsdvalidator',
  ]],
]

foreach f : xmlxx_base_h_cc_files
  source_h_files += f + '.h'
  source_cc_files += f + '.cc'
endforeach

install_headers('libxml++.h', subdir: xmlxx_pcname / 'libxml++')
install_headers(source_h_files, subdir: xmlxx_pcname / 'libxml++')

foreach dir_files : xmlxx_subdir_h_cc_files
  dir = dir_files[0]
  subdir_h_files = []
  foreach f : dir_files[1]
    subdir_h_files += dir / f + '.h'
    source_cc_files += dir / f + '.cc'
  endforeach
  source_h_files += subdir_h_files
  install_headers(subdir_h_files, subdir: xmlxx_pcname / 'libxml++' / dir)
endforeach

xmlxx_cpp_args = [ '-DLIBXMLPP_BUILD=1' ]

# Make sure we are exporting the symbols from the DLL
if is_msvc
  xmlxx_cpp_args += ['-D_WINDLL']
endif

extra_xmlxx_objects = []

# Build the .rc file for Windows builds and link to it
if host_machine.system() == 'windows'
  windows = import('windows')
  xmlxx_res = windows.compile_resources(xmlxx_rc)
  extra_xmlxx_objects += xmlxx_res
endif

extra_include_dirs = ['..']
xmlxx_library = library(xmlxx_libname,
  source_cc_files,
  extra_xmlxx_objects,
  version: xmlxx_libversion,
  include_directories: extra_include_dirs,
  cpp_args: xmlxx_cpp_args,
  dependencies: xmlxx_build_dep,
  install: true,
)

# This is useful in the main project when libxml++ is used as a subproject.
# It's also used when building example programs and test programs.
xmlxx_dep = declare_dependency(
  link_with: xmlxx_library,
  include_directories: extra_include_dirs,
  dependencies: xmlxx_build_dep
)