// Lic:
// Kthura
// Mapdata
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
// Version: 24.02.18
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
using namespace Slyvina::June19;

namespace Slyvina {
	namespace Kthura {
		namespace Editor {

			void KthuraInPaniek(std::string msg,std::string xdata) {
				TQSE::Notify("KTHURA THREW AN ERROR!\n\n" + msg + "\n\n\n" + xdata);
				QCol->Error(msg);
				QCol->Cyan(xdata);
				QCol->Reset();
				std::cout << std::endl;
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
					TheMap = XLoadKthura(MapFile);
					auto Layers{ TheMap->Layers() };
					for (auto Layer : *Layers) TheMap->Layer(Layer)->TotalRemap();
				}
				ScrollX = ToInt(TheMap->Option("Scroll", "X"));
				ScrollY = ToInt(TheMap->Option("Scroll", "Y"));
			}

			inline int ScX() { return ScrollX - MapPanel->DrawX(); }
			inline int ScY() { return ScrollY - MapPanel->DrawY(); }

			void TMapData::DrawLayer(KthuraLayer* L) { Draw->DrawLayer(L, ScX(), ScY()); }
			void TMapData::DrawLayer(std::string LayTag) { Draw->DrawLayer(TheMap->Layer(LayTag), ScX(), ScY()); }
			void TMapData::DrawLayer() {
				if (CurrentLayer()) Draw->DrawLayer(CurrentLayer(), ScX(), ScY());
			}


			void TMapData::StoreTexSettings(UIE* Panel, std::string _kind, std::string _tex) {
				static std::map<bool, std::string> bname{{false, "No"}, { true,"Yes" }};
				auto _tag{ "TEX::" + _kind + "::" + _tex };
				TextureSettings->Value("Kinds", _tex, _kind);
				
				for (auto& gg : Panel->TexLink) {
					switch (gg.second->GetKind()) {
					case j19kind::Button:
					case j19kind::Label:
						TextureSettings->Value(_tag, gg.first, gg.second->Caption);
						break;
					case j19kind::CheckBox:
					case j19kind::RadioButton:
						TextureSettings->Value(_tag, gg.first, bname[gg.second->checked]);
						break;
					//case j19kind::TextField: // ?
					case j19kind::Textfield:
						TextureSettings->Value(_tag, gg.first, gg.second->Text);
						break;
					default:
						throw std::runtime_error(TrSPrintF("StoreTexSettings(<Panel>,\"%s\",\"%s\"): Unknown gadget kind (%d)", _kind.c_str(), _tex.c_str(), (int)gg.second->GetKind()));
					}
				}
			}
			void TMapData::RestoreTexSettings(UIE* Panel, std::string _tex) {
				static std::map<bool, std::string> bname{{false, "No"}, { true,"Yes" }};
				auto _kind{ Panel->MyRadioButton->Caption };
				auto _tag{ "TEX::" + _kind + "::" + _tex };
				TextureSettings->Value("Kinds", _tex, _kind);

				for (auto& gg : Panel->TexLink) {
					switch (gg.second->GetKind()) {
					case j19kind::Button:
					case j19kind::Label:
						TextureSettings->NewValue(_tag, gg.first, gg.second->Caption);
						gg.second->Caption = TextureSettings->Value(_tag, gg.first);
						break;
					case j19kind::CheckBox:
					case j19kind::RadioButton:
						TextureSettings->NewValue(_tag, gg.first, bname[gg.second->checked]);
						gg.second->checked = Upper(TextureSettings->Value(_tag, gg.first)) == "YES";
						break;
					//case j19kind::TextField:
					case j19kind::Textfield:
						TextureSettings->NewValue(_tag, gg.first, gg.second->Text);
						gg.second->Text = TextureSettings->Value(_tag, gg.first);
						break;
					default:
						throw std::runtime_error(TrSPrintF("RestoreTexSettings(<Panel>,\"%s\"): Unknown gadget kind (%d)", _tex.c_str(), (int)gg.second->GetKind()));
					}
				}
			}

			void TMapData::RestoreTexSettings(std::string _kind, std::string _tex) {
				for (auto panelitem : UIE::_Register) {
					auto panel{ &UIE::_Register[panelitem.first] };
					panel->MyRadioButton->checked = _kind == panel->MyRadioButton->Caption;
					if (panel->MyRadioButton->checked) RestoreTexSettings(panel, _tex);
				}
			}
			void TMapData::SaveSettings() {
				QCol->Doing("Saving", TextureSettingsFile());
				TextureSettings->SaveSource(TextureSettingsFile(), "Kthura texture settings!");
			}
			void TMapData::SaveMap() {
				if (!TheMap) {
					QCol->Warn("No map data so no saving!");
				} else {
					QCol->Doing("Saving", MapFile);
					Kthura_Save(TheMap, MapFile);
				}
			}
			std::string TMapData::TexKind(std::string _tex) {
				if (!TextureSettings->HasValue("Kinds", _tex)) return "";
				return TextureSettings->Value("Kinds", _tex);				
			}
		}
	}
}