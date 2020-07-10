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
BLD_App.lss \

FLASH_IMAGE += \
BLD_App.hex \

EEPROM_IMAGE += \
BLD_App.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: BLD_App.elf secondary-outputs

# Tool invocations
BLD_App.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,BLD_App.map -mmcu=atmega32 -o "BLD_App.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

BLD_App.lss: BLD_App.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S BLD_App.elf  >"BLD_App.lss"
	@echo 'Finished building: $@'
	@echo ' '

BLD_App.hex: BLD_App.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex BLD_App.elf  "BLD_App.hex"
	@echo 'Finished building: $@'
	@echo ' '

BLD_App.eep: BLD_App.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex BLD_App.elf  "BLD_App.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: BLD_App.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 BLD_App.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) BLD_App.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
