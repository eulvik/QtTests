using System.Windows;

namespace AngleWpf
{
    public partial class AngleImage
    {
        public static readonly DependencyProperty WireframeProperty =
           DependencyProperty.Register("Wireframe", typeof(bool), typeof(AngleImage),
                                       new FrameworkPropertyMetadata(false, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));
        
        public static readonly DependencyProperty FpsProperty =
            DependencyProperty.Register("Fps", typeof(double), typeof(AngleImage),
                            new PropertyMetadata(0.0));

        public double Fps
        {
            get { return (double)GetValue(FpsProperty); }
            set { SetValue(FpsProperty, value); }
        }

        public static readonly DependencyProperty NativeFpsProperty =
            DependencyProperty.Register("NativeFps", typeof(double), typeof(AngleImage),
                            new PropertyMetadata(0.0));

        public double NativeFps
        {
            get { return (double)GetValue(NativeFpsProperty); }
            set { SetValue(NativeFpsProperty, value); }
        }
    }
}