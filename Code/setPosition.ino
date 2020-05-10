  void setPosition()
  {
  int i;
  pitchOut=50;
  yawOut=90;
  pitchServo.write( pitchOut);
  delay(15);
  yawServo.write(yawOut);
  delay(15);
  while(digitalRead(button)==HIGH);
  for (i=0;i<25; i++)
      readIMU();
  pitchSetPoint=smoothPitch;
  yawSetPoint=smoothYaw;
   Serial.println("////////////////////////////////////////////////////////");
   Serial.println("PitchSetpoint,\t\tYawSetpoint");
   Serial.print(pitchSetPoint); //Serial.print(",");
   Serial.print("\t\t");
   Serial.println(yawSetPoint); //Serial.print(",");
   }
