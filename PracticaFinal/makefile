# Pablo Antonio Bernabeu Cartagena 20083111V

OPTS = -Wall -w -O3 -std=c++0x

potter_bb: potter_bb.cc
	g++ ${OPTS} -o potter_bb potter_bb.cc

tar: potter_bb.cc makefile
	tar cvfz 20083111V.tar.gz potter_bb.cc makefile

clean:
	 -rm potter_bb
