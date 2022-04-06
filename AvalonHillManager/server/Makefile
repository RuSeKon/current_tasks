CXX = g++
CXXFLAGS = -Wall -g
OBJMODULES = GameSession.o GameServer.o EventSelector.o \
			 Errproc.o Manager.o Banker.o Game.o

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

server : main.cpp $(OBJMODULES)
	$(CXX) $(CXXFLAGS) $^ -o $@

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif

deps.mk: $(OBJMODULES:.o=.cpp)
	$(CXX) -MM $^ > $@

clean :
	rm -f *.o server deps.mk

run_tst :
	./server 4878
