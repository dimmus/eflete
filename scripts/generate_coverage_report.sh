#!/usr/bin/env python3

import os
import subprocess
from lxml import html

proj_root = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))
print("Project root:", proj_root)

print("Generating coverage report...")
subprocess.call(["ninja", "-C", "build", "-j", "1", "clean"])
subprocess.call(["ninja", "-C", "build", "-j", "1", "test"])

# Run lcov
outfiles = []
build_root = os.path.join(proj_root, 'build')
log_dir = os.path.join(build_root, 'meson-logs')
source_root = os.path.join(proj_root, 'src')
subproject_root = os.path.join(proj_root, 'subprojects')
lcov_exe = 'lcov'
genhtml_exe = 'genhtml'
htmloutdir = os.path.join(log_dir, 'coveragereport')
html_index = os.path.join(htmloutdir, 'index.html')
covinfo = os.path.join(log_dir, 'coverage.info')
initial_tracefile = covinfo + '.initial'
run_tracefile = covinfo + '.run'
raw_tracefile = covinfo + '.raw'
subprocess.check_call([lcov_exe,
                        '--directory', build_root,
                        '--capture',
                        '--initial',
                        '--output-file',
                        initial_tracefile])
subprocess.check_call([lcov_exe,
                        '--directory', build_root,
                        '--capture',
                        '--output-file', run_tracefile,
                        '--no-checksum',
                        '--rc', 'lcov_branch_coverage=1',
                        ])
# Join initial and test results.
subprocess.check_call([lcov_exe,
                        '-a', initial_tracefile,
                        '-a', run_tracefile,
                        '--rc', 'lcov_branch_coverage=1',
                        '-o', raw_tracefile])
# Remove all directories outside the source_root from the covinfo
subprocess.check_call([lcov_exe,
                        '--extract', raw_tracefile,
                        os.path.join(source_root, '*'),
                        '--rc', 'lcov_branch_coverage=1',
                        '--output-file', covinfo])
# Remove all directories inside subproject dir
subprocess.check_call([lcov_exe,
                        '--remove', covinfo,
                        os.path.join(subproject_root, '*'),
                        '--rc', 'lcov_branch_coverage=1',
                        '--output-file', covinfo])
subprocess.check_call([genhtml_exe,
                        '--prefix', build_root,
                        '--prefix', source_root,
                        '--output-directory', htmloutdir,
                        '--title', 'Code coverage',
                        '--legend',
                        '--show-details',
                        '--branch-coverage',
                        covinfo])

outfiles.append(('Html', html_index))

if outfiles:
        print('')
        for (filetype, path) in outfiles:
            print(filetype + ' coverage report can be found at', path)

