# Imports a library which is not built with cmake.
# The include directories are appended to the PROJECT_INCLUDE_DIRS variable.
# The libraries           are appended to the PROJECT_LIBRARIES    variable.
# Usage:
#   Header Only:
#     import_library(INCLUDE_DIRS)
#   Identical Debug and Release:
#     import_library(INCLUDE_DIRS LIBRARIES)
#   Separate  Debug and Release:
#     import_library(INCLUDE_DIRS DEBUG_LIBRARIES RELEASE_LIBRARIES)
function(import_library INCLUDE_DIRS)
  set (PROJECT_INCLUDE_DIRS ${PROJECT_INCLUDE_DIRS} ${${INCLUDE_DIRS}} PARENT_SCOPE)
  set (_EXTRA_ARGS ${ARGN})
  list(LENGTH _EXTRA_ARGS _EXTRA_ARGS_LENGTH)
  if    (_EXTRA_ARGS_LENGTH EQUAL 1)
    list(GET _EXTRA_ARGS 0 _LIBRARIES)
    set (PROJECT_LIBRARIES ${PROJECT_LIBRARIES} ${${_LIBRARIES}} PARENT_SCOPE)
  elseif(_EXTRA_ARGS_LENGTH EQUAL 2)
    list(GET _EXTRA_ARGS 0 _DEBUG_LIBRARIES  )
    list(GET _EXTRA_ARGS 1 _RELEASE_LIBRARIES)
    set (PROJECT_LIBRARIES ${PROJECT_LIBRARIES} debug ${${_DEBUG_LIBRARIES}} optimized ${${_RELEASE_LIBRARIES}} PARENT_SCOPE)
  endif ()
endfunction(import_library)
