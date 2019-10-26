#include "BitInputStream.hpp"


BitInputStream::BitInputStream(istream & i) : in(i), nbits(8), buf(0) {}

bool BitInputStream::readBit() {
	if(nbits == 8){
		buf = in.get();
		nbits = 0;
	}
	unsigned int getBit = ((buf >> (7-nbits)) & 1);
	nbits++;
    return getBit;
}
