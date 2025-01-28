// Lic:
// Kthura
// Blockmap viewer
//
//
//
// (c) Jeroen P. Broks, 2015-2019, 2024
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

#include <TQSG.hpp>
#include <SlyvString.hpp>

#include <june19.hpp>

#include "../headers/UserInterface.hpp"
#include "../headers/MapData.hpp"

namespace Slyvina {
	namespace Kthura {
		namespace Editor {

			using namespace June19;
			using namespace TQSG;

			UI
				* BWInterface{ nullptr };

			j19gadget
				* BWPanel{ nullptr },
				* BWOkay{ nullptr },
				* BWSize{ nullptr },
				* BWTooBig{ nullptr };

			int BWW{ 0 };
			int BWH{ 0 };

			static void BWOkayDraw(j19gadget*OB,j19action){
				OB->Y(BWPanel->H() - OB->H());
				OB->SetForeground(std::max((byte)1, --OB->FR), 255, std::max((byte)1, --OB->FB), 255);
				OB->SetBackground(0, std::min((byte)25, ++OB->BG), 1);
				BWW = BWPanel->W();
				BWH = OB->Y();
				BWSize->X(OB->W() + 25);
				BWSize->Y(OB->Y());
				BWSize->SetForeground(255, std::max((byte)180, --BWSize->FG), std::max((byte)1, --BWSize->FB),255);
			}

			static void BWOkayAction(j19gadget* OB, j19action) { UI::GoToStage("Editor"); }

			static void DrawTheBlockMap() {
				auto
					M{ MapData->TheMap };
				auto
					L{ MapData->CurrentLayer()};
				auto
					W{ L->BlockWidth() },
					H{ L->BlockHeight() };
				BWSize->Caption = Units::TrSPrintF("BlockMapped Area %dx%d", W, H);
				//printf("HALLO HALLO?\n");
				if (W && H) { // If one of them is 0, skip drawing the blockmap (or divisions by 0 are sure to happen)
					auto
						BW{ (int)floor((double)BWW / (double)(W )) },
						BH{ (int)floor((double)BWH / (double)(H )) };
					if (BW > 1 && BH > 1) {
						for (int _x = 0; _x < W; ++_x) for (int _y = 0; _y < H; ++_y) {
							if (L->Block(_x, _y))
								SetColor(180, 255, 0);
							else
								SetColor(0, 25, 0);
							Rect(_x * BW, _y * BH + BWPanel->DrawY(), BW - 1, BH - 1);
						}
					}
					BWTooBig->Visible = false;
				} else BWTooBig->Visible = true;

			}

			static void SeeTheBlockMapInit() {
				BWInterface = UI::GetStage("BLOCKMAP");
				BWPanel = BWInterface->MainGadget;
				BWPanel->BA = 0;
				BWOkay = CreateButton("Okay", 0, 0, BWPanel);
				BWOkay->CBDraw = BWOkayDraw;
				BWOkay->CBAction = BWOkayAction;
				BWSize = CreateLabel("--", 0, 0, 400, 30, BWPanel);
				BWSize->SetForeground(255, 255, 255, 255);
				BWInterface->PreJune = DrawTheBlockMap;
				BWTooBig = CreateLabel("Blockmap too big to show", 0, BWPanel->H() / 2, BWPanel->W(), 20, BWPanel, 2);
				BWTooBig->SetForeground(255, 0, 0);
				MapData->CurrentLayer()->BuildBlockmap();
			}

			void SeeTheBlockMap(June19::j19gadget*, June19::j19action) {
				if (UI::NewStage("BLOCKMAP")) SeeTheBlockMapInit();
				UI::GoToStage("BLOCKMAP");
			}
		}
	}
}
