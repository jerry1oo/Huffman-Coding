#include "BitOutputStream.hpp"


BitOutputStream::BitOutputStream(ostream & o) : out(o), nbits(0), buf(0) {}

void BitOutputStream::writeBit(bool bit) {
    if(nbits == 8){
		flush();
	}
	if(bit){
		buf |= 1 << (7-nbits);
	}
	else{
		buf &= ~(1 << (7-nbits));
	}
	nbits++;
}

void BitOutputStream::flush() {
    out.put(buf);
	buf = 0;
	nbits = 0;
}

int BitOutputStream::Getnbits(){
	return nbits;
}
