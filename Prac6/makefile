# Pablo Antonio Bernabeu Cartagena 20083111V

OPTS = -Wall -O3 -std=c++0x

potter: potter.cc
	g++ ${OPTS} -o potter potter.cc

tar: potter.cc makefile
	tar cvfz 20083111V.tar.gz potter.cc makefile

clean:
	 -rm potter
