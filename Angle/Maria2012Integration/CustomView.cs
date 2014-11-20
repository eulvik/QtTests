using System.Windows;
using System.Windows.Controls;
using AngleWpf;
using TPG.GeoFramework.Contracts.Geo.Context;
using TPG.GeoFramework.Contracts.Geo.Layer;

namespace Maria2012Integration
{
    public class CustomView : Grid, IGeoLayerView
    {
        private AngleUserControl _angleUserControl;

        public CustomView()
        {
            _angleUserControl = new AngleUserControl();
            _angleUserControl.Width = 200;
            _angleUserControl.Height = 200;
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