// Lic:
// Kthura
// User Interface (header)
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
// Version: 23.09.25
// EndLic

#pragma once
#include <june19.hpp>

namespace Slyvina {
	namespace Kthura {
		namespace Editor {

			extern bool Application_Ended;

			void UserInterFace_Init();
			void UserInterface_Run();

			typedef void(*UIV)();

			class UI;

			class UI {
			private:
				//static bool _initialized;
				//static TrickyUnits::TQSG_AutoImage Mouse;
				static std::map<std::string, UI> Stage;
				std::string _Name{};
				UI(std::string name);
				static UI* _Current;
			public:
				inline UI() {};

				//static void Crash(std::string m);

				June19::j19gadget* MainGadget{ nullptr };
				UIV PreJune{ nullptr };
				UIV PostJune{ nullptr };
				UIV Arrive{ nullptr };

				static void AddStage(std::string st);
				static bool HaveStage(std::string st);
				static bool NewStage(std::string st);
				static UI* GetStage(std::string st);
				static inline UI* CurrentStage() { return _Current; }
				static void GoToStage(std::string st);

			};
		}
	}
}