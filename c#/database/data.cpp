//Реализованы функции для работы с данными, такие как просмотр данных, редактирование, 
//удаление или добавление записи. Также добавлены функции возвращения к исходному меню 
//и функция подсчета средней цены.

#include<iostream>
#include<stdio.h>
#include<string.h>
#include"main.h"
#include"rw.h"
#include"data.h"

using namespace std;

eCMD MenuShow() {
	while(true) {

		puts("\nВыберите действие:");
		puts("1 - Редактировать; 2 - Добавить строку; 3 - Удалить строку; 4 - Отсортировать; 5 - Найти; 6 - Высчитать среднюю цену; 7 - Выход");

		unsigned opt;

		fflush(stdin);
		scanf("%u", &opt);

		switch( opt ) {
			case 1: return CMD_EDIT;
			case 2: return CMD_ADD;
			case 3: return CMD_DELETE;
			case 4: return CMD_SORT;
			case 5: return CMD_FIND;
			case 6: return CMD_AVERAGE;
			case 7: return CMD_EXIT;
			default: puts("Вы ввели неправильную команду");
			system("pause");
		}
	}
}

void EditRecord(Books* data, unsigned &count) {
	int num;

	printf("Введите номер строки: ");
	fflush(stdin);
	scanf("%d", &num);

	printf("Введите автора: \n");
	fflush(stdin);
	scanf("%20[^\n]s", data[num - 1].author);

	printf("Введите название книги: \n");
	fflush(stdin);
	scanf("%20[^\n]s", data[num - 1].bookname);

	printf("Введите цену: \n");
	fflush(stdin);
	scanf("%lf", &data[num - 1].price);

	printf("Введите количество книг: \n");
	fflush(stdin);
	scanf("%3d", &data[num - 1].amount);

	printf("Введите кодовый номер: \n");
	fflush(stdin);
	scanf("%20s", data[num - 1].codenum);
};

void AddRecord(Books* data, unsigned &count) {

	if (count + 1 > 10) {
		printf("Превышено максимальное количество строк\n");
		system("pause");
		return;
	}
	else {
		count = count + 1;
		while (count) {
			printf("Введите автора: \n");
			fflush(stdin);
			scanf("%30[^\n]s", data[count - 1].author);

			printf("Введите название книги: \n");
			fflush(stdin);
			scanf("%30[^\n]s", data[count - 1].bookname);

			printf("Введите цену: \n");
			fflush(stdin);
			scanf("%3lf", &data[count - 1].price);

			printf("Введите количество книг: \n");
			fflush(stdin);
			scanf("%10d", &data[count - 1].amount);

			printf("Введите кодовый номер: \n");
			fflush(stdin);
			scanf("%13s", data[count - 1].codenum);
			break;
		}
	}
};

void DeleteRecord(Books* data, unsigned &count) {
	printf("Введите строку, которую хотите удалить: ");
	int num;
	scanf("%d", &num);

	if (num > count) {
		printf("Вы ввели несуществующую строку.");
		system("pause");
	}
	else {
		for (int i = num - 1; i < count - 1; i++) {
			data[i] = data[i+1];
		}
		count = count - 1;
	};
};

void SortRecord(Books* data, unsigned &count) {
	Books tmp;
	int num;

	printf("1 - Сортировка по автору, 2 - Сортировка по цене\n");
	scanf("%d", &num);
	fflush(stdin);

	switch(num) {

		case 1:
		int n;

		printf("1 - Сортировка по алфавиту, 2 - Сортировка по обpатному алфавиту\n");
		fflush(stdin);
		scanf("%d", &n);

		if (n == 1) {

			for(int j = 0; j < count; j++)
			{
				for(int i = 0; i < count - 1; i++)
				{
					if(stricmp(data[i].author, data[i+1].author) > 0)
					{
						tmp = data[i];
						data[i] = data[i+1];
						data[i+1] = tmp;
					}
				}
			}; break;
		}

		else if (n == 2) {
			for(int j = 0; j < count; j++)
			{
				for(int i = 0; i < count - 1; i++)
				{
					if(stricmp(data[i].author, data[i+1].author) < 0)
					{
						tmp = data[i];
						data[i] = data[i+1];
						data[i+1] = tmp;
					}
				}
			}; break;
		}

		case 2:
		int c;

		printf("1 - Сортировка по возрастанию, 2 - Сортировка по убыванию\n");
		fflush(stdin);
		scanf("%d", &c);

		switch (c) {

		case 1:
			for(int j = 0; j < count; j++)
			{
				for(int i = 0; i < count - 1; i++)
				{
					if(data[i].price > data[i+1].price)
					{
						tmp = data[i];
						data[i] = data[i+1];
						data[i+1] = tmp;
					}
				}
			}; break;

			case 2:
			for(int j = 0; j < count; j++)
			{
				for(int i = 0; i < count - 1; i++)
				{
					if(data[i].price < data[i+1].price)
					{
						tmp = data[i];
						data[i] = data[i+1];
						data[i+1] = tmp;
					}
				}
			}; break;

			default: printf("Error"); break;
		}

		default: printf("Вы ввели неверный номер.\n"); break;
	}
};

void FindRecord(Books* data, unsigned &count) {
	int num;

	printf("1 - Поиск по автору, 2 - Поиск по названию книги\n");
	scanf("%d", &num);


	switch(num) {
		case 1: char find[20];

				printf("Введите символы для поиска:\n");
				fflush(stdin);
				scanf("%20[^\n]s", find);

				 for (int i = 0; i < count; i++)
				{
					if (strstr(data[i].author, find))
					printf("| %-20s | %-20s | %-4.3lf | %-3d | %-13s|\n", data[i].author, data[i].bookname, data[i].price, data[i].amount, data[i].codenum);
				}
				break;
		case 2: char fin[20];

				printf("Введите символы для поиска:\n");
				fflush(stdin);
				scanf("%20[^\n]s", fin);

				 for (int i = 0; i < count; i++)
				{
					if (strstr(data[i].bookname, fin))
					printf("| %-20s | %-20s | %-4.3lf | %-3d | %-13s|\n", data[i].author, data[i].bookname, data[i].price, data[i].amount, data[i].codenum);
				}
				break;
		default: printf("Вы ввели неверный номер.\n"); break;
	}
	system("pause");
};


void AveragePrice(Books* data, unsigned &count) {
	double n = 0;

	for(int i = 0; i < count; i++) {
		n += data[i].price;
    }

	n = n/count;
	printf("%.3lf\n", n);

	system("pause");
};

void ShowData (Books* data, unsigned &count) {

	eCMD cmd = CMD_NONE;

	while( cmd != CMD_EXIT) {
		system("cls");

		char n[] = "№";
		char auth[] = "Автор";
		char bname[] = "Название книги";
		char pric[] = "Цена";
		char num[] = "Количество";
		char code[] = "Кодовый номер";

		printf("*--------------------------------------------------------------------------------------------------*\n");
		printf("| %2s | %-20s | %-30s | %-7s | %-3s | %-13s|\n", n, auth, bname, pric, num, code);
		printf("*--------------------------------------------------------------------------------------------------*\n");

		for (int i = 0; i < count; i++) {
			printf("| %2d | %-20s | %-30s | %-4.3lf | %-10d | %-13s|\n", i + 1, data[i].author, data[i].bookname, data[i].price, data[i].amount, data[i].codenum);
			printf("*--------------------------------------------------------------------------------------------------*\n");
		}

		cmd = MenuShow();

		switch(cmd) {
			case CMD_EDIT: EditRecord(data, count); break;
			case CMD_ADD: AddRecord(data, count); break;
			case CMD_DELETE: DeleteRecord(data, count); break;
			case CMD_SORT: SortRecord(data, count); break;
			case CMD_FIND: FindRecord(data, count); break;
			case CMD_AVERAGE: AveragePrice(data, count); break;
		}
	}
}
