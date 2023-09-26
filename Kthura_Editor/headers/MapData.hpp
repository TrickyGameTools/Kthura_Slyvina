#pragma once

#include <SlyvGINIE.hpp>

#include <Kthura_Core.hpp>

namespace Slyvina {
	namespace Kthura {
		namespace Editor {

			class TMapData;
			extern std::unique_ptr<TMapData> MapData;

			class TMapData {
			private:
			public:
				std::string MapFile{};
				Kthura TheMap{ nullptr };
				UGINIE TextureSettings{ nullptr };
				std::string TextureSettingsDir();
				std::string TextureSettingsFile();

				void Load();
			};

		}
	}
}