EDTSurf : ParsePDB.o ProteinSurface.o EDTSurf.o
	g++ ParsePDB.o ProteinSurface.o EDTSurf.o -o EDTSurf
	rm *.o
ProteinSurface.o : ProteinSurface.cpp
	g++ -c ProteinSurface.cpp -o ProteinSurface.o
ParsePDB.o : ParsePDB.cpp
	g++ -c ParsePDB.cpp -o ParsePDB.o
EDTSurf.o : EDTSurf.cpp
	g++ -c EDTSurf.cpp -o EDTSurf.o