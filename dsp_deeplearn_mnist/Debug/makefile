################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := F:/ti/ccsv6/tools/compiler/ti-cgt-c6000_8.0.4

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./DSPF_sp_dotprod.obj" \
"./DSPF_sp_maxval.obj" \
"./DSPF_sp_vec.obj" \
"./DSP_add16.obj" \
"./DSP_maxval.obj" \
"./IMG_conv_11x11_i16s_c16s.obj" \
"./IMG_conv_5x5_i16s_c16s.obj" \
"./IMG_conv_7x7_i16s_c16s.obj" \
"./classifier.obj" \
"./deep_learning.obj" \
"./main.obj" \
"../C6678_unified.cmd" \
$(GEN_CMDS__FLAG) \
-l"libc.a" \
-l"F:\ti\pdk_C6678_1_1_2_6\packages\ti\csl\lib\ti.csl.ae66" \

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
dsp_deeplearn_mnist.out \

EXE_OUTPUTS__QUOTED += \
"dsp_deeplearn_mnist.out" \

BIN_OUTPUTS += \
dsp_deeplearn_mnist.hex \

BIN_OUTPUTS__QUOTED += \
"dsp_deeplearn_mnist.hex" \


# All Target
all: dsp_deeplearn_mnist.out

# Tool invocations
dsp_deeplearn_mnist.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C6000 Linker'
	"F:/ti/ccsv6/tools/compiler/ti-cgt-c6000_8.0.4/bin/cl6x" -mv6600 --abi=eabi -O3 --opt_for_speed=5 -g --define=__KEYSTONE2 --display_error_number --diag_wrap=off --diag_warning=225 -z -m"dsp_deeplearn_mnist.map" --stack_size=0x1024 --heap_size=0x1024 -i"F:/ti/ccsv6/tools/compiler/ti-cgt-c6000_8.0.4/lib" -i"F:/ti/ccsv6/tools/compiler/ti-cgt-c6000_8.0.4/include" --reread_libs --display_error_number --diag_wrap=off --warn_sections --xml_link_info="dsp_deeplearn_mnist_linkInfo.xml" --rom_model -o "dsp_deeplearn_mnist.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

dsp_deeplearn_mnist.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: C6000 Hex Utility'
	"F:/ti/ccsv6/tools/compiler/ti-cgt-c6000_8.0.4/bin/hex6x"  -o "dsp_deeplearn_mnist.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "DSPF_sp_dotprod.pp" "DSPF_sp_maxval.pp" "DSPF_sp_vec.pp" "DSP_add16.pp" "DSP_maxval.pp" "IMG_conv_11x11_i16s_c16s.pp" "IMG_conv_5x5_i16s_c16s.pp" "IMG_conv_7x7_i16s_c16s.pp" "classifier.pp" "deep_learning.pp" "main.pp" 
	-$(RM) "DSPF_sp_dotprod.obj" "DSPF_sp_maxval.obj" "DSPF_sp_vec.obj" "DSP_add16.obj" "DSP_maxval.obj" "IMG_conv_11x11_i16s_c16s.obj" "IMG_conv_5x5_i16s_c16s.obj" "IMG_conv_7x7_i16s_c16s.obj" "classifier.obj" "deep_learning.obj" "main.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

