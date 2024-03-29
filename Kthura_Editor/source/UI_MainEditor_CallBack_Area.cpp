// Lic:
// Kthura
// Callback Area
// 
// 
// 
// (c) Jeroen P. Broks, 2015-2023, 2024
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
// Version: 24.02.18
// EndLic

#include <SlyvQCol.hpp>
#include <SlyvHSVRGB.hpp>

#include "../headers/MapData.hpp"
#include "../headers/UI_MainEditor_CallBack_Area.hpp"
#include "../headers/UI_Layers.hpp"
#include "../headers/UI_Labels.hpp"
#include "../headers/Resource.hpp"

#define AUTOINSTEX_MODULO


using namespace Slyvina::TQSG;
using namespace Slyvina::Units;

namespace Slyvina {
	namespace Kthura {
		namespace Editor {

			static int
				startx{ 0 }, starty{ 0 },
				recx{ 0 }, recy{ 0 }, recw{ 0 }, rech{ 0 },
				hue{ 0 };
			static bool
				started{ false };


			static void AreaDraw(int x, int y); // Needed header!

			static void AreaPressed(int x, int y) {
				started = true;
				startx = x;
				starty = y;
				recx = x;
				recy = y;
				recw = 0;
				rech = 0;
			}
			
			static void AreaReleased(int x, int y) {
				AreaDraw(x, y);
				if (started && recw && rech) {
					auto D{ UIE::_Current };
					auto L{ MapData->CurrentLayer() };
					auto OKind{ KindName(D->Kind->Caption) };
					if (OKind == KthuraKind::TiledArea || OKind == KthuraKind::StretchedArea) {
						if (D->Tex->Caption == "" || D->Tex->Caption == "...") {
							started = false;
							return;
						}
					}
					 auto O{ L->NewObject(KindName(D->Kind->Caption)) };
					if (O->Kind() == KthuraKind::TiledArea || O->Kind() == KthuraKind::StretchedArea) {
						O->texture(D->Tex->Caption);
						O->animframe(ToInt(D->AnimFrame->Text));
						O->animspeed(ToInt(D->AnimSpeed->Text));
						MapData->StoreTexSettings();
					}
					O->x(recx);
					O->y(recy);
					O->w(recw);
					O->h(rech);
					O->dominance(ToInt(D->Dominance->Text));
					if (O->Kind() != KthuraKind::Zone) {
						O->alpha(ToInt(D->Alpha->Text));
						O->red(ToInt(D->Red->Text));
						O->green(ToInt(D->Green->Text));
						O->blue(ToInt(D->Blue->Text));
					} else {
						hsv _hsv{ (double)hue,1,1 };
						rgb _rgb{ hsv2rgb(_hsv) };
						O->red((byte)floor(_rgb.r * 255));
						O->green((byte)floor(_rgb.g * 255));
						O->blue((byte)floor(_rgb.b * 255));
						CurrentLayer()->RemapTags();
						int t{ 0 };
						std::string tag{};
						do { tag = TrSPrintF("Zone #%04d", ++t); } while (CurrentLayer()->HasTag(tag));
						O->Tag(tag);
					}
					O->forcepassible(D->ForcePassible->checked);
					O->impassible(D->Impassible->checked);
					//O->forcepassible(false); 
					//O->impassible(false);
					if (O->Kind() == KthuraKind::TiledArea) {						
						int insx{ 0 }, insy{ 0 };
						if (D->AutoTexIns->checked) {
							if (x > 0 && y > 0) {
#ifdef AUTOINSTEX_MODULO
								static auto I{ LoadUImage(TexResource(), D->Tex->Caption) };
								static std::string Old{D->Tex->Caption};
								if (Old != D->Tex->Caption) {
									I = LoadUImage(TexResource(), D->Tex->Caption);
									Old = D->Tex->Caption;
								}
								insx = -(recx % I->Width());
								insy = -(recy % I->Height());
#else
								insx = -recx;
								insy = -recy;
#endif
							}
						} else {
							insx = ToInt(D->InsX->Text);
							insy = ToInt(D->InsY->Text);
						}
						O->insertx(insx);
						O->inserty(insy);
					}
					//O->scalex(ToInt(D->ScaleX->Text));
					//O->scaley(ToInt(D->ScaleY->Text));
					O->visible(D->Visible->checked);
					O->labels(GetLabString(UIE::_Current));
					//O->rotatedeg(ToInt(D->Rotate->Text));
					L->TotalRemap();
					QCol->Doing("Created", O->SKind() + TrSPrintF(" (%d) at (%5d,%5d)", O->ID(), O->x(), O->y()));
					TQSE::Flush();
				}
				started = false;
			}
			static void AreaDraw(int x, int y) {
				if (started) {
					hue = (++hue) % 360;
					if (x >= startx) {
						recx = startx;
						recw = x - startx;
						if (GridMode) recw += CurrentLayer()->gridx;
					} else if (x < startx) {
						recx = x;
						recw = startx - x;
					} else recw = 0;
					if (y >= starty) {
						recy = starty;
						rech = y - starty;
						if (GridMode) rech += CurrentLayer()->gridy;
					} else if (y < starty) {
						recy = y;
						rech = starty - y;
					} else rech = 0;
					if (recw && rech) {
						SetColorHSV(hue, 1,1);
						SetAlpha(120);
						ARect((recx + MapPanel->DrawX())-ScrollX, (recy + MapPanel->DrawY())-ScrollY, recw, rech);
						SetAlpha(255);
					}
				}
			}


			void InitEditArea() {
				UIEAct::Reg[UIEType::Area].Pressed = AreaPressed;
				UIEAct::Reg[UIEType::Area].Released = AreaReleased;
				UIEAct::Reg[UIEType::Area].Draw = AreaDraw;
			}
			void StopArea() { started = false; }
		}
	}
}