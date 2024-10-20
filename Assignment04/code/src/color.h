/******************************************************************************
 *                                                                            *
 *  Copyright (c) 2023 Ojaswa Sharma. All rights reserved.                    *
 *                                                                            *
 *  Author: Ojaswa Sharma                                                     *
 *  E-mail: ojaswa@iiitd.ac.in                                                *
 *                                                                            *
 *  This code is provided solely for the purpose of the CSE 333/533 course    *
 *  at IIIT Delhi. Unauthorized reproduction, distribution, or disclosure     *
 *  of this code, in whole or in part, without the prior written consent of   *
 *  the author is strictly prohibited.                                        *
 *                                                                            *
 *  This code is provided "as is", without warranty of any kind, express      *
 *  or implied, including but not limited to the warranties of                *
 *  merchantability, fitness for a particular purpose, and noninfringement.   *
 *                                                                            *
 ******************************************************************************/

//color.h
#ifndef _COLOR_H_
#define _COLOR_H_
class Color
{
public:
    Color(double val) {r = g = b = val;}
    Color(double red, double green, double blue);
    Color(const Color & c) {*this = c;}
    
    void R(double red)  {r = red;}
    void G(double green) {g = green;}
    void B(double blue)  {b = blue;}
    
    double R() const {return r;}
    double G() const {return g;}
    double B() const {return b;}
    
    void clamp();
    //define some operators for this class:
    Color& operator=(const Color& rhs);
    friend Color operator * (const Color& c, double f);
    friend Color operator * (float f, const Color& c);
    friend Color operator * (const Color& c1, const Color& c2);
    friend Color operator / (const Color& c, float f);
    friend Color operator + (const Color& c1, const Color& c2);
    //private:
    //Data memebrs are not private because of performance hits. 
    //Its better to access directly in critical cases than to use functions!    
    double r;
    double g;
    double b;
    
};
#endif
