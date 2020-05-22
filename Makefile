DIR=$(CURDIR)

default: 
	
	g++ main.cpp sh4/*.cpp -I${DIR}/sh4 -I${DIR}/Common -I${DIR}/../libOTe/libOTe -I${DIR}/../libOTe -I${DIR}/../libOTe/cryptoTools -I${DIR}/../libOTe/cryptoTools/thirdparty/linux/boost -I${DIR}/thirdparty/linux/eigen -I${DIR}/thirdparty/linux/function2/include  -L${DIR}/../libOTe/lib -llibOTe -lcryptoTools -L${DIR}/../libOTe/cryptoTools/thirdparty/linux/boost/stage/lib -lboost_system -lboost_thread -lboost_filesystem -pthread



clean: 
	rm -r main.o make.out




