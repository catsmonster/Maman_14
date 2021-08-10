cmake_minimum_required(VERSION 3.19)
project(Maman_14 C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_FLAGS -m32)
set(CMAKE_C_FLAGS -Wall)
set(CMAKE_C_FLAGS -pedantic)

add_executable(Maman_14 main.c fileNameProcessing.c fileNameProcessing.h fileErrors.h fileErrors.c fileContentProcessing.c fileContentProcessing.h listOfCommands.c listOfCommands.h directiveFunctions.c directiveFunctions.h firstIteration.c firstIteration.h dataTable.c dataTable.h inputUtils.c inputUtils.h commands.c commands.h dataImage.c dataImage.h codeImage.c codeImage.h secondIteration.c secondIteration.h entriesList.c entriesList.h printOutput.c printOutput.h)
