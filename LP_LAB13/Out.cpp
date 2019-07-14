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
	OUT getout(wchar_t outfile[])// ������� � ��������� ��������� �����
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
		char** pc = &c;            //����� ������� ���������
		int len = 0;
		while (*pc != "") { // ���� ��� ����������� ����� ����� ���������� �����
			len += strlen(*pc);
			pc++;
		}
		char *str = new char[len + 1];       //������ ��� ������
		str[0] = 0;                       // ������� ������
		pc = &c;                       // ��������� �� 1-� ��������
		while (*pc != "") // ���� ��� ��������� �����
		{
			strcat(str, *pc);             // ���������� 
			pc++;                       // ������������ �� ���������
		}
		out.stream->write(str, len);
		delete[] str;
	}

	void WriteOut(OUT out)
	{
		time_t curTime;
		tm timeStruct;// ��������� � ����� � ��������
		time(&curTime);

		char strTime[30];
		localtime_s(&timeStruct, &curTime);// ����������� ������� � ������� ����

		strftime(strTime, 1024, "%d.%m.%Y %H:%M:%S", &timeStruct);// ����� � ���� � ������
		*out.stream << "---- �������� ���� ------ " << strTime << " ------------------" << std::endl;
	}

	void WriteIn(OUT out, In::IN in)
	{
		*out.stream << "���������� �����   : " << in.lines + 1 << std::endl;
		for (int i = 0; i < in.kolvo; i++)
			*out.stream << in.splitted[i] << std::endl;
	}

	void WriteError(OUT out, Error::ERROR error)
	{
		*out.stream << "������ " << error.id << ": " << error.message << ", ������ " << error.inext.line + 1 << ",������� " << error.inext.col << std::endl;
	}

	void Close(OUT out)
	{
		out.stream->close();
		delete out.stream;
		out.stream = NULL;
	}
}