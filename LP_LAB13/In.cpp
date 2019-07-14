#include "stdafx.h"
#pragma warning(disable : 4996)
#include "In.h"
#include "Error.h"
#include <fstream>
#include <iostream>
#include <string.h>

using namespace std;

namespace In
{
	IN getin(wchar_t infile[])
	{
		IN in;
		in.lines = 0;
		in.ignor = 0;
		in.size = 0;
		in.kolvo = 0;
		int constCode[128] = IN_CODE_TABLE;
		for (int i = 0; i < 128; i++)
			in.code[i] = constCode[i];
		std::ifstream ifs;// открыть для ввода
		ifs.open(infile, std::ifstream::in);//открываем для чтения
		if (ifs.fail()) {
			throw ERROR_THROW(110);
		}
		char c = ifs.get();//чтение по 1 байту
		int curLine = 0;
		int posSymb = 0;
		while (ifs.good()) {//нет ошибки
			posSymb++;
			if ((c == IN_CODE_ENDL) || (c == IN_CODE_END)) {
				in.lines++;
				curLine++;
				posSymb = 0;
				in.size++;
			}
			else if (in.code[c] == IN::F) {
				throw ERROR_THROW_IN(111, curLine, posSymb);
			}
			else if (in.code[c] == IN::I) {
				in.ignor++;
			}
			else if (in.code[c] == IN::T) {
				in.size++;
			}
			else {
				in.size++;
			}
			c = ifs.get();
		}
		ifs.close();

		in.text = new unsigned char[in.size];
		ifs.open(infile, std::ifstream::in);
		c = ifs.get();
		posSymb = 0;
		while (ifs.good()) {
			if (c == IN_CODE_ENDL) {
				in.text[posSymb] = IN_CODE_ENDL;
				posSymb++;
			}
			else if (c == IN_CODE_END) {
				in.text[posSymb] = IN_CODE_END;
				posSymb++;
			}
			else if (in.code[c] == IN::I) {

			}
			else if (in.code[c] == IN::T) {
				in.text[posSymb] = c;
				posSymb++;
			}
			else if (in.code[c] >= 0 && in.code[c] < 256) {
				in.text[posSymb] = in.code[c];
				posSymb++;
			}
			c = ifs.get();
		}
		in.text[posSymb] = 0;// завершаем строку
		ifs.close();
		char* str = new char[in.size];
		for (int z = 0; z < in.size; z++)
			str[z] = in.text[z];
		in.splitted = new char*[256];
		int prevPos = 0;
		int curArrPos = 0;
		for (int i = 0; i <= in.size; i++)
		{
			if ((str[i] == '|'))
			{
				in.splitted[curArrPos] = new char[10];
				strncpy(in.splitted[curArrPos], (char*)(str + prevPos), i - prevPos);
				in.splitted[curArrPos][i - prevPos] = 0;
				curArrPos++;
				prevPos = i + 1;
				in.kolvo++;
			}
			if ((i == in.size))
			{
				in.splitted[curArrPos] = new char[10];
				strncpy(in.splitted[curArrPos], (char*)(str + prevPos), i - prevPos - 1);
				in.splitted[curArrPos][i - prevPos - 1] = 0;
				curArrPos++;
				prevPos = i + 1;
				in.kolvo++;
			}
		}

		return in;
	}
}