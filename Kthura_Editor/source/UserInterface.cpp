// Lic:
// Kthura
// User Interface
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
// Version: 23.09.26
// EndLic

#include <SlyvQCol.hpp>

#include <june19.hpp>

#include <TQSG.hpp>
#include <TQSE.hpp>

#include "../headers/UserInterface.hpp"
#include "../headers/UI_MainEditor_Class.hpp"
#include "../headers/Resource.hpp"

using namespace Slyvina;
using namespace June19;
using namespace Units;
using namespace TQSE;
using namespace TQSG;

namespace Slyvina {
	namespace Kthura {
		namespace Editor {

			bool Application_Ended{ false };

			j19gadget* _WorkScreen;
			j19gadget* _Screen;

#pragma region Main
			void UserInterFace_Init() {
				auto RJ{ Resource() };
				QCol->Doing("Initizing", "User Interface");
				j19gadget::SetDefaultFont(RJ, "DOSFont.jfbf");
				// auto DE{ RJ->Entries() }; for (auto k : *DE) std::cout << "RJ Entry: " << k->Name() << "\n";
				_Screen = Screen();
				_WorkScreen = WorkScreen();
				_WorkScreen->AddMenu("File");
				_WorkScreen->SetForeground(0, 180, 255);
				_WorkScreen->SetBackground(180, 0, 255, 255);
				j19gadget::StatusText("Welcome to Kthura!");

				InitMainEditor();
			}

			void UserInterface_Run() {
				do {
					Cls();
					Poll();
					if (AppTerminate()) Application_Ended = true;
					_Screen->Draw();
					Flip();
				} while (!Application_Ended);
			}
#pragma endregion

			UI* UI::_Current{nullptr};
			std::map<std::string, UI> UI::Stage{};
			
			void UI::AddStage(std::string st) {
				Stage[Upper(st)] = UI(Upper(st));
			}
			bool UI::HaveStage(std::string st) {
				return Stage.count(Upper(st));
			}
			bool UI::NewStage(std::string st) {
				if (HaveStage(st))
					return false;
				else {
					AddStage(st);
					return true;
				}
			}
			UI* UI::GetStage(std::string st) {
				if (HaveStage(st)) return &Stage[Upper(st)];
				return nullptr;
			}
			void UI::GoToStage(std::string st) {
				Trans2Upper(st);
				if (!HaveStage(st)) {
					QCol->Error("INTERNAL ERROR! Non-existent stage: " + st);
					QCol->Yellow("Please report this!");
					QCol->Reset();
					exit(5);
				}
				for (auto si : Stage) 
					Stage[si.first].MainGadget->Visible = false;
				_Current = &Stage[st];
				_Current->MainGadget->Visible = true;				
				if (_Current->Arrive) _Current->Arrive();
			}

			UI::UI(std::string name) {
				_Name = name;
				MainGadget = CreatePanel(0, 0, WorkScreen()->W(), WorkScreen()->H(),WorkScreen());
			}
		}
	}
}