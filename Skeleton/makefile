all: clean compile link 


link: 
	g++ -g -Wall -Weffc++ -std=c++11 -o bin/main bin/MedicalWarehouse.o bin/main.o bin/Action.o bin/Beneficiary.o bin/Volunteer.o bin/SupplyRequest.o

compile: 
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Beneficiary.o src/Beneficiary.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/SupplyRequest.o src/SupplyRequest.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Volunteer.o src/Volunteer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/MedicalWarehouse.o src/MedicalWarehouse.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp


clean:
	rm -f bin/*

cleanObjAndrun:
	rm -f *.o
	clear 
	valgrind --leak-check=full --show-reachable=yes ./bin/main ./config/configFileExample.txt
	