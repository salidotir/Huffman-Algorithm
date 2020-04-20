class Node {

private:
	char data;
	int frequency;
	Node* left;
	Node* right;
	Node* parent;

public:
	Node(char d, int f) {
		data = d;
		frequency = f;
		left = right = NULL;
	}

	char getData() {
		return data;
	}

	int getFrequency() {
		return frequency;
	}

	Node* getLeftChild() {
		return left;
	}

	Node* getRightChild() {
		return right;
	}

	Node* getParent() {
		return parent;
	}

	bool is_leaf() {
		if (right == NULL && left == NULL)
			return true;
		return false;
	}

	void setData(char d) {
		data = d;
	}

	void setFrequency(int num) {
		frequency = num;
	}

	void setLeftChild(Node* l) {
		left = l;
	}

	void setRightChild(Node* r) {
		right = r;
	}

	void setParent(Node* p) {
		parent = p;
	}

};