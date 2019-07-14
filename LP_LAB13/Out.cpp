#include "stdafx.h"
#pragma warning(disable : 4996)
#include "Out.h"
#include "Error.h"
#include "FST.h"
#include <fstream>
#include <stdio.h>  
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <string.h>
#include "Parm.h"

namespace Out
{
	OUT getout(wchar_t outfile[])// создаем и открываем потоковый вывод
	{
		OUT out;
		wcscpy_s(out.outfile, outfile);
		out.stream = new std::ofstream(outfile, std::ofstream::out);
		if (out.stream->fail()) {
			throw ERROR_THROW(112);
		}
		return out;
	}

	void WriteLine(OUT out, char*c, ...)
	{
		char** pc = &c;            //адрес первого указател€
		int len = 0;
		while (*pc != "") { // цикл дл€ определени€ общей длины сцепл€емых строк
			len += strlen(*pc);
			pc++;
		}
		char *str = new char[len + 1];       //пам€ть дл€ строки
		str[0] = 0;                       // очищаем строку
		pc = &c;                       // установка на 1-й параметр
		while (*pc != "") // цикл дл€ сцеплени€ строк
		{
			strcat(str, *pc);             // прицепл€ем 
			pc++;                       // перемещаемс€ на следующую
		}
		out.stream->write(str, len);
		delete[] str;
	}

	void WriteOut(OUT out)
	{
		time_t curTime;
		tm timeStruct;// структура с датой и временем
		time(&curTime);

		char strTime[30];
		localtime_s(&timeStruct, &curTime);// преобразует секунды в текущую дату

		strftime(strTime, 1024, "%d.%m.%Y %H:%M:%S", &timeStruct);// врем€ и дата в строку
		*out.stream << "---- ¬ыходной файл ------ " << strTime << " ------------------" << std::endl;
	}

	void WriteIn(OUT out, In::IN in)
	{
		*out.stream << " оличество строк   : " << in.lines + 1 << std::endl;
		for (int i = 0; i < in.kolvo; i++)
			*out.stream << in.splitted[i] << std::endl;
	}

	void WriteError(OUT out, Error::ERROR error)
	{
		*out.stream << "ќшибка " << error.id << ": " << error.message << ", строка " << error.inext.line + 1 << ",позици€ " << error.inext.col << std::endl;
	}

	void Close(OUT out)
	{
		out.stream->close();
		delete out.stream;
		out.stream = NULL;
	}
}