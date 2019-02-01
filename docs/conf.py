extensions = [
    'breathe',
    'exhale'
]

breathe_projects = {
    "flom": "./doxygen/xml"
}
breathe_default_project = "flom"

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
