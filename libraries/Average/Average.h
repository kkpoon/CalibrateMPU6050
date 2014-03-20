/*************************************************************
 * Useful averaging routines.
 * (c) 2012 Majenki Technologies
 * http://www.majenko.co.uk
 *************************************************************/

/* Note - this is currently a work in progress.  There are many
   more routines to be added. */

#ifndef _AVERAGE_H
#define _AVERAGE_H

/******
 * Rolling average.
 *
 * These routines all take three arguements, and are overloaded for the
 * different data types.
 * 
 * The first arguement is an array of the required data type to store
 * the rolling average data in.  The second arguement is the number of
 * slices in this array.  The third arguement is the new reading to
 * add to the array.  The current rolling average is returned.
 ******/

extern int rollingAverage(int *, int, int);
extern char rollingAverage(char *, int, char);
extern long rollingAverage(long *, int, long);
extern float rollingAverage(float *, int, float);
extern unsigned int rollingAverage(unsigned int *, int, unsigned int);
extern unsigned long rollingAverage(unsigned long *, int, unsigned long);
extern unsigned char rollingAverage(unsigned char *, int, unsigned char);

/******
 * Mean routines.
 * Pass an array, and the number of slices in the array, and it returns
 * the mean average.
 ******/

extern int mean(int *, int);
extern unsigned int mean(unsigned int *, int);
extern char mean(char *, int);
extern unsigned char mean(unsigned char *, int);
extern long mean(long *, int);
extern unsigned long mean(unsigned long *, int);
extern float mean(float *, int);

/******
 * Mode.
 * Pass it an array, and the number of slices in the array, and it returns
 * the value of the most common number in that array.
 ******/

extern int mode(int *, int);
extern unsigned int mode(unsigned int *, int);
extern char mode(char *, int);
extern unsigned char mode(unsigned char *, int);
extern long mode(long *, int);
extern unsigned long mode(unsigned long *, int);
extern float mode(float *, int);

/******
 * Minimum.
 * Pass an array and slice count, and it returns the minimum value from the
 * array.
 ******/

extern int minimum(int *, int);
extern unsigned int minimum(unsigned int *, int);
extern char minimum(char *, int);
extern unsigned char minimum(unsigned char *, int);
extern long minimum(long *, int);
extern unsigned long minimum(unsigned long *, int);
extern float minimum(float *, int);

/******
 * Maximum.
 * Pass an array and slice count, and it returns the maximum value from the
 * array.
 ******/

extern int maximum(int *, int);
extern unsigned int maximum(unsigned int *, int);
extern char maximum(char *, int);
extern unsigned char maximum(unsigned char *, int);
extern long maximum(long *, int);
extern unsigned long maximum(unsigned long *, int);
extern float maximum(float *, int);

/******
 * Standard Deviation
 * This is the tricky one.  Pass it, again, an array and a slice count, and
 * it will return the standard deviation of the contents of the array.  This
 * is the square root of the mean of the squares of the difference of the
 * values to the array mean.  Got it?
 ******/

float stddev(int *, int);
float stddev(unsigned int *, int);
float stddev(char *, int);
float stddev(unsigned char *, int);
float stddev(long *, int);
float stddev(unsigned long *, int);
float stddev(float *, int);

#endif
