# This file is part of libxml++.

project('libxml++', 'cpp',
  version: '5.2.0',
  license: 'LGPLv2.1+',
  default_options: [
    'warning_level=1',
    'cpp_std=c++17',
  ],
  meson_version: '>= 0.60.0', # required for dependency('iconv')
)

xmlxx_api_version = '5.0'
xmlxx_pcname = meson.project_name() + '-' + xmlxx_api_version

xmlxx_version_array = meson.project_version().split('.')
xmlxx_major_version = xmlxx_version_array[0].to_int()
xmlxx_minor_version = xmlxx_version_array[1].to_int()
xmlxx_micro_version = xmlxx_version_array[2].to_int()

# http://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html
# The relation between libtool's current:revison:age interface versioning
# and the .so filename, .so.x.y.z, is
# x = current - age
# y = age
# z = revision
# If libtool_soversion is updated as described in libtool's documentation,
# x.y.z will usually *not* be equal to meson.project_version().
libtool_soversion = [1, 0, 0]
xmlxx_libversion = '@0@.@1@.@2@'.format(
  libtool_soversion[0] - libtool_soversion[2],
  libtool_soversion[2],
  libtool_soversion[1]
)
macos_darwin_versions = [
  libtool_soversion[0] + 1,
  '@0@.@1@'.format(libtool_soversion[0] + 1, libtool_soversion[1])
]

# Source and build root directories of the current (sub)project.
project_source_root = meson.project_source_root()
project_build_root = meson.project_build_root()

cpp_compiler = meson.get_compiler('cpp')
is_msvc = cpp_compiler.get_id() == 'msvc'

python3 = find_program('python3', version: '>=3.5')

# Do we build from a git repository?
# Suppose we do if and only if the meson.build file is tracked by git.
cmd_py = '''
import shutil, subprocess, sys
git_exe = shutil.which('git')
if not git_exe:
  sys.exit(1)
cmd = [ git_exe, 'ls-files', '--error-unmatch', 'meson.build' ]
sys.exit(subprocess.run(cmd, cwd=sys.argv[1], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL).returncode)
'''
is_git_build = run_command(
  python3, '-c', cmd_py,
  project_source_root,
  check: false,
).returncode() == 0

# Are we testing a dist tarball while it's being built?
# There ought to be a better way. https://github.com/mesonbuild/meson/issues/6866
is_dist_check = project_source_root.contains('dist-unpack') and \
                project_build_root.contains('dist-build')

# Options.
maintainer_mode_opt = get_option('maintainer-mode')
maintainer_mode = maintainer_mode_opt == 'true' or \
                 (maintainer_mode_opt == 'if-git-build' and is_git_build)
if is_dist_check
  message('Looks like a tarball is being tested. ' + \
    'Option "dist-warnings" is used instead of "warnings".')
  cpp_warnings = get_option('dist-warnings')
else
  cpp_warnings = get_option('warnings')
endif
if get_option('warning_level') in ['0','1','2','3','4','5']
  warning_level = get_option('warning_level').to_int()
else
  # For instance get_option('warning_level') == 'everything'
  warning_level = 99
endif
werror = get_option('werror')
build_deprecated_api = get_option('build-deprecated-api')
build_documentation_opt = get_option('build-documentation')
build_documentation = build_documentation_opt == 'true' or \
                     (build_documentation_opt == 'if-maintainer-mode' and maintainer_mode)
build_examples = get_option('build-examples')
build_tests = get_option('build-tests')
use_msvc14x_toolset_ver = get_option('msvc14x-parallel-installable')

# Installation directories are relative to {prefix}.
install_prefix = get_option('prefix')
install_includedir = get_option('includedir')
install_libdir = get_option('libdir')
install_datadir = get_option('datadir')
install_pkgconfigdir = install_libdir / 'pkgconfig'

# Dependencies.
# xmlxx_build_dep: Dependencies when building the libxml++ library.
# xmlxx_dep (created in libxml++/meson.build):
#   Dependencies when using the libxml++ library.

xml2_min_ver = '2.7.7'
xml2_req = '>= @0@'.format(xml2_min_ver)
xml2_dep = dependency(
  ['libxml-2.0', 'LibXml2'],
  version: xml2_req,
  required: host_machine.system() != 'windows'
)

# Setup CMake subproject for use, if needed
if not xml2_dep.found()
  cmake = import('cmake')
  opt_var = cmake.subproject_options()
  build_shared = get_option('default_library') != 'static'
  iconv_dep = dependency('iconv', required: false)
  icu_i18n_dep = dependency('icu-i18n', 'ICU',
                              components: 'in',
                            required: false)
  icu_uc_dep = dependency('icu-uc',  'ICU',
                            components: 'uc',
                          required: false)
  lzma_dep = dependency(['liblzma','LibLZMA'], required: false)
  thread_dep = dependency('threads', required: false)
  zlib_dep = dependency('zlib', required: false)
  winsock_dep = cpp_compiler.find_library('ws2_32', required: false)
  bcrypt_dep = cpp_compiler.find_library('bcrypt', required: false)
  cmake_build_type = get_option('buildtype')
  if get_option('buildtype') == 'debugoptimized'
    cmake_build_type = 'RelWithDebInfo'
  elif get_option('buildtype') == 'minsize'
    cmake_build_type = 'MinSizeRel'
  elif get_option('buildtype') == 'plain'
    cmake_build_type = ''
  endif
  opt_var.add_cmake_defines({
    'BUILD_SHARED_LIBS': build_shared,
    'CMAKE_BUILD_TYPE': cmake_build_type,
    'CMAKE_MSVC_RUNTIME_LIBRARY': '',
    'CMAKE_C_FLAGS_INIT': '',
    'CMAKE_C_FLAGS_DEBUG': '',
    'CMAKE_C_FLAGS_RELEASE': '',
    'CMAKE_C_FLAGS_RELWITHDEBINFO': '',
    'CMAKE_C_FLAGS_MINSIZEREL': '',
    'LIBXML2_WITH_HTTP': host_machine.system() != 'windows' or winsock_dep.found(),
    'LIBXML2_WITH_ICONV': iconv_dep.found(),
    'LIBXML2_WITH_ICU': icu_i18n_dep.found() and icu_uc_dep.found(),
    'LIBXML2_WITH_LZMA': lzma_dep.found(),
    'LIBXML2_WITH_PYTHON': false,
    'LIBXML2_WITH_TESTS': build_tests,
    'LIBXML2_WITH_THREADS': thread_dep.found(),
    'LIBXML2_WITH_ZLIB': zlib_dep.found(),
  })
  xml2_sp = cmake.subproject('libxml2_cmake', options: opt_var)
  xml2_dep = xml2_sp.dependency('LibXml2')
endif

xml2_is_subproject = xml2_dep.type_name() == 'internal'

if xml2_is_subproject and build_tests
  test('testchar', xml2_sp.target('testchar'))
  test('testdict', xml2_sp.target('testdict'))
endif

xmlxx_requires = []
libxml2_lib_pkgconfig = ''

# Put libxml-2.0 in the 'Requires:' section in the generated pkg-config file if
# we found it by pkg-config
if xml2_dep.type_name() == 'pkgconfig'
  xmlxx_requires += ['libxml-2.0', xml2_req]
else
  libxml2_lib_pkgconfig = xml2_dep.get_variable(
    cmake:         'LIBXML2_LIBRARIES',
    default_value: 'LibXml2.lib',
  )
endif

xmlxx_requires = ' '.join(xmlxx_requires)

# Make sure we link to libxml-2.0
xmlxx_build_dep = [xml2_dep]

# Some dependencies are required only in maintainer mode and/or if
# reference documentation shall be built.
mm_common_get = find_program('mm-common-get', required: false)
if maintainer_mode and not mm_common_get.found()
  message('Maintainer mode requires the \'mm-common-get\' command. If it is not found,\n' +
          'use \'-Dmaintainer-mode=false\' or install the \'mm-common\' package, version 1.0.0 or higher.')
  # If meson --wrap-mode != forcefallback, Meson falls back to the mm-common
  # subproject only if mm-common-get is required.
  mm_common_get = find_program('mm-common-get', required: true)
endif
doxygen = find_program('doxygen', required: build_documentation)
dot = find_program('dot', required: false) # Used by Doxygen, if found
xsltproc = find_program('xsltproc', required: build_documentation)

if build_documentation and not dot.found()
  message('The \'dot\' command is not found.\n  ' + \
          'This will affect the look of the inheritance diagrams in the documentation.')
endif

# Script files copied to 'untracked/' by mm-common-get.
script_dir = project_source_root / 'untracked' / 'build_scripts'
doc_reference_py = script_dir / 'doc-reference.py'
dist_changelog_py = script_dir / 'dist-changelog.py'
dist_build_scripts_py = script_dir / 'dist-build-scripts.py'

# xmlxx's own script files.
xmlxx_script_dir = project_source_root / 'tools' / 'build_scripts'
tutorial_custom_cmd_py = xmlxx_script_dir / 'tutorial-custom-cmd.py'

if maintainer_mode
  # Copy files to untracked/build_scripts and untracked/docs.
  run_command(mm_common_get, '--force', script_dir,
    project_source_root / 'untracked' / 'docs',
    check: true,
  )
elif not import('fs').is_file(doc_reference_py)
  warning('Missing files in untracked/.\n  ' + \
  'Enable maintainer-mode if you want to build documentation or create a dist tarball.')
endif

# Check if perl is required and available.
doc_perl_prop = run_command(
  python3, doc_reference_py, 'get_script_property',
  '', # MMDOCTOOLDIR is not used
  'requires_perl',
  check: false,
)
if not (doc_perl_prop.returncode() == 0 and doc_perl_prop.stdout() == 'false')
  # Perl is required, if documentation shall be built.
  perl = find_program('perl', required: build_documentation)
endif

# Add toolset version in builds done with Visual Studio 2017 or later
msvc14x_toolset_ver = ''

if is_msvc
  if use_msvc14x_toolset_ver
    if cpp_compiler.version().version_compare('>=19.30')
      msvc14x_toolset_ver = '-vc143'
    elif cpp_compiler.version().version_compare('>=19.20')
      msvc14x_toolset_ver = '-vc142'
    elif cpp_compiler.version().version_compare('>=19.10')
      msvc14x_toolset_ver = '-vc141'
    endif
  endif
endif

xmlxx_libname = 'xml++' + msvc14x_toolset_ver + '-' + xmlxx_api_version

# Set compiler warnings.
# Meson warns if any of the /W1, /W2, /W3, /W4, /Wall, -Wall, -Wextra, -Werror
# compiler options are added with add_project_arguments().
# Avoid such warnings, when possible.
# See _warn_about_builtin_args() in meson/mesonbuild/interpreter/interpreter.py.
warning_flags = []
if cpp_warnings == 'min'
  if warning_level == 0
    if is_msvc
      warning_flags = ['/W2']
    else
      warning_flags = ['-Wall']
    endif
  endif
elif cpp_warnings == 'max' or cpp_warnings == 'fatal'
  if warning_level < 3
    if is_msvc
      warning_flags = ['/W4']
    else
      warning_flags = ['-pedantic', '-Wall', '-Wextra']
    endif
  endif
  if not is_msvc
    warning_flags += '-Wformat-security -Wsuggest-override -Wshadow -Wno-long-long'.split()
  endif
  if cpp_warnings == 'fatal' and not werror
    if is_msvc
      warning_flags += ['/WX']
    else
      warning_flags += ['-Werror']
    endif
  endif
endif

warning_flags = cpp_compiler.get_supported_arguments(warning_flags)
add_project_arguments(warning_flags, language: 'cpp')

# MSVC: Ignore warnings that aren't really harmful, but make those
#       that should not be overlooked stand out.
if is_msvc
  disable_warnings_list = [
    '/utf-8', # Avoid C4819 unicode conversion warnings when building on CJK locales
    '/wd4706', # assignment within conditional expression
  ]

  # Turn off harmless warnings but make potentially dangerous ones glaring,
  # distributed with GLib, if available
  use_recommended_pragmas =  cpp_compiler.get_supported_arguments('/FImsvc_recommended_pragmas.h')
  if use_recommended_pragmas.length() > 0
    add_project_arguments(use_recommended_pragmas, language: 'cpp')
  else
    disable_warnings_list += [
      '/wd4244', # 'conversion' conversion from 'type1' to 'type2', possible loss of data
      '/wd4101', # unreferenced local variable
    ]
  endif

  if host_machine.cpu_family() == 'x86_64' or host_machine.cpu_family() == 'aarch64'
    # 'var' : conversion from 'size_t' to 'type', possible loss of data (applies on 64-bit builds)
    disable_warnings_list += '/wd4267'
  endif
  add_project_arguments(
    cpp_compiler.get_supported_arguments(disable_warnings_list),
    language: 'cpp'
  )
endif

# Configure files
pkg_conf_data = configuration_data()
pkg_conf_data.set('prefix', install_prefix)
pkg_conf_data.set('exec_prefix', '${prefix}')
pkg_conf_data.set('libdir', '${exec_prefix}' / install_libdir)
pkg_conf_data.set('datarootdir', '${prefix}' / install_datadir)
pkg_conf_data.set('datadir', '${datarootdir}')
pkg_conf_data.set('includedir', '${prefix}' / install_includedir)
pkg_conf_data.set('PACKAGE_NAME', meson.project_name()) # MSVC_NMake/libxml++/libxml++.rc
pkg_conf_data.set('PACKAGE_TARNAME', meson.project_name())
pkg_conf_data.set('PACKAGE_VERSION', meson.project_version())
pkg_conf_data.set('LIBXMLXX_MODULE_NAME', xmlxx_pcname)
pkg_conf_data.set('LIBXMLXX_API_VERSION', xmlxx_api_version)
pkg_conf_data.set('LIBXMLXX_MODULES', xmlxx_requires)
pkg_conf_data.set('LIBXML2_LIB_NO_PKGCONFIG', libxml2_lib_pkgconfig)
pkg_conf_data.set('MSVC_TOOLSET_VER', msvc14x_toolset_ver)

if not build_deprecated_api
  pkg_conf_data.set('LIBXMLXX_DISABLE_DEPRECATED', 1)
endif
pkg_conf_data.set('LIBXMLXX_MAJOR_VERSION', xmlxx_major_version)
pkg_conf_data.set('LIBXMLXX_MINOR_VERSION', xmlxx_minor_version)
pkg_conf_data.set('LIBXMLXX_MICRO_VERSION', xmlxx_micro_version)

# Configuration test
if cpp_compiler.compiles(files('tools' / 'conf_tests' / 'have_exception_ptr.cc'))
  pkg_conf_data.set('LIBXMLXX_HAVE_EXCEPTION_PTR', 1)
endif

# Static library?
library_build_type = get_option('default_library')
pkg_conf_data.set('LIBXMLXX_STATIC', library_build_type == 'static')


if cpp_compiler.get_argument_syntax() == 'msvc'
  if library_build_type == 'both'
    error('Dynamic+Static builds are not supported by MSVC-style builds')
  endif
endif

configure_file(
  input: 'libxml++.pc.in',
  output: xmlxx_pcname + '.pc',
  configuration: pkg_conf_data,
  install_dir: install_pkgconfigdir,
)

xmlxxconfig_h_meson = files('libxml++config.h.meson')
install_includeconfigdir = install_libdir / xmlxx_pcname / 'include'
configure_file(
  input: xmlxxconfig_h_meson,
  output: 'libxml++config.h',
  configuration: pkg_conf_data,
  install_dir: install_includeconfigdir,
)

# add_dist_script() is not allowed in a subproject if meson.version() < 0.58.0.
can_add_dist_script = not meson.is_subproject() or meson.version().version_compare('>= 0.58.0')

subdir('MSVC_NMake/libxml++')
subdir('libxml++')
subdir('examples')
subdir('tests')
subdir('docs/reference')
subdir('docs/manual')

if can_add_dist_script
  # Add a ChangeLog file to the distribution directory.
  meson.add_dist_script(
    python3, dist_changelog_py,
    project_source_root,
  )

  # Don't distribute these files and directories.
  dont_distribute = [
    '.github',
  ]
  # Add build scripts to the distribution directory, and delete .gitignore
  # files and an empty $MESON_PROJECT_DIST_ROOT/build/ directory.
  meson.add_dist_script(
    python3, dist_build_scripts_py,
    project_source_root,
    'untracked' / 'build_scripts',
    dont_distribute,
  )
endif

if meson.is_subproject()
  pkgconfig_vars = {
    'htmlrefdir': install_prefix / install_docdir / 'reference' / 'html',
    'htmlrefpub': 'https://libxmlplusplus.github.io/libxmlplusplus/reference/html/'
  }
  if build_documentation
    pkgconfig_vars += {'doxytagfile': tag_file.full_path()}
    # May be used in a main project.
    global_tag_file_target = tag_file
  endif
  xmlxx_dep = declare_dependency(
    dependencies: xmlxx_own_dep,
    variables: pkgconfig_vars,
  )

  # A main project that looks for xmlxx_pcname.pc shall find xmlxx_dep.
  meson.override_dependency(xmlxx_pcname, xmlxx_dep)
endif

# Print a summary.
real_maintainer_mode = ''
if maintainer_mode_opt == 'if-git-build'
  real_maintainer_mode = ' (@0@)'.format(maintainer_mode)
endif

real_build_documentation = ''
if build_documentation_opt == 'if-maintainer-mode'
  real_build_documentation = ' (@0@)'.format(build_documentation)
endif

explain_man = ''
if build_manual_opt and not build_manual
  explain_man = ' (requires that documentation is built)'
endif

validate = get_option('validation') and can_parse_and_validate and build_manual
explain_val = ''
if get_option('validation') and not validate
  if not build_manual
    explain_val = ' (requires that the tutorial is built)'
  else
    explain_val = ' (requires xmllint with Relax NG and DocBook V5.0 support)'
  endif
endif

build_pdf = build_pdf_by_default and can_build_pdf and build_manual
explain_pdf = ''
if build_pdf_by_default and not build_pdf
  if not build_manual
    explain_pdf = ' (requires that the tutorial is built)'
  else
    explain_pdf = ' (requires dblatex or (xsltproc and fop))'
  endif
endif

summary = [
  '',
  '------',
  meson.project_name() + ' ' + meson.project_version(),
  '',
  '         Maintainer mode: @0@@1@'.format(maintainer_mode_opt, real_maintainer_mode),
  '       Compiler warnings: @0@ (warning_level: @1@, werror: @2@)'. \
                             format(cpp_warnings, warning_level, werror),
  '    Build deprecated API: @0@'.format(build_deprecated_api),
  'Build HTML documentation: @0@@1@'.format(build_documentation_opt, real_build_documentation),
  '          Build tutorial: @0@@1@'.format(build_manual, explain_man),
  '          XML validation: @0@@1@'.format(validate, explain_val),
  '               Build PDF: @0@@1@'.format(build_pdf, explain_pdf),
  '  Build example programs: @0@'.format(build_examples),
  '     Build test programs: @0@'.format(build_tests),
  'Directories:',
  '                  prefix: @0@'.format(install_prefix),
  '              includedir: @0@'.format(install_prefix / install_includedir),
  '         includexmlxxdir: @0@'.format(install_prefix / install_includedir / xmlxx_pcname),
  '                  libdir: @0@'.format(install_prefix / install_libdir),
  '        includeconfigdir: @0@'.format(install_prefix / install_includeconfigdir),
  '            pkgconfigdir: @0@'.format(install_prefix / install_pkgconfigdir),
  '                 datadir: @0@'.format(install_prefix / install_datadir),
  '                  docdir: @0@'.format(install_prefix / install_docdir),
  '              devhelpdir: @0@'.format(install_prefix / install_devhelpdir),
  '             tutorialdir: @0@'.format(install_prefix / install_tutorialdir),
  '------',
]

message('\n'.join(summary))
