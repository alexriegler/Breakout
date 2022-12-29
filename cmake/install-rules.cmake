install(
    TARGETS Breakout_exe
    RUNTIME COMPONENT Breakout_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
