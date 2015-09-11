using System;
using System.Windows;

namespace MariaAngleApp
{
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window
	{
		public MainWindow()
		{
			InitializeComponent();
		    DataContext = new ViewModel();
		}

	    private void MainWindow_OnClosed(object sender, EventArgs e)
	    {
	        MariaUserControl.Dispose();
	    }
	}
}
