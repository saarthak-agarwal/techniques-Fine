#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_SIZE 2000
#define MAX_RECURSIVE_DEPTH 10000

#define print(x) printf("%s\n", #x);

#define indent(x) for(int i=0; i<x; i++) putchar('\t');

#define addPrologue print(#include <stdio.h>\nint main()\n{); 

#define addEpilogue print(\n\treturn 0;\n})	

#define beginInitialSwitchCase(x) 		indent(x)\
										print(int ch=1;) indent(x)\
										print(while(ch)) indent(x)\
										print({) indent(x+1)\
										print(scanf("%d", &ch);)indent(x+1)\
										print(switch(ch)) indent(x+1)\
										print({)

#define beginInnerSwitchCase(x) 	indent(x)\
									print(ch=1;) indent(x)\
									print(while(ch)) indent(x)\
									print({) indent(x+1)\
									print(scanf("%d", &ch);)indent(x+1)\
									print(switch(ch)) indent(x+1)\
									print({)

#define endSwitchCase(x) 	indent(x)\
							print(default: break;)\
							indent(x-1) print(})\
							indent(x-2) print(})\
							indent(x-2) print(ch=1;) 


/* Node Structure */
typedef struct node
{
	char key[MAX_INPUT_SIZE];
	struct node* child;
	struct node* sibling;
}node_t;

/* Tree Structure wrapping the node*/
typedef struct tree
{
	node_t *root;		
}tree_t;

/* Structure for stack elements */
typedef struct stack
{
	node_t* node;
	int distance;
} stack_t;

 /**				Stack					**/

stack_t st[MAX_RECURSIVE_DEPTH];
int stack_pointer = 0;

void push(stack_t val)
{
	stack_pointer++;
	st[stack_pointer] = val;
}
void pop()
{
	stack_pointer--;
}
stack_t peek()
{
	return st[stack_pointer];
}

 /**				General Purpose Tree					**/

/* Functions to add child and sibling nodes in the tree*/
void add_sibling(node_t* child, node_t* sibling)
{
	child->sibling = sibling;
	return;
}

void add_child(node_t* parent, node_t* child)
{
	parent->child = child;
	return;
}

/* Function to display the GPT */
void displayUtility(node_t *root, int number_tabs)
{
	if(root == NULL)
		return;
	
	for(int i=0; i<number_tabs; i++) 
		putchar('\t');
	
	printf("%s",root->key);	
	displayUtility(root->child, number_tabs+1);
	displayUtility(root->sibling, number_tabs);
}
// add const wherrever necessary
void displayTree(tree_t *ptr_tree)
{
	if(ptr_tree->root==NULL)
		return;
	displayUtility(ptr_tree->root->child, 0);
}

void traverseTree(node_t* menu_item, int number_tabs, int case_number)
{
	if(menu_item == NULL)
		return;
	
	/* Print the menu item */
	indent(number_tabs); 
	printf("case %d: ", case_number); 
	printf("printf(\"%%s\\n\", \"%s\");\n", menu_item->key);

	/*If the current node has a child, print an inner switch case for it*/
	if(menu_item->child)
	{
		beginInnerSwitchCase(number_tabs+2)
		traverseTree(menu_item->child, number_tabs+4, 1);
	}
	indent(number_tabs+2);
	print(break;)

	/*If the current node has a sibling, print an additional case for it*/
	/*Otherwise end the current switch case*/ 
	if(menu_item->sibling) 
	{
		traverseTree(menu_item->sibling, number_tabs, case_number+1);
	}
	else
	{
		endSwitchCase(number_tabs)
	}

}
 /**				Client					**/
int main()
{
	tree_t root_node;
	stack_t temp;

	/*temp: initial element of the stack representing the root of the tree/the parent of the all the nodes to be entered by the user.*/

	temp.node = (node_t*)malloc(sizeof(node_t));
	temp.node->child = NULL;
	temp.node->sibling = NULL;
	temp.distance = -1;
	root_node.root = temp.node;

	push(temp);

	char* input = malloc(sizeof(char)*MAX_INPUT_SIZE);
	
	while(fgets(input, MAX_INPUT_SIZE, stdin))
	{	
		/* Parse the input and count the number of tabs representing parent - child relationship*/
		int number_tabs = 0, i=0;
		for(; input[i]; i++)
		{
			if(input[i]=='\t')
				number_tabs++;
			else
			{
				break;
			}
		} 

		/*Create a menu item from the input after removing the tabs*/
		char key[MAX_INPUT_SIZE];
		strcpy(key, input+i);
		for(int j=0; key[j]; j++)
		{
			if(key[j]=='\n')
				key[j] = '\0';
		}

		/*Add the latest entrant to the stack for faster interpretation of parent - child relationships*/

		stack_t new;
		node_t* node = (node_t*)malloc(sizeof(node_t));
		strcpy(node->key, key);
		node->child = NULL;
		node->sibling = NULL;
		new.node = node;
		new.distance = number_tabs;

		/*Pop those elements from the stack which are neither the parent nor the sibling of the latest input*/
		stack_t top = peek();

		while(top.distance>number_tabs)
		{
			pop();
			top = peek();
		}

		// note the node at top
		top = peek();

		// If the node's distance is less than the current input, it implies a parent-child relationship
		//Else it implies a sibling relationship

		if(top.distance < number_tabs)
		{
			// add the current node to the stack
			push(new);

			// And as a child of the poped parent
			add_child(top.node, new.node);
		}
		else
		{
			// pop the top node, that is the sibling
			pop();

			// add the current node to the right of the popped sibling
			// i.e. add the new node as sibling of the popped one
			add_sibling(top.node, new.node) ;
			push(new);
		}
	}

	//Now that the tree has been formed
	//generate the code

	addPrologue
	beginInitialSwitchCase(1)
	traverseTree(root_node.root->child, 3, 1);
	addEpilogue

	return 0;
}