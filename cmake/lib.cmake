#
# Copyright 2018 coord.e
#
# This file is part of Flom.
#
# Flom is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Flom is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Flom.  If not, see <http://www.gnu.org/licenses/>.
#

function(flom_add_lib target name)
  flom_set_compile_options(${target})
  set_target_properties(${target} PROPERTIES OUTPUT_NAME "${name}")
  set_target_properties(${target} PROPERTIES POSITION_INDEPENDENT_CODE ${USE_PIC})
  target_link_libraries(${target} PRIVATE flom_proto nlohmann_json::nlohmann_json ${PROTOBUF_LIBRARIES})
endfunction()
