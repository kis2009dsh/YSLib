#
#	Copyright by FrankHB 2009 - 2013.
#
#	This file is part of the YSLib project, and may only be used,
#	modified, and distributed under the terms of the YSLib project
#	license, LICENSE.TXT.  By continuing to use, modify, or distribute
#	this file you indicate that you have read the license and
#	understand and accept it fully.
#
# DS Makefile for YShell Test -> ARM9
# Version = r1288
# Created = 2009-12-18 12:27:40 +0800
# Updated = 2013-07-23 20:10 +0800
# Encoding = ANSI


# This makefile is written based on PALib Makefile by Scognito and Tom and crash.

#----
.SUFFIXES:
#----
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/ds_rules

export TOPDIR		?=	$(CURDIR)

#----
# TARGET is the name of the file to output
# BUILD is the directory where object files and intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
# DATA is a list of directories containing binary files embedded using bin2o
# GRAPHICS is a list of directories containing image files to be converted with grit
#----

export TARGET		:= $(shell basename $(CURDIR))
export BUILD		?= release
export RELEASE		= $(BUILD)
export SOURCES		:= source data
export INCLUDES		:= include data ../YSTest/include \
	../YFramework/include ../YFramework/DS/include \
	../3rdparty/include ../YBase/include
export DATA			:= data
#export GRAPHICS	:= gfx
#export MUSIC		:= music

#----
# options for code generation
#----
ARCH_AS	:=	-mthumb -mthumb-interwork -march=armv5te
ARCH	:=	$(ARCH_AS) -mtune=arm946e-s


# note: arm9tdmi isn't the correct CPU arch, but anything newer and LD
# *insists* it has a FPU or VFP, and it won't take no for an answer!

ASFLAGS	:=	$(ARCH) -mfpu=softfpa -mfpu=softvfp

CFLAGS	:=	-Wall -Wextra \
 			-Wmissing-declarations -Winvalid-pch \
 			-flto=jobserver \
 			-ffast-math \
 			$(ARCH)

CFLAGS	+=	$(INCLUDE) -DARM9
LDFLAGS :=	-specs=ds_arm9.specs $(ARCH) -Wl,-Map,$(notdir $*.map)

ifneq ($(BUILD),debug)
CFLAGS	+=	-O3 -fomit-frame-pointer -DNDEBUG
else
ASFLAGS	+=	-g
CFLAGS	+=	-g -O0 -fno-omit-frame-pointer  -D_GLIBCXX_DEBUG
LDFLAGS	+=	-g
endif

CXXFLAGS	:=	$(CFLAGS) -fno-threadsafe-statics -fuse-cxa-atexit \
 			-std=c++11 -Wctor-dtor-privacy -Wsign-promo
# 			-Wctor-dtor-privacy -Wnoexcept -Wold-style-cast -Wsign-promo

#CFLAGS	:=	-g -Wformat=2 -Winline -Wall \
#		-mcpu=arm946e-s -mtune=arm946e-s -fomit-frame-pointer \
#		-ffast-math \
#		$(ARCH)
#CFLAGS	+=	$(INCLUDE) -I$(PAPATH)/include/nds
#CXXFLAGS	:=	$(CFLAGS) -frepo -fno-threadsafe-statics -fno-rtti -fno-exceptions
#CXXFLAGS	:=	$(CFLAGS) -fno-threadsafe-statics -fuse-cxa-atexit -std=gnu++0x
#	-Weffc++ -Wold-style-cast -Woverloaded-virtual -pedantic
#	-Wextra -Winit-self -Wmissing-include-dirs -Wsign-promo

#LDFLAGS	:=	-specs=ds_arm9.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)
#LDFLAGS=	-specs=ds_arm9.specs -g $(ARCH) -L$(PAPATH)/lib -Wl,-Map,$(notdir $*.map)

#----
# any extra libraries we wish to link with the project
#----
LIBS	:=	-lfat -lnds9

#---- User defined libraries.

LIBPATHEX	:= -L$(TOPDIR)/../YFramework/DS/lib
LIBEX		:= -lFreeImage -lfreetype #-lagg -lloki

YBASE_A := $(TOPDIR)/../YBase/DS/$(RELEASE)/libYBase.a
YFRAMEWORK_A := $(TOPDIR)/../YFramework/DS/$(RELEASE)/libYFramework.a

#----

#----
# list of directories containing libraries, this must be the top level containing
# include and lib
#----
LIBDIRS	:=	$(LIBNDS)

#----
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#----
ifneq ($(BUILD),$(notdir $(CURDIR)))
#----

export ARM9BIN	:=	$(CURDIR)/$(RELEASE)/$(TARGET).arm9
export ARM9ELF	:=	$(CURDIR)/$(BUILD)/$(TARGET).arm9.elf
export DEPSDIR	:=	$(CURDIR)/$(BUILD)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir))

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

#----
# use CXX for linking C++ projects, CC for standard C
#----
ifeq ($(strip $(CPPFILES)),)
	export LD	:=	$(CC)
else
	export LD	:=	$(CXX)
endif

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
					-I$(CURDIR)/$(BUILD)
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) rebuild clean

#----
$(BUILD):
	@echo Building configuration: $(notdir $@) ...
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile
	@echo Built configuration: $(notdir $@) .

#----
clean:
	@echo Cleaning $(TARGET) ...
	@rm -f $(BUILD)/.map
	@rm -f $(BUILD)/*.d
	@rm -f $(BUILD)/*.o
	@rm -f $(ARM9BIN)
	@rm -f $(ARM9ELF)

rebuild: all

all: $(BUILD) $(OUTPUT)

#----
else

DEPENDS	:= $(OFILES:.o=.d)

#----
# main targets
#----

$(ARM9BIN)	:	$(ARM9ELF)
	@$(OBJCOPY) -O binary $< $@
	@echo Built $(notdir $@) .

$(ARM9ELF)	:	$(OFILES)
	@echo Linking $(notdir $@) ...
	$(LD) $(LDFLAGS) $(OFILES) $(YFRAMEWORK_A) $(YBASE_A) $(LIBPATHS) $(LIBS) $(LIBPATHEX) $(LIBEX) -o $@
# IMPORTANT: the order of files is significant.

#----
# you need a rule like this for each extension you use as binary data
#----

#----
# Compile Targets for C/C++
#----

#----
%.o : %.cpp
	@echo $(notdir $<)
	@$(CXX) -MM $(CXXFLAGS) -o $*.d $<
	@$(CXX) $(CXXFLAGS) -c $< -o$@

#----
%.o : %.c
	@echo $(notdir $<)
	@$(CC) -MM $(CFLAGS) -o $*.d $<
	@$(CC)  $(CFLAGS) -c $< -o$@

#----
%.o : %.s
	@echo $(notdir $<)
	@$(CC) -MM $(CFLAGS) -o $*.d $<
	@$(CC)  $(ASFLAGS) -c $< -o$@


# bin2o macro like that from libnds, slightly changed for the PALib naming conventions of resources which doesn't append the file extension to the resource name
define bin2o
	cp $(<) $(*)
	bin2s $(*) | $(AS) $(ARCH_AS) -o $(@)
	rm $(*)

	echo "extern const u8" $(*)"[];" > $(*).h
	echo "extern const u32" $(*)_size";" >> $(*).h
endef

#----
%.o	:	%.mp3
#----
	@echo $(notdir $<)
	@$(bin2o)

#----
%.o	:	%.pcx
#----
	@echo $(notdir $<)
	@$(bin2o)

#----
%.o	:	%.bin
#----
	@echo $(notdir $<)
	@$(bin2o)

#----
%.o	:	%.png
#----
	@echo $(notdir $<)
	@$(bin2o)

#----
%.o	:	%.raw
#----
	@echo $(notdir $<)
	@$(bin2o)

#----
%.o	:	%.pal
#----
	@echo $(notdir $<)
	@$(bin2o)

#----
%.o	:	%.map
#----
	@echo $(notdir $<)
	@$(bin2o)

#----
%.o	:	%.mdl
#----
	@echo $(notdir $<)
	@$(bin2o)

#----
%.o	:	%.jpg
#----
	@echo $(notdir $<)
	@$(bin2o)

#----
%.o	:	%.mod
#----
	@echo $(notdir $<)
	@$(bin2o)

#----
%.o	:	%.gif
#----
	@echo $(notdir $<)
	@$(bin2o)

#----
%.o	:	%.bmp
#----
	@echo $(notdir $<)
	@$(bin2o)

#----
# This rule creates assembly source files using grit
# grit takes an image file and a .grit describing how the file is to be processed
# add additional rules like this for each image extension
# you use in the graphics folders
#----
%.s %.h   : %.png %.grit
#----
	grit $< -fts -o$*

-include $(DEPSDIR)/*.d

#----
endif
#----

