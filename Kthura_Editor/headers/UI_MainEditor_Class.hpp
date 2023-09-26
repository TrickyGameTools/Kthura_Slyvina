// Lic:
// Kthura
// Main Editor Base Class (header)
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

#pragma once
#include "UserInterface.hpp"
namespace Slyvina {
	namespace Kthura {
		namespace Editor {

			enum class UIEType { Unknown, Area, Spot, Modify, Other };

			const int UIENextRadio = 20;
			const int UIEOptionsY = 140;
			const int ZBRightW = 400;
			inline int WBRightCX() { return ZBRightW / 2; }

			extern June19::j19gadget* SidebarLeft;
			extern June19::j19gadget* LayerSelector;
			extern June19::j19gadget* Mascotte;

			class UIE;

			class UIE {
			private:
				static int NextY;
				static UI* _EditorPanel;
				static June19::j19gadget* ZijBalkRechts;
				//static std::map<std::string, June19::j19gadget*> RadioButtons;
				void InitBaseGadgets();
			public:
				static UIE* _Current;
				June19::j19gadget* OptionsPanel;
				std::map<std::string, June19::j19gadget*> TexLink{};
				static std::map<std::string, UIE> _Register;
				UIE();

				UIEType Type{ UIEType::Unknown };
				June19::j19gadget* MyRadioButton{nullptr};

				static UI* EditorStage();
				static inline June19::j19gadget* EditorPanel() { return EditorStage()->MainGadget; }
				static June19::j19gadget* SideBarRight();

				static UIE* NewArea(std::string t);
				static UIE* NewSpot(std::string t);

				June19::j19gadget
					* Kind{nullptr},
					* X{ nullptr }, * Y{ nullptr },
					* InsX{ nullptr }, * InsY{ nullptr },
					* W{ nullptr }, * H{ nullptr },
					* Tex{ nullptr },
					* Dominance{ nullptr },
					* Red{ nullptr }, * Green{ nullptr }, * Blue{ nullptr },
					* Hue{ nullptr }, * Saturation{ nullptr }, * Value{ nullptr },
					* Alpha{ nullptr },
					* Rotate{ nullptr }, * Radians{ nullptr }, // Radians will ALWAYS be disabled, but they are just handy to know!
					* ScaleX{ nullptr }, * ScaleY{ nullptr },
					* Impassible{ nullptr },
					* ForcePassible{ nullptr },
					* AnimFrame{ nullptr },
					* AnimSpeed{ nullptr },
					* AutoTexIns{ nullptr },
					* Visible{ nullptr },
					* Labels{ nullptr }, * Tag{ nullptr };



			};


			void InitMainEditor();
		}
	}



}