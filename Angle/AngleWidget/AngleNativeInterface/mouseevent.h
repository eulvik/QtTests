#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H

struct MouseButtonStates
{
public:

    enum MouseButton
    {
        LeftButton   = 0x0001,
        RightButton  = 0x0002,
        MiddleButton = 0x0004,
    };

private:

    unsigned int _button_state;
    const unsigned int _button;


public:
    MouseButtonStates(bool leftButtonPressed, bool middleButtonPressed, bool rightButtonPressed, int button)
        : _button_state(0x0)
        , _button(button)
    {
        if(leftButtonPressed)   _button_state |= LeftButton;
        if(middleButtonPressed) _button_state |= MiddleButton;
        if(rightButtonPressed)  _button_state |= RightButton;
    }

    MouseButtonStates()
        : _button_state(0x0)
        , _button(0x0)
    {}

    bool isLeftButtonPressed()   const { return (bool)(_button_state & LeftButton);   }
    bool isMiddleButtonPressed() const { return (bool)(_button_state & MiddleButton); }
    bool isRightButtonPressed()  const { return (bool)(_button_state & RightButton);  }

    /// The mouse button that caused the event.
    int getButton() const { return _button; }

};

struct MouseEvent
{
public:
    MouseEvent(double x, double y, double deltaX, double deltaY, MouseButtonStates& buttonStates)
        : _x(x), _y(y), _deltaX(deltaX), _deltaY(deltaY), _deltaZ(0.0), _buttonStates(buttonStates)
    {}

    MouseEvent(double x, double y, double deltaZ, MouseButtonStates& buttonStates)
        : _x(x), _y(y), _deltaX(0.0), _deltaY(0.0), _deltaZ(deltaZ), _buttonStates(buttonStates)
    {}

    double getX() const { return _x; }
    double getDeltaX() const { return _deltaX; }

    double getY() const { return _y; }
    double getDeltaY() const { return _deltaY; }

    double getDeltaZ() const { return _deltaZ; }

    MouseButtonStates getButtonStates() const { return _buttonStates; }

private:
    const double _x;
    const double _y;
    const double _deltaX;
    const double _deltaY;
    const double _deltaZ;

    const MouseButtonStates _buttonStates;
};

#endif // MOUSEEVENT_H
