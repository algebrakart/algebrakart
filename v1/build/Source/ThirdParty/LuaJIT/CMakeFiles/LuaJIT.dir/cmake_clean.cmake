file(REMOVE_RECURSE
  "../../../bin/libLuaJIT.pdb"
  "../../../bin/jit"
  "CMakeFiles/LuaJIT.dir/generated/lj_vm.S.o"
  "CMakeFiles/LuaJIT.dir/src/ljamalg.c.o"
  "generated/lj_bcdef.h"
  "generated/lj_ffdef.h"
  "generated/lj_folddef.h"
  "generated/lj_libdef.h"
  "generated/lj_recdef.h"
  "generated/lj_vm.S"
  "generated/vmdef.lua"
  "libLuaJIT.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang ASM C)
  include(CMakeFiles/LuaJIT.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
