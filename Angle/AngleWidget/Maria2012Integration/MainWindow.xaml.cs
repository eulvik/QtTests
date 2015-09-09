using System.ComponentModel;
using System.Windows;

namespace Maria2012Integration
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

        private void MainWindow_OnClosing(object sender, CancelEventArgs e)
        {
            MariaUserControl.Dispose();
        }
    }
}
