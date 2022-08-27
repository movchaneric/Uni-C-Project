project : main.o fileCheck.o label.o  fileDecoder.o  converter.o  lineCalculator.o secondPassFunc.o preProccess.o
	gcc -g -ansi -Wall -pedantic main.o fileCheck.o label.o  fileDecoder.o  converter.o  lineCalculator.o secondPassFunc.o preProccess.o -lm -o project

main.o : main.c fileCheck.h label.h fileDecoder.h  converter.h lineCalculator.h secondPassFunc.h preProccess.h
	gcc -c -ansi -Wall -pedantic main.c -o main.o

preProccess.o: preProccess.c preProccess.h
	gcc -c -ansi -Wall -pedantic preProccess.c -o preProccess.o

label.o : label.c label.h fileCheck.h
	gcc -c -ansi -Wall -pedantic label.c -o label.o

fileDecoder.o: fileDecoder.c fileDecoder.h
	gcc -c -ansi -Wall -pedantic fileDecoder.c -o fileDecoder.o

fileCheck.o: fileCheck.c fileCheck.h
	gcc -c -ansi -Wall -pedantic fileCheck.c -o fileCheck.o

converter.o: converter.c converter.h
	gcc -c -ansi -Wall -pedantic converter.c -o converter.o

lineCalculator.o: lineCalculator.c lineCalculator.h  fileCheck.h label.h converter.h
	gcc -c -ansi -Wall -pedantic lineCalculator.c -o lineCalculator.o

secondPassFunc.o: secondPassFunc.c fileCheck.h label.h  fileDecoder.h converter.h  lineCalculator.h secondPassFunc.h
	gcc -c -ansi -Wall -pedantic secondPassFunc.c -o secondPassFunc.o
