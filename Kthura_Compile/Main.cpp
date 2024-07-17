// Lic:
// Kthura Compiler
// "Compiles" Kthura maps
// 
// 
// 
// (c) Jeroen P. Broks, 2024
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
// Version: 24.03.22
// EndLic
#include <SlyvQCol.hpp>
#include <SlyvString.hpp>

#include <Kthura_Core.hpp>
#include <Kthura_Compiler.hpp>

#include <JCR6_zlib.hpp>

using namespace Slyvina;
using namespace Units;
using namespace Kthura;

void DoCompile(std::string kmapfile) {
	QCol->Doing("Reading", kmapfile);
	auto kmap{ LoadKthura(kmapfile) };
	QCol->Doing("Compiling", kmapfile);
	CompileStorage = "zlib";
	Compile(kmap, kmapfile + ".CKC");
}

int main(int c, char** a) {
	QCol->LMagenta("Kthura Compiler\n");
	QCol->Doing("Coded by", "Jeroen P. Broks");
	if (c < 2) {
		QCol->Yellow("Usage: ");
		QCol->White(StripAll(a[0]));
		QCol->LCyan(" <Kthura-Map>\n\n");
		QCol->Reset();
		std::cout << "Normal Kthura maps just contain a plain text files, and a data file packaged together\n"
			"a JCR6 file. As this text has to be parsed, larger maps in particular can take more loading time\n"
			"This compiler will convert this into byte code and will also make a dictionary of strings that are\n"
			"repeated all the time, saving time in the process.\n\n"
			"PLEASE NOTE!\n"
			"Compiled Kthura files are NOT a default in Kthura, so an extension driver in order to load a\n"
			"a compiled Kthura file will be required.\n\n";
		return 0;
	}
	JCR_InitZlib();
	for (auto i = 1; i < c; ++i) DoCompile(a[i]);
}