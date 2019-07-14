#include "stdafx.h"
#pragma warning(disable : 4996)
#include "Log.h"
#include "Error.h"
#include <fstream>
#include <stdio.h>  
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <string.h>
#include "Parm.h"

namespace Log
{
	LOG getlog(wchar_t logfile[])// создаем и открываем потоковый вывод
	{
		LOG log;
		wcscpy_s(log.logfile, logfile);
		log.stream = new std::ofstream(logfile, std::ofstream::out);
		if (log.stream->fail()) {
			throw ERROR_THROW(112);
		}
		return log;
	}

	void WriteLine(LOG log, char*c, ...)
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
		log.stream->write(str, len);
		delete[] str;
	}

	void WriteLine(LOG log, wchar_t*c, ...)
	{
		wchar_t** pc = &c; //адрес первого указател€ 
		int len = 0;
		while (*pc != L"") { // цикл дл€ определени€ общей длины сцепл€емых строк
			len += wcslen(*pc);
			pc++;
		}
		wchar_t *str = new wchar_t[len + 1]; //пам€ть дл€ строки 
		char* strl = new char[len + 1];
		str[0] = 0; // очищаем строку
		pc = &c; // установка на 1-й параметр 
		while (*pc != L"") // цикл дл€ сцеплени€ строк 
		{
			wcscat(str, *pc); // прицепл€ем 
			pc++; // перемещаемс€ на следующую 
		}
		wcstombs(strl, str, len + 1);
		log.stream->write(strl, len);
		delete[] str;
	}

	void WriteLog(LOG log)
	{
		time_t curTime;
		tm timeStruct;// структура с датой и временем
		time(&curTime);

		char strTime[30];
		localtime_s(&timeStruct, &curTime);// преобразует секунды в текущую дату

		strftime(strTime, 1024, "%d.%m.%Y %H:%M:%S", &timeStruct);// врем€ и дата в строку
		*log.stream << "---- ѕротокол ------ " << strTime << " ------------------" << std::endl;
	}

	void WriteParm(LOG log, Parm::PARM parm)
	{
		*log.stream << "---- ѕараметры ------" << std::endl;

		char temp[PARM_MAX_SIZE + 1];
		wcstombs(temp, PARM_LOG, wcslen(PARM_LOG) + 1);
		*log.stream << temp << " ";
		wcstombs(temp, parm.log, wcslen(parm.log) + 1);
		*log.stream << temp << std::endl;

		wcstombs(temp, PARM_OUT, wcslen(PARM_OUT) + 1);
		*log.stream << temp << " ";
		wcstombs(temp, parm.out, wcslen(parm.out) + 1);
		*log.stream << temp << std::endl;

		wcstombs(temp, PARM_IN, wcslen(PARM_IN) + 1);
		*log.stream << temp << " ";
		wcstombs(temp, parm.in, wcslen(parm.in) + 1);
		*log.stream << temp << std::endl;
	}

	void WriteIn(LOG log, In::IN in)
	{
		*log.stream << "---- »сходные данные ------" << std::endl;
		*log.stream << " оличество символов: " << in.size << std::endl;
		*log.stream << "ѕроигнорировано    : " << in.ignor << std::endl;
		*log.stream << " оличество строк   : " << in.lines + 1 << std::endl;
	}

	void WriteError(LOG log, Error::ERROR error)
	{
		*log.stream << "ќшибка " << error.id << ": " << error.message << ", строка " << error.inext.line + 1 << ",позици€ " << error.inext.col + 1 << std::endl;
	}

	void Close(LOG log)
	{
		log.stream->close();
		delete log.stream;
		log.stream = NULL;
	}
}