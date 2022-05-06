#!/usr/bin/env python3

#                             argv[1]            argv[2:-1]           argv[-1]
# insert_example_code.py <examples_base_dir> <input_xml_files>... <output_xml_file>

import os
import sys
import re
import glob
import shutil

# Where to insert example code.
source_include_pattern = re.compile(
  r'\s*<para><link xlink:href="&url_examples_base;([/\w]+)">Source Code</link></para>')

# First line not part of leading comment in a source code file.
# The comment typically consists of copyright and license text.
start_of_source_pattern = re.compile(r'[#\w]')

def process_source_file(source_directory, source_basename, outfile):
  source_filename = os.path.join(source_directory, source_basename)
  with open(source_filename, mode='r', encoding='utf-8', errors='surrogateescape') as srcfile:
    outfile.write('<para>File: <filename>' + source_basename + '</filename></para>\n')
    outfile.write('<programlisting>\n<![CDATA[')

    found_start = False
    for line in srcfile:
      if not found_start:
        # Skip leading comment.
        if not start_of_source_pattern.match(line):
          continue
        found_start = True
      outfile.write(line)

    outfile.write(']]></programlisting>\n')

def insert_example_code(examples_base_dir, input_xml_files, output_xml_file):
  if not isinstance(input_xml_files, list):
    input_xml_files = [input_xml_files]

  # Assume that all files are UTF-8 encoded.
  # If illegal UTF-8 bytes in the range 0x80..0xff are encountered, they are
  # replaced by Unicode Private Use characters in the range 0xdc80..0xdcff
  # and restored to their original values when the file is rewritten.
  with open(output_xml_file, mode='w', encoding='utf-8', errors='surrogateescape') as outfile:
    for input_xml_file in input_xml_files:
      with open(input_xml_file, mode='r', encoding='utf-8', errors='surrogateescape') as infile:
        for line in infile:
          # Print the line.
          outfile.write(line)

          # Look for
          # <para><link xlink:href="&url_examples_base;helloworld">Source Code</link></para>
          source_include_match = source_include_pattern.match(line)
          if source_include_match:
            # List all the source files in the examples directory.
            source_directory = os.path.join(examples_base_dir, source_include_match.group(1))
            outfile.write('<!-- start inserted example code -->\n')
            for source_filename in sorted(glob.glob(os.path.join(source_directory, '*.h'))) + \
                                   sorted(glob.glob(os.path.join(source_directory, '*.cc'))):
              source_basename = os.path.basename(source_filename)
              process_source_file(source_directory, source_basename, outfile)
            outfile.write('<!-- end inserted example code -->\n')
  return 0

# ----- Main -----
if __name__ == '__main__':
  if len(sys.argv) < 4:
    print('Usage: ' + sys.argv[0] + ' <examples_base_dir> <input_xml_files>... <output_xml_file>')
    sys.exit(1)

  sys.exit(insert_example_code(sys.argv[1], sys.argv[2:-1], sys.argv[-1]))
