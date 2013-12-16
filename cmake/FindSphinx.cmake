# - Find Sphinx (documentation generation)
# This module defines
# Sphinx_FOUND, If false, do not try to use Sphinx

# Consider as Idontcare licence.

find_program(SPHINX sphinx-build)

if (SPHINX)
    set(Sphinx_FOUND TRUE)
    message(STATUS "found Sphinx")
else ()
    set(Sphinx_FOUND FALSE)
    if (Sphinx_FIND_REQUIRED)
        message(FATAL_ERROR "Sphinx NOT found")
    else ()
        message(STATUS "Sphinx not found")
    endif ()
endif ()
