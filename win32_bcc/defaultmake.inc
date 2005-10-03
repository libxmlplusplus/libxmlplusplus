
#tools
ILINK32=ilink32
LB=tlib
CC=bcc32
RC=brcc32

!if $(USERTLDLL)
EXTRA=-tWR
SYSDEFINES=_RTLDLL
EXTRAEXT=_rtldll
!else
EXTRAEXT=_rtlstatic
!endif

!if $(USEMT)
EXTRA=$(EXTRA) -tWM
EXTRAEXT=$(EXTRAEXT)_mt
!endif

#C/C++ compile/link flags
CFLAGS=-q -6 -tWD $(EXTRA) -w-aus -w-sus -w-pia -w-rch -w-ccc -w-eff -D$(USERDEFINES);$(SYSDEFINES)
CPPFLAGS=-q -6 -tWD $(EXTRA) -RT -x -a8 -b -X- -w-par -w-aus -w-pia -D$(USERDEFINES);$(SYSDEFINES)

!if $(BMODE) == RELEASE
CPPFLAGS=-v- -O2 -k- $(CPPFLAGS)
CFLAGS=-v- -O2 -k- $(CFLAGS)
DBG=
!else
CPPFLAGS=-v -vi- -Od -k -R -Q -M -y -r- $(CPPFLAGS)
CFLAGS=-v -vi- -Od -k -Q -R -M -y -r- $(CFLAGS)
DBG=_Debug
!endif

.cc.obj:
      @$(CC) -c -Pcc $(CPPFLAGS) -I$(INCDIR) -n$(OBJ) $<

.cxx.obj:
      @$(CC) -c -Pcxx $(CPPFLAGS) -I$(INCDIR) -n$(OBJ) $<

.cpp.obj:
      @$(CC) -c $(CPPFLAGS) -I$(INCDIR) -n$(OBJ) $<

.c.obj:
      @$(CC) -c $(CFLAGS) -I$(INCDIR) -n$(OBJ) $<

#This include sequence is added because of some visual studio
#generated rc scripts
#This will only work on CBuilder with installed support for MFC
.rc.res:
      @echo Compiling $(<F)
      @$(RC) $(RFLAGS) -i$(MAKEDIR)\..\Include;$(MAKEDIR)\..\Include\Mfc -fo$@ $< > NUL:
