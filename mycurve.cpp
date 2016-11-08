#include "mycurve.h"

MyCurve::MyCurve( float radius )
{
    _r = radius;
    this->_dm = GM_DERIVATION_EXPLICIT;
}

MyCurve::MyCurve( const MyCurve& copy ) : PCurve<float,3>(copy) {}

MyCurve::~MyCurve() {}


void MyCurve::eval( float t, int d, bool /*l*/ ) {

    this->_p.setDim( d + 1 );

    //const float ct = _r * cos(t);
    //const float st = _r * sin(t);

    const float ct = _r * (3 * cos(-t) + cos(5 * t));
    const float st = _r * (3 * sin(-t) + sin(5 * t));

    this->_p[0][0] = ct;
    this->_p[0][1] = st;
    this->_p[0][2] = float(0);

    if( this->_dm == GM_DERIVATION_EXPLICIT ) {

        if( d > 0 ) {

            this->_p[1][0] = -st;
            this->_p[1][1] =  ct;
            this->_p[1][2] =  float(0);
        }

        if( d > 1 ) {

            this->_p[2][0] = -ct;
            this->_p[2][1] = -st;
            this->_p[2][2] =  float(0);
        }

        if( d > 2 ) {

            this->_p[3][0] =  st;
            this->_p[3][1] = -ct;
            this->_p[3][2] =  float(0);
        }

        if( d > 3 ) {

            this->_p[4][0] =  ct;
            this->_p[4][1] =  st;
            this->_p[4][2] =  float(0);
        }

        if( d > 4 ) {

            this->_p[5][0] = -st;
            this->_p[5][1] =  ct;
            this->_p[5][2] =  float(0);
        }

        if( d > 5 ) {

            this->_p[6][0] = -ct;
            this->_p[6][1] = -st;
            this->_p[6][2] =  float(0);
        }

        if( d > 6 ) {

            this->_p[7][0] =  st;
            this->_p[7][1] = -ct;
            this->_p[7][2] =  float(0);
        }
    }
}


float MyCurve::getEndP() {

    return float( M_2PI );
}

float MyCurve::getStartP() {

    return float(0);
}

float MyCurve::getRadius() {

    return _r;
}

bool MyCurve::isClosed() const {

    return true;
}

void MyCurve::setRadius( float radius ) {

    _r = radius;
}
