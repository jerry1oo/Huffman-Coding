#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include "HCNode.hpp"
#include "HCTree.hpp"
#include "BitOutputStream.hpp"

using namespace std;

void print_usage(char ** argv) {
    cout << "Usage:" << endl;
    cout << "  " << argv[0] << " INFILE OUTFILE [-b]" << endl;
    cout << "Command-line flags:" << endl;
    cout << "  -b: switch to bitwise mode" << endl;
}

/**
 * Reads infile, constructs a Huffman coding tree based on its contents,
 * and produces a compressed version in outfile.
 * For debugging purposes, uses ASCII '0' and '1' rather than bitwise I/O.
 */
void compressAscii(const string & infile, const string & outfile) {
    std::vector<int> freqs (256,0);
	HCTree FT;
	ifstream ifs;
    ifs.open(infile, ios::binary);
    if (!ifs.is_open()) {
		cerr << "no file" << endl;
		return;
	}
	char c ;
	while(ifs.get(c)){
		//for(int i = 0; i<line.length(); i++){
			
			freqs[int((unsigned char)c)]++;
		//}
	}
	ifs.close();


	FT.build(freqs);
	ofstream ofs; 
	ofs.open(outfile, ios::binary);
    if (!ofs.is_open()) {
		cerr << "no file" << endl;
		return;
	}
	ifstream ifso;
    ifso.open(infile, ios::binary);
    if (!ifso.is_open()) {
		cerr << "no file" << endl;
		return;
	}
	for(int i = 0; i<freqs.size(); i++){
		ofs << freqs[i];
		ofs << '\n';
	}
	while(ifso.get(c)){
		//char holder[line.length()+1];
		//std::strcpy(holder, line.c_str());
		//for(int i = 0; i<line.length(); i++){
			FT.encode((unsigned char)c, ofs);
		//}
	}
	ofs.close();
	ifso.close();
	
		
    cerr << "TODO: compress '" << infile << "' -> '"
        << outfile << "' here (ASCII)" << endl;
}

/*
 * Reads infile, constructs a Huffman coding tree based on its contents,
 * and produces a compressed version in outfile.
 * Uses bitwise I/O.
 */
void compressBitwise(const string & infile, const string & outfile) {
    std::vector<int> freqs (256,0);
	HCTree FT;
	ifstream ifs;
    ifs.open(infile, ios::binary);
    if (!ifs.is_open()) {
		cerr << "no file" << endl;
		return;
	}
	char c;
	while(ifs.get(c)){
		//for(int i = 0; i<line.length(); i++){
			freqs[int((unsigned char)c)]++;
		//}
	}
	ifs.close();


	FT.build(freqs);
	ofstream ofs; 
	ofs.open(outfile, ios::binary);
    if (!ofs.is_open()) {
		cerr << "no file" << endl;
		return;
	}
	ifstream ifso;
    ifso.open(infile, ios::binary);
    if (!ifso.is_open()) {
		cerr << "no file" << endl;
		return;
	}
	unsigned char split[4];
	for(int i = 0; i<freqs.size(); i++){
		split[0] = (freqs[i] >> 24) & 0xFF;
		split[1] = (freqs[i] >> 16) & 0xFF;
		split[2] = (freqs[i] >> 8) & 0xFF;
		split[3] = freqs[i] & 0xFF;
		ofs.put(split[3]);
		ofs.put(split[2]);
		ofs.put(split[1]);
		ofs.put(split[0]);
	}
	BitOutputStream out(ofs);
	while(ifso.get(c)){
		//char holder[line.length()+1];
		//std::strcpy(holder, line.c_str());
		//for(int i = 0; i<line.length(); i++){
			FT.encode((unsigned char)c, out);
		//}
	}
	if(out.Getnbits() > 0){
        for(int i = out.Getnbits(); i < 8; i++){
            out.writeBit(0);
        }
        out.flush();
    }

	ofs.close();
	ifso.close();
//ra	
    cerr << "TODO: compress '" << infile << "' -> '"
        << outfile << "' here (bitwise)" << endl;
}

int main(int argc, char ** argv) {
    string infile = "";
    string outfile = "";
    bool bitwise = false;
    for (int i = 1; i < argc; i++) {
        string currentArg = argv[i];
        if (currentArg == "-b") {
            bitwise = true;
        } else if (infile == "") {
            infile = currentArg;
        } else {
            outfile = currentArg;
        }
    }

    if (infile == "" || outfile == "") {
        cout << "ERROR: Must provide input and output files" << endl;
        print_usage(argv);
        return 1;
    }

    if (bitwise) {
        compressBitwise(infile, outfile);
    } else {
        compressAscii(infile, outfile);
    }

    return 0;
}
