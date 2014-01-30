#if defined(BEARING_METHOD_4)

void retrieve_mag() {

////  HMC5883L compass;
////  compass = HMC5883L(); // Construct a new HMC5883 compass.
////  delay(100);
//  compass.SetScale(1.3); // Set the scale of the compass.
//  compass.SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
// Retrieve the raw values from the compass (not scaled).
  MagnetometerRaw raw = compass.ReadRawAxis();
// Retrieved the scaled values from the compass (scaled to the configured scale).
  MagnetometerScaled scaled = compass.ReadScaledAxis();
//
// Calculate heading when the magnetometer is level, then correct for signs of axis.
float heading = atan2(scaled.YAxis, scaled.XAxis);

// Once you have your heading, you must then add your ‘Declination Angle’, which is the ‘Error’ of the magnetic field in your location.
// Find yours here: http://www.magnetic-declination.com/



float declinationAngle = MAGDEC / 1000; 
heading += declinationAngle;

// Correct for when signs are reversed.
if(heading < 0)
heading += 2*PI;

// Check for wrap due to addition of declination.
if(heading > 2*PI)
heading -= 2*PI;

// Convert radians to degrees for readability.
home_bearing = (int)round(heading * 180/M_PI);
}
#endif

