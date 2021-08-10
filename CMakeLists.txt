cmake_minimum_required(VERSION 3.19)
project(Maman_14 C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_FLAGS -m32)
set(CMAKE_C_FLAGS -Wall)
set(CMAKE_C_FLAGS -pedantic)

add_executable(Maman_14 main.c processInput/fileNameProcessing.c includes/fileNameProcessing.h includes/fileErrors.h errorHandling/fileErrors.c processInput/fileContentProcessing.c includes/fileContentProcessing.h dataStructures/listOfCommands.c includes/listOfCommands.h processInput/directiveFunctions.c includes/directiveFunctions.h processInput/firstIteration.c includes/firstIteration.h dataStructures/dataTable.c includes/dataTable.h processInput/inputUtils.c includes/inputUtils.h processInput/commands.c includes/commands.h dataStructures/dataImage.c includes/dataImage.h dataStructures/codeImage.c includes/codeImage.h processInput/secondIteration.c includes/secondIteration.h dataStructures/entriesList.c includes/entriesList.h print/printOutput.c includes/printOutput.h)
