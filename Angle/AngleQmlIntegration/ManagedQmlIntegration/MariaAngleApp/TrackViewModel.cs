using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using TPG.GeoFramework.TrackCore.Contracts;
using TPG.Maria.Common;
using TPG.Maria.Contracts;
using TPG.Maria.TrackContracts;

namespace MariaAngleApp
{
    public class TrackViewModel : INotifyPropertyChanged
    {
        private string _styleXml;

        public IMariaTrackLayer TrackLayer { get; set; }

        public ObservableCollection<ITrackData> SelectedTrackDatas { get; set; }

        public TrackViewModel(IMariaTrackLayer trackLayer)
        {
            TrackLayer = trackLayer;
            TrackLayer.LayerInitialized += OnLayerInitialized;
        }



        private void OnLayerInitialized()
        {
            TrackLayer.TrackLists = new ObservableCollection<string>()
                                    {
                                        "ais.test"
                                    };
            TrackLayer.TrackServices = new ObservableCollection<IMariaService>
                                    {
                                        new MariaService("TrackService")
                                    };


            TrackLayer.ActiveTrackService = TrackLayer.TrackServices.First();
            TrackLayer.ActiveTrackList = TrackLayer.TrackLists.First();
        }

        public event PropertyChangedEventHandler PropertyChanged;

        //[NotifyPropertyChangedInvocator]
        protected virtual void OnPropertyChanged(string propertyName)
        {
            PropertyChangedEventHandler handler = PropertyChanged;
            if (handler != null) handler(this,
                new PropertyChangedEventArgs(propertyName));
        }
    }
}