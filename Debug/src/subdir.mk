################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CommandLauncher.cpp \
../src/Main.cpp \
../src/Task.cpp \
../src/TaskFeeder.cpp \
../src/TaskManager.cpp \
../src/ThreadPool.cpp \
../src/Transcoder.cpp 

OBJS += \
./src/CommandLauncher.o \
./src/Main.o \
./src/Task.o \
./src/TaskFeeder.o \
./src/TaskManager.o \
./src/ThreadPool.o \
./src/Transcoder.o 

CPP_DEPS += \
./src/CommandLauncher.d \
./src/Main.d \
./src/Task.d \
./src/TaskFeeder.d \
./src/TaskManager.d \
./src/ThreadPool.d \
./src/Transcoder.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


