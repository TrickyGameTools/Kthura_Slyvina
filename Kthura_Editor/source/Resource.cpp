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
// Version: 23.09.28
// EndLic

#include <SlyvGINIE.hpp>
#include <SlyvDir.hpp>

#include "../../SupJCR/SupJCR.hpp"
#include "../headers/Project.hpp"
#include "../headers/Resource.hpp"
#include "../headers/ConfigCLI.hpp"

using namespace Slyvina::JCR6;
using namespace Slyvina::Units;

namespace Slyvina {
	namespace Kthura {
		namespace Editor {
			JCR6::JT_Dir _Res{nullptr};
			JCR6::JT_Dir _TexRes{nullptr};

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


			JCR6::JT_Dir TexResource() {
				if (!_TexRes) {
					auto G = LoadUGINIE(ProjectFile()); // Crash prevention. Should not be needed, but somehow, it is!
					_TexRes = std::make_shared<_JT_Dir>();
					auto TL = G->List("Paths.Windows", "TexMaps");
					auto MRG = Upper(G->Value("Paths", "TexMerge")) == "YES";
					QCol->Doing("Scannning", "Textures");
					for (auto D : *TL) {
						if (MRG) {
							QCol->Doing("Tex Main", D);
							auto Dirs{ FileList(D,DirWant::Directories) };
							for (auto PatchDir : *Dirs) {
								auto PD{ ChReplace(D + "/" + PatchDir,'\\','/') };
								QCol->Doing("=> TexDir", PD);
								_TexRes->Patch(PD);
							}
						} else {
							QCol->Doing("TexDir", D);
							_TexRes->Patch(D);
						}
					}
				}
				return _TexRes;
			}

			static VecString _TexLst{nullptr};
			VecString TextureList() {
				if (!_TexLst) {
					_TexLst = NewVecString();
					auto extensies = { "PNG","JPG","JPEG","BMP" };
					auto _Res{ TexResource() };
					QCol->Doing("Indexing", "Textures");
					auto _Entries{ _Res->Entries() };
					auto _JPBF{ std::map<std::string,bool>() };
					for (auto entry : *_Entries) {
						auto _ED{ ExtractDir(entry->Name()) };
						auto _EDU{ Upper(_ED) };
						// std::cout << entry->Name() << " Dir: " << _ED << " -> " << _EDU << "(" << ExtractExt(_EDU) << ")\n"; // debug
						if (ExtractExt(_EDU) == "JPBF") {
							// std::cout << "Bundle:" << _ED << "\n";
							if (!_JPBF.count(_EDU)) {
								_JPBF[_EDU] = true;
								_TexLst->push_back(_ED);	
								//std::cout << "Bundle:" << _ED << " added\n";
							}
						} else if (ExtractExt(_EDU) == "JFBF") {
							// Ignoring FONT bundles
						} else {
							auto herkend{ false };
							for (auto ex : extensies) herkend = herkend || ExtractExt(Upper(entry->Name())) == ex;
							// std::cout << entry->Name() << "; herkend = " << herkend << "\n"; // debug only!
							if (herkend) _TexLst->push_back(entry->Name());
						}
					}
					//SortVecString(_TexLst);
				}
				return _TexLst;
			}
		}
	}
}