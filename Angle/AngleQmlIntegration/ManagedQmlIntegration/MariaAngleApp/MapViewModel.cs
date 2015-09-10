using System.Linq;
using TPG.GeoUnits;
using TPG.Maria.MapContracts;

namespace MariaAngleApp
{
    public class MapViewModel
    {
        public IMariaMapLayer MapLayer { get; set; }
        public GeoPos CenterPosition { get; set; }
        public double Scale
        {
            get
            {
                return MapLayer.GeoContext.CenterScale;
            }
            set
            {
                MapLayer.GeoContext.CenterScale = value;
            }
        }

        public MapViewModel(IMariaMapLayer mapLayer)
        {
            MapLayer = mapLayer;
            MapLayer.LayerInitialized += OnLayerIntialized;
        }

        private void OnLayerIntialized()
        {
            Scale = 100000;
            CenterPosition = new GeoPos(60, 10);

            MapLayer.ExtendedMapLayer.FetchVectorLabels = false;
            MapLayer.ExtendedMapLayer.MapRenderSimple = false;

            MapLayer.ActiveMapTemplate = MapLayer.ActiveMapTemplates.First();
        }
    }
}