# MSVC_NMake/libxml++

# Input: pkg_conf_data, xmlxxconfig_h
# Output: xmlxx_rc

xmlxx_rc = configure_file(
  input: 'libxml++.rc.in',
  output: '@BASENAME@',
  configuration: pkg_conf_data,
)

# Copy the generated configuration header into the MSVC project directory.
configure_file(
  input: xmlxxconfig_h,
  output: 'libxml++config.h',
  copy: true,
)
