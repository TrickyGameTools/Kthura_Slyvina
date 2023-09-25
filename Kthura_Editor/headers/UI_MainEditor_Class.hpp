
#pragma once
#include "UserInterface.hpp"
namespace Slyvina {
	namespace Kthura {
		namespace Editor {

			class UIE;

			class UIE {
			private:
				static UI* _EditorPanel;
				static June19::j19gadget* ZijBalkRechts;
				static std::map<std::string, UIE> _Register;
				static UIE* _Current;
			public:
				static UI* EditorStage();
				static inline June19::j19gadget* EditorPanel() { return EditorStage()->MainGadget; }
				static June19::j19gadget SideBarRight();
			};

		}
	}



}
