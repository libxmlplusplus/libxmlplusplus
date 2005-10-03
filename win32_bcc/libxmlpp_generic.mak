#######################################################
# MAKEFILE for building libxml++ 1.0                  #
#                                                     #
# (c) 2005 by Darko Miletic                           #
# e-mail: kiklop@fibertel.com.ar                      # 
#######################################################

.autodepend

!ifndef BMODE
BMODE=RELEASE
!endif

!if $(BMODE) != RELEASE && $(BMODE) != DEBUG
!  error Illegal value for BMODE option
!endif

!include <defaultmake.inc>


SYSDEFINES=_LIB
OUTDIR=bcc

!if $(BMODE) == RELEASE
	USERDEFINES=NDEBUG
	OBJDIR=bcc\Release$(EXTRAEXT)
	!message Building release version of project
!else
	USERDEFINES=_DEBUG
	OBJDIR=bcc\Debug$(EXTRAEXT)
	!message Building debug version of project
!endif

LIBNAME=libxmlpp_bcc$(DBG)$(EXTRAEXT).lib
INCDIR=..;..\..\libxml2\include;..\..\iconv\include
LIBDIR=.
SRC=
OBJ=.\$(OBJDIR)
BIN=.\$(OUTDIR)
RESFILE=
SYSDEFINES=STRICT;WIN32;_MBCS;NO_MFC;_REENTRANT;$(SYSDEFINES)
.path.cc=..\libxml++;..\libxml++\parsers;..\libxml++\nodes;..\libxml++\io;..\libxml++\exceptions
.path.obj=$(OBJ)

################################
# Target
################################
PROJECT=$(BIN)\$(LIBNAME)
CPPFILES=attribute.cc \
	 document.cc \
	 dtd.cc \
	 keepblanks.cc \
	 noncopyable.cc \
	 domparser.cc \
	 parser.cc \
	 saxparser.cc \
	 cdatanode.cc \
	 commentnode.cc \
	 contentnode.cc \
	 element.cc \
	 entityreference.cc \
	 node.cc \
	 processinginstructionnode.cc \
	 textnode.cc \
	 ostreamoutputbuffer.cc \
	 outputbuffer.cc \
	 exception.cc \
	 internal_error.cc \
	 parse_error.cc \
	 validity_error.cc
	
OBJFILES=$(CPPFILES:.cc=.obj^ )         
ALLOBJS=$(BCC32STARTUP) $(OBJFILES)
LPARAM=/P1024

all: dirs $(PROJECT)

cleanobj:: 
	-@echo Deleting intermediate files for project
	-@if exist $(OBJ)\*.obj del $(OBJ)\*.obj
	-@if exist $(OBJ)\*.res del $(OBJ)\*.res
	-@if exist $(OBJ)\. rd $(OBJ)
!if $(BMODE) == RELEASE
	-@if exist $(BIN)\*.tds del $(BIN)\*.tds
	-@if exist $(BIN)\*.map del $(BIN)\*.map
!endif

cleantgt::
	-@echo Deleting output files for project
	-@if exist $(PROJECT) del $(PROJECT)

clean: cleanobj cleantgt

dirs::
	-@echo Creating output directory
	-@if not exist bcc md bcc
	-@if not exist $(OBJ) md $(OBJ)
	-@if not exist $(BIN) md $(BIN)
	
##################################
# Output
##################################
$(PROJECT): $(OBJFILES)
   @echo Linking $(<F) static library
   @$(LB) $< $(LPARAM) @&&|
    &
   -+$(?: = &^
   -+)
   
| > NUL:

    


