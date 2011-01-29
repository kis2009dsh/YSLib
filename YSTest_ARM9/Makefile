# DS Makefile for YShell -> ARM9 by Franksoft 2009 - 2011
# CodePage = ANSI / GBK;
# CTime = 2009-12-18 12:27:40 + 08:00;
# UTime = 2011-01-17 11:56 + 08:00;
# Version = 0.3329;

# This makefile is written based on PALib Makefile by Scognito and Tom and crash.

#--------------------------------------------------------------------------------
.SUFFIXES:
#--------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/ds_rules

export TOPDIR		?=	$(CURDIR)

#--------------------------------------------------------------------------------
# TARGET is the name of the file to output
# BUILD is the directory where object files and intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
# DATA is a list of directories containing binary files embedded using bin2o
# GRAPHICS is a list of directories containing image files to be converted with grit
#--------------------------------------------------------------------------------

export TARGET		:= $(shell basename $(CURDIR))
export BUILD		?= release
export RELEASE		= $(BUILD)
export SOURCES		:= source data
export INCLUDES		:= include data ../YSTest/include ../CHRLib/include ../YCLib/include ../YSLib/include
export DATA			:= data
#export GRAPHICS	:= gfx
#export MUSIC		:= music

#--------------------------------------------------------------------------------
# options for code generation
#--------------------------------------------------------------------------------
ARCH	:=	-mthumb -mthumb-interwork

# note: arm9tdmi isn't the correct CPU arch, but anything newer and LD
# *insists* it has a FPU or VFP, and it won't take no for an answer!

#ASFLAGS:=	-g $(ARCH) -march=armv5te -mtune=arm946e-s
ASFLAGS	:=	$(ARCH)

#CFLAGS	:=	-g -Wformat=2 -Winline -Wall \
#		-mcpu=arm946e-s -mtune=arm946e-s -fomit-frame-pointer \
#		-ffast-math \
#		$(ARCH)

CFLAGS	:=	-Wall \
			-march=armv5te -mtune=arm946e-s -fomit-frame-pointer \
			-ffast-math \
			$(ARCH)

ifneq ($(BUILD),debug)
CFLAGS	+=	-O3 -DNDEBUG
else
ASFLAGS	+=	-g
CFLAGS	+=	-g -O0 -DDEBUG
endif

CFLAGS	+=	$(INCLUDE) -DARM9
#CFLAGS	+=	$(INCLUDE) -I$(PAPATH)/include/nds
#CXXFLAGS	:=	$(CFLAGS) -frepo -fno-threadsafe-statics -fno-rtti -fno-exceptions
CXXFLAGS	:=	$(CFLAGS) -fno-threadsafe-statics -fuse-cxa-atexit
#	-Weffc++ -Wold-style-cast -Woverloaded-virtual -pedantic
#	-Wextra -Winit-self -Wmissing-include-dirs -Wsign-promo

LDFLAGS	:=	-specs=ds_arm9.specs -g $(ARCH) -mno-fpu -Wl,-Map,$(notdir $*.map)
#LDFLAGS=	-specs=ds_arm9.specs -g $(ARCH) -L$(PAPATH)/lib -Wl,-Map,$(notdir $*.map)

#--------------------------------------------------------------------------------
# path to tools - this can be deleted if you set the path in windows
#--------------------------------------------------------------------------------
# export PATH		:=	/d/dev/ds/devkitARM_r11/bin:/bin

#--------------------------------------------------------------------------------
# PATH to ndslib - just make a system variable called NDSLIBPATH and be done with it
#--------------------------------------------------------------------------------
# NDSLIBPATH	:=	/d/dev/ds/ndslib/

#--------------------------------------------------------------------------------
# the prefix on the compiler executables
#--------------------------------------------------------------------------------
PREFIX			:=	arm-eabi-

#--------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#--------------------------------------------------------------------------------
LIBS	:=	-lfat -lnds9

#---- User defined libraries.

LIBPATHEX	:= -L$(TOPDIR)/../YSTest/lib
LIBEX		:= -lfreetype -lloki
#LIBEX		:= -lfreetype -lagg -lloki

LIBPATH_CHRLIB := -L$(TOPDIR)/../CHRLib/$(RELEASE)
LIB_CHRLIB := -lCHRLib

YCLIB_A := $(TOPDIR)/../YCLib/$(RELEASE)/libYCLib.a
YSLIB_A := $(TOPDIR)/../YSLib/$(RELEASE)/libYSLib.a

#----

#--------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#--------------------------------------------------------------------------------
LIBDIRS	:=	$(LIBNDS)
#LIBDIRPA	:=	$(PAPATH)

#--------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#--------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#--------------------------------------------------------------------------------

export ARM9BIN	:=	$(CURDIR)/$(RELEASE)/$(TARGET).arm9
export ARM9ELF	:=	$(CURDIR)/$(BUILD)/$(TARGET).arm9.elf
export DEPSDIR	:=	$(CURDIR)/$(BUILD)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir))

#export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir))

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
PCXFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.pcx)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

ifeq ($(strip $(AUDIOFILES)),)
	ifeq ($(strip $(ARM7_SELECTED)), ARM7_MAXMOD_DSWIFI)
		TEXT_TO_SAY_2 := No MaxMod-compatible files were found in the audio folder.
		TEXT_TO_SAY_3 := If you are loading the soundbank from FAT or EFS, ignore this message.
		TEXT_TO_SAY_4 := " "
	endif
	DONT_BUILD_MAXMOD_SOUNDBANK = YES
endif

ifneq ($(strip $(DONT_BUILD_MAXMOD_SOUNDBANK)), YES)
	ifeq ($(strip $(ARM7_SELECTED)), ARM7_MAXMOD_DSWIFI)
		BINFILES += soundbank_bin.bin
	endif
endif

PNGFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.png)))
PALFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.pal)))
RAWFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.raw)))
MAPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.map)))
JPEGFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.jpg)))
MODFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.mod)))
GIFFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.gif)))
BMPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.bmp)))
MP3FILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.mp3)))

#--------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#--------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#--------------------------------------------------------------------------------
	export LD	:=	$(CC)
#--------------------------------------------------------------------------------
else
#--------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#--------------------------------------------------------------------------------
endif
#--------------------------------------------------------------------------------

#export OFILES	:=	$(addsuffix .o,$(BINFILES)) \
#					$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

export OFILES	:=	$(MAPFILES:.map=.o) $(RAWFILES:.raw=.o) $(PALFILES:.pal=.o) $(BINFILES:.bin=.o) $(PNGFILES:.png=.o) $(PCXFILES:.pcx=.o) $(JPEGFILES:.jpg=.o) $(MODFILES:.mod=.o) $(GIFFILES:.gif=.o) $(BMPFILES:.bmp=.o) $(MP3FILES:.mp3=.o) \
					$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

#export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
#			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
#			-I$(CURDIR)/$(BUILD)

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include/nds) \
				#	-I$(PAPATH)/include/nds \
					-I$(CURDIR)/$(BUILD)
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) rebuild clean

#--------------------------------------------------------------------------------
$(BUILD):
$(BUILD):
	@echo Building configuration: $(notdir $@) ...
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile
	@echo Built configuration: $(notdir $@) .

#--------------------------------------------------------------------------------
clean:
	@echo Cleaning $(TARGET) ...
	@rm -f $(BUILD)/.map
	@rm -f $(BUILD)/*.d
	@rm -f $(BUILD)/*.o
	@rm -f $(ARM9BIN)
	@rm -f $(ARM9ELF)

rebuild: all

all: $(BUILD) $(OUTPUT)

#--------------------------------------------------------------------------------
else

DEPENDS	:= $(OFILES:.o=.d)

#--------------------------------------------------------------------------------
# main targets
#--------------------------------------------------------------------------------

$(ARM9BIN)	:	$(ARM9ELF)
	@$(OBJCOPY) -O binary $< $@
	@echo Built $(notdir $@) .

$(ARM9ELF)	:	$(OFILES)
	@echo Linking $(notdir $@) ...
	$(LD) $(LDFLAGS) $(OFILES) $(YCLIB_A) $(YSLIB_A) $(LIBPATHS) $(LIBS) $(LIBPATHEX) $(LIBEX) $(LIBPATH_CHRLIB) $(LIB_CHRLIB) -o $@
# IMPORTANT: the order of files is significant.

#--------------------------------------------------------------------------------
# you need a rule like this for each extension you use as binary data
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# Compile Targets for C/C++
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
%.o : %.cpp
	@echo $(notdir $<)
	@$(CXX) -MM $(CXXFLAGS) -o $*.d $<
	@$(CXX) $(CXXFLAGS) -c $< -o$@

#--------------------------------------------------------------------------------
%.o : %.c
	@echo $(notdir $<)
	@$(CC) -MM $(CFLAGS) -o $*.d $<
	@$(CC)  $(CFLAGS) -c $< -o$@

#--------------------------------------------------------------------------------
%.o : %.s
	@echo $(notdir $<)
	@$(CC) -MM $(CFLAGS) -o $*.d $<
	@$(CC)  $(ASFLAGS) -c $< -o$@


# bin2o macro like that from libnds, slightly changed for the PALib naming conventions of resources which doesn't append the file extension to the resource name
define bin2o
	cp $(<) $(*)
	bin2s $(*) | $(AS) $(ARCH) -o $(@)
	rm $(*)

	echo "extern const u8" $(*)"[];" > $(*).h
	echo "extern const u32" $(*)_size";" >> $(*).h
endef

#--------------------------------------------------------------------------------
%.o	:	%.mp3
#--------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#--------------------------------------------------------------------------------
%.o	:	%.pcx
#--------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#--------------------------------------------------------------------------------
%.o	:	%.bin
#--------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#--------------------------------------------------------------------------------
%.o	:	%.png
#--------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#--------------------------------------------------------------------------------
%.o	:	%.raw
#--------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#--------------------------------------------------------------------------------
%.o	:	%.pal
#--------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#--------------------------------------------------------------------------------
%.o	:	%.map
#--------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#--------------------------------------------------------------------------------
%.o	:	%.mdl
#--------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#--------------------------------------------------------------------------------
%.o	:	%.jpg
#--------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#--------------------------------------------------------------------------------
%.o	:	%.mod
#--------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#--------------------------------------------------------------------------------
%.o	:	%.gif
#--------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#--------------------------------------------------------------------------------
%.o	:	%.bmp
#--------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#--------------------------------------------------------------------------------
# This rule creates assembly source files using grit
# grit takes an image file and a .grit describing how the file is to be processed
# add additional rules like this for each image extension
# you use in the graphics folders
#--------------------------------------------------------------------------------
%.s %.h   : %.png %.grit
#--------------------------------------------------------------------------------
	grit $< -fts -o$*

-include $(DEPSDIR)/*.d

#--------------------------------------------------------------------------------
endif
#--------------------------------------------------------------------------------
