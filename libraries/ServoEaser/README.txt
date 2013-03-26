
ServoEaser library for Arduino
==============================
2011, TeamPneumo, Tod E. Kurt, http://todbot.com/blog/


ServoEaser is an Arduino library makes doing animation-style movements with
servos easier.

Features:
- Supports directly telling a servo to ease to a position over a duration
- Supports playing of previously-defined "move lists"
- User-specified easing functions (defaults to easeInOutCubic), see Easing lib
- User-specified "servo arrived" function for async callback when movement done
- Supports both integer angle movement and a "microseconds" mode for fine-grained control with float-ing point angles
- Works with both new Arduino 1.0 and Arduino 0022.


