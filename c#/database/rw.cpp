#include<iostream>
#include<stdio.h>
#include"main.h"

void ReadDatabase(Books* data, unsigned &count){
	char name[50];

	printf ("Введите имя файла:\n");
    fflush(stdin);
    scanf("%s",name);

	FILE* out = fopen(name, "r");

    if(out == NULL)
    {
		printf("Ошибка\n");
    }

	char ch; int n = -1;
	char std[256];

	while(!feof(out)) {
		fgets(std, sizeof(std), out);
		n++;
	}
	rewind(out);

	int j = 0;
	char temp;
	for(int i = 0; i < n; i++)
	{
		strcpy(data[i].author, "b");
		temp = '_';
		while(temp != '|') {
			fscanf(out, "%c", &temp);
			if (temp != '|') { data[i].author[j] = temp; j++;}
			else { data[i].author[j] = '\0'; j = 0; break;}
		}

		strcpy(data[i].bookname, "b");
		temp = '_';
		while(temp != '|') {
			fscanf(out, "%c", &temp);
			if (temp != '|') { data[i].bookname[j] = temp; j++;}
			else { data[i].bookname[j] = '\0'; j = 0; break;}
		}
		fscanf(out, "%lf|%d|%[^|]s", &data[i].price, &data[i].amount, data[i].codenum);
		fseek(out, 3,SEEK_CUR);
	}
	count = n;

	fclose(out);

	system("pause");
}

void SaveNewDatabase(Books* data, unsigned &count) {
	char filename[20];

	printf("Введите имя файла: \n");
	fflush(stdin);
	scanf("%s", filename);

	FILE *inp;

	inp = fopen(filename, "w+");

	if (inp == NULL) {
		printf("Error");
	}

	for (int i = 0; i < count; i++)
	{
		fprintf(inp, "%s|%s|%lf|%d|%s|\n", data[i].author, data[i].bookname, data[i].price, data[i].amount, data[i].codenum);
	}

	printf("Данные записаны.\n");

	fclose(inp);

	system("pause");
};
