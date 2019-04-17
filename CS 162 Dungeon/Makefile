main:		main.cpp openspace.o level.o mazelocation.o wall.o maze.o mazeperson.o student.o TA.o instructor.o
		g++ main.cpp mazelocation.o openspace.o wall.o level.o maze.o mazeperson.o student.o TA.o instructor.o -o main

openspace.o: 	openspace.cpp openspace.hpp
		g++ -c openspace.cpp


wall.o:		wall.cpp wall.hpp
		g++ -c wall.cpp

level.o:	level.cpp level.hpp
		g++ -c level.cpp

mazelocation.o: mazelocation.cpp mazelocation.hpp
		g++ -c mazelocation.cpp

maze.o:		maze.cpp maze.hpp
		g++ -c maze.cpp

mazeperson.o: 	mazeperson.hpp mazeperson.cpp
		g++ -c mazeperson.cpp

student.o:	student.hpp student.cpp
		g++ -c student.cpp

TA.o:		TA.hpp TA.cpp
		g++ -c TA.cpp

instructor.o:	instructor.hpp instructor.cpp
		g++ -c instructor.cpp
clean: 		
		rm -f *.o main
