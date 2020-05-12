DIR=/home/nickpappu/Desktop/ppml_code

default: 
	
	g++ main.cpp sh4/*.cpp -I${DIR}/my_code/sh4 -I${DIR}/my_code/Common -I${DIR}/aby3 -I${DIR}/libOTe/libOTe -I${DIR}/libOTe -I${DIR}/libOTe/cryptoTools -I${DIR}/libOTe/cryptoTools/thirdparty/linux/boost -I${DIR}/aby4/thirdparty/linux/eigen -I${DIR}/aby3/thirdparty/linux/function2/include -L${DIR}/aby3/lib -laby3 -L${DIR}/libOTe/lib -llibOTe -lcryptoTools -L${DIR}/libOTe/cryptoTools/thirdparty/linux/boost/stage/lib -lboost_system -lboost_thread -lboost_filesystem -pthread



clean: 
	rm -r main.o make.out




