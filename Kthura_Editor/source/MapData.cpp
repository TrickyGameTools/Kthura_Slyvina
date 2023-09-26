// Lic:
// Kthura
// Mapdata
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
// Version: 23.09.26
// EndLic
#include <SlyvVolumes.hpp>
#include <SlyvStream.hpp>
#include <SlyvQCol.hpp>
#include <SlyvTime.hpp>

#include "../builddate.hpp"

#include "../headers/ConfigCLI.hpp"
#include "../headers/Project.hpp"
#include "../headers/MapData.hpp"

using namespace Slyvina::Units;

namespace Slyvina {
	namespace Kthura {
		namespace Editor {
			std::unique_ptr<TMapData> MapData{new TMapData()};

			std::string TMapData::TextureSettingsDir() {
				return AVolPath(ProjectDir() + "/Texures_Settings");
			}

			std::string TMapData::TextureSettingsFile() {
				return TextureSettingsDir() + "/" + MapName()+".ini";
			}
			void TMapData::Load() {
				if (!FileExists(TextureSettingsFile())) {
					if (!DirectoryExists(TextureSettingsDir())) {
						QCol->Doing("Creating dir", TextureSettingsDir());
						MakeDir(TextureSettingsDir());
					}
					QCol->Doing("Creating", TextureSettingsFile());
					SaveString(TextureSettingsFile(), TrSPrintF(
						"[creation]\n"
						"Tool=Kthura Editor for Slyvina\n"
						"ToolBuild=" SuperTed_BuildDate "\n"
						"Created=%s; %s",CurrentDate().c_str(),CurrentTime().c_str()
					));
				}
				QCol->Doing("Reading", TextureSettingsFile());
				TextureSettings = LoadUGINIE(TextureSettingsFile());

				if (!FileExists(MapFile)) {
					QCol->Doing("Creating", "New Map");
					TheMap = CreateKthura();
				} else {
					QCol->Doing("Loading", MapFile);
					TheMap = LoadKthura(MapFile);
				}
			}
		}
	}
}