################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Entities/Grid.cpp \
../Entities/GridPoint.cpp \
../Entities/IsoPolygon.cpp \
../Entities/Point.cpp \
../Entities/Segment.cpp 

OBJS += \
./Entities/Grid.o \
./Entities/GridPoint.o \
./Entities/IsoPolygon.o \
./Entities/Point.o \
./Entities/Segment.o 

CPP_DEPS += \
./Entities/Grid.d \
./Entities/GridPoint.d \
./Entities/IsoPolygon.d \
./Entities/Point.d \
./Entities/Segment.d 


# Each subdirectory must supply rules for building sources it contributes
Entities/%.o: ../Entities/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	/usr/local/cuda-7.5/bin/nvcc -G -g -O0 -gencode arch=compute_20,code=sm_20  -odir "Entities" -M -o "$(@:%.o=%.d)" "$<"
	/usr/local/cuda-7.5/bin/nvcc -G -g -O0 --compile  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


