# Convert the source listing to object (.obj) listing in
# another NMake Makefile module, include it, and clean it up.
# This is a "fact-of-life" regarding NMake Makefiles...
# This file does not need to be changed unless one is maintaining the NMake Makefiles

# For those wanting to add things here:
# To add a list, do the following:
# # $(description_of_list)
# if [call create-lists.bat header $(makefile_snippet_file) $(variable_name)]
# endif
#
# if [call create-lists.bat file $(makefile_snippet_file) $(file_name)]
# endif
#
# if [call create-lists.bat footer $(makefile_snippet_file)]
# endif
# ... (repeat the if [call ...] lines in the above order if needed)
# !include $(makefile_snippet_file)
#
# (add the following after checking the entries in $(makefile_snippet_file) is correct)
# (the batch script appends to $(makefile_snippet_file), you will need to clear the file unless the following line is added)
#!if [del /f /q $(makefile_snippet_file)]
#!endif

# In order to obtain the .obj filename that is needed for NMake Makefiles to build DLLs/static LIBs or EXEs, do the following
# instead when doing 'if [call create-lists.bat file $(makefile_snippet_file) $(file_name)]'
# (repeat if there are multiple $(srcext)'s in $(source_list), ignore any headers):
# !if [for %c in ($(source_list)) do @if "%~xc" == ".$(srcext)" @call create-lists.bat file $(makefile_snippet_file) $(intdir)\%~nc.obj]
#
# $(intdir)\%~nc.obj needs to correspond to the rules added in build-rules-msvc.mak
# %~xc gives the file extension of a given file, %c in this case, so if %c is a.cc, %~xc means .cc
# %~nc gives the file name of a given file without extension, %c in this case, so if %c is a.cc, %~nc means a

NULL=

# For libxml++

!if [call create-lists.bat header $(BUILD_SNIPPET_MKFILE) libxmlxx_OBJS]
!endif

!if [for %c in ($(cc_sources)) do @if "%~xc" == ".cc" @call create-lists.bat file $(BUILD_SNIPPET_MKFILE) ^$^(OUTDIR^)\libxmlxx\%~nc.obj]
!endif

!if [@call create-lists.bat file $(BUILD_SNIPPET_MKFILE) ^$^(OUTDIR^)\libxmlxx\libxml++.res]
!endif

!if [call create-lists.bat footer $(BUILD_SNIPPET_MKFILE)]
!endif

!if [call create-lists.bat header $(BUILD_SNIPPET_MKFILE) libxmlxx_real_hdrs]
!endif

!if [for %c in ($(h_sources_public:/=\)) do @call create-lists.bat file $(BUILD_SNIPPET_MKFILE) ..\libxml++\%c]
!endif

!if [call create-lists.bat footer $(BUILD_SNIPPET_MKFILE)]
!endif

!if [for %d in (examples tests) do @call create-lists.bat header $(BUILD_SNIPPET_MKFILE) libxmlxx_%d & @(for /f %t in ('dir /ad /b ..\%d') do @call create-lists.bat file $(BUILD_SNIPPET_MKFILE) $(OUTDIR)\%t.exe) & @call create-lists.bat footer $(BUILD_SNIPPET_MKFILE)]
!endif

!if [for %d in (examples tests) do @for /f %t in ('dir /ad /b ..\%d') do @call create-lists.bat header $(BUILD_SNIPPET_MKFILE) %t_OBJS & @(for %s in (..\%d\%t\*.cc) do @call create-lists.bat file $(BUILD_SNIPPET_MKFILE) $(OUTDIR)\libxmlxx-%d\%t-%~ns.obj) & @call create-lists.bat footer $(BUILD_SNIPPET_MKFILE)]
!endif

!if [for %t in (dom_parser dom_parse_entities) do @echo %t_OBJS = ^$(%t_OBJS) vs^$(VSVER)\^$(CFG)\^$(PLAT)\libxmlxx-examples\testutilities.obj>>$(BUILD_SNIPPET_MKFILE)]
!endif

!if [echo.>>$(BUILD_SNIPPET_MKFILE)]
!endif

!if [for %d in (examples tests) do @for /f %t in ('dir /ad /b ..\%d') do @for %s in (..\%d\%t\*.cc) do @echo ^$^(OUTDIR^)\libxmlxx-%d\%t-%~ns.obj: %s>>$(BUILD_SNIPPET_MKFILE) & @echo. if not exist ^$(@D)\ md ^$(@D)>>$(BUILD_SNIPPET_MKFILE) & @echo.	^$(CXX) ^$(CFLAGS) ^$(LIBXMLXX_EX_CFLAGS) ^$(LIBXMLXX_INCLUDES) /Fo^$(@D)\%t-%~ns.obj /Fd^$(@D)\ ^$** /c>>$(BUILD_SNIPPET_MKFILE) & @echo.>>$(BUILD_SNIPPET_MKFILE)]
!endif

!if [echo vs^$(VSVER)\^$(CFG)\^$(PLAT)\libxmlxx-examples\testutilities.obj: ..\examples\testutilities.cc>>$(BUILD_SNIPPET_MKFILE) & @echo. if not exist ^$(@D)\ md ^$(@D)>>$(BUILD_SNIPPET_MKFILE) & @echo.	^$(CXX)  ^$(CFLAGS) ^$(LIBXMLXX_EX_CFLAGS) ^$(LIBXMLXX_INCLUDES) /Fo^$(@D)\ /Fd^$(@D)\ ^$** /c>>$(BUILD_SNIPPET_MKFILE) & @echo.>>$(BUILD_SNIPPET_MKFILE)]
!endif

!if [for %d in (examples tests) do @for /f %t in ('dir /ad /b ..\%d') do @echo ^$^(OUTDIR^)\%t.exe: ^$(LIBXMLXX_LIB) ^$(%t_OBJS)>>$(BUILD_SNIPPET_MKFILE) & @echo.	link ^$(LDFLAGS) ^$** ^$(DEP_LDFLAGS) /out:^$@>>$(BUILD_SNIPPET_MKFILE) & @echo.>>$(BUILD_SNIPPET_MKFILE)]
!endif

!if [echo.>>$(BUILD_SNIPPET_MKFILE)]
!endif

!include $(BUILD_SNIPPET_MKFILE)

!if [del /f /q $(BUILD_SNIPPET_MKFILE)]
!endif
