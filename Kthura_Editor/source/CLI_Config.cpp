// Lic:
// Kthura
// Command Line Interface decided configuration
// 
// 
// 
// (c) Jeroen P. Broks, 2015-2017, 2019, 2021, 2023
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
// Version: 23.09.25
// EndLic
#include <SlyvArgParse.hpp>
#include <SlyvString.hpp>
#include <SlyvQCol.hpp>

#include "../headers/ConfigCLI.hpp"

using namespace Slyvina;
using namespace Units;
using namespace std;

namespace Slyvina {
	namespace Kthura {
		namespace Editor {
			static ParsedArg CLIDat;
			static string _Project;
			static string _Map;

			string ProjectName() { return _Project; }
			string MapName() { return _Map; }

			std::string Decipher(std::string inp) {
				for (char ch = 1; ch < 127 && ch >= 0; ++ch) {
					char str[2] = { ch,0 };
					//printf(":%d: ", ch); cout << str << endl;
					inp = StReplace(inp, TrSPrintF(":%d:", ch), str);
				}
				return inp;
			}

			bool CLIParse(int c, char** a) {
				FlagConfig cfg;
				AddFlag(cfg, "dc", false);
				CLIDat = ParseArg(c, a, cfg);
				/*    DEGBU ONLY
				for (int i = 0; i < c; i++) printf("%8d> %s\n", i, a[i]); 
				printf("Arguments after parsing: %d\n", (int)CLIDat.arguments.size());
				printf("Set 'dc': %d\n", CLIDat.bool_flags["dc"]);
				//*/
				if (CLIDat.arguments.size()<2) {
					QCol->Error("No project and/or map arguments given");
					return false;
				}
				if (CLIDat.bool_flags["dc"]) {
					//printf("Deciphering!\n");
					_Project = Decipher(CLIDat.arguments[0]);
					_Map = Decipher(CLIDat.arguments[1]);
				} else {
					_Project = CLIDat.arguments[0];
					_Map = CLIDat.arguments[1];
				}
				QCol->Doing("Project", _Project);
				QCol->Doing("Map", _Map);
				return true;
			}
		}
	}
}