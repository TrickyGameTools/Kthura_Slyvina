// Lic:
// Kthura
// Project
// 
// 
// 
// (c) Jeroen P. Broks, 2015, 2016, 2017, 2019, 2021, 2023
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

#include <SlyvQCol.hpp>

#include "../headers/ConfigCLI.hpp"
#include "../headers/Project.hpp"
#include "../headers/Glob.hpp"

using namespace Slyvina::Units;


namespace Slyvina {
	namespace Kthura {
		namespace Editor {
			using namespace Launcher;
			GINIE _PrjCfg{ nullptr };

			std::string ProjectDir() { return ProjectsDir() + "/" + ProjectName(); }
			std::string ProjectFile() { return ProjectDir() + "/" + ProjectName() + ".Project.ini"; }

			Units::GINIE Project() {
				if (!_PrjCfg) {
					QCol->Doing("Loading", ProjectFile());
					_PrjCfg = LoadGINIE(ProjectFile(), ProjectFile(), "Kthura Project\n" + ProjectFile());
					if (!_PrjCfg) {
						QCol->Error("Project file (" + ProjectFile() + ") not read properly!");
						exit(2);
					}
					//std::cout << (int)_PrjCfg.get() << "\n";
					return _PrjCfg;
				}
			}
			std::string MapDir() {
				/* Doesn't work for NO REASON AT ALL!
				auto FuckYou{ Project() }; std::cout << (int)FuckYou.get() << std::endl;
				 FuckYou->Value(
					 "Paths." + Platform(), 
					 "Maps", 
					 ChReplace(Project("Paths." + Platform(), "Maps"), '\\', '/')
				 ); 
				 */
				 return ChReplace( Project("Paths." + Platform(), "Maps"),'\\','/');
			}
			std::string MapFile() {
				return MapDir() + "/" + MapName();
			}
		}
	}
}
