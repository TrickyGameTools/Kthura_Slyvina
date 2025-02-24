// License:
// 
// Kthura
// Glob
// 
// 
// 
// 	(c) Jeroen P. Broks, 2015, 2016, 2017, 2019, 2021, 2023, 2025
// 
// 		This program is free software: you can redistribute it and/or modify
// 		it under the terms of the GNU General Public License as published by
// 		the Free Software Foundation, either version 3 of the License, or
// 		(at your option) any later version.
// 
// 		This program is distributed in the hope that it will be useful,
// 		but WITHOUT ANY WARRANTY; without even the implied warranty of
// 		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// 		GNU General Public License for more details.
// 		You should have received a copy of the GNU General Public License
// 		along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// 	Please note that some references to data like pictures or audio, do not automatically
// 	fall under this licenses. Mostly this is noted in the respective files.
// 
// Version: 25.01.28 I
// End License


#include <string>


#include <SlyvGINIE.hpp>
#include <SlyvDirry.hpp>
#include <SlyvQCol.hpp>
#include <SlyvAsk.hpp>
#include <SlyvString.hpp>
//#include <Platform.hpp>

#include "Glob.hpp"

using namespace std;
using namespace Slyvina::Units;
using namespace Slyvina::JCR6;

namespace Slyvina {
	namespace Kthura {
		namespace Launcher {
			std::string MyDir{ "." };
			JT_Dir JAS{ nullptr };

			static const char* gfile = "$AppSupport$/Kthura.ini";
			static GINIE _globalconfig{};
			//static map<string, string> AltMountRegister;

			static void LoadGlobalConfig(bool force = false) {
				static bool Loaded{ false };
				if ((!Loaded) || force) {
					QCol->Doing("Reading", Dirry(gfile));
					_globalconfig = LoadGINIE(Dirry(gfile),Dirry(gfile));
					//_globalconfig->AutoSave = Dirry(gfile);
					Loaded = true;
				}
			}

			static std::string AltMount(std::string file) {
				auto sfile = *Split(file, ':');
				if (sfile.size() <= 1) return file;
				if (Platform() == "Windows" && sfile[0].size() == 1) return file;
				if (!sfile[0].size() || sfile.size() > 2) { QCol->Error("AltMount: Invalid file " + file); return file; }
				auto drv{ Upper(sfile[0]) };
				auto path{ sfile[1] };
				auto mdrv{ Ask(_globalconfig,"Mount",drv,TrSPrintF("In file \"%s\" the unrecognized drive '%s' appears to be requested.\nI can mount a directory to that.\nPlease give me one: ",file.c_str(),drv.c_str())) };
				return mdrv + "/" + path;
			}



			std::string ProjectsDir() {
				LoadGlobalConfig();
				auto ret = Ask(_globalconfig, "Directories", "Projects", "I need to know in which directory you wish you store your projects", Dirry("$Home$/.SuperTed"));
				ret = AltMount(ret);
				return ret;
			}

			int WW() {
				return AskInt(_globalconfig, "Launcher", "Width", "Prefered Window width for launcher:", 1200);
			}
			int WH() {
				return AskInt(_globalconfig, "Launcher", "Height", "Prefered Window Height for launcher:", 800);
			}

		}
	}
}
