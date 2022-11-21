#pragma once
#include"Vector3.h"

Vector3 easeOutSineVec3(Vector3 startpos, Vector3 endpos,float time);

Vector3 easeOutQuintVec3(Vector3 start, Vector3 end, double time);


double lerp(double a,double b,double t);

double easeInSine(double start, double end, double time);

double easeOutSine(double start, double end, double time);

double easeInOutSine(double start, double end, double time);

double easeOutQuad(double start, double end, double time);

double easeInQuint(double start, double end, double time);

double easeOutQuint(double start, double end, double time);

double easeInOutQuint(double start, double end, double time);

double easeInBack(double start, double end, double time);

double easeOutBack(double start, double end, double time);

double easeOutElastic(double start, double end,double time);