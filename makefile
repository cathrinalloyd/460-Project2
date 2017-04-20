P2.out : Project2.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o
	g++ -g -o P2.out Project2.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o

Project2.o : Project2.cpp SetLimits.h SyntacticalAnalyzer.h
	g++ -g -c Project2.cpp

SetLimits.o : SetLimits.cpp SetLimits.h
	g++ -g -c SetLimits.cpp

LexicalAnalyzer.o : LexicalAnalyzer.save
	cp LexicalAnalyzer.save LexicalAnalyzer.o

SyntacticalAnalyzer.o : SyntacticalAnalyzer.cpp SyntacticalAnalyzer.h LexicalAnalyzer.h
	g++ -g -c SyntacticalAnalyzer.cpp

clean : 
	rm *.o P2.out *.gch

submit : Project2.cpp LexicalAnalyzer.h LexicalAnalyzer.save SyntacticalAnalyzer.h SyntacticalAnalyzer.cpp makefile README.txt
	rm -rf TeamPP2
	mkdir TeamPP2
	cp Project2.cpp TeamPP2
	cp LexicalAnalyzer.h TeamPP2
	cp LexicalAnalyzer.save TeamPP2
	cp SyntacticalAnalyzer.h TeamPP2
	cp SyntacticalAnalyzer.cpp TeamPP2
	cp makefile TeamPP2
	cp README.txt TeamPP2
	tar cfvz TeamPP2.tgz TeamPP2
	cp TeamPP2.tgz ~tiawatts/cs460drop
