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
#include "../headers/UserInterface.hpp"

using namespace Slyvina;
using namespace June19;;

namespace Slyvina {
	namespace Kthura {
		namespace Editor {

			static void AutoScreenUpdate(j19gadget*, j19action);

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
				* TexSentBy{ nullptr };


			static void TexInit() {
				auto TexStage{ UI::GetStage("TexEditor") };
				auto SX{ TexPanel->W() - 500 }, SY{ TexPanel->H() };
				TexPanel = TexStage->MainGadget;
				TexOpt = CreatePanel(SX, 0, 500, TexPanel->H(), TexPanel);
				TexListAll = CreateListBox(0, 0, SX, SY, TexPanel);
				TexListAll->SetForeground(0, 180, 255);
				TexListAll->SetBackground(0, 18, 25);
				TexListUsed = CreateListBox(0, 0, SX, SY, TexPanel);
				TexListUsed->SetForeground(255, 180, 0);
				TexListUsed->SetBackground(255, 18, 0);
				TexListFiltered = CreateListBox(0, 0, SX, SY, TexPanel);
				TexListFiltered->SetForeground(180, 255,0);
				TexListFiltered->SetBackground(18, 255,0);
				TexOkay = CreateButton("Okay", 0, 0, TexOpt);
				TexOkay->SetForeground(0, 255, 0);
				TexOkay->SetBackground(0, 25, 0);
				TexCancel = CreateButton("Cancel", 0, 0, TexOpt);
				TexCancel->SetForeground(255, 0, 0);
				TexCancel->SetBackground(25, 0, 0);
				TexWantUsed = CreateRadioButton("All", 0, 0, 500, 20, TexOpt);
				TexWantUsed->SetForeground(0, 180, 255);
				TexWantUsed = CreateRadioButton("Used", 0, 0, 500, 20, TexOpt);
				TexWantUsed->SetForeground(255, 180, 0);
				TexWantFiltered = CreateRadioButton("Filtered", 0, 0, 500, 20, TexOpt);
				TexWantFiltered->SetForeground(180, 255, 0);
				TexPanel->CBDraw = AutoScreenUpdate;
			}


			static void AutoScreenUpdate(j19gadget*, j19action) {
				TexCancel->X(TexOpt->W() - TexCancel->W());
				TexCancel->Y(TexOpt->H() - TexCancel->H());
				TexOkay->X(TexOpt->W() - TexOkay->W());
				TexOkay->Y(TexCancel->Y() - TexOkay->H());
				TexWantFiltered->Y(TexOkay->Y() - 25);
				TexWantUsed->Y(TexWantFiltered->Y() - 25);
				TexWantAll->Y(TexWantUsed->Y() - 25);
			}

			void ToTexSelector(June19::j19gadget* sender, June19::j19action) {
				if (UI::NewStage("TexSelector")) TexInit();
				TexSentBy = sender;
			}
		}
	}
}