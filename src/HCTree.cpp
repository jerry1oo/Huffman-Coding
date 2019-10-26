#include <stack>
#include <queue>

#include "HCTree.hpp"

/**
 * Destructor for HCTree
 */
HCTree::~HCTree() {
    deleteTrees(root);
	leaves = vector<HCNode*>(256, (HCNode*) 0);
	root = 0;
/*	for(int i = 0; i < leaves.size(); i++){
		if(leaves[i] != 0){
			delete leaves[i];
		}
	}
*/    
}

/** Use the Huffman algorithm to build a Huffman coding tree.
 *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is
 *  the frequency of occurrence of byte i in the message.
 *  POSTCONDITION:  root points to the root of the tree,
 *  and leaves[i] points to the leaf node containing byte i.
 */
void HCTree::build(const vector<int>& freqs) {
	deleteTrees(root);
	leaves = vector<HCNode*>(256, (HCNode*) 0);
	root = 0;
/*	for(int i = 0; i < leaves.size(); i++){
		if(leaves[i] != 0){
			delete leaves[i];
		}
	}    
*/	for(int i = 0; i < freqs.size(); i++){
		if(freqs[i] != 0){
			leaves[i] = new HCNode(freqs[i],i,0,0,0);
		}
	}
	std::priority_queue<HCNode*,std::vector<HCNode*>,HCNodePtrComp> pq;
	for(int i = 0; i < leaves.size(); i++){
		if(leaves[i] != nullptr){
			pq.push(leaves[i]);
		}
	}
	if(pq.empty()) return;
	while(pq.size() > 1){
		HCNode* low1 = pq.top();
		pq.pop();
		HCNode* low2 = pq.top();
		pq.pop();
		byte s_temp;
		if(low1->symbol < low2->symbol){
			s_temp = low2->symbol;
		}
		else{
			s_temp = low1->symbol;
		}
		HCNode* temp = new HCNode((low1->count + low2->count),s_temp,
									low1,low2,0);
		low1->p = temp;
		low2->p = temp;
		pq.push(temp);		
	}
	root = pq.top();
	pq.pop();
}


/** Write to the given ostream
 *  the sequence of bits (as ASCII) coding the given symbol.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
void HCTree::encode(byte symbol, ostream& out) const {
   	HCNode* track = 0;
	for(int i = 0; i < leaves.size(); i++){
		if(leaves[i] != 0){
			if(leaves[i]->symbol == symbol){
				track = leaves[i];
			}
		}
	}
	if(track == 0) return;
	//int s_count = track->count;
	stack<HCNode*> path;
	while(track->p != 0){
		path.push(track);
		track = track->p;
	}
	while(!path.empty()){
		if(track->c0 == path.top()){
			out << '0';
			track = path.top();
			path.pop();			
		}
		else if(track->c1 == path.top()){
			out << '1';
			track = path.top();
			path.pop();			
		}
	} 
}

/** Return the symbol coded in the next sequence of bits (represented as 
 *  ASCII text) from the istream.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
byte HCTree::decode(istream& in) const {
	HCNode* track = root;
	unsigned char nextChar;
	while(track->c0 != 0 && track->c1 != 0){
		in >> nextChar;
		if(nextChar == '1'){
			track = track->c1;
		}
		else if(nextChar == '0'){
			track = track->c0;
		}
	}
	return track->symbol;
}

/** Write to the given BitOutputStream
 *  the sequence of bits coding the given symbol.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
void HCTree::encode(byte symbol, BitOutputStream& out) const {
   	HCNode* track;
	for(int i = 0; i < leaves.size(); i++){
		if(leaves[i] != 0){
			if(leaves[i]->symbol == symbol){
				track = leaves[i];
			}
		}
	}
	//int s_count = track->count;
	stack<HCNode*> path;
	while(track->p != 0){
		path.push(track);
		track = track->p;
	}
	while(!path.empty()){
		if(track->c0 == path.top()){
			out.writeBit(0);
			track = path.top();
			path.pop();			
		}
		else if(track->c1 == path.top()){
			out.writeBit(1);
			track = path.top();
			path.pop();			
		}
	}
/*	if(out.Getnbits() < 7 && out.Getnbits() > 0){
		while(out.Getnbits() < 8){
			out.writeBit(0);
		}
		out.flush();
	}
*/ 
}

/** Return symbol coded in the next sequence of bits from the stream.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
byte HCTree::decode(BitInputStream& in) const {
    HCNode* track = root;
	while(track->c0 != 0 && track->c1 != 0){
		if(in.readBit()){
			track = track->c1;
		}
		else{
			track = track->c0;
		}
	}
	return track->symbol;
}

/**
 * Print the contents of a tree
 */
void HCTree::printTree() const {
    cout << "=== PRINT TREE BEGIN ===" << endl;
    printTreeHelper(root);
    cout << "=== PRINT TREE END =====" << endl;
}

/**
 * Recursive helper function for printTree
 */
void HCTree::printTreeHelper(HCNode * node, string indent) const {
    if (node == nullptr) {
        cout << indent << "nullptr" << endl;
        return;
    }

    cout << indent << *node << endl;
    if (node->c0 != nullptr || node->c1 != nullptr) {
        printTreeHelper(node->c0, indent + "  ");
        printTreeHelper(node->c1, indent + "  ");
    }
}

void HCTree::deleteTrees(HCNode* R){
	if(!R) return;
    deleteTrees(R->c0);
    deleteTrees(R->c1);
    delete R;

}
