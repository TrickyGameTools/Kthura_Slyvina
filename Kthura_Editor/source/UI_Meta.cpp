// Lic:
// Kthura
// Meta Data Editor
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
// Version: 23.09.28
// EndLic

#include <june19.hpp>
#include <SlyvGINIE.hpp>

#include "../headers/UserInterface.hpp"
#include "../headers/Project.hpp"
#include "../headers/MapData.hpp"

using namespace Slyvina::Units;
using namespace Slyvina::June19;

namespace Slyvina {
	namespace Kthura {
		namespace Editor {

			struct MetaField {
				std::string Field{""};
				j19gadget* Label;
				j19gadget* Textfield;
			};
			std::vector<MetaField> Fields{};

			static void CancelAct(June19::j19gadget*, June19::j19action) { UI::GoToStage("Editor"); }
			static void OkayDraw(June19::j19gadget* g, June19::j19action) { g->X(200 - g->W()); }


			static void OkayAct(June19::j19gadget*, June19::j19action) {
				for (auto& fld : Fields) 
					(*MapData->TheMap->MetaData)[fld.Field] = fld.Textfield->Text;
				UI::GoToStage("Editor");
			}
			


			static void MakeUI(){
				auto Stage{ UI::GetStage("Meta") };
				auto Panel{ Stage->MainGadget };
				auto Head{ CreateLabel("Set up the game's metadata",0,0,Panel->W(),20,Panel,2) };
				auto Prj{ LoadUGINIE(ProjectFile()) };
				auto Lst{ Prj->List("MAP","GENERALDATA") };
				auto Y = 40;
				Head->SetForeground(0, 180, 255);
				for (auto fld : *Lst) {
					Fields.push_back({ fld,CreateLabel(fld,0,Y,200,20,Panel),CreateTextfield(200,Y,Panel->W()-210,Panel)});
					Y += 22;
				}
				for (auto& fld : Fields) {
					fld.Textfield->SetForeground(255, 180, 0);
					fld.Textfield->SetBackground(25, 18, 0);
				}
				auto Cancel{ CreateButton("Cancel",200,Panel->H() - 30,Panel) };
				auto Okay{ CreateButton("Okay",200,Panel->H() - 30,Panel) };
				Cancel->SetForeground(255, 0, 0);
				Cancel->SetBackground(25, 0, 0);
				Cancel->CBAction = CancelAct;
				Okay->SetForeground(0, 255, 0);
				Okay->SetBackground(0, 25, 0);
				Okay->CBDraw = OkayDraw;
				Okay->CBAction = OkayAct;
			}

			void GoToMetaData(June19::j19gadget*, June19::j19action) {
				if (UI::NewStage("Meta")) MakeUI();
				for (auto& fld : Fields)
					fld.Textfield->Text = (*MapData->TheMap->MetaData)[fld.Field];
				UI::GoToStage("Meta");
			}
		}
	}
}