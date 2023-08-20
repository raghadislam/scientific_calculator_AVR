################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include application/subdir.mk
-include ATmega32_CTOS/SERVICE/subdir.mk
-include ATmega32_CTOS/MCAL/DIO/subdir.mk
-include ATmega32_CTOS/HAL/keypad/subdir.mk
-include ATmega32_CTOS/HAL/LCD/subdir.mk
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
scientific_calculator.lss \

FLASH_IMAGE += \
scientific_calculator.hex \

SIZEDUMMY += \
sizedummy \


# All Target
all: scientific_calculator.elf secondary-outputs

# Tool invocations
scientific_calculator.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,scientific_calculator.map -mmcu=atmega32 -o "scientific_calculator.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

scientific_calculator.lss: scientific_calculator.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S scientific_calculator.elf  >"scientific_calculator.lss"
	@echo 'Finished building: $@'
	@echo ' '

scientific_calculator.hex: scientific_calculator.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex scientific_calculator.elf  "scientific_calculator.hex"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: scientific_calculator.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 scientific_calculator.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) scientific_calculator.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
