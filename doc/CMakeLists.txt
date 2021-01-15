# Finds and loads settings from an external project.
# More information at https://cmake.org/cmake/help/v3.18/command/find_package.html
find_package(Doxygen
# The dot component is used for graph generation.
# More information at https://cmake.org/cmake/help/v3.18/module/FindDoxygen.html
	COMPONENTS dot)

# Input directory of header files from which the documentation will be formed.
set(DOXYGEN_IN_DIR ../include/)

doxygen_add_docs(Doxygen ALL
  COMMENT "Generating documentation"
  ../README.md
  ../include/
)