DIR := ${/home/nikhil/Desktop}

default:
	
	g++ main.cpp -I/home/nikhil/Desktop/ppml_code/aby3 -I /home/nikhil/Desktop/ppml_code/libOTe/libOTe -I${DIR}/ppml_code/libOTe/cryptoTools -I/home/nikhil/Desktop/ppml_code/libOTe/cryptoTools/thirdparty/linux/boost -I/home/nikhil/Desktop/ppml_code/aby3/thirdparty/linux/eigen -I/home/nikhil/Desktop/ppml_code/aby3/thirdparty/linux/function2/include -L/home/nikhil/Desktop/ppml_code/aby3/lib -laby3 -L/home/nikhil/Desktop/ppml_code/libOTe/lib -llibOTe -lcryptoTools -L/home/nikhil/Desktop/ppml_code/libOTe/cryptoTools/thirdparty/linux/boost/stage/lib -lboost_system -lboost_thread -lboost_filesystem -pthread



clean: 
	rm -r main.o make.out




