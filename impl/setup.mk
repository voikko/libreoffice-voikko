# This file contains implementation-specific setup parameters
# that control the build of actual interface to the spellchecking
# and hyphenation implementation.

# Full human-readable name of the implementation
LF_FULLNAME=Oo2-voikko

# Short name of the implementation that will be used as a part
# of file names etc.
LF_SHORTNAME=oo2-voikko

# Implementation namespace
LF_NAMESPACE=voikko

# Version number
LF_VERSION=1.1

# Name of the file containing the actual implementation
# Note that this file must be released under the LGPL!
LF_IMPL_CODE=voikkoimpl

# Extra flags needed to be passed to linker command line
LF_LINK_EXTRA=-lvoikko

# If the implementation requires additional files to be shipped
# within the package, these files should be placed in a subdirectory
# impl/$(UNOPKG_PLATFORM). The license of these files may be other
# than LGPL, as long as the requirements of LGPL 
# concerning a "work that uses the Library" are followed.
