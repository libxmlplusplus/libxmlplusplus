# libxml++

# Input: xmlxx_build_dep, xmlxx_pcname, xmlxx_libversion, xmlxx_api_version,
#        install_includedir, xmlxx_rc, xmlxx_libname, macos_darwin_versions,
#        xmlxx_pc_requires, libxml2_lib_pkgconfig
# Output: source_h_files, xmlxx_own_dep

# There are no built source files in libxml++-5.0.

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
  'ustring',
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
if is_msvc and get_option('default_library') != 'static'
  xmlxx_cpp_args += ['-D_WINDLL']
endif

xmlxx_all_deps = [xmlxx_build_dep]

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
  darwin_versions: macos_darwin_versions,
  implicit_include_directories: false,
  include_directories: extra_include_dirs,
  cpp_args: xmlxx_cpp_args,
  dependencies: xmlxx_build_dep,
  install: true,
)

# Generate .pc files, used by pkg-config.
pkg_config = import('pkgconfig')
pc_common_variables = [
  'doxytagfile=${docdir}/reference/' + xmlxx_pcname + '.tag',
  'htmlrefdir=${docdir}/reference/html',
  'htmlrefpub=https://libxmlplusplus.github.io/libxmlplusplus/reference/html',
]
pc_variables = [
  'exec_prefix=${prefix}',
  'datarootdir=${datadir}',
  'docdir=${datadir}/doc/' + xmlxx_pcname,
] + pc_common_variables
pc_uninstalled_variables = [
  'docdir=${prefix}/docs',
] + pc_common_variables

pkg_config.generate(xmlxx_library,
  filebase: xmlxx_pcname,
  variables: pc_variables,
  uninstalled_variables: pc_uninstalled_variables,
  name: meson.project_name(),
  description: 'C++ wrapper for libxml2',
  url: 'https://libxmlplusplus.github.io/libxmlplusplus/',
  requires: xmlxx_pc_requires,
  libraries: libxml2_lib_pkgconfig,
  subdirs: [xmlxx_pcname],
  extra_cflags: ['-I${libdir}/' + xmlxx_pcname + '/include'],
)

# This is used when building example programs and test programs.
# It's also a part of xmlxx_dep, when libxml++ is a subproject.
xmlxx_own_dep = declare_dependency(
  link_with: xmlxx_library,
  include_directories: extra_include_dirs,
  dependencies: xmlxx_all_deps
)
