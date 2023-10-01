// Lic:
// Kthura
// Labels
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
// Version: 23.10.01
// EndLic

#include <algorithm>

#include <SlyvQCol.hpp>
#include <SlyvHSVRGB.hpp>

#include <Kthura_Core.hpp>

#include "../headers/UserInterface.hpp"
#include "../headers/UI_MainEditor_Class.hpp"
#include "../headers/UI_MainEditor_Modify.hpp"
#include "../headers/UI_Labels.hpp"

namespace Slyvina {

	using namespace June19;
	using namespace Units;

	namespace Kthura {
		namespace Editor {

			const int FWidth = 250;
			const int FHeight = 25;
			const int FY = 40;

			static std::vector<j19gadget*> Labels{};
			j19gadget
				* LabOkay{ nullptr },
				* LabCancel{ nullptr };
			static std::map<std::string, std::vector<std::string>> VecByKind;
			static std::map<std::string, std::string> StrByKind;

			static void OkayCancelv(j19gadget*, j19action) {
				LabOkay->FR = std::max(--LabOkay->FR, (byte)1);
				LabOkay->FB = LabOkay->FR;
				LabOkay->FG = 255;
				LabCancel->FR = 255;
				LabCancel->FG = LabOkay->FR;
				LabCancel->FB = LabOkay->FR;
				LabOkay->BR = 0;
				LabOkay->BG = std::min(++LabOkay->BR, (byte)25);
				LabOkay->BB = 0;
				LabCancel->BR = LabOkay->BG;
				LabCancel->BG = 0;
				LabCancel->BB = 0;
				LabOkay->Y(LabOkay->GetParent()->H() - LabOkay->H() - 5);
				LabCancel->Y(LabOkay->Y());
				LabCancel->X(LabOkay->W() + 5);
			}

			static void Cancela(j19gadget*, j19action) { UI::GoToStage("Editor"); }

			static void Okaya(j19gadget*, j19action) {
				std::string labelstring;
				std::string kind{"???"};
				std::vector<std::string>* slabels{nullptr};
				auto cnt{ 0 };
				if (!ModifyObject) {
					kind = UIE::_Current->Kind->Caption;
					slabels = &VecByKind[kind];
					slabels->clear();
				}
				for (auto ltb : Labels) {
					auto l{ Trim(ltb->Text) };
					if (l.size()) {
						cnt++;
						if (slabels) slabels->push_back(l);
						if (labelstring.size()) labelstring += ",";
						labelstring += l;
					}
				}
				UIE::_Current->Labels->Caption = std::to_string(cnt);
				if (ModifyObject) ModifyObject->labels(labelstring); else StrByKind[kind] = labelstring;
				UI::GoToStage("Editor");
			}

			static void InitLabels() {
				auto Stage{ UI::GetStage("Labels") };
				auto Panel{ Stage->MainGadget };
				CreateLabel("Please enter the names of the labels", 0, 0, Panel->W(), 20, Panel, 2);
				int x{ 0 }, y{ FY }, hue{ 0 };
				do {
					auto L{ CreateTextfield(x,y,FWidth - 5,Panel) };
					hsv _hsv{ (double)((++hue) % 360), 1.0, 1.0 };
					rgb _rgb{ hsv2rgb(_hsv) };
					// printf(" hue:%f sat:%f val:%f  -> red:%f gre:%f blu:%f", _hsv.h, _hsv.s, _hsv.v, _rgb.r, _rgb.g, _rgb.b); // debug
					L->SetForeground((byte)floor(_rgb.r * 255), (byte)floor(_rgb.g * 255), (byte)floor(_rgb.b * 255));
					L->SetBackground((byte)floor(_rgb.r * 25), (byte)floor(_rgb.g * 25), (byte)floor(_rgb.b * 25));
					x += FWidth;
					if (x + FWidth > Panel->W()) { x = 0; y += FHeight; }					
					Labels.push_back(L);
				} while (y < Panel->H() - (FHeight * 2));
				QCol->Doing("Created", (int)Labels.size(), " "); QCol->Yellow(" label textfields!\n");
				LabOkay = CreateButton("Okay", 0, 0, Panel);
				LabCancel = CreateButton("Cancel", 0, 0, Panel);
				LabOkay->CBDraw = OkayCancelv;
				LabCancel->CBAction = Cancela;
				LabOkay->CBAction = Okaya;
			}


			void GoLabels(June19::j19gadget*, June19::j19action) {
				if (UI::NewStage("Labels")) InitLabels();
				UI::GoToStage("Labels");
				if (ModifyObject) {
					auto slabels{ Split(ModifyObject->labels(),',') };
					for (int ln = 0; ln < Labels.size(); ln++) {
						if (ln < slabels->size()) Labels[ln]->Text = (*slabels)[ln]; else Labels[ln]->Text = "";
					}
				} else {
					auto kind{ UIE::_Current->Kind->Caption };
					auto slabels{ &VecByKind[kind] };
					for (int ln = 0; ln < Labels.size(); ln++) {
						if (ln < slabels->size()) Labels[ln]->Text = (*slabels)[ln]; else Labels[ln]->Text = "";
					}
				}
			}

			std::string GetLabString(UIE* FromUI) { return StrByKind[FromUI->Kind->Caption]; }

		}
	}
}