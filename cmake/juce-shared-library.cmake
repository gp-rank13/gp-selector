# This function is not provided by JUCE itself, so we are going to implement it
# ourselves. For further context, take a look at this thread in the JUCE forum:
# https://forum.juce.com/t/cmake-creating-dynamic-linked-library-with-juce/40530
function(juce_add_shared_library target)
  add_library(${target} SHARED)
  _juce_initialise_target(${target} ${ARGN})
  target_compile_definitions(
    ${target}
    PRIVATE #
            JUCE_STANDALONE_APPLICATION=1 # necessary to suppress some warnings
  )
endfunction(juce_add_shared_library)
