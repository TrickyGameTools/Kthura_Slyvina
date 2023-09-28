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
#include <TQSE.hpp>



#include "../builddate.hpp"

#include "../headers/ConfigCLI.hpp"
#include "../headers/Project.hpp"
#include "../headers/MapData.hpp"
#include "../headers/Resource.hpp"
#include "../headers/UserInterface.hpp"
#include "../headers/UI_MainEditor_Class.hpp"

using namespace Slyvina::Units;

namespace Slyvina {
	namespace Kthura {
		namespace Editor {

			void KthuraInPaniek(std::string msg,std::string xdata) {
				TQSE::Notify("KTHURA THREW AN ERROR!\n\n" + msg + "\n\n\n" + xdata);
				QCol->Error(msg);
				QCol->Cyan(xdata);
				QCol->Reset();
				exit(8);
			}

			std::unique_ptr<TMapData> MapData{nullptr};

			TMapData::TMapData() {
				QCol->Doing("Initizing", "Map Data");
				KthuraPanic = KthuraInPaniek;
				Draw = Init_TQSG_For_Kthura(TexResource());				
			}

			std::string TMapData::TextureSettingsDir() {
				return AVolPath(ProjectDir() + "/Texures_Settings");
			}

			std::string TMapData::TextureSettingsFile() {
				return TextureSettingsDir() + "/" + MapName()+".ini";
			}
			std::string TMapData::CurrentLayerTag() {
				if (LayerSelector->SelectedItem() < 0) return "";
				return LayerSelector->ItemText();
			}

			KthuraLayer* TMapData::CurrentLayer() {
				if (CurrentLayerTag() == "") return nullptr;
				return TheMap->Layer(CurrentLayerTag());
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

			inline int ScX() { return ScrollX - MapPanel->DrawX(); }
			inline int ScY() { return ScrollY - MapPanel->DrawY(); }

			void TMapData::DrawLayer(KthuraLayer* L) { Draw->DrawLayer(L, ScX(), ScY()); }
			void TMapData::DrawLayer(std::string LayTag) { Draw->DrawLayer(TheMap->Layer(LayTag), ScX(), ScY()); }
			void TMapData::DrawLayer() {
				if (CurrentLayer()) Draw->DrawLayer(CurrentLayer(), ScX(), ScY());
			}
		}
	}
}