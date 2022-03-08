OPTCPP = -std=c++11 -O3 -DB_64 -I.
COMPCPP = g++

main: main.cpp
	$(COMPCPP) $(OPTCPP) main.cpp -o main
