#ifndef MainH
#define MainH

enum eCMD {CMD_EXIT = -1, CMD_NONE, CMD_READ, CMD_SHOW, CMD_EDIT,
		   CMD_ADD, CMD_DELETE, CMD_SORT, CMD_FIND, CMD_AVERAGE, CMD_SAVE, CMD_SAVENEW};

const int MAX_SIZE = 10;

struct Books {
	char author[20];
	char bookname[30];
	double price;
    int amount;
	char codenum[20];
};

#endif
