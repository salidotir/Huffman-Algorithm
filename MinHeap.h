#pragma once
#include <iostream>
#include "Node.h"

using namespace std;

class MinHeap {
public:
	Node* arr;
	int heap_size;
	int capacity;

	// make the heap by adding one by one.
	MinHeap(int capacity) {
		this->arr = (Node*)malloc(capacity*sizeof(Node));
		heap_size = 1;
		this->capacity = capacity;
	}

	// make the heap by input array.
	MinHeap(Node* newArr, int newArrSize, int capacity) {
		this->arr = newArr;
		int last_non_leaf = newArrSize / 2;
		for (int i = last_non_leaf; i > 0; i--) {      // index starts at 1. && Leaf nodes do not need heapifying.
			heapify(this->arr, newArrSize, i);
		}
		heap_size = newArrSize;
	}

	~MinHeap() {
		delete[] this->arr;
		cout << "deleted min heap!" << endl;
	}

	// remove the root(smallest element).
	Node* extract_root() {
		if (heap_size > 1) {
			Node* res = new Node(arr[1].getData(), arr[1].getFrequency());
			res->setLeftChild(arr[1].getLeftChild());
			res->setRightChild(arr[1].getRightChild());
			
			arr[1] = arr[heap_size-1];        // index starts at 1.
			heap_size--;

			//heapify(arr, heap_size, 1);

			for (int i = heap_size-1; i > 0; i--)
			{
				heapify(arr, heap_size, i);
			}

			return res;                         // return the removed element(the root).
		}

		cout << "Heap is empty" << endl;
		return NULL;
	}

	bool is_empty() {
		if (heap_size > 0)
			return false;
		return true;
	}

	void heapify(Node* arr, int size, int i) {
		int smallest = i;
		int left = 2 * i;
		int right = 2 * i + 1;

		if (left < size && arr[left].getFrequency() < arr[smallest].getFrequency())
			smallest = left;

		if (right < size && arr[right].getFrequency() < arr[smallest].getFrequency())
			smallest = right;

		if (smallest != i) {
			//swap smallest node and i_th node if we need to heapify the subtree.

			int data_tmp = arr[smallest].getData();
			int freq_tmp = arr[smallest].getFrequency();
			Node* left_tmp = arr[smallest].getLeftChild();
			Node* right_tmp = arr[smallest].getRightChild();

			arr[smallest].setData(arr[i].getData());
			arr[smallest].setFrequency(arr[i].getFrequency());
			arr[smallest].setLeftChild(arr[i].getLeftChild());
			arr[smallest].setRightChild(arr[i].getRightChild());
			
			arr[i].setData(data_tmp);
			arr[i].setFrequency(freq_tmp);
			arr[i].setLeftChild(left_tmp);
			arr[i].setRightChild(right_tmp);

			heapify(arr, size, smallest);
		}
	}

	bool insert(Node* newNode) {
		if (heap_size < capacity) {
			arr[heap_size] = *newNode;
			heap_size++;
			int last_non_leaf = heap_size / 2;

			for (int i = last_non_leaf; i > 0; i--) {
				heapify(arr, heap_size, i);
			}

			return true;
		}

		cout << "Heap is full!" << endl;
		return false;           // could not add to heap.
	}

	// removes num from heap.
	void remove(Node* node) {
		int index;
		Node* res = NULL;

		for (int i = 1; i < heap_size; i++) {
			if (arr[i].getData() == node->getData() && arr[i].getFrequency() == node->getFrequency()) {
				index = i;
				break;
			}
		}

		arr[index] = arr[--heap_size];

		for (int i = index; i > 0; i--) {
			heapify(arr, heap_size - 1, i);
		}
	}

	int getSize() {
		return heap_size;
	}

	int getCapacity() {
		return capacity;
	}

	void print_heap() {
		for (int i = 1; i < heap_size; i++) {
			cout << "data : " << arr[i].getData() << "  frequency : " << arr[i].getFrequency() << endl;
		}

	}

};