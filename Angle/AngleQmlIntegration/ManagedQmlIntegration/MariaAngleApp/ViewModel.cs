using System.Collections.ObjectModel;
using TPG.Maria.Contracts;
using TPG.Maria.CustomLayer;
using TPG.Maria.MapContracts;
using TPG.Maria.MapLayer;

namespace MariaAngleApp
{
    public class ViewModel
    {
        public ObservableCollection<IMariaLayer> Layers { get; set; }

        public MapViewModel MapViewModel { get; set; }
        private readonly IMariaMapLayer _mapLayer;
        private readonly CustomLayer<CustomViewModel> _customLayer;

        public ViewModel()
        {

            Layers = new ObservableCollection<IMariaLayer>();

            _mapLayer = new MapLayer();
            MapViewModel = new MapViewModel(_mapLayer);
            Layers.Add(_mapLayer);

            _customLayer = new CustomLayer<CustomViewModel>(new CustomLayerViewModelFactory());
            Layers.Add(_customLayer);
        }
    }
}