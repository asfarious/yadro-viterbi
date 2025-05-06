# YADRO Viterbi decoder test task

CC = g++
CFLAGS = -Wall

SRC_PATH = src
BUILD_PATH = bin
TARGET = viterbi
VPATH = src

OBJ = main.o fsm.o convcode.o viterbi.o
FILE_OBJ = $(addprefix ${BUILD_PATH}/, ${OBJ})

${TARGET}: ${FILE_OBJ}
	${CC} -o ${TARGET} ${FILE_OBJ}
${BUILD_PATH}/%.o: %.cpp
	${CC} ${CFLAGS} -o $@ -c $<
clean:
	rm -f $(BUILD_PATH)/*.o
