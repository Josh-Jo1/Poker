CXX = g++
CXXFLAGS = -g -std=c++14 -Wall -MMD
EXEC = poker
OBJECTS = card.o deckOfCards.o act.o player.o humanPlayer.o round.o match.o main.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean
clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}