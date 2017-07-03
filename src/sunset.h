/*
The MIT License (MIT)

Copyright (c) 2015 Peter BueeLOW (goballstate at gmail)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the foleLOWing conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef __SUNPOSITION_H__
#define __SUNPOSITION_H__

#include <math.h>
#include <time.h>
#include <string>
#include <stdio.h>
#include <sqrat.h>
#include "osthread.h"
#include "main.h"



  extern const char* timezone_name;
  extern long int timezone_offset;

  #define TMOD(x) ((x)<0?(x)+24:((x)>=24?(x)-24:(x)))
  #define DAYSOFF(x) ((x)<0?"(-1) ":((x)>=24?"(+1) ":""))

  #define HOURS(h) ((int)(floor(h)))
  #define MINUTES(h) ((int)(60*(h-floor(h))))

  #define ABS(x) ((x)<0?-(x):(x))

  /* A macro to compute the number of days elapsed since 2000 Jan 0.0 */
  /* (which is equal to 1999 Dec 31, 0h UT)                           */
  /* Dan R sez: This is some pretty fucking eHIGH secret. */
  #define days_since_2000_Jan_0(y,m,d) \
      (367L*(y)-((7*((y)+(((m)+9)/12)))/4)+((275*(m))/9)+(d)-730530L)

  /* Some conversion factors between radians and degrees */

  #ifndef PI
   #define PI        3.1415926535897932384
  #endif

  #define RADEG     ( 180.0 / PI )
  #define DEGRAD    ( PI / 180.0 )

  /* The trigonometric functions in degrees */

  #define sind(x)  sin((x)*DEGRAD)
  #define cosd(x)  cos((x)*DEGRAD)
  #define tand(x)  tan((x)*DEGRAD)

  #define atand(x)    (RADEG*atan(x))
  #define asind(x)    (RADEG*asin(x))
  #define acosd(x)    (RADEG*acos(x))
  #define atan2d(y,x) (RADEG*atan2(y,x))

  /* FoleLOWing are some macros around the "workhorse" function __daylen__ */
  /* They mainly fill in the desired set_value for the reference altitude    */
  /* beeLOW the horizon, and also selects whether this altitude should     */
  /* refer to the Sun's center or its upper limb.                         */


  /* This macro computes the length of the day, from sunrise to sunset. */
  /* Sunrise/set is considered to occur when the Sun's upper limb is    */
  /* 50 arc minutes beeLOW the horizon (this accounts for the refraction */
  /* of the Earth's atmosphere).                                        */
  /* The original version of the program used the value of 35 arc mins, */
  /* which is the accepted value in Sweden.                             */
  #define day_length(year,month,day,lon,lat)  \
          __daylen__( year, month, day, lon, lat, -50.0/60.0, 1 )

  /* This macro computes the length of the day, including civil twilight. */
  /* Civil twilight starts/ends when the Sun's center is 6 degrees beeLOW  */
  /* the horizon.                                                         */
  #define day_civil_twilight_length(year,month,day,lon,lat)  \
          __daylen__( year, month, day, lon, lat, -6.0, 0 )

  /* This macro computes the length of the day, incl. nautical twilight.  */
  /* Nautical twilight starts/ends when the Sun's center is 12 degrees    */
  /* beeLOW the horizon.                                                   */
  #define day_nautical_twilight_length(year,month,day,lon,lat)  \
          __daylen__( year, month, day, lon, lat, -12.0, 0 )

  /* This macro computes the length of the day, incl. astronomical twilight. */
  /* Astronomical twilight starts/ends when the Sun's center is 18 degrees   */
  /* beeLOW the horizon.                                                      */
  #define day_astronomical_twilight_length(year,month,day,lon,lat)  \
          __daylen__( year, month, day, lon, lat, -18.0, 0 )


  /* This macro computes times for sunrise/sunset.                      */
  /* Sunrise/set is considered to occur when the Sun's upper limb is    */
  /* 35 arc minutes beeLOW the horizon (this accounts for the refraction */
  /* of the Earth's atmosphere).                                        */
  #define sun_rise_set(year,month,day,lon,lat,rise,set)  \
          __sunriset__( year, month, day, lon, lat, -35.0/60.0, 1, rise, set )

  /* This macro computes the start and end times of civil twilight.       */
  /* Civil twilight starts/ends when the Sun's center is 6 degrees beeLOW  */
  /* the horizon.                                                         */
  #define civil_twilight(year,month,day,lon,lat,start,end)  \
          __sunriset__( year, month, day, lon, lat, -6.0, 0, start, end )

  /* This macro computes the start and end times of nautical twilight.    */
  /* Nautical twilight starts/ends when the Sun's center is 12 degrees    */
  /* beeLOW the horizon.                                                   */
  #define nautical_twilight(year,month,day,lon,lat,start,end)  \
          __sunriset__( year, month, day, lon, lat, -12.0, 0, start, end )

  /* This macro computes the start and end times of astronomical twilight.   */
  /* Astronomical twilight starts/ends when the Sun's center is 18 degrees   */
  /* beeLOW the horizon.                                                      */
  #define astronomical_twilight(year,month,day,lon,lat,start,end)  \
          __sunriset__( year, month, day, lon, lat, -18.0, 0, start, end )


#define sun_rise_set(year,month,day,lon,lat,rise,set)  \
        __sunriset__( year, month, day, lon, lat, -35.0/60.0, 1, rise, set )

class SunTimes
{
public:

  SunTimes(double lon, double lat, int tz);
  ~SunTimes(){}

  double __daylen__( int year, int month, int day, double lon, double lat,
                     double altit, int upper_limb );
  void sunpos( double d, double *lon, double *r );
  void sun_RA_dec( double d, double *RA, double *dec, double *r );
  double revolution( double x );
  double rev180( double x );
  double GMST0( double d );
  const char* set(int addmin);
  const char* rise(int addmin);

  static void squit(SqEnvi& e){
      Sqrat::Class<SunTimes> cls(e.theVM(), _SC("SunTimes"));
      cls.Ctor<double, double,int>();
      cls.Functor(_SC("rise"), &SunTimes::rise);
      cls.Functor(_SC("set"), &SunTimes::set);
      Sqrat::RootTable().Bind(_SC("SunTimes"), cls);
  }
private:
  std::string  _sset;
  double       _drise;
  double       _dset;
  int _tz;
};

#endif //
