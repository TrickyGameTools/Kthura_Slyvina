#include <TQSE.hpp>
#include <SlyvQCol.hpp>
#include <SlyvHSVRGB.hpp>
#include "../headers/UI_MainEditor_Class.hpp"

namespace Slyvina {
	namespace Kthura {
		namespace Editor {
			
			using namespace std;
			using namespace Units;
			using namespace June19;

#pragma region "Laziness functions"
			static void Amber(j19gadget* g, bool back = true) {
				g->SetForeground(255, 180, 0, 255);
				if (back) g->SetBackground(25, 18, 0, 255);
			}

#pragma endregion

#pragma region "Callback functions"
			static void UIETypeSelect(j19gadget* g, j19action a) {
				for (auto& item : UIE::_Register) {
					auto ITM{ &UIE::_Register[item.first] };
					ITM->OptionsPanel->Visible = ITM->MyRadioButton == g;
				}
				UIE::_Current = &UIE::_Register[g->Caption];
			}

			static void HitTextureSelector(j19gadget* g, j19action a) {
				TQSE::Notify("Textures selection not yet operative");
			}

			static void HueLoop(j19gadget* g, j19action a) {
				static hsv _hsv;
				static int _hue;
				_hue = (_hue + 1) % 360;
				_hsv.h = _hue;
				_hsv.s = 1;
				_hsv.v = 1;
				auto rgb = hsv2rgb(_hsv);
				//cout << rgb.r << "/" << rgb.g << "/" << rgb.b;
				auto
					_r{ (byte)(rgb.r * 255) },
					_g{ (byte)(rgb.g * 255) },
					_b{ (byte)(rgb.b * 255) };
				g->SetForeground(_r, _g, _b);
				g->SetBackground(_r/10, _g/10, _b/10);
			}

#pragma endregion

#pragma region "Static field definitions"
			June19::j19gadget* UIE::ZijBalkRechts{nullptr};
			UI* UIE::_EditorPanel{nullptr};
			UIE* UIE::_Current{ nullptr };
			std::map<std::string, UIE> UIE::_Register{};
			//std::map<std::string, j19gadget*> UIE::RadioButtons{};
			int UIE::NextY {0};
#pragma endregion

#pragma region "UIE Methods"		
			UIE::UIE() {
				OptionsPanel = CreatePanel(0, UIEOptionsY, SideBarRight()->W(), SideBarRight()->H() - UIEOptionsY,SideBarRight());
				OptionsPanel->SetBackground(50, 0, 63, 255);
				if (!_Current) _Current = this;
			}

			June19::j19gadget* UIE::SideBarRight() {
				if (!ZijBalkRechts) {
					QCol->Doing("Creating", "Right side bar for editor interface");
					ZijBalkRechts = CreatePanel(EditorPanel()->W() - ZBRightW, 0, ZBRightW, EditorPanel()->H(), EditorPanel());
					ZijBalkRechts->SetBackground(100, 0, 127,255);
					cout << "DEBUG: Zijbalk Rechts (" << ZijBalkRechts->X() << "," << ZijBalkRechts->Y() << "); " << ZijBalkRechts->W() << "x" << ZijBalkRechts->H() << ";\n"; // debug only!
				}
				return ZijBalkRechts;
			}
			UI* UIE::EditorStage() {
				if (!_EditorPanel) {
					QCol->Doing("Creating", "Editor stage and panel");
					UI::AddStage("Editor");
					_EditorPanel = UI::GetStage("Editor");
				}
				return _EditorPanel;
			}


			UIE* UIE::NewArea(std::string t) {
				_Register[t].Type = UIEType::Area;
				_Register[t].MyRadioButton = CreateRadioButton(t, 0, NextY, SideBarRight()->W(), UIENextRadio, SideBarRight(), t == "TiledArea");
				_Register[t].MyRadioButton->CBAction = UIETypeSelect;
				NextY += UIENextRadio; //cout << "Mext Y = " << NextY << endl;
				QCol->Doing("Added Area", t);
				_Register[t].InitBaseGadgets();
				return &_Register[t];
			}

			UIE* UIE::NewSpot(std::string t) {
				_Register[t].Type = UIEType::Spot;
				_Register[t].MyRadioButton = CreateRadioButton(t, 0, NextY, SideBarRight()->W(), UIENextRadio, SideBarRight());
				_Register[t].MyRadioButton->CBAction = UIETypeSelect;
				_Register[t].InitBaseGadgets();
				NextY += UIENextRadio;
				QCol->Doing("Added Spot", t);
				return &_Register[t];

			}

			void UIE::InitBaseGadgets() {
				auto CX{ WBRightCX() };
				OptionsPanel->Visible= MyRadioButton->Caption == "TiledArea";
				CreateLabel("Kind:", 0, 0, CX, 20, OptionsPanel);
				Kind = CreateLabel(MyRadioButton->Caption, CX, 0, CX, 20, OptionsPanel);
				if (MyRadioButton->Caption == "Modify") Kind->Caption = "Nothing";
				Amber(Kind, false);

				CreateLabel("Coords:", 0, 20, CX, 20, OptionsPanel);
				X = CreateTextfield(CX, 20, (CX / 2)-5, 20, OptionsPanel);
				Y = CreateTextfield(CX + (CX / 2), 20, (CX / 2)-5, 20, OptionsPanel);
				Amber(X); X->Enabled = false; //MyRadioButton->Caption == "Modify";
				Amber(Y); Y->Enabled = false; //MyRadioButton->Caption == "Modify";
				
				CreateLabel("Insert:", 0, 40, CX, 20, OptionsPanel);
				InsX = CreateTextfield(CX, 40, (CX / 2) - 5, 20, OptionsPanel);
				InsY = CreateTextfield(CX + (CX / 2), 40, (CX / 2) - 5, 20, OptionsPanel);
				Amber(InsX); InsX->Enabled = MyRadioButton->Caption == "TiledArea";
				Amber(InsY); InsY->Enabled = MyRadioButton->Caption == "TiledArea";

				CreateLabel("Size:", 0, 60, CX, 20, OptionsPanel);
				W = CreateTextfield(CX, 60, (CX / 2) - 5, 20, OptionsPanel);
				H = CreateTextfield(CX + (CX / 2), 60, (CX / 2) - 5, 20, OptionsPanel);
				Amber(W); W->Enabled = false;
				Amber(H); H->Enabled = false;

				CreateLabel("Texture:", 0, 80, CX, 20, OptionsPanel);
				Tex = CreateButton("...", CX, 80, OptionsPanel);
				Amber(Tex); Tex->Enabled = MyRadioButton->Caption == "TiledArea" || MyRadioButton->Caption == "StretchedArea" || MyRadioButton->Caption == "Obstacle";
				Tex->CBAction = HitTextureSelector;

				CreateLabel("Dominance:", 0, 105, CX, 20, OptionsPanel);
				Dominance = CreateTextfield(CX, 105, CX - 5, 20, OptionsPanel);
				Amber(Dominance);
				TexLink["Dominance"] = Dominance;

				{
					CreateLabel("Color RGB:", 0, 125, CX, 20, OptionsPanel);
					auto C3{ (CX-10)  / 3 };
					Red = CreateTextfield(CX, 125, C3, OptionsPanel, "255");
					Green = CreateTextfield(CX+C3, 125, C3, OptionsPanel, "255");
					Blue = CreateTextfield(CX+C3+C3, 125, C3, OptionsPanel, "255");
					Red->SetForeground(255, 0, 0, 255);
					Red->SetBackground(25, 0, 0, 255);
					Green->SetForeground(0, 255, 0, 255);
					Green->SetBackground(0,25, 0, 255);
					Blue->SetForeground(0, 0, 255, 255);
					Blue->SetBackground(0, 0, 25, 255);
					TexLink["Red"] = Red;
					TexLink["Green"] = Green;
					TexLink["Blue"] = Blue;

					CreateLabel("Color HSV:", 0, 145, CX, 20, OptionsPanel);
					Hue = CreateTextfield(CX, 145, C3, OptionsPanel, "0");
					Saturation = CreateTextfield(CX + C3, 145, C3, OptionsPanel, "100");
					Value = CreateTextfield(CX + C3 + C3, 145, C3, OptionsPanel, "100");
					Hue->CBDraw = HueLoop;
					Saturation->SetForeground(0, 0, 0, 255);
					Saturation->SetBackground(255,255,255, 255);
					Value->SetBackground(0, 0, 0, 255);
					Value->SetForeground(255, 255, 255, 255);
					TexLink["Hue"] = Hue;
					TexLink["Saturation"] = Green;
					TexLink["Value"] = Blue;

				}

			}
#pragma endregion

			void InitMainEditor() {
				// Tiled Area
				auto citem{ UIE::NewArea("TiledArea") };

				// Obstacle
				citem = UIE::NewSpot("Obstacle");

				// StretchedArea
				citem = UIE::NewArea("StretchedArea");

				// Rect
				citem= UIE::NewArea("Rect") ;

				// Zone
				citem = UIE::NewArea("Zone");

				// Other

			}
		}
	}
}
