using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Input;

namespace AngleWpf
{
    /// <summary>
    /// Interaction logic for AngleUserControl.xaml
    /// </summary>
    public partial class AngleUserControl : UserControl
    {
        public AngleUserControl()
        {
            InitializeComponent();

            SetBinding(FpsProperty, new Binding("Fps") { Source = AngleImage, Mode = BindingMode.OneWay });
            SetBinding(NativeFpsProperty, new Binding("NativeFps") { Source = AngleImage, Mode = BindingMode.OneWay });
            BindingOperations.SetBinding(AngleImage, WireframeProperty, new Binding("Wireframe") { Source = this, Mode = BindingMode.TwoWay });
        }

        private void UserControl_Loaded(object sender, RoutedEventArgs e)
        {
            int w = (int)ActualWidth;
            int h = (int)ActualHeight;
            Window window = Window.GetWindow(this);
            AngleImage.InitGlobe(w, h);
        }

        public void Shutdown3D()
        {
            AngleImage.TerminateGlobe();
        }

        public void Simulate()
        {
            AngleImage.Simulate();
        }

        private void RenderImage_OnSizeChanged(object sender, SizeChangedEventArgs e)
        {
            if (e.NewSize.Width <= 0 || e.NewSize.Height <= 0)
            {
                e.Handled = false;
                return;
            }
            AngleImage.ResizeImage(e.NewSize);
            e.Handled = true;
        }

        private void RenderImage_OnMouseMove(object sender, MouseEventArgs e)
        {
            var point = e.GetPosition(this);
            AngleImage.HandleMouseMove(point, e);
        }

        private void RenderImage_OnMouseDown(object sender, MouseButtonEventArgs e)
        {
            AngleImage.HandleMouseDown(e.GetPosition(this), e);
        }

        private void RenderImage_OnMouseWheel(object sender, MouseWheelEventArgs e)
        {
            AngleImage.HandleMouseWheel(e.GetPosition(this), e);
        }

        private void RenderImage_OnMouseEnter(object sender, MouseEventArgs e)
        {
            AngleImage.HandleMouseEnter(e.GetPosition(this), e);
        }

        private void RenderImage_OnMouseLeave(object sender, MouseEventArgs e)
        {
            AngleImage.HandleMouseLeave(e);
        }

        private void RenderImage_OnMouseUp(object sender, MouseButtonEventArgs e)
        {
            AngleImage.HandleMouseUp(e.GetPosition(this), e);
        }

        public static readonly DependencyProperty WireframeProperty =
            AngleImage.WireframeProperty.AddOwner(typeof(AngleUserControl),
            new FrameworkPropertyMetadata(false, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));

        public bool Wireframe
        {
            get { return (bool)GetValue(WireframeProperty); }
            set { SetValue(WireframeProperty, value); }
        }

        public static readonly DependencyProperty FpsProperty =
            AngleImage.FpsProperty.AddOwner(typeof(AngleUserControl));

        public double Fps
        {
            get { return (double)GetValue(FpsProperty); }
            set { SetValue(FpsProperty, value); }
        }

        public static readonly DependencyProperty NativeFpsProperty =
            AngleImage.NativeFpsProperty.AddOwner(typeof(AngleUserControl));

        public double NativeFps
        {
            get { return (double)GetValue(NativeFpsProperty); }
            set { SetValue(NativeFpsProperty, value); }
        }

    

        public AngleImage Angle { get { return AngleImage; } }
    }
}
