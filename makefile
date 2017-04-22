LDLIBS=-lwiringPi
proove: Proove.h Proove.cpp ProoveMain.cpp
	g++ -std=c++11 -Wall -o proove Proove.h Proove.cpp ProoveMain.cpp $(LDLIBS)
