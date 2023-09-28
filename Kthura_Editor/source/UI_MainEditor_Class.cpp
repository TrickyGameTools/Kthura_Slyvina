// Lic:
// Kthura
// Main Editor Base Class
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
// Version: 23.09.28
// EndLic

#include <TQSE.hpp>
#include <SlyvQCol.hpp>
#include <SlyvHSVRGB.hpp>

#include <Kthura_Draw.hpp>

#include "../headers/ConfigCLI.hpp"
#include "../headers/Resource.hpp"
#include "../headers/MapData.hpp"
#include "../headers/UI_MainEditor_Class.hpp"
#include "../headers/UI_Layers.hpp"
#include "../headers/UI_Textures.hpp"
#include "../headers/UI_MainEditor_CallBack_Spot.hpp"
#include "../headers/UI_MainEditor_CallBack_Area.hpp"




namespace Slyvina {
	namespace Kthura {
		namespace Editor {
			
			using namespace std;
			using namespace Units;
			using namespace June19;
			using namespace TQSG;
			using namespace TQSE;

#pragma region Locals
			j19gadget* MapPanel{nullptr };
#pragma endregion

#pragma region "Left Sidebar"
			June19::j19gadget* SidebarLeft{nullptr};
			June19::j19gadget* LayerSelector{nullptr};
			June19::j19gadget* Mascotte{nullptr};
#pragma endregion


#pragma region "General globals"
			int ScrollX{ 0 }, ScrollY{ 0 };
			bool DrawGrid{ true }, GridMode{ true };
#pragma endregion


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

			static void CheckShow(j19gadget* g, j19action) {
				if (g->checked) {
					g->Caption = "Yes";
					g->SetForeground(180, 255, 0);
				} else {
					g->Caption = "No";
					g->SetForeground(255, 0, 0);
				}
			}

			static void CheckImpassible(j19gadget* g, j19action) {
				UIE::_Current->ForcePassible->Enabled = !g->checked;
				if (!g->checked)UIE::_Current->ForcePassible->checked = false;
			}
			static void CheckForcePassible(j19gadget* g, j19action) {
				UIE::_Current->Impassible->Enabled = !g->checked;
				if (!g->checked)UIE::_Current->Impassible->checked = false;
			}

			static void CheckAnimSpeed(j19gadget* g, j19action) {
				UIE::_Current->AnimFrame->Enabled = ToInt(g->Text) < 0;
			}

			static void AutoTexInsCheck(j19gadget* g, j19action) {
				if (UIE::_Register["TiledArea"].AutoTexIns != g) return;
				UIE::_Register["TiledArea"].InsX->Enabled = !g->checked;
				UIE::_Register["TiledArea"].InsY->Enabled = !g->checked;
			}

			static void Act_HSV(j19gadget*, j19action) {
				auto CP{ UIE::_Current };
				CP->Hue->Text = TrSPrintF("%d", ToInt(CP->Hue->Text) % 360);
				CP->Saturation->Text = TrSPrintF("%d", ToInt(CP->Saturation->Text) % 101);
				CP->Value->Text = TrSPrintF("%d", ToInt(CP->Value->Text) % 101);
				hsv _hsv{
					(double)(ToInt(CP->Hue->Text) % 360),
						((double)ToInt(CP->Saturation->Text)) / 100,
						((double)ToInt(CP->Value->Text)) / 100
				};
				auto 
					rgb{ hsv2rgb(_hsv) };
				auto
					_r{ (byte)(rgb.r * 255) },
					_g{ (byte)(rgb.g * 255) },
					_b{ (byte)(rgb.b * 255) };
				CP->Red->Text = TrSPrintF("%d", _r);
				CP->Green->Text = TrSPrintF("%d", _g);
				CP->Blue->Text = TrSPrintF("%d", _b);				
			}

			static void Act_RGB(j19gadget*, j19action) {
				auto CP{ UIE::_Current };
				CP->Red->Text = TrSPrintF("%d", ToInt(CP->Red->Text) % 256);
				CP->Green->Text = TrSPrintF("%d", ToInt(CP->Green->Text) % 256);
				CP->Blue->Text = TrSPrintF("%d", ToInt(CP->Blue->Text) % 256);
				rgb _rgb{
					(double)ToInt(CP->Red->Text) / 255,
					(double)ToInt(CP->Green->Text) / 255,
					(double)ToInt(CP->Blue->Text) / 255
				};
				auto _hsv = rgb2hsv(_rgb);
				CP->Hue->Text = TrSPrintF("%d", (int)_hsv.h);
				CP->Saturation->Text = TrSPrintF("%d", (int)floor( _hsv.s*100));
				CP->Value->Text = TrSPrintF("%d", (int)floor(_hsv.v * 100));
			}

#pragma endregion

#pragma region "Static field definitions"
			June19::j19gadget* UIE::ZijBalkRechts{nullptr};
			UI* UIE::_EditorPanel{nullptr};
			UIE* UIE::_Current{ nullptr };
			std::map<std::string, UIE> UIE::_Register{};
			//std::map<std::string, j19gadget*> UIE::RadioButtons{};
			int UIE::NextY {0};
			std::map<UIEType, UIEAct> UIEAct::Reg{};
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
				OptionsPanel->Visible = MyRadioButton->Caption == "TiledArea";
				CreateLabel("Kind:", 0, 0, CX, 20, OptionsPanel);
				Kind = CreateLabel(MyRadioButton->Caption, CX, 0, CX, 20, OptionsPanel);
				if (MyRadioButton->Caption == "Modify") Kind->Caption = "Nothing";
				Amber(Kind, false);

				CreateLabel("Coords:", 0, 20, CX, 20, OptionsPanel);
				X = CreateTextfield(CX, 20, (CX / 2) - 5, 20, OptionsPanel);
				Y = CreateTextfield(CX + (CX / 2), 20, (CX / 2) - 5, 20, OptionsPanel);
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
				//Tex->CBAction = HitTextureSelector;
				Tex->CBAction = ToTexSelector;

				CreateLabel("Dominance:", 0, 105, CX, 20, OptionsPanel);
				Dominance = CreateTextfield(CX, 105, CX - 5, 20, OptionsPanel, "20");
				Amber(Dominance);
				TexLink["Dominance"] = Dominance;

				{
					auto nozone{ MyRadioButton->Caption != "Zone" };
					CreateLabel("Color RGB:", 0, 125, CX, 20, OptionsPanel);
					auto C3{ (CX - 10) / 3 };
					Red = CreateTextfield(CX, 125, C3, OptionsPanel, "255");
					Green = CreateTextfield(CX + C3, 125, C3, OptionsPanel, "255");
					Blue = CreateTextfield(CX + C3 + C3, 125, C3, OptionsPanel, "255");
					Red->SetForeground(255, 0, 0, 255);
					Red->SetBackground(25, 0, 0, 255);
					Green->SetForeground(0, 255, 0, 255);
					Green->SetBackground(0, 25, 0, 255);
					Blue->SetForeground(0, 0, 255, 255);
					Blue->SetBackground(0, 0, 25, 255);
					TexLink["Red"] = Red;
					TexLink["Green"] = Green;
					TexLink["Blue"] = Blue;
					Red->CBAction = Act_RGB;
					Green->CBAction = Act_RGB;
					Blue->CBAction = Act_RGB;
					Red->Enabled = nozone;
					Green->Enabled = nozone;
					Blue->Enabled = nozone;

					CreateLabel("Color HSV:", 0, 145, CX, 20, OptionsPanel);
					Hue = CreateTextfield(CX, 145, C3, OptionsPanel, "0");
					Saturation = CreateTextfield(CX + C3, 145, C3, OptionsPanel, "100");
					Value = CreateTextfield(CX + C3 + C3, 145, C3, OptionsPanel, "100");
					Hue->Enabled = nozone;
					Hue->CBDraw = HueLoop;
					Saturation->SetForeground(0, 0, 0, 255);
					Saturation->SetBackground(255, 255, 255, 255);
					Saturation->Enabled = nozone;
					Value->SetBackground(0, 0, 0, 255);
					Value->SetForeground(255, 255, 255, 255);
					Value->Enabled = nozone;
					TexLink["Hue"] = Hue;
					TexLink["Saturation"] = Saturation;
					TexLink["Value"] = Value;
					Hue->CBAction = Act_HSV;
					Saturation->CBAction = Act_HSV;
					Value->CBAction = Act_HSV;
					Act_RGB(nullptr, j19action::Unknown);

				}
				CreateLabel("Alpha:", 0, 165, CX, 20, OptionsPanel);
				Alpha = CreateTextfield(CX, 165, CX - 30, OptionsPanel, "255");
				Amber(Alpha);
				Alpha->Enabled = MyRadioButton->Caption != "Zone";
				TexLink["Alpha"] = Alpha;
				CreateLabel("Rotate:", 0, 185, CX, 20, OptionsPanel);
				Rotate = CreateTextfield(CX, 185, (CX / 2) - 30, OptionsPanel, "0");
				Amber(Rotate);
				Rotate->Enabled = MyRadioButton->Caption == "Obstacle";
				TexLink["Rotate"] = Rotate;
				Radians = CreateTextfield(CX + (CX / 2), 185, (CX / 2) - 30, OptionsPanel, "0.00");
				Radians->Enabled = false;
				Amber(Radians);

				CreateLabel("Scale:", 0, 205, CX, 20, OptionsPanel);
				ScaleX = CreateTextfield(CX, 205, (CX / 2) - 30, OptionsPanel, "1000");
				ScaleY = CreateTextfield(CX + (CX / 2), 205, (CX / 2) - 30, OptionsPanel, "1000");
				Amber(ScaleX);
				Amber(ScaleY);
				TexLink["Scale.X"] = ScaleX;
				TexLink["Scale.Y"] = ScaleY;
				ScaleX->Enabled = MyRadioButton->Caption == "Obstacle";
				ScaleY->Enabled = MyRadioButton->Caption == "Obstacle";

				CreateLabel("Impassible:", 0, 225, CX, 20, OptionsPanel);
				Impassible = CreateCheckBox("", CX, 225, CX, 20, OptionsPanel);
				Impassible->CBDraw = CheckShow;
				TexLink["Impassible"] = Impassible;
				CreateLabel("Force Passible:", 0, 245, CX, 20, OptionsPanel);
				ForcePassible = CreateCheckBox("", CX, 245, CX, 20, OptionsPanel);
				ForcePassible->CBDraw = CheckShow;
				TexLink["ForcePassible"] = ForcePassible;
				Impassible->Enabled = Type == UIEType::Area;
				ForcePassible->Enabled = Type == UIEType::Area;
				Impassible->CBAction = CheckImpassible;
				ForcePassible->CBAction = CheckForcePassible;

				CreateLabel("AnimFrame:", 0, 265, CX, 20, OptionsPanel);
				AnimFrame = CreateTextfield(CX, 265, (CX / 2) - 30, OptionsPanel, "0");
				AnimFrame->Enabled = MyRadioButton->Caption == "Obstacle" || MyRadioButton->Caption == "TiledArea" || MyRadioButton->Caption == "StretchedArea";
				CreateLabel("AnimSpeed:", 0, 285, CX, 20, OptionsPanel);
				AnimSpeed = CreateTextfield(CX, 285, (CX / 2) - 30, OptionsPanel, "-1");
				AnimSpeed->Enabled = MyRadioButton->Caption == "Obstacle" || MyRadioButton->Caption == "TiledArea" || MyRadioButton->Caption == "StretchedArea";
				AnimSpeed->CBDraw = CheckAnimSpeed;
				TexLink["Animation.Speed"] = AnimSpeed;
				TexLink["Animation.Frame"] = AnimFrame;

				CreateLabel("AutoTexIns:", 0, 305, CX, 20, OptionsPanel);
				AutoTexIns = CreateCheckBox("", CX, 305, CX, 20, OptionsPanel);
				AutoTexIns->CBDraw = CheckShow;
				AutoTexIns->Enabled = MyRadioButton->Caption == "TiledArea";
				AutoTexIns->CBAction = AutoTexInsCheck;
				TexLink["Automated_Texture_Insertion"] = AutoTexIns;

				CreateLabel("Visble:", 0, 325, CX, 20, OptionsPanel);
				Visible = CreateCheckBox("", CX, 325, CX, 20, OptionsPanel);
				Visible->CBDraw = CheckShow;
				Visible->checked = true;
				TexLink["Visible"] = Visible;

				CreateLabel("Labels:", 0, 350, CX, 20, OptionsPanel);
				Labels = CreateButton("0", CX, 348, OptionsPanel);
				Amber(Labels);
				CreateLabel("Tag:", 0, 375, CX, 20, OptionsPanel);
				Tag = CreateButton("...", CX, 372, OptionsPanel);
				Amber(Tag);
				Tag->Enabled = false;

			}
#pragma endregion

#pragma region "Global Callbacks"
			static void MainEditorBack() {
				uint32
					GX{ CurrentLayer()->gridx },
					GY{ CurrentLayer()->gridy };
				//SetColor(255, 255, 255);  Mascotte->Image()->Draw(500, 500); // Debug
				if (ScrollX < 0) {
					SetColor(255, 0, 0);
					Rect(MapPanel->DrawX(), MapPanel->DrawY(), abs(ScrollX), MapPanel->H());
				}
				if (ScrollY < 0) {
					SetColor(255, 0, 0);
					Rect(MapPanel->DrawX(), MapPanel->DrawY(), MapPanel->W(), abs(ScrollY));
				}
				SetColor(100, 100, 100, 255);
				if (DrawGrid) {
					for (int x = (ScrollX % GX); x <= MapPanel->W(); x += GX) Line(MapPanel->DrawX() + x, 0, MapPanel->DrawX() + x, ScreenHeight());
					for (int y = (ScrollY % GY); y <= MapPanel->H(); y += GY) Line(0, MapPanel->DrawY() + y, ScreenWidth(), MapPanel->DrawY() + y);
				}
				string coords{ "" };
				string strgridmode{ " " }; if (GridMode) strgridmode = "Grid Mode";
				MapData->DrawLayer();
				if (MouseX() >= MapPanel->DrawX() && MouseY() >= MapPanel->DrawY() && MouseX() < MapPanel->DrawX() + MapPanel->W() && MouseY() < MapPanel->DrawY() + MapPanel->H()) {
					int
						MX{ (MouseX() - MapPanel->DrawX()) + ScrollX },
						MY{ (MouseY() - MapPanel->DrawY()) + ScrollY },
						DX{ MX },
						DY{ MY };
					if (GridMode) {
						switch (UIE::_Current->Type) {
						case UIEType::Area: 
							DX = floor(((double)MX) / CurrentLayer()->gridx) * CurrentLayer()->gridx;
							DY = floor(((double)MY) / CurrentLayer()->gridy) * CurrentLayer()->gridy;
							break;
						case UIEType::Spot:
							DX = (floor(((double)MX) / CurrentLayer()->gridx) * CurrentLayer()->gridx) + (CurrentLayer()->gridx / 2) - (ScrollX % CurrentLayer()->gridx);
							DY = (floor(((double)MY) / CurrentLayer()->gridy) * CurrentLayer()->gridy) + CurrentLayer()->gridy - (ScrollY % CurrentLayer()->gridy);
							break;
						}
					}
					coords = TrSPrintF("Mouse(%4d,%4d) -> (%4d,%4d)", MX, MY, DX, DY);
					UIEAct::Reg[UIE::_Current->Type].Draw(DX, DY);
					if (MouseHit(SDL_BUTTON_LEFT)) UIEAct::Reg[UIE::_Current->Type].Pressed(DX, DY);
					if (MouseReleased(SDL_BUTTON_LEFT)) UIEAct::Reg[UIE::_Current->Type].Released(DX, DY);
				}
				j19gadget::StatusText(ProjectName() + "::" + MapName() + "\t" + MapData->CurrentLayerTag()+"\t"+ coords + "\tScroll" + TrSPrintF("(%4d,%4d)", ScrollX, ScrollY) + "\t" + strgridmode);
				MapData->Draw->AllowDraw[KthuraKind::Zone] = UIE::_Current && (UIE::_Current->Kind->Caption == "Zone" || UIE::_Current->Kind->Caption == "Modify");
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

				// Modify

				// Left
				auto IMascotte{ TQSG::LoadImage(Resource(),"Kthura.png") };
				UIE::EditorPanel()->BA = 0;
				SidebarLeft = CreatePanel(0, 0, 125, UIE::EditorPanel()->H(), UIE::EditorPanel());
				LayerSelector = CreateListBox(2, 2, 121, UIE::EditorPanel()->H() - IMascotte->Height(), UIE::EditorPanel());
				SidebarLeft->SetBackground(111, 0, 127);
				LayerSelector->SetForeground(0, 180, 255);
				LayerSelector->SetBackground(0, 18, 25);
				LayerSelector->CBAction = Act_LayerSelector;
				Mascotte = CreatePicture(0, UIE::EditorPanel()->H() - IMascotte->Height(), IMascotte->Width(), IMascotte->Height(),UIE::EditorPanel());
				Mascotte->Image(IMascotte);
				UpdateLayerSelector();

				MapPanel = CreatePanel(SidebarLeft->W(), 0, UIE::SideBarRight()->X() - SidebarLeft->W(), UIE::EditorPanel()->H(), UIE::EditorPanel());
				MapPanel->Visible = false; // only needed for measuring


				// Crude callbacks
				auto ES{ UI::GetStage("Editor") };
				ES->PreJune = MainEditorBack;

				// UIE types callbacks
				InitEditArea(); 
				InitEditSpot();

			}
		}
	}
}