#include <SlyvQCol.hpp>
#include "../headers/UI_MainEditor_Class.hpp"

namespace Slyvina {
	namespace Kthura {
		namespace Editor {

			using namespace Units;
			using namespace June19;

#pragma region "Static field definitions"
			June19::j19gadget* UIE::ZijBalkRechts{nullptr};
			UI* UIE::_EditorPanel{nullptr};
			UIE* UIE::_Current{ nullptr };
			std::map<std::string, UIE> UIE::_Register{};
#pragma endregion
			
			
			June19::j19gadget UIE::SideBarRight() {
				if (!ZijBalkRechts) {
					QCol->Doing("Creating", "Right side bar for editor interface");
					ZijBalkRechts = CreatePanel(EditorPanel()->W() - 250, 0, EditorPanel()->W(), EditorPanel()->H() - EditorPanel()->DrawY(), EditorPanel());
					ZijBalkRechts->SetBackground(100, 0, 127);
				}
				return June19::j19gadget();
			}
			UI* UIE::EditorStage() {
				if (!_EditorPanel) {
					QCol->Doing("Creating","Editor stage and panel");
					UI::AddStage("Editor");
					_EditorPanel = UI::GetStage("Editor");
				}
				return _EditorPanel;
			}
		}
	}
}
