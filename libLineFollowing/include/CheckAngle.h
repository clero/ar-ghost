/* $************* KCG Version 6.4 beta3 (build i9) **************
** Command: kcg64.exe -config U:/Windows/Bureau/ProjetDRONE/AR-Ghost/KCG/config.txt
** Generation date: 2014-12-10T14:20:16
*************************************************************$ */
#ifndef _CheckAngle_H_
#define _CheckAngle_H_

#include "kcg_types.h"

/* =====================  no input structure  ====================== */

/* ========================  context type  ========================= */
typedef struct {
    /* ---------------------------  outputs  --------------------------- */
    kcg_bool /* CheckAngle::AngleOk */ AngleOk;
    kcg_bool /* CheckAngle::AngleNotOk */ AngleNotOk;
    kcg_real /* CheckAngle::YawSpeed */ YawSpeed;
    /* -----------------------  no local probes  ----------------------- */
    /* -------------------- initialization variables  ------------------ */
    kcg_bool init;
    /* ----------------------- local memories  ------------------------- */
    kcg_real /* CheckAngle::angleCible */ angleCible;
    /* -------------------- no sub nodes' contexts  -------------------- */
    /* ----------------- no clocks of observable data ------------------ */
} outC_CheckAngle;

/* ===========  node initialization and cycle functions  =========== */
/* CheckAngle */
extern void CheckAngle(
    /* CheckAngle::currentAngle */ kcg_real currentAngle,
    /* CheckAngle::LineAngle */ kcg_real LineAngle,
    /* CheckAngle::ImageUpdate */ kcg_bool ImageUpdate,
    outC_CheckAngle* outC);

extern void CheckAngle_reset(outC_CheckAngle* outC);
extern void CheckAngle_init(outC_CheckAngle* outC);
#endif /* _CheckAngle_H_ */
/* $************* KCG Version 6.4 beta3 (build i9) **************
** CheckAngle.h
** Generation date: 2014-12-10T14:20:16
*************************************************************$ */
