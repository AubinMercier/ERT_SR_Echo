#ifndef Data_h
#define Data_h
#define NBDATA 9
union Data {
  uint16_t altitude;
  double orientation [3];
  double acceleration [3];
  int8_t temperature;
  uint8_t battery_level;
  };

#endif
