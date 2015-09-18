// AngleInteractionMixedMode.h

#pragma once

#include <rendercontrolnativeinterface.h>
#include <msclr/marshal_cppstd.h>

using namespace System;
using System::Windows::Point;
using System::Windows::Size;

using namespace System::Windows::Input;

namespace AngleInteractionMixedMode 
{

	public ref class AngleInterface
	{
	public:
		AngleInterface(void)
		{
			_angleQmlNativeInterface = new RenderControlNativeInterface();
		}

		~AngleInterface()
		{
			Shutdown();
			delete _angleQmlNativeInterface;
			_angleQmlNativeInterface = NULL;
		}

		void Shutdown()
		{
			_angleQmlNativeInterface->shutdown();
		}

		void Simulate()
		{
			_angleQmlNativeInterface->animate();
		}

		bool IntializeAngle(IntPtr ^hwnd, int width, int height)
		{
			HWND hwndNative = (HWND) hwnd->ToPointer();
			return _angleQmlNativeInterface->initialize(hwndNative, width, height);
		}

		void RenderFrame()
		{
			_angleQmlNativeInterface->renderFrame();
		}

		IntPtr GetBackBufferPointer()
		{
			void* hwndNative = _angleQmlNativeInterface->getBackBufferPointer();
			IntPtr hwndManaged = IntPtr(hwndNative);
			return hwndManaged;
		}

		void ResizeRenderSurface(Size newSize)
		{
			_angleQmlNativeInterface->resizeSurface(newSize.Width, newSize.Height);
		}

		bool MouseMoved(Point position, Point delta, MouseEventArgs ^mouseEventArgs)
		{
			//MouseEvent ev = _mapMouseEvent(position, delta, mouseEventArgs);
			//return _angleQmlNativeInterface->mouseMoved(ev);
			return true;
		}

		bool MouseWheel(Point position, MouseWheelEventArgs^ mouseWheelEventArgs)
		{
			//MouseEvent ev = _mapMouseEvent(position, mouseWheelEventArgs);
			//return _angleQmlNativeInterface->mouseWheel(ev);
			return true;
		}

		bool MouseDown(Point position, MouseButtonEventArgs ^mouseButtonEventArgs)
		{
			//MouseEvent& ev = _mapMouseEvent(position, mouseButtonEventArgs);
			//return _angleQmlNativeInterface->mouseDown(ev);
			return true;
		}

		bool MouseUp(Point position, MouseButtonEventArgs ^mouseButtonEventArgs)
		{
			/*MouseEvent& ev = _mapMouseEvent(position, mouseButtonEventArgs);
			return _angleQmlNativeInterface->mouseUp(ev);*/
			return true;
		}

	private:
		RenderControlNativeInterface* _angleQmlNativeInterface;

		/*MouseEvent _mapMouseEvent(Point position, MouseButtonEventArgs ^mouseButtonEventArgs)
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
		}*/
	};
}
