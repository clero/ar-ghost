/* $************* KCG Version 6.4 beta3 (build i9) **************
** Command: kcg64.exe -config U:/Windows/Bureau/Projet DRONE/v2/KCG/config.txt
** Generation date: 2014-12-17T14:43:37
*************************************************************$ */
#ifndef _SystemFlying_H_
#define _SystemFlying_H_

#include "kcg_types.h"
#include "CheckAngle.h"

/* =====================  no input structure  ====================== */

/* ========================  context type  ========================= */
typedef struct {
    /* ---------------------------  outputs  --------------------------- */
    kcg_real /* SystemFlying::Roll */ Roll;
    kcg_real /* SystemFlying::Pitch */ Pitch;
    kcg_real /* SystemFlying::Gaz */ Gaz;
    kcg_real /* SystemFlying::Yaw */ Yaw;
    /* -----------------------  no local probes  ----------------------- */
    /* -------------------- initialization variables  ------------------ */
    kcg_bool init2;
    kcg_bool init1;
    kcg_bool init;
    /* ----------------------- local memories  ------------------------- */
    kcg_int times_7;
    kcg_int times_8;
    SSM_ST_FlyingMode /* SystemFlying::FlyingMode */ FlyingMode_state_nxt;
    kcg_bool /* SystemFlying::FlyingMode */ FlyingMode_reset_act;
    /* ---------------------  sub nodes' contexts  --------------------- */
    outC_CheckAngle /* 1 */ Context_1;
    /* ----------------- no clocks of observable data ------------------ */
} outC_SystemFlying;

/* ===========  node initialization and cycle functions  =========== */
/* SystemFlying */
extern void SystemFlying(
    /* SystemFlying::LineAngle */ kcg_real LineAngle,
    /* SystemFlying::RightWay */ kcg_bool RightWay,
    /* SystemFlying::GoLeft */ kcg_bool GoLeft,
    /* SystemFlying::GoRight */ kcg_bool GoRight,
    /* SystemFlying::currentAngle */ kcg_real currentAngle,
    /* SystemFlying::ImageUpdate */ kcg_bool ImageUpdate,
    outC_SystemFlying* outC);

extern void SystemFlying_reset(outC_SystemFlying* outC);
extern void SystemFlying_init(outC_SystemFlying* outC);
#endif /* _SystemFlying_H_ */
/* $************* KCG Version 6.4 beta3 (build i9) **************
** SystemFlying.h
** Generation date: 2014-12-17T14:43:37
*************************************************************$ */
