void readIMU()
{
  int readFlag=0;
  while(readFlag==0)
  {
  if (MPU.read()) {                                        // get the latest data if ready yet
    roll=MPU.m_dmpEulerPose[0]*180/PI;
    pitch=MPU.m_dmpEulerPose[1]*180/PI;
    yaw=MPU.m_dmpEulerPose[2]*180/PI;
    smoothYaw = smoothYaw - (LPFYaw_Beta * (smoothYaw - yaw));   //low pass filter implementation
    smoothPitch = smoothPitch - (LPFPitch_Beta * (smoothPitch - pitch));
    Serial.println("Pitch,\t\tYaw,\t\tRoll");
    Serial.print(smoothPitch); //Serial.print(",");
    Serial.print(",\t\t");
    Serial.print(smoothYaw); //Serial.println(",");
    Serial.print(",\t\t");
    Serial.print(roll); //Serial.println(",");
    Serial.println();
    readFlag=1;
  }
  }
}
