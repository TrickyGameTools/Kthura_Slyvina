// Lic:
// Kthura
// Exit placement
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
// Version: 23.10.03
// EndLic

#include <string>

#include <june19.hpp>

#include "../headers/UserInterface.hpp"
#include "../headers/UI_Layers.hpp"

namespace Slyvina {
	namespace Kthura {
		namespace Editor {

			using namespace June19;
			using namespace Units;

			j19gadget
				* ExitWind{ nullptr },
				* ExitTag{ nullptr },
				* ExitTagExists{ nullptr },
				* ExitOkay{ nullptr },
				* ExitCancel{ nullptr };
			static int exx{ 0 }, exy{ 0 };


			struct col { byte r, g, b; };
			static std::map<std::string, col> WCols {
				{ "North", { 180, 255, 255 } },
				{ "South", { 255, 100, 0 } },
				{ "East", { 255, 180, 0 } },
				{ "West", { 0, 180, 255 } }
			};

			static void ShowWind(j19gadget* me, j19action) {
				if (ExitWind->SelectedItem() < 0) ExitWind->SelectItem(0);
				auto c{ &WCols[ExitWind->ItemText()] };
				me->SetForeground(c->r, c->g, c->b);
				me->SetBackground(c->r / 10, c->g / 10, c->b / 10);
			}

			static void ShowOkay(j19gadget*, j19action) {
				auto hastag{ ExitTag->Text != "" && CurrentLayer()->HasTag(ExitTag->Text) };
				ExitTagExists->Visible = hastag;
				ExitOkay->Enabled = ExitTag->Text != "" && (!hastag);
				ExitOkay->X(ExitOkay->W());
				ExitOkay->Y(ExitOkay->GetParent()->H() - ExitOkay->H() - 10);
				ExitCancel->Y(ExitOkay->Y());
				ExitCancel->X((ExitOkay->X() * 2) + ExitCancel->W());
			}

			static void ActCancel(j19gadget*, j19action) { UI::GoToStage("Editor"); }

			static void ActOkay(j19gadget*, j19action) {
				auto o{ CurrentLayer()->NewObject(KthuraKind::Exit) };
				o->Tag(ExitTag->Text);
				o->data("Wind", ExitWind->ItemText());
				o->x(exx);
				o->y(exy);
				UI::GoToStage("Editor");
			}

			static void InitExit(){
				auto stage{ UI::GetStage("Other_Exit") };
				auto panel{ stage->MainGadget };
				CreateLabel("Please set up the data for the exit spot to place", 0, 0, panel->W(), 20, panel, 2);
				CreateLabel("Wind:", 0, 40, 200, 20, panel);
				ExitWind = CreateListBox(210,40,300,140,panel);
				ExitWind->AddItem("North");
				ExitWind->AddItem("South");
				ExitWind->AddItem("East");
				ExitWind->AddItem("West");
				ExitWind->CBDraw = ShowWind;
				CreateLabel("Tag", 0, 250, 200, 20, panel);
				ExitTag = CreateTextfield(210, 250, 300, panel);
				ExitTag->CBDraw = ShowWind;
				ExitTagExists = CreateLabel("Tag already exists", 600, 250, 200, 40, panel);
				ExitTagExists->SetForeground(255, 0, 0);
				ExitOkay = CreateButton("Okay", 0, 0, panel);
				ExitOkay->SetForeground(0, 255, 0);
				ExitOkay->SetBackground(0, 25, 0);
				ExitOkay->CBDraw = ShowOkay;
				ExitOkay->CBAction = ActOkay;
				ExitCancel = CreateButton("Cancel", 0, 0, panel);
				ExitCancel->SetForeground(255, 0, 0);
				ExitCancel->SetBackground(25, 0, 0);
				ExitCancel->CBAction = ActCancel;
			}

			void GoToExit(int x, int y, std::string item) {
				static int cnt = 0;
				std::string _concept{};
				if (UI::NewStage("Other_Exit")) InitExit();
				UI::GoToStage("Other_Exit");
				do { _concept = TrSPrintF("Exit_%08x", cnt++); } while (CurrentLayer()->HasTag(_concept));
				ExitTag->Text = _concept;
				exx = x;
				exy = y;
			}

		}
	}
}
