// Lic:
// Kthura
// MapData (header)
//
//
//
// (c) Jeroen P. Broks, 2015-2019, 2023
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
// Version: 23.09.28
// EndLic
#pragma once

#include <SlyvGINIE.hpp>

#include <Kthura_Core.hpp>
#include <Kthura_Save.hpp>
#include <Kthura_Draw.hpp>
#include <Kthura_Draw_TQSG.hpp>

#include "../headers/UI_MainEditor_Class.hpp"

namespace Slyvina {
	namespace Kthura {
		namespace Editor {

			class TMapData;
			extern std::unique_ptr<TMapData> MapData;

			class TMapData {
			private:
			public:
				TMapData();
				std::string MapFile{};
				Kthura TheMap{ nullptr };
				KthuraDraw Draw{ nullptr };
				Units::UGINIE TextureSettings{ nullptr };
				std::string TextureSettingsDir();
				std::string TextureSettingsFile();


				std::string CurrentLayerTag();
				KthuraLayer* CurrentLayer();

				void Load();
				void DrawLayer(KthuraLayer* L);
				void DrawLayer(std::string LayTag);
				void DrawLayer();


				void StoreTexSettings(UIE* Panel, std::string _kind, std::string _tex);
				inline void StoreTexSettings(UIE* Panel, std::string k) { StoreTexSettings(Panel, Panel->Kind->Caption, k); }
				inline void StoreTexSettings(UIE* Panel) { StoreTexSettings(Panel, Panel->Tex->Caption); }
				inline void StoreTexSettings() { StoreTexSettings(UIE::_Current); }

				void RestoreTexSettings(UIE* Panel, std::string _tex);
				void RestoreTexSettings(std::string _kind, std::string _tex);

				void SaveSettings();
				void SaveMap();
				inline void Save() { SaveSettings(); SaveMap(); }

				std::string TexKind(std::string _tex);

				inline ~TMapData() { Save(); }

			};

		}
	}
}
