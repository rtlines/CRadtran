#########################################################################
# Program:
#    CRadtran
# Author:
#    Alexander Marvin
# Summary: 
#    These files are for CRadtran. Short for C++ Radative Transfer Code
#########################################################################

#########################################################################
# Building executable of CRadtran
#########################################################################
CRadtran: calculate.o CRadtran.o brightness_temp.o datafile.o oxygenAtten.o
	g++ -o CRadtran calculate.o CRadtran.o brightness_temp.o datafile.o oxygenAtten.o

#########################################################################
# Build static library
#########################################################################
static: calculate.o CRadtran.o brightness_temp.o datafile.o oxygenAtten.o
	ar rcs libCRadtran.a calculate.o CRadtran.o brightness_temp.o datafile.o oxygenAtten.o

#########################################################################
# Build testDataFile
#########################################################################
testDataFile: testDataFile.o datafile.o
	g++ -o testDataFile testDataFile.o datafile.o

#########################################################################
# Build testAtmTemp
#########################################################################
testAtmTemp: us_atm_temp.o testAtmTemp.o file.o
	g++ -o testAtmTemp testAtmTemp.o us_atm_temp.o file.o

#########################################################################
# Build test testOxyAtten
#########################################################################
testOxyAtten: testOxyAtten.o oxygenAtten.o datafile.o
	g++ -o testOxyAtten testOxyAtten.o oxygenAtten.o datafile.o

#########################################################################
# Individual files
#########################################################################
calculate.o: calculate.cpp calculate.h brightness_temp.h
	g++ -c calculate.cpp

CRadtran.o: CRadtran.cpp calculate.h
	g++ -c CRadtran.cpp

brightness_temp.o: brightness_temp.cpp brightness_temp.h
	g++ -c brightness_temp.cpp

datafile.o: datafile.cpp datafile.h
	g++ -c datafile.cpp

testDataFile.o: testDataFile.cpp datafile.h
	g++ -c testDataFile.cpp

testAtmTemp.o: testAtmTemp.cpp us_atm_temp.h
	g++ -c testAtmTemp.cpp

testOxyAtten.o: testOxyAtten.cpp atten.h
	g++ -c testOxyAtten.cpp

oxygenAtten.o: oxygenAtten.cpp atten.h
	g++ -c oxygenAtten.cpp

us_atm_temp.o: us_atm_temp.cpp us_atm_temp.h
	g++ -c us_atm_temp.cpp


#########################################################################
# General rules
#########################################################################
all: CRadtran

clean:
	rm -f *~ *.o *.a CRadtran testDataFile testOxyAtten testAtmTemp

library_static: static

package:
	tar -cf CRadtran.tar makefile *.cpp *.h TIGR0001 input3.txt

package_all:
	make clean
	rm -f CRadtran.tar
	tar -cf CRadtran.tar *
