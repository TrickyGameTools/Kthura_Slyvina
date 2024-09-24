// Lic:
// Kthura
// Start Up Editor
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
// Version: 24.02.18
// EndLic

#define ERROR_CATCHING

#include <SlyvStream.hpp>
#include <SlyvString.hpp>
#include <SlyvQCol.hpp>

#include "builddate.hpp"
#include "headers/ConfigCLI.hpp"
#include "headers/Project.hpp"
#include "headers/Glob.hpp"
#include "headers/UserInterface.hpp"
#include "headers/MapData.hpp"
#include "../SupJCR/SupJCR.hpp"

#include <JCR6_Core.hpp>
#include <JCR6_RealDir.hpp>
#include <JCR6_JQL.hpp>
#include <JCR6_zlib.hpp>

#include <TQSG.hpp>

using namespace Slyvina;
using namespace Units;
using namespace Kthura;
using namespace Editor;
//using namespace Launcher;
using namespace TQSG;

int main(int ac, char** arg) {
#ifdef ERROR_CATCHING
	try {
#endif
		auto MyDir = ChReplace(ExtractDir(arg[0]), '\\', '/');
		// Start
		QCol->LGreen("Kthura - Editor\n");
		QCol->Magenta("(c) 2015, 2016, 2017, 2019, 2021, 2023, 2024 Jeroen P. Broks - Released under the terms of the GPL3\n\n");
		QCol->Doing("Coded by", "Jeroen P. Broks");
		QCol->Doing("Build", SuperTed_BuildDate);
		QCol->Doing("Platform", Platform());
		QCol->Doing("PlatformX", Platform(false));
		QCol->Doing("Kthura Dir", MyDir);
		QCol->Doing("Called from", ChReplace(CurrentDir(), '\\', '/'));		
		if (!CLIParse(ac, arg)) return 1;
		Slyvina::Kthura::OldAlpha = true; // Will allow Kthura to convert old Alpha tags to Alpha255 tags.
		std::cout << "\n\n";
		JCR6::InitJQL();
		JCR6::JCR6_InitRealDir();
		JCR6::init_zlib();
		//QCol->Doing("Loading map", MapFile());
		QCol->Doing("Initizing", "Graphics Window");
		Graphics(WW(), WH(), "Kthura Map Editor - (c) Jeroen P. Broks"); Cls(); Flip();
		MapData = std::unique_ptr<TMapData>(new TMapData());
		MapData->MapFile = MapFile();
		MapData->Load();
		UserInterFace_Init();
		UserInterface_Run();
#ifdef ERROR_CATCHING
	} catch (std::runtime_error	e) {
		QCol->Error(e.what());
	}
#endif
		return 0;
}