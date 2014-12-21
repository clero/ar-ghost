/* $************* KCG Version 6.4 beta3 (build i9) **************
** Command: kcg64.exe -config U:/Windows/Bureau/Projet DRONE/v2/KCG/config.txt
** Generation date: 2014-12-17T14:43:37
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "CheckAngle.h"

void CheckAngle_init(outC_CheckAngle* outC)
{
    outC->init = kcg_true;
    outC->angleCible = 0.0;
    outC->YawSpeed = 0.0;
    outC->AngleNotOk = kcg_true;
    outC->AngleOk = kcg_true;
}


void CheckAngle_reset(outC_CheckAngle* outC)
{
    outC->init = kcg_true;
}

/* CheckAngle */
void CheckAngle(
    /* CheckAngle::currentAngle */ kcg_real currentAngle,
    /* CheckAngle::LineAngle */ kcg_real LineAngle,
    /* CheckAngle::ImageUpdate */ kcg_bool ImageUpdate,
    outC_CheckAngle* outC)
{
    /* CheckAngle::_L10 */
    kcg_real _L10;
    /* CheckAngle::_L16 */ kcg_real _L16;

    _L16 = LineAngle + currentAngle;
    if (ImageUpdate) {
        if (LineAngle > 5.0) {
            outC->angleCible = _L16;
        } else if (LineAngle < -5.0) {
            outC->angleCible = _L16;
        } else {
            outC->angleCible = currentAngle;
        }
    } else if (outC->init) {
        outC->angleCible = 0.0;
    }
    _L16 = outC->angleCible - currentAngle;
    if (0. <= _L16) {
        _L10 = _L16;
    } else {
        _L10 = -_L16;
    }
    if (_L10 >= 10.0) {
        outC->AngleNotOk = kcg_true;
    } else {
        outC->AngleNotOk = kcg_false;
    }
    if (_L10 <= 5.0) {
        outC->AngleOk = kcg_true;
    } else {
        outC->AngleOk = kcg_false;
    }
    if (LineAngle > 10.0) {
        outC->YawSpeed = -0.2;
    } else if (LineAngle < -10.0) {
        outC->YawSpeed = 0.2;
    } else {
        outC->YawSpeed = 0.0;
    }
    outC->init = kcg_false;
}

/* $************* KCG Version 6.4 beta3 (build i9) **************
** CheckAngle.c
** Generation date: 2014-12-17T14:43:37
*************************************************************$ */
