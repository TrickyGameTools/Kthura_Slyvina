// Lic:
// Kthura
// Resource
// 
// 
// 
// (c) Jeroen P. Broks, 2015-2023
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
#include "../../SupJCR/SupJCR.hpp"
#include "../headers/Resource.hpp"
#include "../headers/ConfigCLI.hpp"

namespace Slyvina {
	namespace Kthura {
		namespace Editor {
			JCR6::JT_Dir _Res{nullptr};

			JCR6::JT_Dir Resource() {
				if (!_Res) {
					_Res = SupJCR6::STED_Assets(ExeDir());
					auto LE{ JCR6::Last() };
					if (LE->Error) {
						QCol->Error(LE->ErrorMessage);
						QCol->Doing("Main", LE->MainFile);
						QCol->Doing("Entry", LE->Entry);
						exit(4);
					}
				}
				return _Res;
			}
		}
	}
}