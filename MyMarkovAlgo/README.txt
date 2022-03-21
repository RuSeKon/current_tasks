#This is the Markov's chain algorithm implementation without STL.
#(except std::string). The main purpose is create user-defined classes
#for work with algoithm.

#Compilation key for preprocessing:
#
#MAXGEN - maximum size(in letter) of generated text by algorithm / default 1000;
#
#NPREF - prefix size(in letter) / default 2;
#
#TABSIZE - size of table for store letters combinations (the size of the table determines 
#the performance on large input data) / default 4096;
#
#DEBUG - define during compilation for turn on debug print. If you change the value to 2 or more, also 
# enable printing of each Point during creation table elements;




