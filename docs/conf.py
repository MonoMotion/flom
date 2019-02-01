project = 'flom'
copyright = '2019 coord.e'
author = 'coord.e <me@coord-e.com>'

import subprocess
release = subprocess.run(["../version.sh"], stdout=subprocess.PIPE).stdout.decode()
version = subprocess.run(["../version.sh", "simple"], stdout=subprocess.PIPE).stdout.decode()

extensions = [
    'breathe',
    'exhale'
]

breathe_projects = {
    "flom": "./doxygen/xml"
}
breathe_default_project = "flom"

master_doc = 'index'

exhale_args = {
    "containmentFolder":     "./api",
    "rootFileName":          "library_root.rst",
    "rootFileTitle":         "API Documentation",
    "doxygenStripFromPath":  "..",
    "createTreeView":        True,
    "exhaleExecutesDoxygen": True,
    "exhaleDoxygenStdin":    "INPUT = ../include"
}

html_theme = 'sphinx_rtd_theme'

primary_domain = 'cpp'
highlight_language = 'cpp'
