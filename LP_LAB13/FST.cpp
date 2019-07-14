#include "stdafx.h"
#include "FST.h"
#include "In.h"
#include "Out.h"

#include "string.h" 
#include <iostream>

namespace Fst
{
	RELATION::RELATION(char c, short nn)
	{
		symbol = c;
		nnode = nn;
	};

	NODE::NODE()
	{
		n_relation = 0;
		RELATION *relaions = NULL;
	};

	NODE::NODE(short n, RELATION rel, ...)
	{
		n_relation = n;
		RELATION *p = &rel;
		relations = new RELATION[n];
		for (short i = 0; i < n; i++)
			relations[i] = p[i];
	};

	FST::FST(char* s, short ns, NODE n, ...)
	{
		string = s;
		nstates = ns;
		nodes = new NODE[ns];
		NODE *p = &n;
		for (int k = 0; k < ns; k++)
			nodes[k] = p[k];
		rstates = new short[nstates];
		memset(rstates, 0xff, sizeof(short)*nstates);
		rstates[0] = 0;
		position = -1;
	}

	bool step(FST& fst, short* &rstates)
	{
		bool rc = false;
		std::swap(rstates, fst.rstates);
		for (short i = 0; i < fst.nstates; i++)
		{
			if (rstates[i] == fst.position)
				for (short j = 0; j < fst.nodes[i].n_relation; j++)
				{
					if (fst.nodes[i].relations[j].symbol == fst.string[fst.position])
					{
						fst.rstates[fst.nodes[i].relations[j].nnode] = fst.position + 1;
						rc = true;
					};
				};
		};
		return rc;
	};

	bool execute(FST& fst)
	{
		short* rstates = new short[fst.nstates];
		short lstring = strlen(fst.string);
		bool rc = true;
		for (short i = 0; i < lstring && rc; i++)
		{
			fst.position++;
			rc = step(fst, rstates);
		}
		delete[] rstates;
		return (rc ? (fst.rstates[fst.nstates - 1] == lstring) : rc);
	};

	int executee(FST& fst)
	{
		short* rstates = new short[fst.nstates];
		short lstring = strlen(fst.string);
		bool rc = true;
		for (short i = 0; i < lstring && rc; i++)
		{
			fst.position++;
			rc = step(fst, rstates);
		}
		delete[] rstates;
		return (rc ? 0 : fst.position);
	};

	void fstWrite(In::IN in, Out::OUT out)
	{
		for (int i = 0; i < in.kolvo; i++)
		{
			FST fst(
				in.splitted[i],
				7,
				NODE(1, RELATION('a', 1)),
				NODE(3, RELATION('b', 1), RELATION('c', 2), RELATION('h', 5)),
				NODE(3, RELATION('d', 3), RELATION('f', 4), RELATION('h', 5)),
				NODE(3, RELATION('y', 3), RELATION('e', 5), RELATION('h', 5)),
				NODE(2, RELATION('x', 4), RELATION('e', 5)),
				NODE(2, RELATION('b', 5), RELATION('g', 6)),
				NODE()
			);

			if (execute(fst))
				*out.stream << "Цепочка " << fst.string << " распознана" << std::endl;
			else
			{
				*out.stream << "Цепочка " << fst.string << " не распознана. ";
				*out.stream << "Строка " << i + 1;
				*out.stream << " ,позиция " << executee(fst) << std::endl;
			}
		}
	}
}