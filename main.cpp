#include "Huffman.h"

int main()
{
    cout << endl << "**** Huffman algorithm ****" << endl << endl;
    cout << "Please enter input file name : ";
    string input_file;
    cin >> input_file;
    cout << endl;

    // Part I

    Huffman* huffman = new Huffman();
    Node* huffman_tree_root = huffman->huffman_process(input_file);

    int arr[1000];
    huffman->make_huffman_text_file();
    huffman->make_zip_text_file(input_file);

    // Part II

    Node* root = huffman->restore_minheap_tree();
    huffman->make_input_text_file(root);
    

    return 0;
}