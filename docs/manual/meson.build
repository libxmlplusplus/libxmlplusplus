# docs/manual

# input: install_datadir, xmlxx_pcname, tutorial_custom_cmd_py, python3,
#        build_documentation, book_name, can_add_dist_script
# output: can_parse_and_validate, build_pdf_by_default, can_build_pdf,
#         install_tutorialdir

# xsltproc is required by tutorial_custom_cmd_py html.
xsltproc = find_program('xsltproc', required: build_documentation)
xmllint = find_program('xmllint', required: false)

can_parse_and_validate = xmllint.found()

validate = get_option('validation') ? 'true' : 'false'

dblatex = find_program('dblatex', required: false)
can_build_pdf = dblatex.found() or (xmllint.found() and \
                find_program('docbook2pdf', required: false).found())
build_pdf_by_default = get_option('build-pdf')

# Installation directories are relative to {prefix}.
install_tutorialdir = install_datadir / 'doc' / book_name / 'manual'

if not build_documentation
  # Documentation shall not be built or installed.
  # Return to the calling meson.build file.
  subdir_done()
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
    meson.current_source_dir() / 'docbook-customisation.xsl', # stylesheet
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
  # Prefer dblatex, if both dblatex and docbook2pdf are available.
  custom_target('manual_pdf',
    input: xml_manual_docbook,
    output: 'libxml++.pdf',
    command: [
      python3, tutorial_custom_cmd_py,
      dblatex.found() ? 'dblatex' : 'docbook2pdf',
      '@INPUT@',
      '@OUTPUT@'
    ],
    build_by_default: build_pdf_by_default,
  )
endif

if can_add_dist_script
  # Distribute built files.
  meson.add_dist_script(
    python3.path(), tutorial_custom_cmd_py, 'dist_doc',
    doc_dist_dir,
    meson.current_build_dir(),
    meson.current_build_dir() / 'libxml++.xml',
    meson.current_build_dir() / 'libxml++.pdf',
  )
endif
