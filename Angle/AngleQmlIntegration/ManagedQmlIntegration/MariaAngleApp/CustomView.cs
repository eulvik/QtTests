using System.Windows;
using System.Windows.Controls;
using AngleWpf;
using TPG.GeoFramework.Contracts.Geo.Context;
using TPG.GeoFramework.Contracts.Geo.Layer;

namespace MariaAngleApp
{
    public class CustomView : Grid, IGeoLayerView
    {
        private AngleUserControl _angleUserControl;

        public CustomView()
        {
            _angleUserControl = new AngleUserControl();
            _angleUserControl.Width = 300;
            _angleUserControl.Height = 300;
            _angleUserControl.VerticalAlignment = VerticalAlignment.Top;
            _angleUserControl.HorizontalAlignment = HorizontalAlignment.Right;

            Children.Add(_angleUserControl);
        }

        public void Generate()
        {
        }

        public void Render(IGeoContext geoContext)
        {
            _angleUserControl.Simulate();
        }
    }
}