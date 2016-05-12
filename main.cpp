#include <iostream>
#include <queue>
#include <iomanip>
#include <string>

using namespace std;

struct H_Node {
	string cares;
	int counts;
	vector<bool> code;
	H_Node* left;
	H_Node* right;
	H_Node();
	H_Node(char, int);
	H_Node(H_Node*, H_Node*);
};

void Print_Tree(H_Node*);
void Print_Node(H_Node*);
vector<H_Node> Make_Code(H_Node*);
void Make_Code(H_Node*, vector<bool>&, vector<H_Node>&);

//this class alllows a priority_queue to compare H_Nodes
//by defining a strict weak order over H_Nodes
class CompareNodes {
public:
	bool operator()(H_Node& thing1, H_Node& thing2)
	{
		if (thing1.counts > thing2.counts) return true;
		return false;
	}
};

int main(){
	priority_queue<H_Node, vector<H_Node>, CompareNodes> priq;

	string input = "";

	for (string line; getline(cin, line);) {
		input += line;
	}

	vector<H_Node> nodules;
	bool make_node = true;

	for (int i(0); i < input.length(); i++){
		for (int j(0); j < nodules.size(); j++){
			//check if letter is in nodules
			if (input[i] == nodules[j].cares[0]){
				//if so increment count and set the "node_made" flag to false  
				++nodules[j].counts;
				make_node = false;
			}
		}
		//if not make a new node and set its count to 1
		if (make_node) nodules.push_back(H_Node(input[i], 1));

		//reset the flag to true each time the outer loop executes
		//so we always assume a node will be made unless a
		//node is found by the above inner loop
		make_node = true;
	}

	//push everything in the "nodules" vector onto a priority queue 
	for (int i(0); i < nodules.size(); ++i)
		priq.push(nodules[i]);

	while (priq.size() > 1) {
		//pop the top two nodes
		H_Node* thing1 = new H_Node(priq.top());
		priq.pop();

		H_Node* thing2 = new H_Node(priq.top());
		priq.pop();

		//make a new parent node from the top two nodes and push it onto the queue
		priq.push(H_Node(thing1, thing2));
	}

	Print_Tree(&priq.top());
	cout << endl << endl;

	//make codes for each node and make an encoder to encode the message 
	vector<H_Node> encoder(Make_Code(&priq.top()));
	cout << endl;
	//encode the message
	vector<bool> message;
	for (int i(0); i < input.length(); i++){
		for (int j(0); j < encoder.size(); j++){
			//find letters in the encoder
			if (input[i] == encoder[j].cares[0]){
				//push the character's code onto the message
				for (int k(0); k < encoder[j].code.size(); k++)
					message.push_back(encoder[j].code[k]);
			}
		}
	}

	for (int i(0); i < message.size(); i++)
		cout << message[i];
	cout << endl << endl;

	cout << "Size before compression: " << input.length() * sizeof(char) * 8 << " bits" << endl
		 << "Size after compression:  " << message.size() << " bits" << endl;

	return 0;
}

H_Node::H_Node(){
	this->cares = "";
	this->counts = 0;
	this->left = nullptr;
	this->right = nullptr;
}

H_Node::H_Node(char cares, int counts){
	this->cares = cares;
	this->counts = counts;
	this->left = nullptr;
	this->right = nullptr;
}

H_Node::H_Node(H_Node* left, H_Node* right){
	this->left = left;
	this->right = right;
	this->counts = left->counts + right->counts;
	this->cares = left->cares + right->cares;
}

void Print_Tree(H_Node* node_ptr){
	//print the node
	Print_Node(node_ptr);

	//search left 
	if (node_ptr->left)
		Print_Tree(node_ptr->left);
	else cout << '/';

	//search right
	if (node_ptr->right)
		Print_Tree(node_ptr->right);
	else cout << '/';
}

void Print_Node(H_Node* node_ptr){
	if (node_ptr->cares.length() > 1)
		cout << '*';
	else {
		cout << node_ptr->cares[0];
	}
}

vector<H_Node> Make_Code(H_Node* node_ptr){
	vector<bool> temp;
	vector<H_Node> coded_nodes;
	Make_Code(node_ptr, temp, coded_nodes);
	return coded_nodes;
}

void Make_Code(H_Node* node_ptr, vector<bool>& temp, vector<H_Node>& coded_nodes){

	//left pushes a 0
	if (node_ptr->left){
		temp.push_back(0);
		Make_Code(node_ptr->left, temp, coded_nodes);
	}
	//right pushes a 1
	if (node_ptr->right){
		temp.push_back(1);
		Make_Code(node_ptr->right, temp, coded_nodes);
	}
	if (!node_ptr->left && !node_ptr->right){

		//copy temp to leaf node's code member and store node for fast retrival  
		node_ptr->code = temp;
		coded_nodes.push_back(*node_ptr);

		cout << node_ptr->cares[0] << " = ";
		for (int i(0); i < node_ptr->code.size(); i++)
			cout << node_ptr->code[i];
		cout << endl;
	}
	if (temp.size() > 0)
		temp.pop_back();
}