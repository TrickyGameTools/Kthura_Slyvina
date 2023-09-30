// Lic:
// Kthura
// Tagging
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
// Version: 23.09.30
// EndLic

#include <SlyvQCol.hpp>

#include "../headers/UI_MainEditor_Class.hpp"
#include "../headers/UI_MainEditor_Modify.hpp"
#include "../headers/UI_Layers.hpp"

namespace Slyvina {

	using namespace Units;
	using namespace June19;

	namespace Kthura {
		namespace Editor {

			static j19gadget
				* TagCaption{ nullptr },
				* TagTextField{ nullptr },
				* TagExists{ nullptr },
				* TagOkay{ nullptr },
				* TagCancel{ nullptr };

#pragma region Callbacks
			static void ActCancel(j19gadget*, j19action) { UI::GoToStage("Editor"); }
			static void UpdateCancel(j19gadget*, j19action) { TagCancel->X(TagOkay->X() + TagOkay->W() + 5); }
			static void UpdateOkay(j19gadget* g, j19action) {
				//TagTextField->Text = TagTextField->Text;
				TagExists->Visible = CurrentLayer()->HasTag(Trim(TagTextField->Text));
				g->Enabled = TagTextField->Text.size() && (!TagExists->Visible);
			}
			static void ActOkay(j19gadget*, j19action) {
				ModifyObject->Tag(Trim(TagTextField->Text));
				CurrentLayer()->RemapTags();
				UIE::_Register["Modify"].Tag->Caption = TagTextField->Text;
				UI::GoToStage("Editor");
			}
#pragma endregion




			void InitTagStage(){
				auto stage{ UI::GetStage("Tagging") };
				auto panel{ stage->MainGadget };
				TagCaption = CreateLabel("", 5, 5, panel->W(), 20, panel);
				TagExists = CreateLabel("A Tag with that name already exists", 0, 50, panel->W(), 20, panel, 2);
				TagExists->SetForeground(255, 0, 0);
				TagTextField = CreateTextfield(5, 100, panel->W() - 10, panel);
				TagTextField->SetForeground(255, 255, 255);
				TagTextField->SetBackground(0, 0, 100);
				TagOkay = CreateButton("Okay", 5, 150, panel);
				TagOkay->SetForeground(0, 255, 0);
				TagOkay->SetBackground(0, 25, 0);
				TagOkay->CBDraw = UpdateOkay;
				TagOkay->CBAction = ActOkay;
				TagCancel = CreateButton("Cancel", 0, 150, panel);
				TagCancel->SetForeground(255, 0, 0);
				TagCancel->SetBackground(25, 0, 0);
				TagCancel->CBDraw = UpdateCancel;
				TagCancel->CBAction = ActCancel;
			}

			void TagButton(June19::j19gadget* b, June19::j19action) {
				if (!ModifyObject) {
					QCol->Error("Tag edit requested, but there's no object selected! How's that possible?");
					return;
				}
				if (UI::NewStage("Tagging")) InitTagStage();
				TagTextField->Text = ModifyObject->Tag();
				UI::GoToStage("Tagging");
			}
		}
	}
}