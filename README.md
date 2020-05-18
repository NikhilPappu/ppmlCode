# Semi Honest 4PC Q2 protocol 

## Installation
 
```
git clone --recursive https://github.com/osu-crypto/libOTe.git
cd libOTe/cryptoTools/thirdparty/linux
bash boost.get
cd ../../..
cmake . -DENABLE_CIRCUITS=ON
make -j
cd ../
git clone https://github.com/NikhilPappu/ppmlCode.git
cd ppmlCode/thirdparty/linux
bash eigen.get
bash function2.get
cd ../..
```
Now, modify the `${DIR}` variable in the Makefile to the path of the parent directory of the code.

Then, run

```
make
./a.out
```
