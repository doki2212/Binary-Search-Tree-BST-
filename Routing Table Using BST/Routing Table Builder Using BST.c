#define _CRT_NO_SECURE_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct RouteNode {
	int destination;
	int nexthop;
	struct RouteNode* left;
	struct RouteNode* right;
}route;

route* createnode(int destination, int nexthop);
route* insertnode(route* root, int destination, int nexthop);
route* searchroute(route* root, int destination);
route* delete(route* root, int destination);
void inordertraversal(route* root);
void freetree(route* root);

int main() {
	int choice;
	int destination;
	int nexthop;
	route* root = NULL;

	while (1) {
		printf("\n--- Routing Table Menu ---\n");
		printf("1. Add route\n");
		printf("2. Search route\n");
		printf("3. Delete route\n");
		printf("4. Update route\n");
		printf("5. Display all routes (Inorder)\n");
		printf("6. Exit\n");
		printf("Enter your choice: ");
		scanf("%d", &choice);

		switch (choice) {
		case 1:
			printf("Enter destination address.\n");
			scanf("%d", &destination);
			printf("Enter next hop.\n");
			scanf("%d", &nexthop);
			root = insertnode(root, destination, nexthop);
			break;

		case 2:
			printf("Enter destination to be searched.\n");
			scanf("%d", &destination);
			route* found = searchroute(root, destination);
			if (found != NULL) {
				printf("Route Found: Destination = %d , Next Hop = %d ", found->destination, found->nexthop);
			}
			else {
				printf("Route not found.\n");
			}
			break;

		case 3:
			printf("Enter Address of the destination to be deleted.\n");
			scanf("%d", &destination);
			root = delete(root, destination);
			break;

		case 4:
			printf("Enter the destination of the node to be deleted.\n");
			scanf("%d", &destination);
			printf("Enter the next hop address.\n");
			scanf("%d", &nexthop);
			root = delete(root, destination);
			root = insertnode(root, destination, nexthop);
			break;

		case 5:
			printf("Routing Table.\n");
			inordertraversal(root);
			break;

		case 6:
			freetree(root);
			return 0;

		default:
			printf("Invalid Choice.\n");
		}
	}
}

route* createnode(int destination, int nexthop) {
	route* newnode = (route*)malloc(sizeof(route));
	newnode->destination = destination;
	newnode->nexthop = nexthop;
	newnode->left = newnode->right = NULL;
	return newnode;
 }

route* insertnode(route* root, int destination, int nexthop) {
	if (root == NULL) {
		return createnode(destination, nexthop);
	}
	if (destination < root->destination) {
		root->left = insertnode(root->left, destination, nexthop);
	}
	else if (destination > root->destination) {
		root->right = insertnode(root->right, destination, nexthop);
	}
	else {
		root->nexthop = nexthop;
	}
	return root;
}

route* searchroute(route* root, int destination) {
	if (root == NULL || root->destination == destination) {
		return root;
	}
	if (destination < root->destination) {
		return searchroute(root->left, destination);
	}
	else {
		return searchroute(root->right, destination);
	}
}

route* delete(route* root, int destination) {
	if (root == NULL) return NULL;

	if (destination < root->destination) {
		root->left = delete(root->left, destination);
	}
	else if (destination > root->destination) {
		root->right = delete(root->right, destination);
	}
	else {
		// Node found
		if (root->left == NULL) {
			route* temp = root->right;
			free(root);
			return temp;
		}
		else if (root->right == NULL) {
			route* temp = root->left;
			free(root);
			return temp;
		}
		else {
			// Find in-order successor
			route* succ = root->right;
			while (succ->left != NULL)
				succ = succ->left;

			root->destination = succ->destination;
			root->nexthop = succ->nexthop;
			root->right = delete(root->right, succ->destination);
		}
	}
	return root;
}

void inordertraversal(route* root) {
	if (root != NULL) {
		inordertraversal(root->left);
		printf("Destination: %d, Next Hop: %d\n", root->destination, root->nexthop);
		inordertraversal(root->right);
	}
}

void freetree(route* root) {
	if (root != NULL) {
		freetree(root->left);
		freetree(root->right);
		free(root);
	}
}
