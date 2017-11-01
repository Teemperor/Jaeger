function(make_unittest)
  set(options)
  set(oneValueArgs NAME)
  set(multiValueArgs FILES)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(ARG_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "Unparsed arguments to make_unittest: ${ARG_UNPARSED_ARGUMENTS}")
  endif()
  if(NOT ARG_FILES)
    set(ARG_FILES "${ARG_NAME}.cpp")
  endif()

  add_executable(${ARG_NAME} ${ARG_FILES})
  target_link_libraries(${ARG_NAME} argos_core)
  add_test(NAME    "unittest:${ARG_NAME}"
           COMMAND "$<TARGET_FILE:${ARG_NAME}>"
           WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
endfunction()

make_unittest(NAME DummyTest)
