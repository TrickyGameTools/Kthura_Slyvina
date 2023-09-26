

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

		}
	}
}
