# Makefile for unix systems
# this requires GNU make

APPNAME=CG1Application

# Use pkg-config to search installed libraries
USE_PKGCONFIG=1

# Compiler flags

# enable all warnings in general, disable some
# (we initially provide a rough skeleton of the framework,
# so some unused variables might be present, which should not be
# considered as a problem)
WARNFLAGS= -Wall -Wextra \
	   -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-function \
	   -Wno-unused-parameter -Wno-unknown-pragmas

BASECFLAGS =
BASECXXFLAGS = -std=c++11

# optimize flags, only used for RELEASE=1 builds
# for OpenMP, you might want to add -fopenmp
OPTIMIZEFLAGS = -ffast-math -mtune=native -march=native -O4 -DNDEBUG

ifeq ($(RELEASE), 1)
#
CFLAGS =   $(BASECFLAGS) $(WARNFLAGS) $(OPTIMIZEFLAGS) -s
CXXFLAGS = $(BASECXXFLAGS) $(WARNFLAGS) $(OPTIMIZEFLAGS) -s
BANNER = "RELEASE"
else
CFLAGS =   $(BASECFLAGS) $(WARNFLAGS) -g
CXXFLAGS = $(BASECXXFLAGS) $(WARNFLAGS) -g
BANNER = "DEBUG"
endif

# OpenGL Libraries
CPPFLAGS += -Idependencies/glad/include
LINK_GL =

ifeq ($(USE_PKGCONFIG),1)
# Try to find the system's GLFW3 library via pkg-config

ifeq ($(shell pkg-config --atleast-version=3 glfw3 && echo 1 || echo 0),1)
CPPFLAGS += $(shell pkg-config --cflags glfw3)
LINK_GL += $(shell pkg-config --static --libs glfw3)
else
$(error GLFW3 library not found via pkg-config, please install it, or try using the local one by running make USE_PKGCONFIG=0)
endif

else
# use the local version of GLFW3 and default GL
CPPFLAGS += -Idependencies/GLFW/include
LINK_GL += -L dependencies/GLFW/lib-linux-x86_64 -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor
endif

# all needed libraries
LINK = $(LINK_GL) -lm -lrt -ldl

# headers relative to "source" directory tree
CPPFLAGS += -Isource

# Files

SUBDIRS=dependencies/glad/src \
	source/CGContext \
	source/Applications \
	source/Applications/Project \
	source/Applications/Uebungen \
	source/ShaderPrograms \
	source/Utilities \
	source/Utilities/CG1Helper

BUILDDIR=build

# "shared" files are all files needed both for the uebung and the project
CFILES_SHARED := $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.c))
CPPFILES_SHARED := $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.cpp))
INCFILES_SHARED := $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.h))
SRCFILES_SHARED=$(CFILES_SHARED) $(CPPFILES_SHARED)
PRJFILES_SHARED=Makefile CG1Application.sln CG1Application.vcxproj CG1Application.vcxproj.filters
OBJECTS_SHARED =$(patsubst %.cpp,$(BUILDDIR)/%.o,$(CPPFILES_SHARED)) $(patsubst %.c,$(BUILDDIR)/%.o,$(CFILES_SHARED))

# build rules
.PHONY: all
all:	banner $(APPNAME)

# build and start with "make run"
.PHONY: run
run:	banner $(APPNAME)
	@echo Starting $(APPNAME)
	@__GL_SYNC_TO_VBLANK=1 ./$(APPNAME)
	@echo Exited...

$(BUILDDIR)/dir:
	@mkdir -p $(BUILDDIR) $(patsubst %,$(BUILDDIR)/%,$(SUBDIRS))
	@touch $(BUILDDIR)/dir

$(BUILDDIR)/%.o: %.cpp $(BUILDDIR)/dir
	@echo "compiling [C++]" $<
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: %.c $(BUILDDIR)/dir
	@echo "compiling [C  ]" $< 
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# automatic dependency generation
# create $(DEPDIR) (and an empty file dir)
# create a .d file for every .c/.cpp source file which contains
# 		   all dependencies for that file
DEPDIR   = $(BUILDDIR)/dep
.PHONY: depend
depend: $(DEPDIR)/depends
DEPFILES = $(patsubst %.c,$(DEPDIR)/%.d,$(CFILES_SHARED)) $(patsubst %.cpp,$(DEPDIR)/%.d,$(CPPFILES_SHARED))
$(DEPDIR)/depends: $(DEPDIR)/dir $(DEPFILES)
	@cat $(DEPFILES) > $(DEPDIR)/depends
$(DEPDIR)/dir:
	@mkdir -p $(DEPDIR) $(patsubst %,$(DEPDIR)/%,$(SUBDIRS))
	@touch $(DEPDIR)/dir
$(DEPDIR)/%.d: %.c $(DEPDIR)/dir
	@echo rebuilding dependencies for $<
	@set -e; $(CC) -M $(CPPFLAGS) $<	\
	| sed 's,\(.*\)\.o[ :]*,$(BUILDDIR)/$*.o $@ : ,g' \
	> $@; [ -s $@ ] || rm -f $@
$(DEPDIR)/%.d: %.cpp $(DEPDIR)/dir
	@echo rebuilding dependencies for $<
	@set -e; $(CXX) -M $(CPPFLAGS) $<	\
	| sed 's,\(.*\)\.o[ :]*,$(BUILDDIR)/$*.o $@ : ,g' \
	> $@; [ -s $@ ] || rm -f $@
-include $(DEPDIR)/depends

# rules to build applications
$(APPNAME): $(OBJECTS_SHARED) $(DEPDIR)/depends
	@echo linking $(APPNAME)
	@$(CXX) $(CFLAGS) $(OBJECTS_SHARED) $(LDFLAGS) $(LINK) -o$(APPNAME)

# remove all unneeded files
.PHONY: clean
clean:
	@echo removing binaries: $(APPNAME)
	@rm -f $(APPNAME)
	@echo removing dependency files
	@rm -rf $(DEPDIR)
	@echo removing object files in $(BUILDDIR)
	@rm -f $(OBJECTS_SHARED)
	@rm -rf $(BUILDDIR)
	@echo removing tags
	@rm -f tags

.PHONY: banner
banner:
	@echo "------------------------------------------------------------------------------"
	@echo "$(APPNAME): $(BANNER) build"
	@echo "C/C++ preprocessor flags: $(CPPFLAGS)"
	@echo "C: $(CC) $(CFLAGS)"
	@echo "C++: $(CXX) $(CXXFLAGS)"
	@echo "linker flags: $(LDFLAGS) $(LINK)"
	@echo "------------------------------------------------------------------------------"

# update the tags file
.PHONY: TAGS
TAGS:	tags

tags:	$(SRCFILES_SHARED) $(INCFILES_SHARED) 
	ctags $(SRCFILES_SHARED) $(INCFILES_SHARED)

# look for 'TODO' in all relevant files
.PHONY: todo
todo:
	-egrep -in "TODO" $(SRCFILES_SHARED) $(INCFILES_SHARED)

