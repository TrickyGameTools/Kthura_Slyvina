// Lic:
// Kthura
// User Interface Layers
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

#include <SlyvQCol.hpp>
#include <june19.hpp>

#include "../headers/MapData.hpp"
#include "../headers/UI_MainEditor_Class.hpp"
#include "../headers/UI_Layers.hpp"

namespace Slyvina {
	namespace Kthura {
		namespace Editor {

			using namespace June19;
			using namespace Units;

#pragma region Variables
			enum class LayAct{Unknown,Create,Rename};

			static UI* LayerStage{nullptr};
			static j19gadget
				* LayerPanel{nullptr},
				* LayerCaption{ nullptr },
				* LayerTextField{ nullptr },
				* LayerExists{ nullptr },
				* LayerOkay{ nullptr },
				* LayerCancel{ nullptr };

			std::string CurrentLayerTag{""};
#pragma endregion

#pragma region "Update Main UI"

			void UpdateLayerSelector() {
				auto LS{ LayerSelector };
				auto Lays{ MapData->TheMap->Layers() };
				auto SL{ 0 };
				LS->ClearItems();
				for (int i = 0; i < Lays->size();++i) {
					auto LSI{ (*Lays)[i] };
					LS->AddItem(LSI);
					if (CurrentLayerTag == "") CurrentLayerTag = LSI;
					if (CurrentLayerTag == LSI) SL = i;
				}
				LS->SelectItem(SL);
			}
			KthuraLayer* CurrentLayer() { return MapData->TheMap->Layer(CurrentLayerTag); }
#pragma endregion

#pragma region Callbacks
			static void UpdateCancel(j19gadget*, j19action) { LayerCancel->X(LayerOkay->X() + LayerOkay->W() + 5); }
			static void UpdateOkay(j19gadget* g, j19action) {
				LayerTextField->Text = Trim(Upper(LayerTextField->Text));
				LayerExists->Visible = MapData->TheMap->HasLayer(LayerTextField->Text);
				g->Enabled = LayerTextField->Text.size() && (!LayerExists->Visible);
			}
			static void ActCancel(j19gadget*, j19action) {
				UI::GoToStage("Editor");
			}
#pragma endregion

#pragma region "Navigate here"
			inline void CreateLayerStage() {
				if (UI::NewStage("Layers")) {
					QCol->Doing("Creating", "Layer stage UI");
					LayerStage = UI::GetStage("Layers");
					LayerPanel = LayerStage->MainGadget;
					LayerCaption = CreateLabel("", 5, 5, LayerPanel->W(), 20,LayerPanel);
					LayerExists = CreateLabel("A layer with that name already exists", 0, 50, LayerPanel->W(), 20, LayerPanel, 2);
					LayerExists->SetForeground(255, 0, 0);
					LayerTextField = CreateTextfield(5, 100, LayerPanel->W() - 10, LayerPanel);
					LayerTextField->SetForeground(255, 255, 255);
					LayerTextField->SetBackground(0, 0, 100);
					LayerOkay = CreateButton("Okay", 5, 150, LayerPanel);
					LayerOkay->SetForeground(0, 255, 0);
					LayerOkay->SetBackground(0, 25, 0);
					LayerOkay->CBDraw = UpdateOkay;
					LayerCancel = CreateButton("Cancel", 0, 150, LayerPanel);
					LayerCancel->SetForeground(255, 0, 0);
					LayerCancel->SetBackground(25, 0, 0);
					LayerCancel->CBDraw = UpdateCancel;
					LayerCancel->CBAction = ActCancel;
				}
			}

			void PDM_NewLayer(j19gadget*, j19action) {
				QCol->Doing("Request", "Create a new layer");
				CreateLayerStage();
				LayerCaption->Caption = "Please enter a name for the new layer";
				UI::GoToStage("Layers");
			}
		}
#pragma endregion
	}
}