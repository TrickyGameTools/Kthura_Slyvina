// Lic:
// Kthura
// SupJCR
// 
// 
// 
// (c) Jeroen P. Broks, 2015, 2016, 2017, 2019, 2021, 2022, 2023
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
#include "SupJCR.hpp"
#include <jcr6_zlib.hpp>
#include <SlyvGINIE.hpp>
#include <SlyvString.hpp>

#define sJCR6 Slyvina::JCR6

//using namespace jcr6;

namespace Slyvina {
	using namespace Units;

	namespace Kthura {
		namespace SupJCR6 {

			static bool Loaded{ false };
			static Slyvina::JCR6::JT_Dir STEDA;

			static GINIE STEDA_ID;

			Slyvina::JCR6::JT_Dir STED_Assets(std::string d) {

				if (!Loaded) {
					QCol->Doing("Initializing", "JCR6"); // init_JCR6(); // Will be done automatically
					QCol->Doing("Initializing", "JCR6 zlib driver"); sJCR6::init_zlib();
					QCol->Doing("Analyzing", "Kthura.JCR");
					STEDA = sJCR6::JCR6_Dir(d + "/Kthura.JCR");
					STEDA_ID = ParseGINIE(STEDA->GetString("ID/ID.ini"));
					QCol->Doing("Checking", "Kthura.JCR");
					if (Lower(STEDA_ID->Value("ID", "Sig")) != "893f304d4") { QCol->Error("Kthura.JCR signature incorrect!"); exit(255); }
					QCol->Doing("JCR file build", STEDA_ID->Value("Build", "Date"));
				}
				return STEDA;
			}
		}
	}
}