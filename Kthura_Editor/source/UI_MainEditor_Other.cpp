// Lic:
// Kthura
// Other
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
// Version: 23.09.29
// EndLic

#include "../headers/UserInterface.hpp"
#include "../headers/UI_MainEditor_Class.hpp"

namespace Slyvina {

	using namespace June19;

	namespace Kthura {

		namespace Editor {
			

#pragma region "Call backs for editing"
			void ODraw(int x, int y) {}

#pragma endregion

			void InitOther() {
				UIEAct::Reg[UIEType::Other].Draw = ODraw;
				UIEAct::Reg[UIEType::Other].Pressed = ODraw; // Actual function comes later!
				UIEAct::Reg[UIEType::Other].Released = ODraw; // Actual function comes later!
				auto t{ "Other" };
				auto Other{ &UIE::_Register[t] };
				UIE::_Register[t].Type = UIEType::Other;
				UIE::_Register[t].MyRadioButton = CreateRadioButton(t, 0, UIE::NextY, UIE::SideBarRight()->W(), UIENextRadio, UIE::SideBarRight(), false);
				UIE::_Register[t].MyRadioButton->CBAction = UIETypeSelect;
				Other->Kind = Other->MyRadioButton; // Dirty code straight from Hell! Do NOT try this at home, kids!
				Other->OptionsPanel->Visible = false;
				Other->OptionsPanel->SetBackground(0, 25, 0);
			}
		}
	}
}