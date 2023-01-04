# MSVC_NMake/libxml++

# Input: pkg_conf_data, xmlxxconfig_h_meson
# Output: xmlxx_rc

xmlxx_rc = configure_file(
  input: 'libxml++.rc.in',
  output: '@BASENAME@',
  configuration: pkg_conf_data,
)

# Make a copy of the generated configuration header in the MSVC project directory.
configure_file(
  input: xmlxxconfig_h_meson,
  output: 'libxml++config.h',
  configuration: pkg_conf_data,
)
