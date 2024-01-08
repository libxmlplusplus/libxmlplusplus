# docs/manual

# input: install_datadir, xmlxx_pcname, tutorial_custom_cmd_py, python3,
#        build_documentation, book_name, can_add_dist_script, xsltproc
# output: can_parse_and_validate, build_pdf_by_default, can_build_pdf,
#         install_tutorialdir, build_manual_opt, build_manual

build_manual_opt = get_option('build-manual')
build_manual = build_manual_opt and build_documentation

xmllint = find_program('xmllint', required: false)
can_parse_and_validate = xmllint.found()
validate = get_option('validation') ? 'true' : 'false'

dblatex = find_program('dblatex', required: false)
can_build_pdf = dblatex.found() or (xsltproc.found() and \
                find_program('fop', required: false).found())
build_pdf_by_default = get_option('build-pdf')

# Installation directories are relative to {prefix}.
install_tutorialdir = install_datadir / 'doc' / book_name / 'manual'

if not build_manual
  # The manual shall not be built or installed.
  # Return to the calling meson.build file.
  subdir_done()
endif

# Check if xmllint can be used.
if xmllint.found()
  can_parse_and_validate = run_command(
    python3, tutorial_custom_cmd_py, 'xmllint',
    validate,
    meson.current_source_dir() / 'can_use_xmllint.xml',
    meson.current_build_dir() / 'can_use_xmllint.stamp',
    check: false,
  ).returncode() == 0
  if not can_parse_and_validate
    # The DocBook V5.0 package is called docbook5-xml in Ubuntu,
    # docbook5-schemas in Fedora. It may have other names in other distros.
    warning('Can\'t validate XML file.\n' +
      'xmllint does not support Relax NG schemas and DocBook V5.0.\n' +
      'DocBook V5.0 support may require docbook5-xml, docbook5-schemas or a similar package.'
    )
  endif
endif

doc_dist_dir = 'untracked' / 'docs' / 'manual' # Relative to MESON_DIST_ROOT

# Create a DocBook XML file with the examples' source code included.
xml_manual_docbook = custom_target('libxml++.xml',
  input: 'libxml++_without_code.xml',
  output: 'libxml++.xml',
  command: [
    python3, tutorial_custom_cmd_py, 'insert_example_code',
    meson.current_source_dir(),
    project_source_root / 'examples',
    '@INPUT@',
    '@OUTPUT@',
  ],
  build_by_default: true
)

# Create an html version of the DocBook.
custom_target('manual_html',
  input: xml_manual_docbook,
  output: 'html',
  command: [
    python3, tutorial_custom_cmd_py, 'html',
    '@INPUT@',
    '@OUTPUT@',
  ],
  build_by_default: true,
  install: true,
  install_dir: install_tutorialdir
)

if can_parse_and_validate
  # Parse and possibly validate the DocBook.
  custom_target('manual_xmllint',
    input: xml_manual_docbook,
    output: 'manual_xmllint.stamp',
    command: [
      python3, tutorial_custom_cmd_py, 'xmllint',
      validate,
      '@INPUT@',
      '@OUTPUT@'
    ],
    build_by_default: true,
  )
endif

if can_build_pdf
  # Create a PDF file of the DocBook.
  # Prefer dblatex, if both dblatex and fop are available.
  custom_target('manual_pdf',
    input: xml_manual_docbook,
    output: 'libxml++.pdf',
    command: [
      python3, tutorial_custom_cmd_py,
      dblatex.found() ? 'dblatex' : 'fop',
      '@INPUT@',
      '@OUTPUT@'
    ],
    build_by_default: build_pdf_by_default,
  )
endif

if can_add_dist_script
  # Distribute built files.
  meson.add_dist_script(
    python3, tutorial_custom_cmd_py, 'dist_doc',
    doc_dist_dir,
    meson.current_build_dir(),
    meson.current_build_dir() / 'libxml++.xml',
    meson.current_build_dir() / 'libxml++.pdf',
  )
endif
