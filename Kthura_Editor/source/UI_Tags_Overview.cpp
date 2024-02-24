// Lic:
// Kthura
// Tags overview
// 
// 
// 
// (c) Jeroen P. Broks, 2024
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
// Version: 24.02.24
// EndLic
#include "../headers/MapData.hpp"
#include "../headers/UserInterface.hpp"
#include "../headers/UI_Tags_Overview.hpp"


using namespace std;
using namespace Slyvina::June19;

namespace Slyvina {
	namespace Kthura {
		namespace Editor {

			const int mTypes = 8;
			const char Types[mTypes][200]{
				"All",  // 0
				"TiledArea", // 1
				"Obstacle", //2
				"Rect", // 3
				"Zone", // 4
				"Pivot", // 5
				"Exit", // 6
				"Custom" // 7
			};

			vector<j19gadget*> PickType;
			j19gadget
				* TagList{ nullptr },
				* Desc{ nullptr },
				* Bye{ nullptr };

			static void ByeAction(June19::j19gadget* g, June19::j19action) {
				UI::GoToStage("Editor");
			}

			static void DrawTagList(June19::j19gadget* g, June19::j19action) {
				static int hue{}; hue = (++hue) % 360;
				g->SetForegroundHSV(hue, 1, 1);
				g->SetBackgroundHSV(hue, 1, .1);
				Bye->Y(g->GetParent()->H() - Bye->H());
				if (g->SelectedItem() < 0) {
					Desc->Caption = "";
					return;
				}
				auto L{ MapData->CurrentLayer() };
				auto o{ L->Obj(g->ItemText()) };
				if (o->w() || o->h())
					Desc->Caption = TrSPrintF("Object #%d; %s (%d,%d) %dx%d", o->ID(), o->SKind().c_str(), o->x(), o->y(), o->w(), o->h());
				else
					Desc->Caption = TrSPrintF("Object #%d; %s (%d,%d)", o->ID(), o->SKind().c_str(), o->x(), o->y());
				
			}

			static void Refresh(June19::j19gadget* g, June19::j19action){
				auto L{ MapData->CurrentLayer() };
				auto V{ NewVecString() };
				TagList->ClearItems();
				cout << "Refreshing. Only " << g->Caption << " this time!\n";; // debug
				if (g->checked) {
					for (auto o = L->FirstObject(); o; o = o->Next()) {
						if (o->Tag() != "") {
							if (g->Caption == "All" || g->Caption == o->SKind() || (g->Caption == "Custom" && o->Kind() == KthuraKind::Custom)) {
								V->push_back(o->Tag());
							}
						}
					}
				}
				SortVecString(V);
				for (auto T : *V) TagList->AddItem(T);
			}

			static void Refresh() {
				for (auto g : PickType) if (g->checked) Refresh(g, j19action::Check);
			}

#pragma region Init
			static void CreateTagOverview() {
				auto P{ UI::GetStage("TagsOverview")->MainGadget };
				for (int i = 0; i < mTypes; ++i) {
					auto g{ CreateRadioButton(Types[i], 0, i * 30, 250, 30, P, i == 0) };
					g->CBAction = Refresh;
					PickType.push_back(g);
				}
				TagList = CreateListBox(250, 0, P->W() - 300, P->H() - 40, P);
				Desc = CreateLabel("", 0, P->H() - 30, P->W(), 30, P, 2);
				Desc->SetForeground(255, 180, 0, 255);
				Bye = CreateButton("Okay", 0, 0, P);
				Bye->SetForeground(0, 255, 0);
				Bye->SetBackground(0, 25, 0);
				Bye->CBAction = ByeAction;
				TagList->CBDraw = DrawTagList;
			}
#pragma endregion

#pragma region "Chain to here"
			void GoToTagOverview(June19::j19gadget*, June19::j19action) { GoToTagOverview(); }
			
			void GoToTagOverview() {
				if (UI::NewStage("TagsOverview")) CreateTagOverview();
				Refresh();
				UI::GoToStage("TagsOverview");
			}
#pragma endregion
		}
	}
}