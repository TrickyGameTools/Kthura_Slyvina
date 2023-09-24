using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TrickyUnits;

namespace Kthura_NewProjectWizard {
	static internal class Stuff {
		internal const string upGlobalConfigFile = "$AppSupport$/KthuraMapEditor.Config.INI";
		static internal string GlobalConfigFile => Dirry.C(upGlobalConfigFile).Replace('\\','/');

		static internal GINIE GlobalConfig = null;

		static internal GINIE Project = GINIE.FromSource("# Nothing to see here");

		static Stuff() {
			Debug.WriteLine($"Loading: {GlobalConfigFile}");
			GlobalConfig = GINIE.FromFile(GlobalConfigFile);
		}
	}
}
