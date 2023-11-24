#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

size_t MAXNAMESIZE = 100;
size_t MAXTREEDEEP = 20;

#define LEFT 1;
#define RIGHT 0;

struct Node // структура для представления узлов дерева
{
	char* key = 0;
	Node* left = 0;
	Node* right = 0;
};

int free_tree(Node* tree)
{
	free(tree->key);
	free_tree(tree->left);
	free_tree(tree->right);
	free(tree);
	return 0;
}

Node* create_node(char* key)
{
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->key = (char*)malloc(MAXNAMESIZE);
	strcpy(newNode->key, key);
	newNode->left = 0;
	newNode->right = 0;
    return newNode;
}

int insert(Node** p, char* k) // вставка ключа k в узел p (возможно нулевой)
{
	if( !p )
    {
		*p = create_node(k);
		return 0;
    }

	strcpy((*p)->key, k);

	return 0;
}

int getanswer()
{
	int a = getchar();

	if (a == 'Y') return 1;
	else if (a == 'N') return 0;
	else return getanswer();
}

int PrintTree(Node* tree)
{
	if (!tree) {
		printf(" <nil>");
		return 0;
	}

	printf("(<%s>", tree->key);
	PrintTree(tree->left);
	PrintTree(tree->right);
	printf(")");

	return 0;
}

int _FilePrintTree(FILE* file, Node* tree)
{
	if (!tree) {
		fprintf(file, " <nil>");
		return 0;
	}

	fprintf(file, "(<%s>", tree->key);
	_FilePrintTree(file, tree->left);
	_FilePrintTree(file, tree->right);
	fprintf(file, ")");

	return 0;
}

int FilePrintTree(Node* tree)
{
	FILE* base_new = fopen("NewDataBase.txt", "w");

	_FilePrintTree(base_new, tree);
	system("dot Tree.dot -T png -o Tree.png");
	return 0;
}

int Write_database(Node* tree)
{
	printf("obnovit bazu dannih?\n[Y/N] ");
	if (!getanswer()) return 0;

	FilePrintTree(tree);
	return 0;
}

int read_base(Node** tree, FILE* file)
{
	int a = 0;
	char word[100] = {0};

	a = fgetc(file);
	// fscanf(file, "%c", &a);
	while (a == ')') fscanf(file, "%c", &a);

	// printf("readed %c = %d\n", a, a);


	switch (a) {
	case '(': 	fscanf(file, "<%[^<>]>", word);
					// printf("1 - readed %s\n", word);
					(*tree) = create_node(word);
					// printf("     parsing left\n");
					read_base( &((*tree)->left), file);
					// printf("     parsing right\n");
					read_base( &((*tree)->right), file);
					return 0;
	case ' ': 	fscanf(file, "<%[^<>]>", word);
					// printf("2 - readed %s\n", word);
					if (strcmp(word, "nil") != 0) (*tree) = create_node(word);
					return 0;
	case ')': 	return 0;
	default: 	printf("invalid syntaxis, readed %c", a); return 0;
	}
}

int add_new_chel(Node *tree, char* object_name)
{
	printf("vi hotite dobavit' etot object v bazu dannih?\n[Y/N] ");

	if (!getanswer()) return 0;


	printf("vvedite crtiterij razlichiya %s i %s V KOVICHKAH < >\n", tree->key, object_name);
	printf("%s ", object_name);
	char criteria[MAXNAMESIZE] = {0};

	scanf("%c", criteria);
	scanf("%[^<>]", criteria);

	printf("\n|%s|", criteria);

	Node* Node_right = create_node(tree->key);
	Node* Node_left = create_node(object_name);

	tree->left = Node_left;
	tree->right = Node_right;
	strcpy(tree->key, criteria);

	return 0;
}

int Guess(Node* tree)
{
	printf("eto %s?\n[Y/N] ", tree->key);

	int a = getanswer();

	if (a == 1) {
		if (tree->left != 0) {
			Guess(tree->left);
			return 0;
		}
		else {
			printf("nooorm ugadal na izi\n");
		}
	}

	else if (a == 0) {
		if (tree->right != 0) {
			Guess(tree->right);
			return 0;
		}
		else {
			printf("nu hz... a kto eto?\n");
			char object_name[MAXNAMESIZE] = {0};
			scanf("%s", object_name);
			add_new_chel(tree, object_name);
			return 0;
		}
	}

	return 0;
}

int _dump(FILE* file, Node* tree, int* counter)
{
	int counter_now = *counter;
	if (tree != 0) {
		fprintf(file, "		Node_%d [label = \"%s\", shape = \"ellipse\", style = \"filled\", fillcolor = \"#1f77b4\"]\n", (*counter), tree->key);
		if (tree->left != 0) {
			fprintf(file, "		Node_%d -> Node_%d\n", counter_now, (*counter) + 1);
			(*counter)++;
			_dump(file, tree->left, counter);
		}
		if (tree->right != 0) {
			fprintf(file, "		Node_%d -> Node_%d\n", counter_now, (*counter) + 1);
			(*counter)++;
			_dump(file, tree->right, counter);
		}
	}
	return 0;
}

int DUMP(Node* tree)
{
	FILE* dumpfile = fopen("Tree.dot", "w");
	fprintf(dumpfile, "strict digraph {\n");
	int counter = 0;
	_dump(dumpfile, tree, &counter);
	fprintf(dumpfile, "}");
	fclose(dumpfile);
	return 0;
}

int _get_objects(Node* tree, int* counter)
{

	if (tree->left != 0) _get_objects(tree->left, counter);

	if (tree->right != 0) _get_objects(tree->right, counter);

	if ((tree->right == 0) && (tree->left == 0)) {
		printf("%d: %s\n", *counter, tree->key);
		*counter += 1;
	}

	return 0;
}

int get_objects(Node* tree)
{
	int counter = 0;

	printf("\n");
	_get_objects(tree, &counter);
	printf("\n");

	return 0;
}

print_path(Node** path)
{
	if (path == 0) {
		printf("path is 0");
		return 0;
	}

	for (int i = 0; (i < MAXTREEDEEP) && (path[i] != 0); i++) {
		printf("%d: %s -> ", i, path[i]->key);
	}
	printf("X\n");
	return 0;
}

int _getpath(Node* tree, char* word, int level, Node** path)
{
	// printf("checing element %s on level %d\n", tree->key, level);
	path[level] = tree;
	// print_path(path);

	if (strcmp(tree->key, word) == 0) {
		// printf("yeeeeeeeeeeeeeee found %s\n", tree->key);
		return 0;
	}

	else {
		if (tree->left != 0) {
			if (_getpath(tree->left, word, level + 1, path)) {
				path[level] == 0;
				if (tree->right == 0) return 1;
			}
			else {
				return 0;
			}
		}
		if (tree->right != 0) {
			if (_getpath(tree->right, word, level + 1, path)) {
				path[level] == 0;
				return 1;
			}
			else {
				return 0;
			}
		}
		else {
			path[level] = 0;
			return 1;
		}
	}
}


Node** getpath(Node* tree, char* word) {
	Node** path = (Node**) calloc(sizeof(Node*), MAXTREEDEEP);

	_getpath(tree, word, 0, path);

	return path;
}

#define MIN(x,y) (y)>(x)?(x):(y)

int getyesno(Node** path, int* yesno)
{
	for (int i = 0; path[i+1] != 0; i++)
	{
		// printf("now %s (%p), left is %s (%p), right is %s (%p), next is %s (%p)\n", path[i]->key, path[i], path[i]->left->key, path[i]->left, path[i]->right->key, path[i]->right, path[i+1]->key, path[i+1]);
		if (path[i]->right == path[i+1]) {
			// printf("writed no\n");
			yesno[i] = 0;
		}
		else {
			// printf("writed yes\n");
			yesno[i] = 1;
		}
	}
	printf("\n");
	return 0;
}

int describe(Node** path, int* yesno, int position)
{
	for (int i = position; path[i+1] != 0; i++)
	{
		if (i != position) printf("and ");
		if (yesno[i] == 0) printf("not ");
		printf("%s ", path[i]->key);
	}
	return 0;
}

int diff(Node* tree)
{
	char word1[MAXNAMESIZE] = {0};
	char word2[MAXNAMESIZE] = {0};

	printf("enter word1:\n");
	scanf("%s", word1);
	Node** path1 = getpath(tree, word1);

	printf("enter word2:\n");
	scanf("%s", word2);
	Node** path2 = getpath(tree, word2);

	print_path(path1);
	printf("\n");
	print_path(path2);

	int yesno1[MAXTREEDEEP] = {-1};
	int yesno2[MAXTREEDEEP] = {-1};

	for (int i = 0; i < MAXTREEDEEP; i++)
	{
		yesno1[i] = -1;
		yesno2[i] = -1;
	}

	getyesno(path1, yesno1);
	getyesno(path2, yesno2);

// 	for (int i = 0; (i < MAXTREEDEEP) && (yesno1[i] != -1); i++)
// 	{
// 		printf("yesno1[%d] = %d\n", i, yesno1[i]);
// 	}
//
// 	printf("\n");
// 	for (int i = 0;(i < MAXTREEDEEP) && (yesno2[i] != -1); i++)
// 	{
// 		printf("yesno2[%d] = %d\n", i, yesno2[i]);
// 	}

	int i = 0;
	printf("%s and %s are similar because they both are ", word1, word2);
	while ( (yesno1[i] == yesno2[i]) && (yesno1[i] != -1) ) {
		if (yesno1[i] == 0) printf("not ");
		printf("%s ", path1[i]->key);
		i++;
	}

	if (yesno1[i] != -1) {
		printf("but they are different because ");
		printf("%s is ", word1);
		describe(path1, yesno1, i);
	}

	if (yesno2[i] != -1) {
		printf("\nand\n");
		printf("%s is ", word2);
		describe(path2, yesno2, i);
	}

	return 0;
}

int Start(Node* tree)
{
	printf("akinator programm, press any key to continue\n");
	getchar();
	char command[100] = {0};

	for (int i = 0; i < 100; i++) {
		printf("what do you want?\nguess / objects / data / diff / exit\n");
		scanf("%s", command);


		if (strcmp(command, "guess") == 0) {
			Guess(tree);
		}
		else if (strcmp(command, "objects") == 0) {
			get_objects(tree);
		}
		else if (strcmp(command, "data") == 0) {
			DUMP(tree);
			Write_database(tree);
		}
		else if (strcmp(command, "diff") == 0) {
			diff(tree);
		}
		else if (strcmp(command, "exit") == 0) {
			return 0;
		}
		else {
			printf("\ninvalid syntaxis\n\n");
		}

	}
	return 0;
}

int main(const int argc, const char* argv[])
{
	Node* tree = 0;
	const char* filename = "database.txt";

	FILE* database_file = fopen(filename, "r");

	read_base(&tree, database_file);
	fclose(database_file);

	Start(tree);


//
// 	printf("exited recursia\n");
// 	PrintTree(tree);

	return 0;
}

