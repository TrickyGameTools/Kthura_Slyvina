// Lic:
// Kthura
// Stuff for Project Creation Wizard
// 
// 
// 
// (c) Jeroen P. Broks, 2015, 2016, 2017, 2019, 2021
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
// Version: 23.09.25
// EndLic
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