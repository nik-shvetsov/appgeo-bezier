#include "mycurve.h"

MyCurve::MyCurve( float scalefactor )
{
    this->_dm = GMlib::GM_DERIVATION_EXPLICIT;
    _scf = scalefactor;
}

MyCurve::~MyCurve() {}

void MyCurve::eval( float t, int d, bool /*l*/ ) {

    this->_p.setDim( d + 1 );

    //const float ct = _r * cos(t);
    //const float st = _r * sin(t);

    const float ct = _scf * (3 * cos(-t) + cos(5 * t));
    const float st = _scf * (3 * sin(-t) + sin(5 * t));

    const float ct_d = _scf * (-3 * sin(t) - 5 * sin(5 * t));
    const float st_d = _scf * (-3 * cos(t) + 5 * cos(5 * t) );

    const float ct_dd = _scf * (-3 * cos(t) - 25 * cos(5 * t));
    const float st_dd = _scf * (3 * sin(t) - 25 * sin(5 * t));

    const float ct_ddd = _scf * (3 * sin(t) + 125 * sin(5 * t));
    const float st_ddd = _scf * (3 * cos(t) - 125 * cos(5 * t));

    const float ct_dddd = _scf * (3 * cos(t) + 625 * cos(5 * t));
    const float st_dddd = _scf * (-3 * sin(t) + 625 * sin(5 * t));

    this->_p[0][0] = ct;
    this->_p[0][1] = st;
    this->_p[0][2] = float(0);

    if( this->_dm == GMlib::GM_DERIVATION_EXPLICIT ) {

        if( d > 0 ) {

            this->_p[1][0] = ct_d;
            this->_p[1][1] = st_d;
            this->_p[1][2] =  float(0);
        }

        if( d > 1 ) {

            this->_p[2][0] = ct_dd;
            this->_p[2][1] = st_dd;
            this->_p[2][2] =  float(0);
        }

        if( d > 2 ) {

            this->_p[3][0] = ct_ddd;
            this->_p[3][1] = st_ddd;
            this->_p[3][2] =  float(0);
        }

        if( d > 3 ) {

            this->_p[4][0] =  ct_dddd;
            this->_p[4][1] =  st_dddd;
            this->_p[4][2] =  float(0);
        }

//        if( d > 4 ) {

//            this->_p[5][0] = -st;
//            this->_p[5][1] =  ct;
//            this->_p[5][2] =  float(0);
//        }

//        if( d > 5 ) {

//            this->_p[6][0] = -ct;
//            this->_p[6][1] = -st;
//            this->_p[6][2] =  float(0);
//        }

//        if( d > 6 ) {

//            this->_p[7][0] =  st;
//            this->_p[7][1] = -ct;
//            this->_p[7][2] =  float(0);
//        }
    }
}


float MyCurve::getEndP() {

    return float(M_2PI);
}

float MyCurve::getStartP() {

    return float(0);
}

float MyCurve::getScaleFactor() {

    return _scf;
}

bool MyCurve::isClosed() const {

    return true;
}

void MyCurve::setScaleFactor( float scalefactor ) {

    _scf = scalefactor;
}
