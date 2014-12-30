/* $************* KCG Version 6.4 beta3 (build i9) **************
** Command: kcg64.exe -config U:/Windows/Bureau/ProjetDRONE/AR-Ghost/KCG/config.txt
** Generation date: 2014-12-10T14:20:16
*************************************************************$ */
#ifndef _SystemDrone_H_
#define _SystemDrone_H_

#include "kcg_types.h"
#include "CheckAngle.h"

/* ========================  input structure  ====================== */
typedef struct {
    kcg_real /* SystemDrone::LineAngle */ LineAngle;
    kcg_bool /* SystemDrone::RightWay */ RightWay;
    kcg_bool /* SystemDrone::GoLeft */ GoLeft;
    kcg_bool /* SystemDrone::GoRight */ GoRight;
    kcg_real /* SystemDrone::currentAngle */ currentAngle;
    kcg_bool /* SystemDrone::ImageUpdate */ ImageUpdate;
} inC_SystemDrone;

/* ========================  context type  ========================= */
typedef struct {
    /* ---------------------------  outputs  --------------------------- */
    kcg_real /* SystemDrone::Roll */ Roll;
    kcg_real /* SystemDrone::Pitch */ Pitch;
    kcg_real /* SystemDrone::Gaz */ Gaz;
    kcg_real /* SystemDrone::Yaw */ Yaw;
    /* -----------------------  no local probes  ----------------------- */
    /* -------------------- initialization variables  ------------------ */
    kcg_bool init3;
    kcg_bool init2;
    kcg_bool init1;
    kcg_bool init;
    /* ----------------------- local memories  ------------------------- */
    kcg_int times_5;
    kcg_int times_6;
    kcg_int times_4;
    SSM_ST_DroneMovement /* SystemDrone::DroneMovement */ DroneMovement_state_nxt;
    kcg_bool /* SystemDrone::DroneMovement */ DroneMovement_reset_act;
    /* ---------------------  sub nodes' contexts  --------------------- */
    outC_CheckAngle /* 1 */ Context_1;
    /* ----------------- no clocks of observable data ------------------ */
} outC_SystemDrone;

/* ===========  node initialization and cycle functions  =========== */
/* SystemDrone */
extern void SystemDrone(inC_SystemDrone* inC, outC_SystemDrone* outC);

extern void SystemDrone_reset(outC_SystemDrone* outC);
extern void SystemDrone_init(outC_SystemDrone* outC);
#endif /* _SystemDrone_H_ */
/* $************* KCG Version 6.4 beta3 (build i9) **************
** SystemDrone.h
** Generation date: 2014-12-10T14:20:16
*************************************************************$ */
