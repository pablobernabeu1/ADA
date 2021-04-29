# Pablo Antonio Bernabeu Cartagena 20083111V

OPTS = -Wall -O3 -std=c++0x

potter_bt: potter_bt.cc
	g++ ${OPTS} -o potter_bt potter_bt.cc

tar: potter_bt.cc makefile
	tar cvfz 20083111V.tar.gz potter_bt.cc makefile

clean:
	 -rm potter_bt
