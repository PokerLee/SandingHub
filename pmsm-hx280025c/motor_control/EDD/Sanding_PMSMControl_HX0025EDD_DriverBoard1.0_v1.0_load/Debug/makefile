################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf *.s *.map *.asm

# All of the sources participating in the build are defined here
-include sources.mk
-include src_control/subdir.mk
-include src_board/subdir.mk
-include src/subdir.mk
-include haawking-drivers/haawking-dsc280025_edd-board/common/subdir.mk
-include haawking-drivers/haawking-dsc280025_edd-board/subdir.mk
-include device/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
Sanding_PMSMControl_HX0025EDD_DriverBoard1.0_v1.0_load.hex \

SECONDARY_SIZE += \
Sanding_PMSMControl_HX0025EDD_DriverBoard1.0_v1.0_load.siz \


# All Target
all: Sanding_PMSMControl_HX0025EDD_DriverBoard1.0_v1.0_load.elf secondary-outputs

# print-size
print-size:
	llvm-nm -n --print-size "Sanding_PMSMControl_HX0025EDD_DriverBoard1.0_v1.0_load.elf">>"Sanding_PMSMControl_HX0025EDD_DriverBoard1.0_v1.0_load.map"

# Tool invocations
Sanding_PMSMControl_HX0025EDD_DriverBoard1.0_v1.0_load.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	riscv32-haawking-elf-gcc -march=rv32imfc_xhaawkaa1p0_xhaawkab1p0_xhaawkac1p0 -D__RUNNING_IN_FLASH_ -T "D:/workspace_haawking/haawking-workspace-V2.3.3Pre/Sanding_PMSMControl_HX0025EDD_DriverBoard1.0_v1.0_load/haawking-drivers/ldscripts/HXS320F280025C_EDD_link_FLASH.ld" -mabi=ilp32f -mcmodel=medlow -mno-save-restore -menable-experimental-extensions --target=riscv32-unknown-elf --sysroot="D:/Haawking-IDE_V2.3.3Pre\haawking-tools/compiler/haawking-elf-gcc/riscv32-unknown-elf" --gcc-toolchain="D:/Haawking-IDE_V2.3.3Pre\haawking-tools/compiler/haawking-elf-gcc" -Odefault -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-inline-functions -cl-single-precision-constant -Wall -Wextra -Wl,--no-warn-rwx-segments  -g3 -gdwarf-3 -Wl,--defsym,IDE_VERSION_2_3_3Pre=0 -nostartfiles -nostdlib -Xlinker --gc-sections -L../lib -L../haawking-drivers/ldscripts -L"D:/Haawking-IDE_V2.3.3Pre/haawking-tools/segger_lib" -Wl,-Map,"Sanding_PMSMControl_HX0025EDD_DriverBoard1.0_v1.0_load.map" -o "Sanding_PMSMControl_HX0025EDD_DriverBoard1.0_v1.0_load.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	$(MAKE) --no-print-directory print-size

Sanding_PMSMControl_HX0025EDD_DriverBoard1.0_v1.0_load.hex: Sanding_PMSMControl_HX0025EDD_DriverBoard1.0_v1.0_load.elf
	@riscv32-haawking-elf-objcopy -O ihex "Sanding_PMSMControl_HX0025EDD_DriverBoard1.0_v1.0_load.elf"  "Sanding_PMSMControl_HX0025EDD_DriverBoard1.0_v1.0_load.hex"

Sanding_PMSMControl_HX0025EDD_DriverBoard1.0_v1.0_load.siz: Sanding_PMSMControl_HX0025EDD_DriverBoard1.0_v1.0_load.elf
	riscv32-haawking-elf-size --format=berkeley "Sanding_PMSMControl_HX0025EDD_DriverBoard1.0_v1.0_load.elf"

# Other Targets
clean:
	-$(RM) $(SECONDARY_LLVMASM)$(CLA_DEPS)$(SECONDARY_GCCASM)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) Sanding_PMSMControl_HX0025EDD_DriverBoard1.0_v1.0_load.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
