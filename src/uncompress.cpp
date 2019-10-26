#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdint.h>

#include "HCNode.hpp"
#include "HCTree.hpp"
#include "BitInputStream.hpp"

using namespace std;

void print_usage(char ** argv) {
    cout << "Usage:" << endl;
    cout << "  " << argv[0] << " INFILE OUTFILE [-b]" << endl;
    cout << "Command-line flags:" << endl;
    cout << "  -b: switch to bitwise mode" << endl;
}

/**
 * Reads infile, constructs a Huffman coding tree based on its header,
 * and produces an uncompressed version in outfile.
 * For debugging purposes, uses ASCII '0' and '1' rather than bitwise I/O.
 */
void uncompressAscii(const string & infile, const string & outfile) {
    vector<int> freqs(256,0);
	HCTree FT;
    ifstream ifs;
    ifs.open(infile, ios::binary);
    if (!ifs.is_open()) {
        cerr << "no file" << endl;
        return;
    }
	int totalS = 0;
    string line;
    for(int i = 0; i<freqs.size(); i++){
    	//getline(ifs,line);
		//freqs[i] = atoi(line.c_str());
		ifs >> freqs[i];
		totalS += freqs[i];	
	}
	FT.build(freqs); 
	ofstream ofs;
    ofs.open(outfile, ios::binary);
    if (!ofs.is_open()) {
        cerr << "no file" << endl;
        return;
    }
	for(int i = 0; i < totalS; i++){
		ofs << FT.decode(ifs);
	}
	ifs.close();
	ofs.close();
    cerr << "TODO: uncompress '" << infile << "' -> '"
        << outfile << "' here (ASCII)" << endl;
}

/**
 * Reads infile, constructs a Huffman coding tree based on its header,
 * and produces an uncompressed version in outfile.
 * Uses bitwise I/O.
 */
void uncompressBitwise(const string & infile, const string & outfile) {
    vector<int> freqs(256,0);
	HCTree FT;
    ifstream ifs;
    ifs.open(infile, ios::binary);
    if (!ifs.is_open()) {
        cerr << "no file" << endl;
        return;
    }
	unsigned int totalS = 0;
    string line;
	unsigned char split[4];
    for(int i = 0; i < freqs.size(); i++){
    	//getline(ifs,line);
		//freqs[i] = atoi(line.c_str());
		split[0] = (unsigned char)ifs.get(); 
		split[1] = (unsigned char)ifs.get(); 
		split[2] = (unsigned char)ifs.get(); 
		split[3] = (unsigned char)ifs.get(); 
	//	ifs >> split[0]; 
	//	ifs >> split[1]; 
	//	ifs >> split[2]; 
	//	ifs >> split[3]; 
		freqs[i] = *(int *)split;
		cerr << (unsigned int)freqs[i] << endl;
		totalS += freqs[i];	
	}
	FT.build(freqs); 
	ofstream ofs;
    ofs.open(outfile, ios::binary);
    if (!ofs.is_open()) {
        cerr << "no file" << endl;
        return;
    }
	BitInputStream in(ifs);
	for(int i = 0; i < totalS; i++){
		ofs << FT.decode(in);
	}
	ifs.close();
	ofs.close();
//re
    cerr << "TODO: uncompress '" << infile << "' -> '"
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
        uncompressBitwise(infile, outfile);
    } else {
        uncompressAscii(infile, outfile);
    }

    return 0;
}
