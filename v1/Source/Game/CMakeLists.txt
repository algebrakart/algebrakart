#
# Copyright (c) 2008-2018 the Urho3D project.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

# Set project name
project (Urho3D-Game)

# Enable ODBC Database (Server logging)
set(ODBC_INCLUDE_DIRS "/usr/include/")
set (URHO3D_DATABASE_ODBC 1)

# Find Urho3D library
find_package (Urho3D REQUIRED)
include_directories (${URHO3D_INCLUDE_DIRS})

# Include common to all game components
set (COMMON_GAME_H_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Game.h" "${CMAKE_CURRENT_SOURCE_DIR}/Game.inl")

# Define dependency libs
set (INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

# Macro for adding game subdirectory
macro (add_game_subdirectory SOURCE_DIR)
    if (DEFINED ENV{included_game})
        if (DEFINED ENV{INCLUDED_SAMPLE_${SOURCE_DIR}})
            add_subdirectory (${SOURCE_DIR} ${ARGN})
        endif ()
    elseif (NOT DEFINED ENV{EXCLUDED_SAMPLE_${SOURCE_DIR}})
        add_subdirectory (${SOURCE_DIR} ${ARGN})
    endif ()
endmacro ()

# Add Game
file (GLOB_RECURSE DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CMakeLists.txt)
list (SORT DIRS)
foreach (DIR ${DIRS})
    get_filename_component (DIR ${DIR} PATH)
    if (DIR)
        add_game_subdirectory (${DIR})
    endif ()
endforeach ()