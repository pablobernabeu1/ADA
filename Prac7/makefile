# Pablo Antonio Bernabeu Cartagena 20083111V

OPTS = -Wall -O3 -std=c++0x

potter_greedy: potter_greedy.cc
	g++ ${OPTS} -o potter_greedy potter_greedy.cc

tar: potter_greedy.cc makefile
	tar cvfz 20083111V.tar.gz potter_greedy.cc makefile

clean:
	 -rm potter_greedy
