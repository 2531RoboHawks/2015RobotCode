FILE(REMOVE_RECURSE
  "CMakeFiles/my-code.dir/main.cpp.obj"
  "my-code.pdb"
  "my-code"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang CXX)
  INCLUDE(CMakeFiles/my-code.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
