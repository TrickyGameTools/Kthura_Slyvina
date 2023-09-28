// Lic:
// Kthura
// Texture Selector
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
// Version: 23.09.27
// EndLic

#pragma once
#include <SlyvQCol.hpp>
#include "../headers/UserInterface.hpp"
#include "../headers/Resource.hpp"
#include "../headers/UI_MainEditor_Class.hpp"

using namespace Slyvina;
using namespace June19;
using namespace Units;

namespace Slyvina {
	namespace Kthura {
		namespace Editor {

			static void AutoScreenUpdate(j19gadget*, j19action);
			static void SelectTex(j19gadget* g, j19action a);
			static void CancelTex(j19gadget*, j19action) { UI::GoToStage("Editor"); }
			static void OkayTex(j19gadget*, j19action);

			static j19gadget
				* TexPanel{ nullptr },
				* TexOpt{ nullptr },
				* TexListAll{ nullptr },
				* TexListUsed{ nullptr },
				* TexListFiltered{ nullptr },
				* TexOkay{ nullptr },
				* TexCancel{ nullptr },
				* TexWantAll{ nullptr },
				* TexWantUsed{ nullptr },
				* TexWantFiltered{ nullptr },
				* TexCheckSettings{ nullptr },
				* TexSentBy{ nullptr };


			static void TexInit() {
				QCol->Doing("Creating stage", "Texture selector");
				auto TexStage{ UI::GetStage("TexSelector") };
				TexPanel = TexStage->MainGadget;
				auto SX{ TexPanel->W() - 500 }, SY{ TexPanel->H() };
				TexOpt = CreatePanel(SX, 0, 500, TexPanel->H(), TexPanel);
				TexListAll = CreateListBox(0, 0, SX, SY, TexPanel);
				TexListAll->SetForeground(0, 180, 255);
				TexListAll->SetBackground(0, 18, 25);
				TexListUsed = CreateListBox(0, 0, SX, SY, TexPanel);
				TexListUsed->SetForeground(255, 180, 0);
				TexListUsed->SetBackground(25, 18, 0);
				TexListFiltered = CreateListBox(0, 0, SX, SY, TexPanel);
				TexListFiltered->SetForeground(180, 255,0);
				TexListFiltered->SetBackground(18, 25,0);
				TexOkay = CreateButton("Okay", 0, 0, TexOpt);
				TexOkay->SetForeground(0, 255, 0);
				TexOkay->SetBackground(0, 25, 0);
				TexCancel = CreateButton("Cancel", 0, 0, TexOpt);
				TexCancel->SetForeground(255, 0, 0);
				TexCancel->SetBackground(25, 0, 0);
				TexWantAll = CreateRadioButton("All", 0, 0, 500, 20, TexOpt, true);
				TexWantAll->SetForeground(0, 180, 255);
				TexWantUsed = CreateRadioButton("Used", 0, 0, 500, 20, TexOpt);
				TexWantUsed->SetForeground(255, 180, 0);
				TexWantFiltered = CreateRadioButton("Filtered", 0, 0, 500, 20, TexOpt);
				TexWantFiltered->SetForeground(180, 255, 0);
				TexCheckSettings = CreateCheckBox("Auto Settings", 0, 0, 500, 20, TexOpt, true);
				TexCheckSettings->SetForeground(180, 0, 255);
				TexPanel->CBDraw = AutoScreenUpdate;

				TexListAll->ClearItems();
				auto TexFiles{ TextureList() };
				for (auto Tex : *TexFiles) {
					//std::cout << "All: " << Tex << "\n";
					TexListAll->AddItem(Tex);
				}

				TexListAll->CBAction = SelectTex;
				TexListUsed->CBAction = SelectTex;
				TexListFiltered->CBAction = SelectTex;
				TexCancel->CBAction = CancelTex;
				TexOkay->CBAction = OkayTex;

			}


			static void AutoScreenUpdate(j19gadget*, j19action) {
				TexCancel->X(TexOpt->W() - TexCancel->W());
				TexCancel->Y(TexOpt->H() - TexCancel->H());
				TexOkay->X(TexOpt->W() - TexOkay->W());
				TexOkay->Y(TexCancel->Y() - TexOkay->H());
				TexWantFiltered->Y(TexOkay->Y() - 25);
				TexWantUsed->Y(TexWantFiltered->Y() - 25);
				TexWantAll->Y(TexWantUsed->Y() - 25);
				TexCheckSettings->Y(TexWantAll->Y() - 75);
				TexListAll->Visible = TexWantAll->checked;
				TexListUsed->Visible = TexWantUsed->checked;
				TexListFiltered->Visible = TexWantFiltered->checked;
				TexOkay->Enabled =
					(TexWantFiltered->checked && TexListFiltered->SelectedItem() >= 0) ||
					(TexWantUsed->checked && TexListUsed->SelectedItem() >= 0) ||
					(TexWantAll->checked && TexListAll->SelectedItem() >= 0);
			}

			static void OkayTex(j19gadget*, j19action) {
				if (TexWantAll->checked) SelectTex(TexListAll, j19action::DoubleClick);
				if (TexWantUsed->checked) SelectTex(TexListUsed, j19action::DoubleClick);
				if (TexWantFiltered->checked) SelectTex(TexListFiltered, j19action::DoubleClick);
			}


			static void SelectTex(j19gadget* g, j19action a) {
				if (a == j19action::DoubleClick) {
					if (g->SelectedItem() < 0) return;
					if (TexSentBy) {
						TexSentBy->Caption = g->ItemText();
						UI::GoToStage("Editor");
						return;
					}
					std::cout << "\7No action known for the current situation\n";
				}

			}

			void ToTexSelector(June19::j19gadget* sender, June19::j19action) {
				if (UI::NewStage("TexSelector")) TexInit();
				TexSentBy = sender;
				j19gadget::StatusText("Please select a texture file or bundle");
				UI::GoToStage("TexSelector");
			}
		}
	}
}