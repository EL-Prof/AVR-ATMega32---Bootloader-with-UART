################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
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
LSS += \
BLD_FlashingTool.lss \

FLASH_IMAGE += \
BLD_FlashingTool.hex \

EEPROM_IMAGE += \
BLD_FlashingTool.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: BLD_FlashingTool.elf secondary-outputs

# Tool invocations
BLD_FlashingTool.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,BLD_FlashingTool.map -Ttext=0x7000 -mmcu=atmega32 -o "BLD_FlashingTool.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

BLD_FlashingTool.lss: BLD_FlashingTool.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S BLD_FlashingTool.elf  >"BLD_FlashingTool.lss"
	@echo 'Finished building: $@'
	@echo ' '

BLD_FlashingTool.hex: BLD_FlashingTool.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex BLD_FlashingTool.elf  "BLD_FlashingTool.hex"
	@echo 'Finished building: $@'
	@echo ' '

BLD_FlashingTool.eep: BLD_FlashingTool.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex BLD_FlashingTool.elf  "BLD_FlashingTool.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: BLD_FlashingTool.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 BLD_FlashingTool.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) BLD_FlashingTool.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
