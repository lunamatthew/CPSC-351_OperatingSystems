#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

// Programmed by: Matthew Luna
// Assignment 3 (Thread Review)

// generateNumInsert() will have threads genereate 100 random integers [0 - 100000] and
// insert them into a shared binary search tree between all threads.

// Binary search tree will be implmented using structs (node).

struct node {
	int key;
	struct node* left;
	struct node* right;
};

// This global root node will act as the head (root) of the binary search tree
node *root = NULL;
// mutex1 will be the semaphore to protect the critical section of the code from race conditions
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

void* generateNumInsert(void* param) {
	int randNum = 0;
	printf("ENTERING generateNumInsert()!\n");
	for (int i = 0; i < 100; ++i) {
		randNum = rand() % 100000 + 0;

		node* tempInsert = new node;
		tempInsert->key = randNum;
	

		pthread_mutex_lock(&mutex1);

		if (root == NULL) {
			root = tempInsert;
		} 
		else 
		{
			node* tempCheck = root;
			while (tempCheck != NULL) {
				if (tempCheck->key < tempInsert->key) {// insert val is greater
					if (tempCheck->right == NULL) {
						tempCheck->right = tempInsert;
						break;
						//continue;
					} 
					else 
						tempCheck = tempCheck->right;
				}
				else if (tempCheck->key > tempInsert->key) {// insert val is less
					if (tempCheck->left == NULL) {
						tempCheck->left = tempInsert;
						break;
						//continue;
					} 
					else
						tempCheck = tempCheck->left;
				}
				else {// if insert value is duplicate
				      	printf("DUPLICATE!\n");
					break;
					//continue;
				}
			}
			//pthread_mutex_unlock(&mutex1);
		}
		pthread_mutex_unlock(&mutex1);
	}
	
	pthread_exit(0);
}

void printBinaryTree(node* bNode) {
	if (bNode == NULL)
		return;
	printBinaryTree(bNode->right);
	fprintf(stderr, "%d\n", bNode->key);
	printBinaryTree(bNode->left);
}

int main(int argc, char** argv) {
	if (argc < 2) {
		fprintf(stderr, "USAGE: %s <NUMBER OF THREADS (INTEGER)>\n", argv[0]);
		exit(1);
	}

	srand(time(NULL));

	int numOfThreads = atoi(argv[1]);
	pthread_t threadPool[numOfThreads];

	for (int i = 0; i < numOfThreads; ++i) {
		if (pthread_create(&threadPool[i], NULL, generateNumInsert, NULL) != 0) {
			perror("pthread_create");
			exit(1);
		}
	}


	for (int i = 0; i < numOfThreads; ++i) {
		if (pthread_join(threadPool[i], NULL) != 0) {
			perror("pthread_join");
			exit(1);
		}
	}

	//printBinaryTree(root);

	// we also need to destroy any dynamically allocated memory here before termination

	return 0;
}
