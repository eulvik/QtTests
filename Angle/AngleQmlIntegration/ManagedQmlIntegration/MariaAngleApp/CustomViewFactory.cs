using TPG.GeoFramework.Contracts.Geo.Layer;

namespace MariaAngleApp
{
    public class CustomViewFactory : IGeoLayerViewFactory
    {
        private CustomView _instance;

        public IGeoLayerView New()
        {
            return _instance ?? (_instance = new CustomView());
        }
    }
}