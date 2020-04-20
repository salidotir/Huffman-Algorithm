#include "MinHeap.h"
#include <bits/stdc++.h> 
#include <string>
#include <fstream>
#include <map>

class Huffman {
private:
    int* freq_array;
    MinHeap* freq_heap;
    Node* huffman_tree_root;
    map<pair<char, int>, string> encoded_table;

    int* make_freq_array(string input_file) {
        fstream my_file;
        my_file.open(input_file, ios::binary | ios::in);
        if (my_file.is_open()) {
            char temp_char;
            while (my_file.get(temp_char)) {
                freq_array[temp_char]++;
            }
            return freq_array;
        }

        cout << "Could not open input file!" << endl;
        return NULL;
    }

    MinHeap* make_freq_heap(int* freq_array) {
        for (int i = 0; i < 256; i++) {
            if (freq_array[i] != 0) {
                Node* newNode = new Node(char(i), freq_array[i]);
                freq_heap->insert(newNode);
            }
        }
        return freq_heap;
    }

    // returns the root node of the huffman tree
    Node* make_huffman_tree(MinHeap* freq_heap) {
        Node *root;
        Node *left;
        Node *right;
        
        while (freq_heap->getSize() > 2) {

            // extract the two minimum nodes.
            left = freq_heap->extract_root();
            right = freq_heap->extract_root();

            // create a new node with sum of frequencies of two nodes with data of '~'.
            root = new Node('~', left->getFrequency() + right->getFrequency());
            root->setLeftChild(left);
            root->setRightChild(right);

            freq_heap->insert(root);
        }

        // extract the root node.
        huffman_tree_root = freq_heap->extract_root();
        return huffman_tree_root;
    }

public:
    Huffman() {
        freq_array = (int*) calloc(256, sizeof(int));      // initialize the array to zero.
        freq_heap = new MinHeap(256);                      // maximum capacity : 256
        huffman_tree_root = NULL;                          // the root of huffman tree.
    }

    Node* huffman_process(string input_file) {
        make_freq_array(input_file);
        make_freq_heap(freq_array);
        make_huffman_tree(freq_heap);
        return huffman_tree_root;
    }

    // using huffman_tree_root to go through the tree.
    // using arr to store codes in.
    // top is the arr size in each level.
    void make_Huffman_encoded_table(Node* huffman_tree_root, int arr[], int top) {
        string tmp_code = "";

        // Assign 0 to left child and do the recursion.
        if (huffman_tree_root->getLeftChild() != NULL) {
            arr[top] = 0;
            tmp_code += "0";
            make_Huffman_encoded_table(huffman_tree_root->getLeftChild(), arr, top + 1);
        }

        // Assign 1 to right child and do the recursion.
        if (huffman_tree_root->getRightChild() != NULL) {
            arr[top] = 1;
            tmp_code += "1";
            make_Huffman_encoded_table(huffman_tree_root->getRightChild(), arr, top + 1);
        }

        // if this is a leaf node, it is one of the input characters.
        // its code till now is stored in arr.
        if (huffman_tree_root->is_leaf() == true) {
            for (int i = 0; i < top; i++) {
                tmp_code += std::to_string(arr[i]);
            }
            pair<char, int> tmp;
            tmp.first = huffman_tree_root->getData();
            tmp.second = huffman_tree_root->getFrequency();
            encoded_table.insert(pair<pair<char,int>, string>(tmp, tmp_code));
        }
    }

    void make_huffman_text_file() {
        int arr1[1000];
        make_Huffman_encoded_table(huffman_tree_root, arr1, 0);

        fstream my_file;
        my_file.open("huffman.txt", ios::out);
        if (my_file.is_open()) {
            for (auto& i : encoded_table) {

                // write frequency
                my_file << i.first.first << "\t" << i.first.second << "\t" << i.second.c_str() << endl;
            }
        }
            
            my_file.close();
    }

    string binary_code(char charachter) {

        for (auto& i : encoded_table) {
            if (i.first.first == charachter) {
                return i.second;
            }
        }
        cout << "Could not find charachtor in the encoded_table" << endl;
        return "";
    }

    string decimal2binary(int num) {
        // returned string
        string res;

        // array to store binary number 
        int binaryNum[8];
        for (int i = 0; i < 8; i++) {
            binaryNum[i] = 0;
        }

        // counter for binary array 
        int i = 7;
        while (num > 0) {

            // storing remainder in binary array 
            binaryNum[i] = num % 2;
            num = num / 2;
            i--;
        }

        for (int i = 0; i < 8; i++) {
            res += std::to_string(binaryNum[i]);
        }
        return res;
    }

    int binary2decimal(string num) {
        // returned integer
        int res = 0;
        int two = 1;

        for (int i = 7; i > 0; i--) {
            res += (num[i]-'0') * two;
            two *= 2;
        }
        return res;
    }

    // make zip.bin using input.txt
    void make_zip_text_file(string input_file) {
        int num_of_extra_zeros = 0;
        string content = "";
        fstream my_file;
        my_file.open(input_file, ios::binary | ios::in);
        if (my_file.is_open()) {
            char temp_char;
            while (my_file.get(temp_char)) {
                content += temp_char;
            }
            my_file.close();
        }

        string decoded_content = "";
        for (int i = 0; i < content.size(); i++) {
            decoded_content += binary_code(content[i]);
        }

        num_of_extra_zeros = 8 - (decoded_content.size() % 8);
        
        // we must write num_of_extra_zeros in a binary format in 8 bits at the beginig of the zip.bin file.
        // then we write the decoded content to zip.bin file.
        // since the size of the file must be a multiplicant of 8, we add num_of_extra_zeros 0's to the end of zip.bin file.
        string extra_zeros = decimal2binary(num_of_extra_zeros);
        extra_zeros += decoded_content;
        
        for (int i = 0; i < num_of_extra_zeros; i++) {
            extra_zeros += std::to_string(0);
        }

        fstream my_out_file;
        my_out_file.open("zip.txt", ios::binary | ios::out);
        if (my_out_file.is_open()) {
            my_out_file << extra_zeros.c_str();
        }
            my_file.close();

    }

    // restore huffman tree using huffman.txt file.
    Node* restore_minheap_tree() {
        MinHeap* newFreqHeap = new MinHeap(256);

        std::ifstream file("huffman.txt");
        std::string str; 
        while (std::getline(file, str)) {
            string tokens[3]; 
            stringstream check(str);
            string intermediate;
            for (int i = 0; i < 3; i++) {
                getline(check, intermediate, '\t');
                tokens[i] = intermediate;
                // if(i == 2)
                //     cout << tokens[0] << "  " << tokens[1] << endl;    
            }

            char c = tokens[0].c_str()[0];
            Node* newNode = new Node(c, stoi(tokens[1]));
            newFreqHeap->insert(newNode);
        }

        Node* newHuffmanTree = make_huffman_tree(newFreqHeap);
        return newHuffmanTree;
    }

    // unzip zip.bin file using huffman.txt to restore_input.txt
    void make_input_text_file(Node* huffman_tree_root) {

        Node* curr = restore_minheap_tree();

        string content = "";
        string ans = "";

        fstream my_file;
        my_file.open("zip.txt", ios::binary | ios::in);
        if (my_file.is_open()) {
            char temp_char;
            while (my_file.get(temp_char)) {
                content += temp_char;
            }
            my_file.close();
        }

        int num_of_extra_zeros = binary2decimal(content.substr(0, 8));
        string new_content = content.substr(8, content.size()-num_of_extra_zeros-8);

        for (int i = 0; i < new_content.size(); i++) {
            if (new_content[i] == '0')
                curr = curr->getLeftChild();
            else
                curr = curr->getRightChild();

            // reached leaf node 
            if (curr->getLeftChild() == NULL && curr->getRightChild() == NULL)
            {
                ans += curr->getData();
                curr = huffman_tree_root;
            }
        }
        ans += '\0';

        fstream my_ans;
        my_ans.open("restore_input.txt", ios::out);
        if (my_ans.is_open()) {
            my_ans << ans.c_str();
            my_ans.close();
        }
    }

};