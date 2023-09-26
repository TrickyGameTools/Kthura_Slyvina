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