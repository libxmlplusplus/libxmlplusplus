# NMake Makefile snippet for copying the built libraries, utilities and headers to
# a path under $(PREFIX).

install: all
	@if not exist $(PREFIX)\bin\ md $(PREFIX)\bin
	@if not exist $(PREFIX)\lib\libxml++-$(LIBXMLXX_MAJOR_VERSION).$(LIBXMLXX_MINOR_VERSION)\include\ md $(PREFIX)\lib\libxml++-$(LIBXMLXX_MAJOR_VERSION).$(LIBXMLXX_MINOR_VERSION)\include
	@if not exist $(PREFIX)\include\libxml++-$(LIBXMLXX_MAJOR_VERSION).$(LIBXMLXX_MINOR_VERSION)\libxml++\ @md $(PREFIX)\include\libxml++-$(LIBXMLXX_MAJOR_VERSION).$(LIBXMLXX_MINOR_VERSION)\libxml++
	@for /f %d in ('dir /ad /b ..\libxml++') do @if not exist $(PREFIX)\include\libxml++-$(LIBXMLXX_MAJOR_VERSION).$(LIBXMLXX_MINOR_VERSION)\libxml++\%d\ @md $(PREFIX)\include\libxml++-$(LIBXMLXX_MAJOR_VERSION).$(LIBXMLXX_MINOR_VERSION)\libxml++\%d
	@copy /b "$(LIBXMLXX_DLL)" "$(PREFIX)\bin\"
	@copy /b "$(LIBXMLXX_DLL:dll=pdb)" "$(PREFIX)\bin\"
	@copy /b "$(LIBXMLXX_LIB)" "$(PREFIX)\lib\"
	@for %h in (libxml++.h $(h_root_sources_public:/=\)) do @copy "..\libxml++\%h" "$(PREFIX)\include\libxml++-$(LIBXMLXX_MAJOR_VERSION).$(LIBXMLXX_MINOR_VERSION)\libxml++\"
	@for %h in ($(h_exceptions_sources_public:/=\)) do @copy "..\libxml++\%h" "$(PREFIX)\include\libxml++-$(LIBXMLXX_MAJOR_VERSION).$(LIBXMLXX_MINOR_VERSION)\libxml++\exceptions\"
	@for %h in ($(h_io_sources_public:/=\)) do @copy "..\libxml++\%h" "$(PREFIX)\include\libxml++-$(LIBXMLXX_MAJOR_VERSION).$(LIBXMLXX_MINOR_VERSION)\libxml++\io\"
	@for %h in ($(h_nodes_sources_public:/=\)) do @copy "..\libxml++\%h" "$(PREFIX)\include\libxml++-$(LIBXMLXX_MAJOR_VERSION).$(LIBXMLXX_MINOR_VERSION)\libxml++\nodes\"
	@for %h in ($(h_parsers_sources_public:/=\)) do @copy "..\libxml++\%h" "$(PREFIX)\include\libxml++-$(LIBXMLXX_MAJOR_VERSION).$(LIBXMLXX_MINOR_VERSION)\libxml++\parsers\"
	@for %h in ($(h_validators_sources_public:/=\)) do @copy "..\libxml++\%h" "$(PREFIX)\include\libxml++-$(LIBXMLXX_MAJOR_VERSION).$(LIBXMLXX_MINOR_VERSION)\libxml++\validators\"
	@copy ".\libxml++\libxml++config.h" "$(PREFIX)\lib\libxml++-$(LIBXMLXX_MAJOR_VERSION).$(LIBXMLXX_MINOR_VERSION)\include\"
