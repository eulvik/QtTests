// AngleInteractionMixedMode.h

#pragma once

#include <anglenativeinterface.h>
#include <msclr/marshal_cppstd.h>

using namespace System;
using System::Windows::Point;
using System::Windows::Size;

using namespace System::Windows::Input;

namespace AngleInteractionMixedMode 
{

	public ref class AngleMixedModeInterface
	{
	public:
		AngleMixedModeInterface(void)
		{
			_angleNativeInterface = new AngleNativeInterface();
		}

		~AngleMixedModeInterface()
		{
			Shutdown();
			delete _angleNativeInterface;
			_angleNativeInterface = NULL;
		}

		void Shutdown()
		{
			_angleNativeInterface->shutdown();
		}

		void Simulate()
		{
			_angleNativeInterface->animate();
		}

		bool IntializeAngle(IntPtr ^hwnd, int width, int height)
		{
			HWND hwndNative = (HWND) hwnd->ToPointer();
			return _angleNativeInterface->initializeAngle(hwndNative, width, height);
		}

		void RenderFrame()
		{
			_angleNativeInterface->renderFrame();
		}

		IntPtr GetBackBufferPointer()
		{
			void* hwndNative = _angleNativeInterface->getBackBufferPointer();
			IntPtr hwndManaged = IntPtr(hwndNative);
			return hwndManaged;
		}

		void ResizeRenderSurface(Size newSize)
		{
			_angleNativeInterface->resizeRenderSurface(newSize.Width, newSize.Height);
		}

		void SetWireFrameRenderMode(bool wireFrameMode)
		{
			_angleNativeInterface->setWireframeRenderMode(wireFrameMode);
		}

		bool GetWireFrameRenderMode()
		{
			return _angleNativeInterface->getWireframeRenderMode();
		}

		bool MouseMoved(Point position, Point delta, MouseEventArgs ^mouseEventArgs)
		{
			MouseEvent ev = _mapMouseEvent(position, delta, mouseEventArgs);
			return _angleNativeInterface->mouseMoved(ev);
		}

		bool MouseWheel(Point position, MouseWheelEventArgs^ mouseWheelEventArgs)
		{
			MouseEvent ev = _mapMouseEvent(position, mouseWheelEventArgs);
			return _angleNativeInterface->mouseWheel(ev);
		}

		bool MouseDown(Point position, MouseButtonEventArgs ^mouseButtonEventArgs)
		{
			MouseEvent& ev = _mapMouseEvent(position, mouseButtonEventArgs);
			return _angleNativeInterface->mouseDown(ev);
		}

		bool MouseUp(Point position, MouseButtonEventArgs ^mouseButtonEventArgs)
		{
			MouseEvent& ev = _mapMouseEvent(position, mouseButtonEventArgs);
			return _angleNativeInterface->mouseUp(ev);
		}

	private:
		AngleNativeInterface* _angleNativeInterface;

		MouseEvent _mapMouseEvent(Point position, MouseButtonEventArgs ^mouseButtonEventArgs)
		{
			MouseButtonStates& buttonStates = _mapMouseButtonStates(mouseButtonEventArgs);

			MouseEvent& me = MouseEvent(position.X, position.Y, 0.0, 0.0, buttonStates);

			return me;
		}

		MouseEvent _mapMouseEvent(Point position, MouseWheelEventArgs ^mouseWheelEventArgs)
		{
			MouseButtonStates& buttonStates = _mapMouseButtonStates(mouseWheelEventArgs);

			MouseEvent& me = MouseEvent(position.X, position.Y, mouseWheelEventArgs->Delta, buttonStates);

			return me;
		}

		MouseEvent _mapMouseEvent(Point position, Point delta, MouseEventArgs ^mouseEventArgs)
		{
			MouseButtonStates& buttonStates = _mapMouseButtonStates(mouseEventArgs);
			MouseEvent& me = MouseEvent(position.X, position.Y, delta.X, delta.Y, buttonStates);

			return me;
		}

		MouseButtonStates _mapMouseButtonStates(MouseEventArgs ^mouseEventArgs)
		{
			bool leftButtonPressed = mouseEventArgs->LeftButton == MouseButtonState::Pressed;
			bool middleButtonPressed = mouseEventArgs->MiddleButton == MouseButtonState::Pressed;
			bool rightButtonPressed = mouseEventArgs->RightButton == MouseButtonState::Pressed;
			return MouseButtonStates(leftButtonPressed, middleButtonPressed, rightButtonPressed, 0);
		}

		MouseButtonStates _mapMouseButtonStates(MouseButtonEventArgs ^mouseButtonEventArgs)
		{
			bool leftButtonPressed = mouseButtonEventArgs->LeftButton == MouseButtonState::Pressed;
			bool middleButtonPressed = mouseButtonEventArgs->MiddleButton == MouseButtonState::Pressed;
			bool rightButtonPressed = mouseButtonEventArgs->RightButton == MouseButtonState::Pressed;

			int button = 0;
			switch (mouseButtonEventArgs->ChangedButton)
			{
			case MouseButton::Left:
				button = MouseButtonStates::LeftButton;
				break;
			case MouseButton::Middle:
				button = MouseButtonStates::MiddleButton;
				break;
			case MouseButton::Right:
				button = MouseButtonStates::RightButton;
				break;
			default:
				break;
			}

			return MouseButtonStates(leftButtonPressed, middleButtonPressed, rightButtonPressed, button);
		}
	};
}
