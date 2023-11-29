#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <windows.h>

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

int tree_free(Node* tree)
{
	if (!tree) return 0;
	free(tree->key);
	tree_free(tree->left);
	tree_free(tree->right);
	free(tree);
	return 0;
}

Node* create_node(const char* key)
{
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->key = (char*)malloc(MAXNAMESIZE);
	strcpy(newNode->key, key);
	newNode->left = 0;
	newNode->right = 0;
    return newNode;
}

int insert(Node** p, const char* k) // вставка ключа k в узел p (возможно нулевой)
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

	if ((a == 'Y') || (a == 'N')) return a;
	else return getanswer();
}

int tree_print(Node* tree)
{
	if (!tree) {
		printf(" <nil>");
		return 0;
	}

	printf("(<%s>", tree->key);
	tree_print(tree->left);
	tree_print(tree->right);
	printf(")");

	return 0;
}

int _tree_print_file(FILE* file, Node* tree)
{
	if (!tree) {
		fprintf(file, " <nil>");
		return 0;
	}

	fprintf(file, "(<%s>", tree->key);
	_tree_print_file(file, tree->left);
	_tree_print_file(file, tree->right);
	fprintf(file, ")");

	return 0;
}

int tree_print_file(Node* tree)
{
	FILE* base_new = fopen("NewDataBase.txt", "w");

	_tree_print_file(base_new, tree);
	fclose(base_new);
	return 0;
}

int Write_database(Node* tree)
{
	printf("obnovit bazu dannih?\n[Y/N] ");
	if (!getanswer()) return 0;

	tree_print_file(tree);
	return 0;
}

int read_base(Node** tree, FILE* file)
{
	int a = 0;
	char word[MAXNAMESIZE] = {0};

	a = fgetc(file);
	while (a == ')') fscanf(file, "%c", &a);



	switch (a) {
	case '(': 	fscanf(file, "<%[^<>]>", word);
					(*tree) = create_node(word);
					read_base( &((*tree)->left), file);
					read_base( &((*tree)->right), file);
					return 0;
	case ' ': 	fscanf(file, "<%[^<>]>", word);
					if (strcmp(word, "nil") != 0) (*tree) = create_node(word);
					return 0;
	case ')': 	return 0;
	default: 	printf("invalid syntaxis, readed %c", a); return 0;
	}
}

int add_new_chel(Node *tree, const char* object_name)
{
	printf("vi hotite dobavit' etot object v bazu dannih?\n[Y/N] ");

	if (!getanswer()) return 0;


	printf("vvedite crtiterij razlichiya %s i %s V KOVICHKAH < >\n", tree->key, object_name);
	printf("%s ", object_name);
	char criteria[MAXNAMESIZE] = {0};

	scanf("%c", criteria);
	scanf("<%[^<>]>", criteria);

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

	if (a == 'Y') {
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
	system("dot Tree.dot -T png -o Tree.png");
	system(".\\Tree.png");
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

int print_path(Node** path)
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

int _getpath(Node* tree, const char* word, int level, Node** path)
{
	path[level] = tree;
	// print_path(path);

	if (strcmp(tree->key, word) == 0) {
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

Node** getpath(Node* tree, const char* word) {
	Node** path = (Node**) calloc(sizeof(Node*), MAXTREEDEEP);

	_getpath(tree, word, 0, path);

	return path;
}

#define MIN(x,y) (y)>(x)?(x):(y)

int getyesno(Node** path, int* yesno)
{
	for (int i = 0; path[i+1] != 0; i++)
	{
		if (path[i]->right == path[i+1]) {
			yesno[i] = 0;
		}
		else {
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



	int i = 0;
	if ((yesno1[i] == yesno2[i]) && (yesno1[i] != -1))
		printf("%s and %s are similar because they both are ", word1, word2);

	while ( (yesno1[i] == yesno2[i]) && (yesno1[i] != -1) ) {
		if (yesno1[i] == 0) printf("not ");
		printf("%s ", path1[i]->key);
		i++;
	}

	if (yesno1[i] != -1) {
		if (i != 0) printf("but");
		printf("they are different because ");
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

#define MAXNAMESIZE
int Start(Node* tree)
{
	printf("akinator programm, press any key to continue\n");
	getchar();
	char command[MAXNAMESIZE] = {0};

	while (true) {
		printf("what do you want?\n[g]uess / [o]bjects / [s]how / [d]iff / [e]xit\n");
		scanf("%s", command);

		if ((strcmp(command, "guess") == 0) || (strcmp(command, "g") == 0)) {
			Guess(tree);
		}
		else if ((strcmp(command, "objects") == 0) || (strcmp(command, "o") == 0)) {
			get_objects(tree);
		}
		else if ((strcmp(command, "show") == 0) || (strcmp(command, "s") == 0)){
			DUMP(tree);
		}
		else if ((strcmp(command, "diff") == 0) || (strcmp(command, "d") == 0)){
			diff(tree);
		}
		else if ((strcmp(command, "exit") == 0) || (strcmp(command, "e") == 0)) {
			Write_database(tree);

			return 0;
		}
		else {
			printf("\ninvalid syntaxis <%s>\n\n", command);
		}

	}
	return 0;
}

// TODO: Проверки на то что у тебя вводимая строка влезает в MAXNAMESIZE
// TODO: Use CleanBuffer from SquareEquation
// TODO: или норм английский или русский язык
int main(const int argc, const char* argv[])
{
	SetConsoleOutputCP(CP_UTF8);

	Node* tree = 0;
	const char* filename = "database.txt";

	FILE* database_file = fopen(filename, "r");

	read_base(&tree, database_file);
	fclose(database_file);

	Start(tree);

	return 0;
}

