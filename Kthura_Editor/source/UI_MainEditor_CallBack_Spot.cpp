
#include <TQSG.hpp>
#include <TQSE.hpp>

#include <SlyvQCol.hpp>

#include "../headers/MapData.hpp"
#include "../headers/UI_MainEditor_Class.hpp"

namespace Slyvina {
	namespace Kthura {
		namespace Editor {
			using namespace TQSG;
			using namespace Units;

			static void SpotPressed(int x, int y) {
				auto D{ UIE::_Current };
				auto L{ MapData->CurrentLayer() };
				if (D->Tex->Caption == "" || D->Tex->Caption == "...") return;
				auto O{ L->NewObject(KindName(D->Kind->Caption)) };
				O->texture(D->Tex->Caption);
				O->x(x);
				O->y(y);
				O->dominance(ToInt(D->Dominance->Text));
				O->alpha(ToInt(D->Alpha->Text));
				O->red(ToInt(D->Red->Text));
				O->green(ToInt(D -> Green->Text));
				O->blue(ToInt(D->Blue->Text));
				O->animframe(ToInt(D->AnimFrame->Text));
				O->animspeed(ToInt(D->AnimSpeed->Text));
				//O->forcepassible(D->ForcePassible->checked);
				//O->impassible(D->Impassible->checked);
				O->forcepassible(false); // Not applicable for spot based objects!
				O->impassible(false);
				O->insertx(0);
				O->inserty(0);
				O->scalex(ToInt(D->ScaleX->Text));
				O->scaley(ToInt(D->ScaleY->Text));
				O->visible(D->Visible->checked);
				// O->labels // Comes later!
				O->rotatedeg(ToInt(D->Rotate->Text));
				L->TotalRemap();
				QCol->Doing("Created", O->SKind() + TrSPrintF(" (%d) at (%5d,%5d)", O->ID(),O->x(),O->y()));
				TQSE::Flush();
			}


			static void SpotReleased(int x, int y) {}
			static void SpotDraw(int x, int y) {
				static auto
					EPX{ MapPanel->DrawX() },
					EPY{ MapPanel->DrawY() };
				int
					dx = (x - ScrollX) + EPX,
					dy = (y - ScrollY) + EPY;
				// printf("(%d,%d) -> (%d,%d)\n", x, y, dx, dy); // DEBUG ONLY!!
				SetColor(200,200,200);
				Line(dx - 5, dy, dx + 5, dy);
				Line(dx, dy - 5, dx, dy + 5);
			}


			void InitEditSpot() {
				UIEAct::Reg[UIEType::Spot].Pressed = SpotPressed;
				UIEAct::Reg[UIEType::Spot].Released = SpotReleased;
				UIEAct::Reg[UIEType::Spot].Draw = SpotDraw;
			}
		}
	}
}