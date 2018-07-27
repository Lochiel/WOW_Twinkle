Proto-Repository for Star Twinkler for WOW! Children's Museum. This message will be removed when documentation is complete

This will contain two versions; one using a single Sparkfun RedBoard and one a TLC5940 to drive the PWM.

RedBoard version was originally installed and later replaced with the TLC54940. As the RedBoard version was the first it has slightly older code. As this documents how things were installed, and not their ideals, this older code will be documented for the RedBoard version.

The Exhibit

In the Forest of Light is a hollowed out tree. Inside that tree, the ceiling has a number of stars. The stars are the ends of fiber optics illuminated by LEDs. Each LED controls a number of stars. While there are few constellations that will be stable, the majority of the stars need to twinkle.

This controller uses PWM pins to create the twinkle. With the RedBoard version, the LEDs are driven through transistors controlled by the PWM pins on the RedBoard. With the TLC5940 version, the LEDs are controlled by the TLC5940, a Texas Instruments IC that controls 16 PWM constant current sink pins.