################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := F:/ti/ccsv6/tools/compiler/c6000_7.4.12

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./DSP_add16.obj" \
"./DSP_maxval.obj" \
"./IMG_conv_5x5_i16s_c16s_c.obj" \
"./IMG_conv_7x7_i16s_c16s_c.obj" \
"./boot.obj" \
"./deep_learning.obj" \
"./main.obj" \
"../lnk.cmd" \
$(GEN_CMDS__FLAG) \
-l"libc.a" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
deeplearn_evm5432_teradeep.out \

EXE_OUTPUTS__QUOTED += \
"deeplearn_evm5432_teradeep.out" \

BIN_OUTPUTS += \
deeplearn_evm5432_teradeep.hex \

BIN_OUTPUTS__QUOTED += \
"deeplearn_evm5432_teradeep.hex" \


# All Target
all: deeplearn_evm5432_teradeep.out

# Tool invocations
deeplearn_evm5432_teradeep.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C6000 Linker'
	"F:/ti/ccsv6/tools/compiler/c6000_7.4.12/bin/cl6x" -mvtesla --abi=eabi -O3 -g --define=_TMS320C6400_PLUS --display_error_number --diag_warning=225 --diag_wrap=off -z --stack_size=0x1000 -m"deeplearn_evm5432_teradeep.map" --heap_size=0x8000000 -i"F:/ti/ccsv6/tools/compiler/c6000_7.4.12/lib" -i"F:/ti/ccsv6/tools/compiler/c6000_7.4.12/include" --reread_libs --warn_sections --display_error_number --diag_wrap=off --xml_link_info="deeplearn_evm5432_teradeep_linkInfo.xml" --rom_model -o "deeplearn_evm5432_teradeep.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

deeplearn_evm5432_teradeep.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: C6000 Hex Utility'
	"F:/ti/ccsv6/tools/compiler/c6000_7.4.12/bin/hex6x"  -o "deeplearn_evm5432_teradeep.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "DSP_add16.pp" "DSP_maxval.pp" "IMG_conv_5x5_i16s_c16s_c.pp" "IMG_conv_7x7_i16s_c16s_c.pp" "deep_learning.pp" "main.pp" 
	-$(RM) "DSP_add16.obj" "DSP_maxval.obj" "IMG_conv_5x5_i16s_c16s_c.obj" "IMG_conv_7x7_i16s_c16s_c.obj" "boot.obj" "deep_learning.obj" "main.obj" 
	-$(RM) "boot.pp" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

