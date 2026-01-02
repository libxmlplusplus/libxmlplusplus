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

{..\libxml++\}.cc{$(OUTDIR)\libxmlxx\}.obj::
	@if not exist $(OUTDIR)\libxmlxx\ md $(OUTDIR)\libxmlxx
	$(CXX) $(CFLAGS) $(LIBXMLXX_CFLAGS) $(LIBXMLXX_INCLUDES) /Fo$(OUTDIR)\libxmlxx\ /Fd$(OUTDIR)\libxmlxx\ /c @<<
$<
<<

{..\libxml++\exceptions\}.cc{$(OUTDIR)\libxmlxx\}.obj::
	@if not exist $(OUTDIR)\libxmlxx\ md $(OUTDIR)\libxmlxx
	$(CXX) $(CFLAGS) $(LIBXMLXX_CFLAGS) $(LIBXMLXX_INCLUDES) /Fo$(OUTDIR)\libxmlxx\ /Fd$(OUTDIR)\libxmlxx\ /c @<<
$<
<<

{..\libxml++\io\}.cc{$(OUTDIR)\libxmlxx\}.obj::
	@if not exist $(OUTDIR)\libxmlxx\ md $(OUTDIR)\libxmlxx
	$(CXX) $(CFLAGS) $(LIBXMLXX_CFLAGS) $(LIBXMLXX_INCLUDES) /Fo$(OUTDIR)\libxmlxx\ /Fd$(OUTDIR)\libxmlxx\ /c @<<
$<
<<

{..\libxml++\nodes\}.cc{$(OUTDIR)\libxmlxx\}.obj::
	@if not exist $(OUTDIR)\libxmlxx\ md $(OUTDIR)\libxmlxx
	$(CXX) $(CFLAGS) $(LIBXMLXX_CFLAGS) $(LIBXMLXX_INCLUDES) /Fo$(OUTDIR)\libxmlxx\ /Fd$(OUTDIR)\libxmlxx\ /c @<<
$<
<<

{..\libxml++\parsers\}.cc{$(OUTDIR)\libxmlxx\}.obj::
	@if not exist $(OUTDIR)\libxmlxx\ md $(OUTDIR)\libxmlxx
	$(CXX) $(CFLAGS) $(LIBXMLXX_CFLAGS) $(LIBXMLXX_INCLUDES) /Fo$(OUTDIR)\libxmlxx\ /Fd$(OUTDIR)\libxmlxx\ /c @<<
$<
<<

{..\libxml++\validators\}.cc{$(OUTDIR)\libxmlxx\}.obj::
	@if not exist $(OUTDIR)\libxmlxx\ md $(OUTDIR)\libxmlxx
	$(CXX) $(CFLAGS) $(LIBXMLXX_CFLAGS) $(LIBXMLXX_INCLUDES) /Fo$(OUTDIR)\libxmlxx\ /Fd$(OUTDIR)\libxmlxx\ /c @<<
$<
<<

{.\libxml++\}.rc{$(OUTDIR)\libxmlxx\}.res:
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
$(LIBXMLXX_DLL): $(libxmlxx_OBJS)
	link /DLL $(LDFLAGS) $(DEP_LDFLAGS) /implib:$(LIBXMLXX_LIB) -out:$@ @<<
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

clean:
	@-del /f /q $(OUTDIR)\*.exe
	@-del /f /q $(OUTDIR)\*.dll
	@-del /f /q $(OUTDIR)\*.pdb
	@-del /f /q $(OUTDIR)\*.ilk
	@-del /f /q $(OUTDIR)\*.exp
	@-del /f /q $(OUTDIR)\*.lib
	@-del /f /q $(OUTDIR)\libxmlxx-tests\*.obj
	@-del /f /q $(OUTDIR)\libxmlxx-tests\*.pdb
	@-del /f /q $(OUTDIR)\libxmlxx-examples\*.obj
	@-del /f /q $(OUTDIR)\libxmlxx-examples\*.pdb
	@-del /f /q $(OUTDIR)\libxmlxx\*.res
	@-del /f /q $(OUTDIR)\libxmlxx\*.obj
	@-del /f /q $(OUTDIR)\libxmlxx\*.pdb
	@-rd $(OUTDIR)\libxmlxx-tests
	@-rd $(OUTDIR)\libxmlxx-examples
	@-rd $(OUTDIR)\libxmlxx
