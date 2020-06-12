# NMake Makefile portion for compilation rules
# Items in here should not need to be edited unless
# one is maintaining the NMake build files.  The format
# of NMake Makefiles here are different from the GNU
# Makefiles.  Please see the comments about these formats.

# Inference rules for compiling the .obj files.
# Used for libs and programs with more than a single source file.
# Format is as follows
# (all dirs must have a trailing '\'):
#
# {$(srcdir)}.$(srcext){$(destdir)}.obj::
# 	$(CC)|$(CXX) $(cflags) /Fo$(destdir) /c @<<
# $<
# <<

{..\libxml++\}.cc{vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\}.obj::
	@if not exist vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\ md vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx
	$(CXX) $(LIBXMLXX_CFLAGS) $(CFLAGS_NOGL) /Fovs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\ /Fdvs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\ /c @<<
$<
<<

{..\libxml++\exceptions\}.cc{vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\}.obj::
	@if not exist vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\ md vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx
	$(CXX) $(LIBXMLXX_CFLAGS) $(CFLAGS_NOGL) /Fovs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\ /Fdvs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\ /c @<<
$<
<<

{..\libxml++\io\}.cc{vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\}.obj::
	@if not exist vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\ md vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx
	$(CXX) $(LIBXMLXX_CFLAGS) $(CFLAGS_NOGL) /Fovs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\ /Fdvs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\ /c @<<
$<
<<

{..\libxml++\nodes\}.cc{vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\}.obj::
	@if not exist vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\ md vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx
	$(CXX) $(LIBXMLXX_CFLAGS) $(CFLAGS_NOGL) /Fovs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\ /Fdvs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\ /c @<<
$<
<<

{..\libxml++\parsers\}.cc{vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\}.obj::
	@if not exist vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\ md vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx
	$(CXX) $(LIBXMLXX_CFLAGS) $(CFLAGS_NOGL) /Fovs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\ /Fdvs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\ /c @<<
$<
<<

{..\libxml++\validators\}.cc{vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\}.obj::
	@if not exist vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\ md vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx
	$(CXX) $(LIBXMLXX_CFLAGS) $(CFLAGS_NOGL) /Fovs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\ /Fdvs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\ /c @<<
$<
<<

{.\libxml++\}.rc{vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\}.res:
	@if not exist $(@D)\ md $(@D)
	rc /fo$@ $<

# Rules for building .lib files
$(LIBXMLXX_LIB): $(LIBXMLXX_DLL)

# Rules for linking DLLs
# Format is as follows (the mt command is needed for MSVC 2005/2008 builds):
# $(dll_name_with_path): $(dependent_libs_files_objects_and_items)
#	link /DLL [$(linker_flags)] [$(dependent_libs)] [/def:$(def_file_if_used)] [/implib:$(lib_name_if_needed)] -out:$@ @<<
# $(dependent_objects)
# <<
# 	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;2
$(LIBXMLXX_DLL): $(libxmlxx_OBJS) vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\libxml++.def
	link /DLL $(LDFLAGS_NOLTCG) $(GLIBMM_LIB) $(LIBXML2_LIBS) $(GOBJECT_LIBS) $(LIBSIGC_LIB) /implib:$(LIBXMLXX_LIB) /def:vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\libxml++.def -out:$@ @<<
$(libxmlxx_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;2

# Rules for linking Executables
# Format is as follows (the mt command is needed for MSVC 2005/2008 builds):
# $(dll_name_with_path): $(dependent_libs_files_objects_and_items)
#	link [$(linker_flags)] [$(dependent_libs)] -out:$@ @<<
# $(dependent_objects)
# <<
# 	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;1
$(GENDEF): gendef\gendef.cc
	@if not exist $(@D)\gendef\ md $(@D)\gendef
	$(CXX) $(CFLAGS) /Fo$(@D)\gendef\ /Fd$(@D)\gendef\ $** /Fe:$@ /link $(LDFLAGS) -out:$@ 
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;1

clean:
	@-del /f /q vs$(VSVER)\$(CFG)\$(PLAT)\*.exe
	@-del /f /q vs$(VSVER)\$(CFG)\$(PLAT)\*.dll
	@-del /f /q vs$(VSVER)\$(CFG)\$(PLAT)\*.pdb
	@-del /f /q vs$(VSVER)\$(CFG)\$(PLAT)\*.ilk
	@-del /f /q vs$(VSVER)\$(CFG)\$(PLAT)\*.exp
	@-del /f /q vs$(VSVER)\$(CFG)\$(PLAT)\*.lib
	@-del /f /q vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx-tests\*.obj
	@-del /f /q vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx-tests\*.pdb
	@-del /f /q vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx-examples\*.obj
	@-del /f /q vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx-examples\*.pdb
	@-del /f /q vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\*.def
	@-del /f /q vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\*.res
	@-del /f /q vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\*.obj
	@-del /f /q vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx\*.pdb
	@-del /f /q vs$(VSVER)\$(CFG)\$(PLAT)\gendef\*.obj
	@-del /f /q vs$(VSVER)\$(CFG)\$(PLAT)\gendef\*.pdb
	@-rd vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx-tests
	@-rd vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx-examples
	@-rd vs$(VSVER)\$(CFG)\$(PLAT)\libxmlxx
	@-rd vs$(VSVER)\$(CFG)\$(PLAT)\gendef
