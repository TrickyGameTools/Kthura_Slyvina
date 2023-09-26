
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
					* Hue{ nullptr }, * Saturation{ nullptr }, * Value{ nullptr };





			};


			void InitMainEditor();
		}
	}



}
