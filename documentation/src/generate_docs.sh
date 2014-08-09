#!/bin/bash -eu
# Copyright 2014 Google Inc. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Generate html documentation from markdown and doxygen comments in the source.
# This requires at doxygen version 1.8.5 or above.

declare -r root_dir="$(cd $(dirname $0)/../../ ; pwd)"
declare -r docs_source_dir=${root_dir}/documentation/src
declare -r docs_output_dir=${root_dir}/documentation

declare -r doc_dirs="api_reference building programmers_guide"

declare -r footer_file="${root_dir}/documentation/src/footer.html"

usage() {
  echo "
Build documentation from markdown using doxygen.

Usage: $(basename $0) [-h]

-h: Display this help text.
" >&2
  exit 1
}

main() {
  while getopts "h" options; do
    case ${options} in
      h) usage;;
    esac
  done

  # Clean output directory.
  for output_dir in ${doc_dirs}; do
    rm -rf ${docs_output_dir}/${output_dir}/html/*
  done

  # Build documentation.
  for source_dir in ${doc_dirs}; do
    echo '
<html>
<head>
<meta http-equiv="refresh" content="0;url='"${source_dir}"'/html/index.html">
</head>
<body>
<a href="'"${source_dir}"'/html/index.html">Click here if you are not
redirected.</a>
' > "${docs_output_dir}/${source_dir}.html"
    cat "${footer_file}" >> "${docs_output_dir}/${source_dir}.html"
    pushd "${docs_source_dir}/${source_dir}" >/dev/null
    doxygen
    popd >/dev/null
    # Clean up some of the output html.
    index_html="${docs_output_dir}/${source_dir}/html/index.html"
    awk '/ Documentation<\/div>/ { sub(/ Documentation/, ""); } { print $0 }' \
      "${index_html}" > "${index_html}.new" && \
      mv "${index_html}.new" "${index_html}"
  done

  # Check built documentation.
  if [[ "$(which linklint)" != "" ]]; then
    results="${root_dir}/linklint_results"
    # NOTE: -no_anchors is used since linklint generates spurious errors about
    # missing named anchors where they're present.
    linklint -no_anchors -orphan -root "${docs_output_dir}" /@ -doc "${results}"
    if grep -q ERROR "${results}/index.html"; then
      echo -e "\nlinklint detected errors: results are available for"\
              "inspection in" \
              "\n  ${results}" >&2
      exit 1
    fi
    # Report an error if an https link has been added.
    # github does not support https links
    if grep -r https "${results}"; then
      echo -e "\nERROR linklint detected https links: "\
              "github does not support https." >&2
      exit 1
    fi
  else
    echo "Unable to find linklint, install to validate the" \
         "generated documentation." >&2
    exit 1
  fi
}

main "${@}"
