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