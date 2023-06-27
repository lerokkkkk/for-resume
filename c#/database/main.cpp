#include<iostream>
#include<stdio.h>
#include<string.h>
#include"main.h"
#include"File2.h"
#include"File3.h"

using namespace std;

Books Data[MAX_SIZE] = {
	{ "Федор Достоевский", "Преступление и наказание", 321.32, 21, "1238948273932"},
	{ "Иван Тургенев", "Отцы и дети", 321.313, 31, "2372829201928"},
	{ "Виктор Гюго", "Отверженные", 563.29, 21, "1234567891928"},
	{"Антон Чехов", "Рассказы", 321.42, 31, "0293716272910"}
};

unsigned Count = 4;

eCMD Menu()
{
	while(true) {
		system("cls");

		puts("Выберите действие:");
		puts("1 - Открыть файл");
		puts("2 - Просмотр данных");
		puts("3 - Записать данные в новый файл");
		puts("4 - Выход из программы");

		unsigned opt;

		fflush(stdin);
		scanf("%u", &opt);

		switch(opt) {
			case 1: return CMD_READ;
			case 2: return CMD_SHOW;
			case 3: return CMD_SAVENEW;
			case 4: return CMD_EXIT;
			default: puts("Вы ввели неправильную команду");
			system("pause");
		}
	}
}

int main() {
	system("chcp 1251");
	eCMD cmd = CMD_NONE;

	while(true) {
		if(cmd == CMD_EXIT) break;

		cmd = Menu();

		switch(cmd) {
			case CMD_READ: ReadDatabase(Data, Count); break;
			case CMD_SHOW: ShowData(Data, Count); break;
			case CMD_SAVENEW: SaveNewDatabase(Data, Count); break;
		}
	}

	puts("Работа закончена");

	system("pause");
	return 0;
}
