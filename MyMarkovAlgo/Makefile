CXX = g++
CXXFLAGS = -WAll -g 

server : main.cpp markov.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

clean :
	rm -f *.o server