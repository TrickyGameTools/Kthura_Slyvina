// Lic:
// Kthura
// Kthura Play Utility
// 
// 
// 
// (c) Jeroen P. Broks, 2024
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
// Version: 24.07.17
// EndLic
#include <SlyvQCol.hpp>
#include <SlyvString.hpp>
#include <SlyvDir.hpp>
#include <SlyvAsk.hpp>
#include <Kthura_Core.hpp>
#include <Kthura_Save.hpp>

using namespace std;
using namespace Slyvina;
using namespace Units;

#define QCol Slyvina::Units::QCol
#define QErr(errmsg) { QCol->Error(errmsg); return 1; }
#define Forever while (true)

struct LM {
	Slyvina::Kthura::Kthura KthMap;
	bool modified{ false };
};

map<string, LM> LoadedMaps;

LM* GetMap(string map) {
	if (!LoadedMaps.count(map)) {
		QCol->Doing("Loading map", map);
		LoadedMaps[map].KthMap = Slyvina::Kthura::LoadKthura(map);
	}
	return &(LoadedMaps[map]);
}

typedef int(*KPACT)(string);

bool KPYes(LM* M, string ctg, string vr, string question, bool alwaysperm=false) {
	auto mp{ M->KthMap };
	auto opt{ mp->Option("KTHURA_PLAY::" + ctg,vr) };
	if (opt.size()) return Upper(Left(opt, 1)) == "Y";
	Forever{
		QCol->Dark(TrSPrintF("%d: ",alwaysperm));
	QCol->Yellow(question);
	QCol->Magenta(" ?");
	QCol->Cyan(" <Y/N> ");
	QCol->Green("");
	auto A{ Trim(ReadLine()) };
	if (alwaysperm) Trans2Upper(A);
	if (A.size()) switch (A[0]) {
	case 'Y': mp->Option("KTHURA_PLAY::" + ctg, vr, "YES"); M->modified = true; // FALLTHROUGH!
	case 'y': return true;
	case 'N': mp->Option("KTHURA_PLAY::" + ctg, vr, "NO"); M->modified = true; // FALLTHROUGH!
	case 'n': return false;
	}
	}
}

static string QRL(string question = "string") {
	QCol->Doing(question, "", "");
	return ReadLine();
}

static int QRLint(string question = "integer") {
	string aw{};
opnieuw:
	aw = QRL(question);
	try{
		return stoi(aw);
	} catch(runtime_error e) {
		QCol->Red("?");
		QCol->Yellow("Redo from start");
		goto opnieuw;
	}
}

static bool QRLB(string question = "Are you sure") {
	do {
		QCol->Yellow(question);
		QCol->Magenta(" ?");
		QCol->Cyan(" <Y/N> ");
		QCol->Green("");
		auto A{ Trim(ReadLine()) };

		if (A.size()) 
			switch (A[0]) {
			case 'Y':
			case 'y': return true;
			case 'N':
			case 'n': return false;
			}
	} Forever;
}


static int KPA_Origin(string mymap) {
	int errs{ 0 };
	auto m{ GetMap(mymap) };
	if (!m->KthMap) { QCol->Error("Map-Load error: " + mymap + " (origin)"); return 1; }
	auto layers{ m->KthMap->Layers() };	
	QCol->Doing("Origin", mymap);	
	for (auto layname : *layers) {
		QCol->Doing("= Layer", layname, ""); QCol->Magenta(" ... ");
		auto lay{ m->KthMap->Layer(layname) };
		int x{ 0 }, y{ 0 };
		if (!lay->FirstObject()) {
			QCol->Red("No objects!\n");
		} else {
			x = lay->FirstObject()->x();
			y = lay->FirstObject()->y();
			for (auto o = lay->FirstObject(); o; o = o->Next()) {
				x = std::min(o->x(), x);
				y = std::min(o->y(), y);
			}
			if (x == 0 && y == 0) {
				QCol->Green("All clear!\n");
			} else {
				QCol->White(TrSPrintF("Off: (%d,%d)\n", x, y));
				if (
					KPYes(m, "Origin", "ALL_LAYERS_ORIGIN", "Do you want me to perform the origin correction for all layers automatically", true) ||
					KPYes(m, "Origin", "ORIGIN." + layname, "Do you want me to perform the origin correction for layer \"" + layname + "\"")
					) {
					int ax{ abs(x) }, ay{ abs(y) };
					if (ax % (int)(lay->gridx) || ay % (int)lay->gridy) {
						auto matter_offgrid{
							KPYes(m,"origin","ALL_LAYERS_MATTEROFFGRID","Does off-grid matter in all layers",true) ||
							KPYes(m,"origin","MATTER_OFFGRID_" + layname,"Layer \"" + layname + "\" is off-grid! Does that matter")
						};
						if (matter_offgrid) {
							auto mx{ ax % (int)(lay->gridx) }, my{ ay % (int)lay->gridy };
							auto cx{ 0 }, cy{ 0 };
							QCol->Doing("Off-grid", TrSPrintF("%d,%d", mx, my));
							if (x < 0) {
								cx = x; while (abs(--cx) % (int)lay->gridx) --x; --x;

								//QCol->Error("Off-grid correction with negative x not yet supported"); errs++; continue;
							} else {
								cx = x - mx;
								x -= mx;
							}
							if (y < 0) {
								cy = y; while (abs(--cy) % (int)lay->gridy) --y; --y;
								//QCol->Error("Off-grid correction with negative y not yet supported"); errs++; continue;
							} else {
								cy = y - my;
								y -= my;
							}
							string TAG{ "KTHURA_PLAY_CORRECTION_ZONE" };
							while (lay->HasTag(TAG)) {
								static uint64 num{0};
								TAG = TrSPrintF("KTHURA_PLAY_CORRECTION_ZONE_%9X", ++num);
							}
							QCol->Doing("Creating", TAG);
							auto co{ lay->NewObject(Slyvina::Kthura::KthuraKind::Zone) };
							co->x(cx);
							co->y(cy);
							co->w(2);
							co->h(2);
							co->Tag(TAG);
							co->visible(true);
							//QCol->Error("Off-grid correction not yet supported"); errs++; continue;
						}
					}
					// Correct!
					for (auto o = lay->FirstObject(); o; o = o->Next()) {
						QCol->Doing("Correcting", TrSPrintF("Object #%04d", o->ID()), "\r");
						o->x(o->x() - x);
						o->y(o->y() - y);
						m->modified = true;
					}
					QCol->Doing("Correcting", "Completed                      ");

				}
			}
		}

	}
	return errs;
}

static int KPA_Copy(string mymap) {
	auto m{ GetMap(mymap) };
	auto k{ m->KthMap };
	auto err{ 0 };
	auto
		FromLayer{ QRL("Copy from layer") },
		ToLayer{ QRL("Copy to layer") };
	auto
		StartX{ QRLint("Area start X") },
		StartY{ QRLint("Area start Y") },
		Width{ QRLint("Width") }, EndX{ StartX + Width },
		Height{ QRLint("Height") }, EndY{ StartY + Height },
		TarModX{ QRLint("Target X modification") },
		TarModY{ QRLint("Target Y modification") };
	if (!k->HasLayer(FromLayer)) QErr("Source layer does not exist!");
	if (!k->HasLayer(ToLayer)) {
		if (!QRLB("Target layer does not exist! Create it")) QErr("Target layer does not exist and not allowed to create it");
		k->NewLayer(ToLayer);
		m->modified = true;
	}
	auto
		fl{ k->Layer(FromLayer) },
		tl{ k->Layer(ToLayer) };
	for (auto o = fl->FirstObject(); o; o=o->Next()) {
		using namespace Slyvina::Kthura;
		auto copy{ false };
		switch (o->Kind()) {
		case KthuraKind::Unknown:
			err++;
			QCol->Error(TrSPrintF("Unknown object kind (Object #%d)", o->ID()));
			copy = false;
			break;
		case KthuraKind::Actor:
			err++;
			QCol->Error(TrSPrintF("Actor (Object #%d)", o->ID()));
			copy = false;
			// The loader should already reject maps containing an actor, however better some extra protection. Just in case!
			break;
		case KthuraKind::Picture:
			err++;
			QCol->Error(TrSPrintF("Picture kind not supported (Object #%d)", o->ID()));
			copy = false;
			break;

		case KthuraKind::Pivot:
		case KthuraKind::Obstacle:
		case KthuraKind::Exit:
		case KthuraKind::Custom:
			copy = o->x() > StartX && o->y() > StartY && o->x() < EndX && o->y() < EndY;
			break;
		case KthuraKind::TiledArea:
		case KthuraKind::StretchedArea:
		case KthuraKind::Zone:
		case KthuraKind::Rect:
		{
			auto oex{ o->x() + o->w() }, oey{ o->y() + o->h() };
			copy = (o->x() > StartX && o->y() > StartY && o->x() < EndX && o->y() < EndY) || (oex > StartX && oey > StartY && oex < EndX && oey < EndY);
		}
		break;
		default:
			QCol->Error(TrSPrintF("Kind error %d (Object #%d)", (int)o->Kind(), o->ID()));
		}
		if (copy) {
			m->modified = true;
			auto no{ tl->NewObject(o->Kind()) };
			QCol->Doing("Copying object", o->ID(), "\t"); QCol->LGreen(TrSPrintF("%d\t",no->ID()));  QCol->LMagenta(o->SKind() + "\n");
			no->a(o->a());
			no->animframe(o->animframe());
			no->animskip(o->animskip());
			no->animspeed(o->animspeed());
			no->b(o->b());
			no->blend(o->blend());
			no->dominance(o->dominance());
			no->forcepassible(o->forcepassible());
			no->g(o->g());
			no->h(o->h());
			no->impassible(o->impassible());
			no->insertx();
			no->inserty();
			no->labels(o->labels());
			no->r(o->r());
			no->scalex(o->scalex());
			no->scaley(o->scaley());
			no->texture(o->texture());
			no->visible(o->visible());
			no->w(o->w());
			no->x(o->x()+TarModX);
			no->y(o->y()+TarModY);		
			// Data
			auto dta{ no->data() };
			for (auto d : *dta) no->data(d.first, d.second);
			// Tag
			auto NewTag{ o->Tag() };
			static auto NN{ 0u };
			while (NewTag.size() && tl->HasTag(NewTag)) {
				QCol->Error("Tag " + o->Tag() + " is already present in the target layer!"); err++;
				if (QRLB("New Tag")) {
					NewTag = QRL("New Tag");
				} else if (o->Kind()==KthuraKind::Exit || o->Kind()==KthuraKind::Zone ) {
					do {
						NewTag = TrSPrintF(string(o->SKind() + ":%9x").c_str(), ++NN);
					} while (tl->HasTag(NewTag));
				}
			}
			if (NewTag.size()) {
				QCol->Doing("= Retagging", o->Tag(), " ");
				QCol->Doing("to", NewTag);
				no->Tag(NewTag);
				tl->RemapTags();
			}
		}
	}
	return err;
}


map<string, KPACT> ActFunc{
	{ "origin", KPA_Origin},
	{ "copy",KPA_Copy }
};

static void Usage() {
	QCol->White("Usage: Kthura_Play <actions> <maps>\n");
	QCol->Grey("Actions should be prefixed with a -\n\n");
}

int main(int ac, char** a) {
	vector<string>
		actions{},
		maps{};
	int
		errors{ 0 };
	QCol->Magenta("Kthura Play!\t");
	QCol->Yellow((string)__DATE__+"\t"+__TIME__+"\n");
	QCol->Doing("Coded by", "Jeroen P. Broks");
	for (int i = 1; i < ac; ++i) {
		switch (a[i][0]) {
		case '-':
			actions.push_back(Lower(a[i]).substr(1));
			break;
		default:
			if (DirectoryExists(a[i])) {
				QCol->Doing("Analysing dir", ChReplace(a[i], '\\', '/'));
				auto l{ GetTree(a[i]) };
				if (!l->size()) { errors++; QCol->Error("Directory is empty"); }
				for (auto f : *l) maps.push_back(ChReplace(a[i],'\\','/')+"/"+f);
			} else if (FileExists(a[i])) {
				maps.push_back(a[i]);
			} else {
				QCol->Error("No file or directory named \"" + string(a[i]) + "\" found");
				errors++;
			}
			break;
		}
	}
	if (!actions.size()) {
		QCol->Error("No actions");
		errors++;
		Usage();
	} else if (!maps.size()) {
		QCol->Error("No maps");
		errors++;
		Usage();
	}

	for (auto m : maps) {
		for (auto act : actions) {
			if (!ActFunc.count(act)) {
				QCol->Error("I don't know action \"" + act + "\" (for map: "+m+") !");
				errors++;
			} else {
				errors += (ActFunc[act])(m);
			}
		}
	}

	// Ending
	for (auto& mp : LoadedMaps) {
		if (mp.second.KthMap && mp.second.modified) {
			QCol->Doing("Saving", mp.first);
			Slyvina::Kthura::Kthura_Save(mp.second.KthMap, mp.first);
		}
	}
	switch (errors) {
	case 0:
		QCol->Green("No errors occured\n");
		break;
	case 1:
		QCol->Red("There was ");
		QCol->Yellow("1");
		QCol->Red(" error\n");
		break;
	default:
		QCol->Red("There wer ");
		QCol->Yellow(to_string(errors));
		QCol->Red(" errors\n");
		break;
	}
	QCol->Reset();
	return errors;
}