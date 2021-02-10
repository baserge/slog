# Licensed under GPLv3 or later, see the COPYING file.
set(DOC_FILES overview.rst)
configure_file(./doxygen.conf.in ./doxygen.conf)
configure_file(./conf.py.in ./conf.py)

add_custom_target(html-dir
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/html)

add_custom_target(pdf-dir
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/pdf)

add_custom_target(
                doc-doxy
                COMMAND ${DOXYGEN_EXECUTABLE}
                    ${CMAKE_CURRENT_BINARY_DIR}/doxygen.conf
                  )
add_dependencies(doc-doxy slog html-dir pdf-dir)
add_custom_target(
                doc-html
                COMMAND ${SPHINX} -b html
                        -c ${CMAKE_CURRENT_BINARY_DIR}
                        ${CMAKE_CURRENT_SOURCE_DIR}
                        ${CMAKE_CURRENT_BINARY_DIR}/html
                  )
add_dependencies(doc-html doc-doxy)
add_custom_target(
                doc-pdf
                COMMAND ${SPHINX} -b latex
                        -c ${CMAKE_CURRENT_BINARY_DIR}
                        ${CMAKE_CURRENT_SOURCE_DIR}
                        ${CMAKE_CURRENT_BINARY_DIR}/pdf
                COMMAND ${MAKE} -f
                        ${CMAKE_CURRENT_BINARY_DIR}/pdf/Makefile
                        "all-pdf"
                WORKING_DIRECTORY
                        ${CMAKE_CURRENT_BINARY_DIR}/pdf
                  )
add_dependencies(doc-pdf doc-doxy)
