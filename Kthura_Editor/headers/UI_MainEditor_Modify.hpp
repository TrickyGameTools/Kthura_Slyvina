// Lic:
// Kthura
// Modify Object (header)
// 
// 
// 
// (c) Jeroen P. Broks, 2015, 2016, 2017, 2019, 2021, 2023, 2024
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
// Version: 24.07.17
// EndLic

#pragma once
#include <Kthura_Core.hpp>
#include <june19_core.hpp>

namespace Slyvina {
	namespace Kthura {
		namespace Editor {
			extern KthuraObject* ModifyObject;

			void InitModify();
			void ModifyUpdateWorkPanel(KthuraObject* o);
			void ModifyUpdateWorkPanel();
			void DoDelLabels(Slyvina::June19::j19gadget*, Slyvina::June19::j19action);
		}
	}
}