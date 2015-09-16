using System.Collections.ObjectModel;
using TPG.Maria.Contracts;
using TPG.Maria.CustomLayer;
using TPG.Maria.MapContracts;
using TPG.Maria.MapLayer;
using TPG.Maria.TrackContracts;
using TPG.Maria.TrackLayer;

namespace MariaAngleApp
{
    public class ViewModel
    {
        public ObservableCollection<IMariaLayer> Layers { get; set; }

        public MapViewModel MapViewModel { get; set; }
        public TrackViewModel TrackViewModel { get; set; }
        private readonly IMariaMapLayer _mapLayer;
        private readonly CustomLayer<CustomViewModel> _customLayer;
        private IMariaTrackLayer _trackLayer;

        public ViewModel()
        {

            Layers = new ObservableCollection<IMariaLayer>();

            _mapLayer = new MapLayer();
            MapViewModel = new MapViewModel(_mapLayer);
            Layers.Add(_mapLayer);

            _customLayer = new CustomLayer<CustomViewModel>(new CustomLayerViewModelFactory());
            Layers.Add(_customLayer);

            _trackLayer = new TrackLayer();
            TrackViewModel = new TrackViewModel(_trackLayer);
            Layers.Add(_trackLayer);
        }
    }
}