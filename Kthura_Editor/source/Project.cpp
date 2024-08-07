// Lic:
// Kthura
// Project
// 
// 
// 
// (c) Jeroen P. Broks, 2015, 2016, 2017, 2019, 2021, 2023, 2024
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
// Version: 24.07.17
// EndLic

#include <SlyvQCol.hpp>

#include "../headers/ConfigCLI.hpp"
#include "../headers/Project.hpp"
#include "../headers/Glob.hpp"

using namespace Slyvina::Units;


namespace Slyvina {
	namespace Kthura {
		namespace Editor {
			
			UGINIE _PrjCfg{ nullptr };

			std::string ProjectDir() { return ProjectsDir() + "/" + ProjectName(); }
			std::string ProjectFile() { return ProjectDir() + "/" + ProjectName() + ".Project.ini"; }

			Units::RawGINIE* Project() {
				if (!_PrjCfg) {
					QCol->Doing("Loading", ProjectFile());
					_PrjCfg = LoadUGINIE(ProjectFile(), ProjectFile(), "Kthura Project\n" + ProjectFile());
					if (!_PrjCfg) {
						QCol->Error("Project file (" + ProjectFile() + ") not read properly!");
						exit(2);
					}
					//std::cout << (int)_PrjCfg.get() << "\n";					
				}
				return _PrjCfg.get();
			}
			std::string Project(std::string cat, std::string key) { return Project()->Value(cat, key); }

			void Project(std::string cat, std::string key, std::string value) { Project()->Value(cat, key, value); }
			
			std::string MapDir() {
				/* Doesn't work for NO REASON AT ALL!
				auto FuckYou{ Project() }; std::cout << (int)FuckYou.get() << std::endl;
				 FuckYou->Value(
					 "Paths." + Platform(), 
					 "Maps", 
					 ChReplace(Project("Paths." + Platform(), "Maps"), '\\', '/')
				 ); 
				 */
				
				auto prj{ Project() }; if (!prj) QCol->Error("For some reason the GINIE data did not load");
				auto ret{ prj->Value("Paths." + Platform(), "Maps") };
				return ChReplace(ret, '\\', '/');
			}

			std::string MapFile() {				
				return MapDir() + "/" + MapName();
			}
		}
	}
}