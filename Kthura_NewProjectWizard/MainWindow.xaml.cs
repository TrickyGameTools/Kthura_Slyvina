using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using TrickyUnits;

namespace Kthura_NewProjectWizard {
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window {

		List<TextBox> NoIntern = new List<TextBox>();
		Dictionary<TextBox, Button> EnableLink = new Dictionary<TextBox, Button>();
		bool CanCheck = false;

		public MainWindow() {
			InitializeComponent();
			NoIntern.Clear();
			NoIntern.Add(DirMaps); EnableLink[DirMaps] = DirMaps_Pick;
			NoIntern.Add(TexMaps); EnableLink[TexMaps] = TexMaps_Pick;
			CanCheck = true;
			ChkEnabled();
		}

		void ChkEnabled() {
			if (CanCheck) {
				foreach (TextBox txt in NoIntern) {
					txt.IsEnabled = InternalProject.IsChecked != true;
				}
				foreach (var tl in EnableLink) tl.Value.IsEnabled = tl.Key.IsEnabled == true;
				TexMerge.IsEnabled = InternalProject.IsChecked != true;

				TexMerge.IsChecked = TexMerge.IsChecked == true && InternalProject.IsChecked != true;
			}
		}

		private void NameProject_TextChanged(object sender, TextChangedEventArgs e) {
			Stuff.Project["Meta", "Project"] = NameProject.Text;
			if (InternalProject.IsChecked == true) {
				DirMaps.Text = $"{Stuff.GlobalConfig["Windows", "WorkSpace"]}/{NameProject.Text}/Maps";
				TexMaps.Text = $"{Stuff.GlobalConfig["Windows", "WorkSpace"]}/{NameProject.Text}/Textures";
			}

		}

		private void NameAuthor_TextChanged(object sender, TextChangedEventArgs e) => Stuff.Project["Meta", "Author"] = NameProject.Text;

		private void InternalProject_Checked(object sender, RoutedEventArgs e) {
			if (InternalProject.IsChecked == true) InternalProject.Content = "Yes"; else InternalProject.Content = "No";
			ChkEnabled();
		}

		private void MapsDir_TextChanged(object sender, TextChangedEventArgs e) => Stuff.Project["Paths.Windows", "Maps"] = DirMaps.Text;

		private void DirMaps_Pick_Hit(object sender, RoutedEventArgs e) {
			var file = FFS.RequestDir();
			if (file != "") DirMaps.Text = file;
		}

		private void MapsTex_TextChanged(object sender, TextChangedEventArgs e) {
			var L = Stuff.Project.List("Paths.Windows", "TexMaps");
			L.Clear();
			L.Add(TexMaps.Text);
		}

		private void DirTex_Pick_Hit(object sender, RoutedEventArgs e) {
			var file = FFS.RequestDir();
			if (file != "") TexMaps.Text = file;
		}

		private void TexMerge_Checked(object sender, RoutedEventArgs e) {
			if (TexMerge.IsChecked == true) TexMerge.Content = "Yes"; else TexMerge.Content = "No";
			Stuff.Project["Paths", "TexMerge"] = TexMerge.Content.ToString().ToUpper();
		}

		private void Type_GenData(object sender, TextChangedEventArgs e) {
			var Dat = GenData.Text.Replace("\r", "");
			var SDT = Dat.Split('\n');
			var L = Stuff.Project.List("Map", "GeneralData");
			L.Clear();
			foreach (var DF in SDT) L.Add(DF.Trim());
		}

		private void GaanMetDieBanaan_Actie(object sender, RoutedEventArgs e) {
			if (Stuff.Project["META", "PROJECT"] == "") { Confirm.Error("Project doesn't have a name!"); return; }
			if (Stuff.Project["META", "AUTHOR"] == "") {
				if (Confirm.Yes("No author?\n\nOr are you Jeroen P. Broks himself?")) {
					//Stuff.Project["META", "AUTHOR"] = "Jeroen P. Broks";
					NameAuthor.Text = "Jeroen P. Broks";
				}
			}
			if (Stuff.Project["Paths.Windows", "Maps"] == "") { Confirm.Error("No maps directory!"); return; }
			if (TexMaps.Text=="") { Confirm.Error("No Texture Directory"); return; }
			string
				PrjDir = $"{Stuff.GlobalConfig["Windows", "WorkSpace"]}/{Stuff.Project["META", "PROJECT"]}",
				PrjFile = $"{PrjDir}/{Stuff.Project["META", "PROJECT"]}.Project.ini";
			if (Directory.Exists(PrjDir)) {
				Confirm.Error($"The project directory {PrjDir} already exists! Continuing would harm that project!\n\n\nIf you wanna create a project with that name, either remove or rename that old folder first!");
				return;
			}
			Stuff.Project["META", "Created"] = DateTime.Now.ToString();
			Directory.CreateDirectory(PrjDir);
			Stuff.Project.SaveSource(PrjFile);
			Directory.CreateDirectory(Stuff.Project["Paths.Windows", "Maps"]);
			Directory.CreateDirectory(TexMaps.Text);
			Environment.Exit(0);
		}
	}
}
