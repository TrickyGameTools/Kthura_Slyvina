// Lic:
// ***********************************************************
// Kthura_CompiledTest/Kthura_CompiledTest.cpp
// This particular file has been released in the public domain
// and is therefore free of any restriction. You are allowed
// to credit me as the original author, but this is not
// required.
// This file was setup/modified in:
// 2024
// If the law of your country does not support the concept
// of a product being released in the public domain, while
// the original author is still alive, or if his death was
// not longer than 70 years ago, you can deem this file
// "(c) Jeroen Broks - licensed under the CC0 License",
// with basically comes down to the same lack of
// restriction the public domain offers. (YAY!)
// ***********************************************************
// Version 24.09.24
// EndLic
#include <JCR6_zlib.hpp>
#include <SlyvQCol.hpp>

#include <Kthura_Core.hpp>
#include <Kthura_LoadCompiled.hpp>

using namespace Slyvina;
using namespace Units;
using namespace Kthura;
using namespace std;

static string AppExe, AppShort;

int main(int c, char** a) {
	JCR6::init_zlib();
	AppExe = a[0]; AppShort = StripAll(a[0]);
	QCol->Doing(AppShort, (string)__DATE__ +"; "+ __TIME__);
	QCol->Doing("Coded by", "Jeroen P. Broks");
	RegCompiledXLoader();
	for (int i = 1; i < c; ++i) {
		string cf = a[i];
		if (FileExists(cf)) {
			auto J{ JCR6::JCR6_Dir(cf) };
			auto Rec{ XRecognizeKthura(J) };
			QCol->LMagenta("\n\n" + cf + "\n");
			QCol->Doing("Recognized", Rec);
			QCol->Doing("Compiled", boolstring(IsCompiledKthura(J)));
			if (Rec == "unknown") { QCol->Error("Unrecognized map"); continue; }
			auto KM{ XLoadKthura(J) }; if (!KM) { QCol->Error("Kthura map failed to load!"); QCol->LMagenta(CompiledKthuraError() + "\n"); continue; }
			auto KL{ KM->Layers() };
			QCol->Doing("Layers", KL->size());
			for (int i = 0; i < KL->size(); i++) QCol->Doing(TrSPrintF("L %d/%d", i + 1, KL->size()), (*KL)[i]);
			map<string, uint32> CObjects{};
			for (auto LayName : *KL) {
				auto Lay{ KM->Layer(LayName) };
				CObjects["total"] = 0;
				for (auto o = Lay->FirstObject(); o; o = o->Next()) {
					CObjects["total"]++;
					if (!CObjects.count(o->SKind())) CObjects[o->SKind()] = 0;
					CObjects[o->SKind()]++;
				}
				QCol->Doing("Layer", LayName);
				for (auto COI : CObjects) QCol->Doing(COI.first, COI.second);
			}
			
		} else {
			QCol->Error("File not found: " + cf);
		}
	}
	QCol->Reset();
	return 0;
}