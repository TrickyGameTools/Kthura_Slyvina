// Lic:
// Kthura
// Modify Object
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


#undef Modify_Debug

#include <SlyvQCol.hpp>

#include <Kthura_Core.hpp>
#include <Kthura_Draw.hpp>

#include "../headers/UI_MainEditor_Class.hpp"
#include "../headers/UI_MainEditor_Modify.hpp"
#include "../headers/MapData.hpp"

namespace Slyvina {

	using namespace std;
	using namespace Units;
	using namespace June19;

	namespace Kthura {
		namespace Editor {

			KthuraObject* ModifyObject{ nullptr };

#pragma region "ONLY WHEN AN OBJECT IS SELECTED"
			void ONLY_WHEN_AN_OBJECT_IS_SELECTED(j19gadget* g, j19action) {
				g->Enabled = ModifyObject != nullptr;
			}

#pragma region "Call backs for modifications"

			static void ModifyUpdateWorkPanel(KthuraObject* o) {
				static auto mui{ &UIE::_Register["Modify"] };
				static auto pan{ mui->OptionsPanel };
				if (o) {
					mui->ModifyKind->Caption = o->SKind();
					mui->Alpha->Text = to_string(o->alpha());
					mui->Alpha->Enabled = o->Kind() != KthuraKind::Zone && o->Kind() != KthuraKind::Pivot && o->Kind() != KthuraKind::Exit && o->Kind() != KthuraKind::Custom;
					mui->Red->Text = to_string(o->red());
					mui->Green->Text = to_string(o->green());
					mui->Blue->Text = to_string(o->blue());
					mui->Red->CBAction(mui->Red, j19action::Type);
					mui->Red->Enabled = mui->Alpha->Enabled;
					mui->Green->Enabled = mui->Alpha->Enabled;
					mui->Blue->Enabled = mui->Alpha->Enabled;
					mui->Hue->Enabled = mui->Alpha->Enabled;
					mui->Saturation->Enabled = mui->Alpha->Enabled;
					mui->Value->Enabled = mui->Alpha->Enabled;

					mui->X->Text = to_string(o->x());
					mui->Y->Text = to_string(o->y());
					mui->X->Enabled = true;
					mui->Y->Enabled = true;

					mui->InsX->Text = to_string(o->insertx());
					mui->InsY->Text = to_string(o->inserty());
					mui->InsX->Enabled = o->Kind() == KthuraKind::TiledArea;
					mui->InsY->Enabled = o->Kind() == KthuraKind::TiledArea;

					mui->W->Text = to_string(o->w());
					mui->H->Text = to_string(o->h());
					mui->W->Enabled = o->Kind() == KthuraKind::TiledArea || o->Kind() == KthuraKind::StretchedArea || o->Kind() == KthuraKind::Rect || o->Kind() == KthuraKind::Zone;
					mui->H->Enabled = mui->W->Enabled;

					mui->Dominance->Text = to_string(o->dominance());
					mui->Dominance->Enabled = true;

					mui->Rotate->Text = to_string(o->rotatedeg());
					mui->Rotate->Enabled = o->Kind() == KthuraKind::Obstacle;

					mui->ScaleX->Text = to_string(o->scalex());
					mui->ScaleY->Text = to_string(o->scaley());
					mui->ScaleX->Enabled = o->Kind() == KthuraKind::Obstacle;
					mui->ScaleY->Enabled = o->Kind() == KthuraKind::Obstacle;

					mui->Impassible->checked = o->impassible();
					mui->Impassible->Enabled = mui->W->Enabled && (!o->forcepassible());
					mui->ForcePassible->checked = o->forcepassible();
					mui->ForcePassible->Enabled = mui->W->Enabled && (!o->impassible());

					mui->AnimFrame->Text = to_string(o->animframe());
					mui->AnimFrame->Enabled = (o->animspeed() < 0) && (o->Kind() == KthuraKind::TiledArea || o->Kind() == KthuraKind::StretchedArea || o->Kind() == KthuraKind::Obstacle);
					mui->AnimSpeed->Text = to_string(o->animspeed());
					mui->AnimSpeed->Enabled = (o->Kind() == KthuraKind::TiledArea || o->Kind() == KthuraKind::StretchedArea || o->Kind() == KthuraKind::Obstacle);

					mui->AutoTexIns->checked = false;
					mui->AutoTexIns->Caption = "NO WAY!";
					mui->AutoTexIns->Enabled = false;

					mui->Visible->checked = o->visible();
					mui->Visible->Enabled = true;

					mui->Labels->Enabled = true;
					mui->Tag->Enabled = true;
					if (o->Tag().size()) mui->Tag->Caption = o->Tag(); else mui->Tag->Caption = "...";
				} else {
					mui->ModifyKind->Caption = "Nothing";
				}
			}

			inline void ModifyUpdateWorkPanel() { ModifyUpdateWorkPanel(ModifyObject); }

			void MPressed(int x, int y) {
				ModifyObject = nullptr;
				for (auto o = MapData->CurrentLayer()->DomFirst; o; o=o->DomNext) {
#ifdef Modify_Debug
					auto chk{ MapData->Draw->ObjectSize(o) };
					printf("Object #%4d (%d,%d) %dx%d (%s)\n", o->ID(), chk.x, chk.y, chk.w, chk.h,o->SKind().c_str());
#endif
					if (MapData->Draw->InsideObject(o, x, y)) ModifyObject = o;
				}
#ifdef Modify_Debug
				if (ModifyObject) { QCol->Doing("Selected", ModifyObject->ID()); } else { QCol->Doing("Selected", "Nothing"); }
#endif
				ModifyUpdateWorkPanel();
			}
#pragma endregion


			void InitModify() {
				UIEAct::Reg[UIEType::Modify].Pressed = MPressed;
				auto citem{ UIE::NewSpot("Modify") };
				citem->Type = UIEType::Modify;
				citem->OptionsPanel->SetBackground(25, 0, 0);
				citem->OptionsPanel->CBDraw = ONLY_WHEN_AN_OBJECT_IS_SELECTED;
			}

		}
	}
}