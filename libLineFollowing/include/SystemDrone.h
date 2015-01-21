/* $************* KCG Version 6.4 beta3 (build i9) **************
** Command: kcg64.exe -config U:/Windows/Bureau/Projet DRONE/v2/KCG/config.txt
** Generation date: 2014-12-17T14:43:37
*************************************************************$ */
#ifndef _SystemDrone_H_
#define _SystemDrone_H_

#include "kcg_types.h"
#include "SystemFlying.h"
#include "SystemEmergency.h"

/* ========================  input structure  ====================== */
typedef struct {
    kcg_real /* SystemDrone::LineAngle */ LineAngle;
    kcg_bool /* SystemDrone::RightWay */ RightWay;
    kcg_bool /* SystemDrone::GoLeft */ GoLeft;
    kcg_bool /* SystemDrone::GoRight */ GoRight;
    kcg_real /* SystemDrone::currentAngle */ currentAngle;
    kcg_bool /* SystemDrone::ImageUpdate */ ImageUpdate;
    kcg_bool /* SystemDrone::LineDetected */ LineDetected;
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
    kcg_bool init;
    /* ----------------------- local memories  ------------------------- */
    SSM_ST_Drone /* SystemDrone::Drone */ Drone_state_nxt;
    /* ---------------------  sub nodes' contexts  --------------------- */
    outC_SystemEmergency /* 1 */ Context_1;
    outC_SystemFlying /* 5 */ Context_5;
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
** Generation date: 2014-12-17T14:43:37
*************************************************************$ */
