// (C) Sebastian Fiault
#ifndef MATH_UTILS_H
#define MATH_UTILS_H


float mapValueExponentialFloat32(float factor, float value, float maxValue);
double mapValueExponentialDouble64(double factor, double value, double maxValue);


mapValueExponential(factor, value) _Generic((factor)
	float: _Generic((value)
		float: mapValueExponentialFloat32
		default: //TODO
	),
	double: _Generic((value)
		double : mapValueExponentialDouble64
		default: //TODO
	) // TODO ask on SO how to make it print clean error messages
) (factor, value)

#endif
