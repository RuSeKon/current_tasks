CXX = g++
CXXFLAGS = -Wall -g
OBJMODULES = client.o EventSelector.o errproc.o

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

client : main.cpp $(OBJMODULES)
	$(CXX) $(CXXFLAGS) $^ -o $@

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif

deps.mk : $(OBJMODULES:.o=.cpp)
	$(CXX) -MM $^ > $@

clean: 
	rm -f *.o client deps.mk

run_tst :
	./client 127.0.0.1 4878
