#######################################################
# MAKEFILE for building all  libxml++    libs         #
#                                                     #
#                                                     #
# (c) 2005 by Darko Miletic                           #
# e-mail: kiklop@fibertel.com.ar                      # 
#######################################################
.autodepend

!ifndef MK
MK=make
!endif

all: debug debug_st debug_allstat_st debug_allstat_mt release release_st release_allstat_st release_allstat_mt


clean::
	@$(MK) -DMK=$(MK) -flibxmlpp_generic.mak -DBMODE=DEBUG clean
	@$(MK) -DMK=$(MK) -flibxmlpp_generic.mak clean
	@$(MK) -DMK=$(MK) -flibxmlpp_generic.mak -DUSERTLDLL -DUSEMT -DBMODE=DEBUG clean
	@$(MK) -DMK=$(MK) -flibxmlpp_generic.mak -DUSERTLDLL -DUSEMT clean
	@$(MK) -DMK=$(MK) -flibxmlpp_generic.mak -DUSERTLDLL -DBMODE=DEBUG clean
	@$(MK) -DMK=$(MK) -flibxmlpp_generic.mak -DUSERTLDLL clean
	@$(MK) -DMK=$(MK) -flibxmlpp_generic.mak -DUSEMT -DBMODE=DEBUG clean
	@$(MK) -DMK=$(MK) -flibxmlpp_generic.mak -DUSEMT clean
	@rmdir bcc

#Debug with RTL DLL and multithreaded)
debug:
	@$(MK) -DMK=$(MK) -flibxmlpp_generic.mak -DUSERTLDLL -DUSEMT -DBMODE=DEBUG

#Release with RTL DLL and multithreaded)
release:
	@$(MK) -DMK=$(MK) -flibxmlpp_generic.mak -DUSERTLDLL -DUSEMT

#Debug with RTL DLL and singlethreaded
debug_st:
	@$(MK) -DMK=$(MK) -flibxmlpp_generic.mak -DUSERTLDLL -DBMODE=DEBUG

release_st:
	@$(MK) -DMK=$(MK) -flibxmlpp_generic.mak -DUSERTLDLL
	
debug_allstat_st:
	@$(MK) -DMK=$(MK) -flibxmlpp_generic.mak -DBMODE=DEBUG

release_allstat_st:
	@$(MK) -DMK=$(MK) -flibxmlpp_generic.mak

debug_allstat_mt:
	@$(MK) -DMK=$(MK) -flibxmlpp_generic.mak -DUSEMT -DBMODE=DEBUG

release_allstat_mt:
	@$(MK) -DMK=$(MK) -flibxmlpp_generic.mak -DUSEMT
