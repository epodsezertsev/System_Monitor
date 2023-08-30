#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) {
  int HH, MM, SS;
  string output, strHH, strMM, strSS;
  HH = seconds/3600;
  MM = (seconds - (HH * 3600))/60;
  SS = seconds % 60;
  strHH = std::to_string(HH);
  if (MM < 10) {
    strMM = "0" + std::to_string(MM);
  } else {
    strMM = std::to_string(MM);
  }
  if (SS < 10) {
    strSS = "0" + std::to_string(SS);
  } else {
    strSS = std::to_string(SS);
  }
  output = strHH + ":" + strMM + ":" + strSS;
  return output;
}