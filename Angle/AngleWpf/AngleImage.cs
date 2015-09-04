using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Windows;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using AngleWpf.Annotations;
using AngleInteractionMixedMode;

namespace AngleWpf
{
    public partial class AngleImage : D3DImage, INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        private AngleMixedModeInterface _angleInteractionAPI;
        private bool _imageSourceValid;
        private Size _newSize;
        private Size _currentSize;
        private Point lastPoint;
        private long _timerMeasurement;
        private long _nativeTimerMeasurement;
        private int _frameNumber;
        private const int numFrames = 60;

        public AngleImage()
        {
            IsFrontBufferAvailableChanged += OnIsFrontBufferAvailableChanged;

            BeginRenderingScene();
        }

        public void ResizeImage(Size newSize)
        {
            _newSize = newSize;
        }

        private void OnIsFrontBufferAvailableChanged(object sender, DependencyPropertyChangedEventArgs dependencyPropertyChangedEventArgs)
        {
            // if the front buffer is available, then WPF has just created a new
            // D3D device, so we need to start rendering our custom scene
            if (IsFrontBufferAvailable)
            {
                BeginRenderingScene();
            }
            else
            {
                // If the front buffer is no longer available, then WPF has lost its
                // D3D device so there is no reason to waste cycles rendering our
                // custom scene until a new device is created.
                StopRenderingScene();
            }
        }

        private void StopRenderingScene()
        {
            CompositionTarget.Rendering -= _rendering;
            DetachRenderTarget(true, false);
        }

        private void BeginRenderingScene()
        {
            if (IsFrontBufferAvailable)
            {
                CompositionTarget.Rendering += _rendering;
            }
        }

        public bool InitGlobe(int width, int height)
        {
            if (DesignerProperties.GetIsInDesignMode(this))
                return false;

            IntPtr hwnd = IntPtr.Zero;
            foreach (PresentationSource source in PresentationSource.CurrentSources)
            {
                var hwndSource = (source as HwndSource);
                if (hwndSource != null)
                {
                    hwnd = hwndSource.Handle;
                    break;
                }
            }

            if (hwnd == IntPtr.Zero)
                return false;

            if (width == 0 || height == 0)
                return false;

            _angleInteractionAPI = new AngleMixedModeInterface();

            if (!_angleInteractionAPI.IntializeAngle(hwnd, width, height))
            {
                _angleInteractionAPI = null;
                return false;
            }

            return true;
        }

        private void _rendering(object sender, EventArgs e)
        {
            if (_angleInteractionAPI == null)
                return;

            var timer = Stopwatch.StartNew();

            if (_newSize != _currentSize)
            {
                DetachRenderTarget(true, false);
                _angleInteractionAPI.ResizeRenderSurface(_newSize);
                _currentSize = _newSize;
            }

            if (!_imageSourceValid)
                if (!AttachRenderTarget(false))
                    return;

            Lock();

            Stopwatch nativeTimer = Stopwatch.StartNew();
            _angleInteractionAPI.RenderFrame();
            nativeTimer.Stop();
            _nativeTimerMeasurement += nativeTimer.ElapsedTicks;
            AddDirtyRect(new Int32Rect(0, 0, PixelWidth, PixelHeight));
            Unlock();
            timer.Stop();
            _timerMeasurement += timer.ElapsedTicks;

            if (++_frameNumber == numFrames)
            {
                double frameTime = (double)_timerMeasurement / Stopwatch.Frequency;
                double nativeFrameTime = (double)_nativeTimerMeasurement / Stopwatch.Frequency;

                Fps = numFrames / frameTime;
                NativeFps = numFrames / nativeFrameTime;
                _frameNumber = 0;
                _timerMeasurement = 0;
                _nativeTimerMeasurement = 0;
            }
        }

        public void TerminateGlobe()
        {
            if (_angleInteractionAPI == null)
                return;

            StopRenderingScene();

            _angleInteractionAPI.Shutdown();
            _angleInteractionAPI = null;
        }

        public void Simulate()
        {
            if (_angleInteractionAPI == null)
                return;

            _angleInteractionAPI.Simulate();
        }

        protected virtual bool AttachRenderTarget(bool attachEvent)
        {
            if (!_imageSourceValid)
            {
                Lock();
                try
                {
                    IntPtr surface = _angleInteractionAPI.GetBackBufferPointer();
                    if (surface != IntPtr.Zero)
                    {
                        SetBackBuffer(D3DResourceType.IDirect3DSurface9, surface);
                        _imageSourceValid = true;
                    }
                }
                finally
                {
                    Unlock();
                }
            }
            return _imageSourceValid;
        }

        protected virtual void DetachRenderTarget(bool detatchSurface, bool detatchEvent)
        {
            if (detatchSurface && _imageSourceValid)
            {
                Lock();
                try
                {
                    SetBackBuffer(D3DResourceType.IDirect3DSurface9, IntPtr.Zero);
                    _imageSourceValid = false;
                }
                finally
                {
                    Unlock();
                }
            }
        }

        public void HandleMouseMove(Point mousePosition, MouseEventArgs mouseEventArgs)
        {
            if (_angleInteractionAPI == null)
                return;
            Vector diff = mousePosition - lastPoint;
            mouseEventArgs.Handled = _angleInteractionAPI.MouseMoved(mousePosition, new Point(diff.X, diff.Y), mouseEventArgs);
            lastPoint = mousePosition;
        }

        public void HandleMouseWheel(Point position, MouseWheelEventArgs mouseWheelEventArgs)
        {
            if (_angleInteractionAPI == null)
                return;
            mouseWheelEventArgs.Handled = _angleInteractionAPI.MouseWheel(position, mouseWheelEventArgs);
        }

        public void HandleMouseEnter(Point enterPoint, MouseEventArgs mouseEventArgs)
        {
            lastPoint = enterPoint;
            mouseEventArgs.Handled = true;
        }

        public void HandleMouseLeave(MouseEventArgs mouseEventArgs)
        {
            lastPoint = new Point(-1, -1);
            mouseEventArgs.Handled = true;
        }

        public void HandleMouseDown(Point position, MouseButtonEventArgs mouseButtonEventArgs)
        {
            if (_angleInteractionAPI == null)
                return;
            mouseButtonEventArgs.Handled = _angleInteractionAPI.MouseDown(position, mouseButtonEventArgs);
        }

        public void HandleMouseUp(Point position, MouseButtonEventArgs mouseButtonEventArgs)
        {
            if (_angleInteractionAPI == null)
                return;
            mouseButtonEventArgs.Handled = _angleInteractionAPI.MouseUp(position, mouseButtonEventArgs);
        }
        
        [NotifyPropertyChangedInvocator]
        protected virtual void OnPropertyChanged(string propertyName)
        {
            PropertyChangedEventHandler handler = PropertyChanged;
            if (handler != null) handler(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}