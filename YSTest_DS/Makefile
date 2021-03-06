# DS Makefile for YSTest by FrankHB 2009 - 2013
# CodePage = ANSI / GBK
# CTime = 2009-11-12 21:26:30 +0800
# UTime = 2013-02-19 05:41 +0800
# Version = r1079

# This makefile is written based on devkitPro example template.

#----
.SUFFIXES:
#----
export PLATFORM		?= devkitARM

ifeq ($(PLATFORM),devkitARM)

	ifeq ($(strip $(DEVKITARM)),)
		$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
	endif

	include $(DEVKITARM)/ds_rules

endif

export TOPDIR		?=	$(CURDIR)/..

#----

export TARGET		:=	YSTest
#export TARGET		:=	$(shell basename $(CURDIR))
export BUILD		?=	release
export RELEASE		=	$(BUILD)
export GBUILD		=	$(CURDIR)/$(BUILD)/$(TARGET)
export OUTPUT		=	$(CURDIR)/$(RELEASE)/$(TARGET)

#----
# Be sure to change these default banner TEXTs. This is the name your project will
# display in the DS menu (including some flash cards). Create a custom LOGO.BMP
# icon for your project, too!
#----

TITLE		:= YSTest
SUBTITLE1 	:= FrankHB
SUBTITLE2 	:= 2009 - 2013
ICON		:= -b $(CURDIR)/data/logo.bmp

#----
# If you're using EFS_lib, uncomment "USE_EFS = YES" here.
#----
#USE_EFS = YES

PROGNAME = YSTest
OFILES +=
ADD_LIBS +=

ARM7OBJ := $(TOPDIR)/$(TARGET)_ARM7/$(BUILD)/$(TARGET)_ARM7.arm7
ARM9OBJ := $(TOPDIR)/$(TARGET)_ARM9/$(BUILD)/$(TARGET)_ARM9.arm9

.PHONY: $(BUILD) clean rebuild

#----
# main targets
#----
all : $(BUILD) $(OUTPUT).nds
#----

%.ds.gba: %.nds
	@dsbuild $<
	@echo Built: $(notdir $(OUTPUT)).nds .
	@echo Built: $(notdir $@) .
#	@cp $(CURDIR)/$(RELEASE)/$(notdir $@) $(CURDIR)/$(RELEASE)/$(notdir $(OUTPUT)).sc.nds
#	@echo Built: $(notdir $(OUTPUT)).sc.nds .
ifeq ($(strip $(USE_EFS)), YES)
	@$(CURDIR)/../efs $(OUTPUT).nds
#	@$(CURDIR)/../efs $(OUTPUT).ds.gba
#	@$(CURDIR)/../efs $(OUTPUT).sc.nds
endif

$(OUTPUT).nds	:	$(ARM9OBJ)
	@echo Building $(notdir $@) ...
ifeq ($(strip $(USE_EFS)), YES)
	ndstool -c $(OUTPUT).nds -7 $(ARM7OBJ) -9 $(ARM9OBJ) $(ICON) "$(TITLE);$(SUBTITLE1);$(SUBTITLE2)" -d $(TOPDIR)/efsroot
	$(TOPDIR)/efs.exe $(OUTPUT).nds
else
	ndstool -c $(OUTPUT).nds -7 $(ARM7OBJ) -9 $(ARM9OBJ) $(ICON) "$(TITLE);$(SUBTITLE1);$(SUBTITLE2)"
#	@cp $(OUTPUT).nds $(OUTPUT)_.nds
endif
	@echo Built $(notdir $@) .

$(BUILD):
# Before compiling, show any warning messages that might have detected
ifneq ($(strip $(TEXT_TO_SAY_1)),)
ifeq ($(strip $(MSG_IS_ERROR)),)
	@echo " ===================================="
	@echo "/ YShell Project Compilation Warning /"
	@echo "==================================== "
else
	@echo " =================================="
	@echo "/ YShell Project Compilation Error /"
	@echo "================================== "
endif
	@echo.
	@echo $(TEXT_TO_SAY_1)
endif
ifneq ($(strip $(TEXT_TO_SAY_2)),)
	@echo $(TEXT_TO_SAY_2)
endif
ifneq ($(strip $(TEXT_TO_SAY_3)),)
	@echo $(TEXT_TO_SAY_3)
endif
ifneq ($(strip $(TEXT_TO_SAY_4)),)
	@echo $(TEXT_TO_SAY_4)
endif
ifneq ($(strip $(NL_AND_PAUSE)),)
	@echo.
ifeq ($(strip $(MSG_IS_ERROR)),)
	@read -n 1 -p "Press any key to continue the compilation..."
	@echo.
else
	@read -n 1 -p "Press any key to exit..."
endif
	@echo.
endif
ifeq ($(strip $(MSG_IS_ERROR)),)
	@[ -d $(CURDIR)/$(RELEASE) ] || mkdir -p $(CURDIR)/$(RELEASE)
	@[ -d $(CURDIR)/$(BUILD) ] || mkdir -p $(CURDIR)/$(BUILD)
	@[ -d $@ ] || mkdir -p $@
#	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile
endif

clean:
	@echo Cleaning releases ...
	@rm -f $(BUILD)/*.d
	@rm -f $(BUILD)/*.o
	@rm -f $(RELEASE)/*.*ds*
	@echo Cleaned.

#----

rebuild: all

