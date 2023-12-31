// Lic:
// Kthura
// User Interface Layers (header)
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


#pragma once
#include <string>
#include <Kthura_Core.hpp>
#include <june19.hpp>

namespace Slyvina {
	namespace Kthura {
		namespace Editor {

			extern std::string CurrentLayerTag;


			void UpdateLayerSelector();
			KthuraLayer* CurrentLayer();

			void PDM_NewLayer(June19::j19gadget*, June19::j19action);
			void PDM_RenameLayer(June19::j19gadget*, June19::j19action);
			void Act_LayerSelector(June19::j19gadget*, June19::j19action);
			void Act_RemoveLayer(June19::j19gadget*, June19::j19action);

		}
	}
}