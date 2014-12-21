/* $************* KCG Version 6.4 beta3 (build i9) **************
** Command: kcg64.exe -config U:/Windows/Bureau/Projet DRONE/v2/KCG/config.txt
** Generation date: 2014-12-17T14:43:37
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "SystemDrone.h"

void SystemDrone_init(outC_SystemDrone* outC)
{
    outC->init = kcg_true;
    outC->Drone_state_nxt = SSM_st_FLYING_Drone;
    outC->Yaw = 0.0;
    outC->Gaz = 0.0;
    outC->Pitch = 0.0;
    outC->Roll = 0.0;
    SystemFlying_init(&outC->Context_5);
    SystemEmergency_init(&outC->Context_1);
}


void SystemDrone_reset(outC_SystemDrone* outC)
{
    outC->init = kcg_true;
    /* 5 */ SystemFlying_reset(&outC->Context_5);
    /* 1 */ SystemEmergency_reset(&outC->Context_1);
}

/* SystemDrone */
void SystemDrone(inC_SystemDrone* inC, outC_SystemDrone* outC)
{
    /* SystemDrone::Drone */
    SSM_ST_Drone Drone_state_sel;
    /* SystemDrone::Drone */ SSM_ST_Drone Drone_state_act;
    /* SystemDrone::Drone */ kcg_bool Drone_reset_act;

    if (outC->init) {
        Drone_state_sel = SSM_st_WAITING_Drone;
    } else {
        Drone_state_sel = outC->Drone_state_nxt;
    }
    switch (Drone_state_sel) {
    case SSM_st_FLYING_Drone:
        Drone_reset_act = !inC->LineDetected;
        if (Drone_reset_act) {
            Drone_state_act = SSM_st_EMERGENCY_Drone;
        } else {
            Drone_state_act = SSM_st_FLYING_Drone;
        }
        break;
    case SSM_st_EMERGENCY_Drone:
        if (inC->LineDetected) {
            Drone_state_act = SSM_st_FLYING_Drone;
        } else {
            Drone_state_act = SSM_st_EMERGENCY_Drone;
        }
        Drone_reset_act = inC->LineDetected;
        break;
    case SSM_st_WAITING_Drone:
        if (inC->LineDetected) {
            Drone_state_act = SSM_st_FLYING_Drone;
        } else {
            Drone_state_act = SSM_st_WAITING_Drone;
        }
        Drone_reset_act = inC->LineDetected;
        break;

    }
    switch (Drone_state_act) {
    case SSM_st_FLYING_Drone:
        if (Drone_reset_act) {
            /* 5 */
            SystemFlying_reset(&outC->Context_5);
        }
        /* 5 */
        SystemFlying(
            inC->LineAngle,
            inC->RightWay,
            inC->GoLeft,
            inC->GoRight,
            inC->currentAngle,
            inC->ImageUpdate,
            &outC->Context_5);
        outC->Drone_state_nxt = SSM_st_FLYING_Drone;
        outC->Yaw = outC->Context_5.Yaw;
        outC->Gaz = outC->Context_5.Gaz;
        outC->Pitch = outC->Context_5.Pitch;
        outC->Roll = outC->Context_5.Roll;
        break;
    case SSM_st_EMERGENCY_Drone:
        if (Drone_reset_act) {
            /* 1 */
            SystemEmergency_reset(&outC->Context_1);
        }
        /* 1 */ SystemEmergency(&outC->Context_1);
        outC->Drone_state_nxt = SSM_st_EMERGENCY_Drone;
        outC->Yaw = outC->Context_1.Yaw;
        outC->Gaz = outC->Context_1.Gaz;
        outC->Pitch = outC->Context_1.Pitch;
        outC->Roll = outC->Context_1.Roll;
        break;
    case SSM_st_WAITING_Drone:
        outC->Drone_state_nxt = SSM_st_WAITING_Drone;
        outC->Yaw = 0.0;
        outC->Gaz = 0.0;
        outC->Pitch = 0.0;
        outC->Roll = 0.0;
        break;

    }
    outC->init = kcg_false;
}

/* $************* KCG Version 6.4 beta3 (build i9) **************
** SystemDrone.c
** Generation date: 2014-12-17T14:43:37
*************************************************************$ */
