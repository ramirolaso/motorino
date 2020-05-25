################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../jump.c \
../motorino_basics.c \
../motorino_collisions.c \
../motorino_world.c 

OBJS += \
./jump.o \
./motorino_basics.o \
./motorino_collisions.o \
./motorino_world.o 

C_DEPS += \
./jump.d \
./motorino_basics.d \
./motorino_collisions.d \
./motorino_world.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


