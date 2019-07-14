// LPLab13.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"


#include "Error.h"
#include "In.h"
#include "Log.h"
#include "Parm.h"
#include "FST.h"
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_CTYPE, "rus");
	std::cout << "---тест Error::getrror---" << std::endl << std::endl;
	try
	{
		throw ERROR_THROW(100);
	}
	catch (Error::ERROR e)
	{
		std::cout << "Ошибка " << e.id << ": " << e.message << std::endl << std::endl;
	};

	std::cout << "---тест Error::getrrorin---" << std::endl << std::endl;
	try
	{
		throw ERROR_THROW_IN(111, 7, 7);
	}
	catch (Error::ERROR e)
	{
		std::cout << "Ошибка " << e.id << ": " << e.message
			<< ", строка " << e.inext.line
			<< ", позиция " << e.inext.col << std::endl << std::endl;
	};

	std::cout << "--тест Parm::getparm ---" << std::endl << std::endl;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		std::wcout << "-in:" << parm.in << ", -out:" << parm.out << ", log:" << parm.log << std::endl << std::endl;
	}
	catch (Error::ERROR e)
	{
		std::cout << "Ошибка " << e.id << " : " << e.message << std::endl << std::endl;

	};

	std::cout << "---тест In:getin---" << std::endl << std::endl;
	try {
		Parm::PARM parm = Parm::getparm(argc, argv);
		In::IN in = In::getin(parm.in);
		std::cout << in.text << std::endl;
		std::cout << "Всего символов: " << in.size << std::endl;
		std::cout << "Всего строк: " << in.lines + 1 << std::endl;
		std::cout << "Пропущено: " << in.ignor << std::endl;
	}
	catch (Error::ERROR e) {
		std::cout << "Ошибка " << e.id << ": " << e.message << std::endl << std::endl;
		std::cout << "Строка " << e.inext.line + 1 << " позиция " << e.inext.col + 1 << std::endl << std::endl;;
	};

	Log::LOG log = Log::INITLOG;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		Log::WriteLine(log, "Тест:", " без ошибок \n", "");
		Log::WriteLine(log, L"Тест:", L" без ошибок \n", L"");
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		In::IN in = In::getin(parm.in);
		Log::WriteIn(log, in);
		Log::Close(log);
	}
	catch (Error::ERROR e) {
		Log::WriteError(log, e);
	};

	Out::OUT out = Out::INITOUT;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		out = Out::getout(parm.out);
		Out::WriteLine(out, "Тест:", " без ошибок \n", "");
		Out::WriteOut(out);
		In::IN in = In::getin(parm.in);
		Fst::fstWrite(in, out);
		Out::WriteIn(out, in);
		Out::Close(out);
	}
	catch (Error::ERROR e) {
		Out::WriteError(out, e);
	};

	system("pause");
	return 0;
}


