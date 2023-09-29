// Lic:
// Kthura
// Fixer (main)
// 
// 
// 
// (c) Jeroen P. Broks, 2023
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Please note that some references to data like pictures or audio, do not automatically
// fall under this licenses. Mostly this is noted in the respective files.
// 
// Version: 23.09.29
// EndLic

#include "builddate.hpp"
#include "Kthura_Fix_Work.hpp"

#include <SlyvQCol.hpp>

#include <JCR6_zlib.hpp>

using namespace Slyvina;
using namespace JCR6;
using namespace Units;
using namespace Kthura;
using namespace Fixer;
using namespace std;

void Help() {
	QCol->Reset();
	cout << "Kthura Fixer is a small program which will try to analyse a Kthura file for the most common issues and try to fix this.\n";
	cout << "Kthura Fixer will however make no bones about the situation.\nIt *is* recommendable to backup your maps BEFORE you try this tool!\n\n";
	QCol->White("Usage: ");
	QCol->Yellow("Kthura_Fix ");
	QCol->Cyan("<file1> [<file2> [<file3......]]]");
	cout << endl << endl;
}

int main(int countargs,char** args) {
	QCol->Green("Kthura Fixer\n");
	QCol->Doing("Build", SuperTed_BuildDate);
	QCol->Doing("Coded by:", "Jeroen P. Broks");
	QCol->Magenta("\n\nCopyright Jeroen P. Broks - Licensed under the terms of the GPL3\n\n");
	if (countargs<=1) {
		Help();
	} else {
		init_zlib();
		for (int i = 1; i < countargs; ++i)
			Process(i, countargs - 1, args[i]);
	}
	QCol->Reset();
	cout << "\n\n";
	return 0;
}