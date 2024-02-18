// Lic:
// Kthura
// Fixer (code)
// 
// 
// 
// (c) Jeroen P. Broks, 2023, 2024
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

#include <string>

#include <SlyvConInput.hpp>
#include <SlyvQCol.hpp>
#include <SlyvString.hpp>

#include <Kthura_Save.hpp>


#include "Kthura_Fix_Work.hpp"



namespace Slyvina {
	namespace Kthura {
		namespace Fixer {

			using namespace Units;

			uint32 TotalProblems{ 0 };

			static void Problem(std::string p, uint32* ProbCount) {
				QCol->Red(TrSPrintF("\7Problem %4d> ",++(*ProbCount)));
				QCol->Yellow(p);				
				std::cout << "\n";
				
			}

			static void CheckTex(KthuraObject* o, std::vector<KthuraObject*>* Victims, uint32* Prob, uint32* Fixed) {
				if (o->texture() == "") {
					Problem(TrSPrintF("Object #%d (%s) has no texture!", o->ID(), o->SKind().c_str()), Prob);
					if (o->Tag().size()) QCol->Yellow("=> Object was tagged: " + o->Tag() + "\n");
					if (!VecSearch(Victims, o)) Victims->push_back(o); else QCol->Doing("Skipped", "Already on the kill list");
				}
			}
			static void CheckSize(KthuraObject* o, std::vector<KthuraObject*>* Victims, uint32* Prob, uint32* Fixed) {
				if (!(o->w() && o->h())) {
					Problem(TrSPrintF("Object #%d (%s) has an illegal size (%dx%d)!", o->ID(), o->SKind().c_str(),o->w(),o->h()), Prob);
					Victims->push_back(o);
				}
			}

			static void CheckScale(KthuraObject* o, std::vector<KthuraObject*>* Victims, uint32* Prob, uint32* Fixed) {
				if (!(abs(o->scalex()) && abs(o->scaley()))) {
					Problem(TrSPrintF("Object #%d (%s) has an illegal scaling (%dx%d)!", o->ID(), o->SKind().c_str(), o->scalex(), o->scaley()), Prob);
					Victims->push_back(o);
				}
			}

			static void CheckFrame(KthuraObject* o, std::vector<KthuraObject*>* Victims, uint32* Prob, uint32* Fixed) {
				if (o->animframe()) {
					if (Upper(ExtractExt(o->texture())) != "JPBF") {
						Problem(TrSPrintF("Object #%d (%s) has an animation frame set, but not an animated texture! (%s)", o->ID(), o->SKind().c_str(), o->texture().c_str()),Prob);
						QCol->Pink("How to deal with this issue?\n");
						QCol->LGreen("R\t"); QCol->White("Remove\n");
						QCol->LGreen("Z\t"); QCol->White("Set frame to zero\n");
						do {
							QCol->Yellow("Let me know please: ");
							QCol->Cyan("");
							auto a = Upper(Trim(ReadLine()))+"*";
							switch (a[0]) {
							case 'R': if (!VecSearch(Victims,o)) Victims->push_back(o); return;
							case 'Z': o->animframe(0); (*Fixed)++; return;
							}
						} while (1);
					} else {
						QCol->Warn(TrSPrintF("Object #%d (%s) has an animation frame set. Texture is also animated, so it doesn't need to be a problem (%s)", o->ID(), o->SKind().c_str(), o->texture().c_str()));
					}
				}
			}


			void Process(int n, int m, std::string File) {
				uint32 Problems{ 0 };
				uint32 Fixed{ 0 };
				QCol->Doing("Processing", File, "\t");
				QCol->Pink(TrSPrintF("(%d/%d)\n", n, m));
				QCol->Doing("Loading", "Map");
				auto Map{ LoadKthura(File) };
				auto Layers{ Map->Layers() };
				auto Victims{ std::vector<KthuraObject*>() };
				for (auto Lay : *Layers) {
					QCol->Doing("Layer", Lay);
					for (auto o = Map->Layer(Lay)->FirstObject(); o; o = o->Next()) {
						QCol->Doing("Object", TrSPrintF("#%d", o->ID()), "\r");
						switch(o->Kind()){
						case KthuraKind::TiledArea:
						case KthuraKind::StretchedArea:
							CheckTex(o, &Victims, &Problems, &Fixed);
							CheckSize(o, &Victims, &Problems, &Fixed);
							CheckFrame(o, &Victims, &Problems, &Fixed);
							break;
						case KthuraKind::Rect:
						case KthuraKind::Zone:
							CheckSize(o, &Victims, &Problems, &Fixed);
							break;
						case KthuraKind::Obstacle:
							CheckScale(o, &Victims, &Problems, &Fixed);
							CheckFrame(o, &Victims, &Problems, &Fixed);
							// Falltrhough
						case KthuraKind::Picture:
							CheckTex(o, &Victims, &Problems, &Fixed);
							break;
						case KthuraKind::Exit:
							if (!o->Tag().size()) {
								Problem(TrSPrintF("Exit #%d has no tag!", o->ID()),&Problems);
								Victims.push_back(o);
							}  else if (!o->data("Wind").size()) {
								Problem(TrSPrintF("Exit \"%s\" (Object #%d) has no wind set. Assuming North!", o->Tag().c_str(), o->ID()),&Problems);
								o->data("Wind", "North");
								Fixed++;
							}
						}
					}
				}
				if (Victims.size()) {
					QCol->Yellow("Killing ");
					QCol->Cyan(std::to_string(Victims.size()));
					if (Victims.size() == 1)
						QCol->Yellow(" object\n");
					else
						QCol->Yellow(" objects\n");
					for (auto Victim : Victims) {
						std::cout << (uint32)Victim << "(" << Victim->ID() << ")" << std::endl; // debug
						Victim->__KillMe(true);						
						Fixed++;
					}
				}
				if (Fixed) {
					QCol->Doing("Saving", "Fixed map");
					Kthura_Save(Map, File);
				}
				QCol->Doing("Problems", Problems);
				QCol->Doing("Fixed", Fixed, "\n\n");
			}
		}
	}
}