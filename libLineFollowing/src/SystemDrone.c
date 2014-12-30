/* $************* KCG Version 5.4 beta3 (build i9) **************
** Command: kcg64.exe -config U:/Windows/Bureau/ProjetDRONE/AR-Ghost/KCG/config.txt
** Generation date: 2014-12-10T14:20:16
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "SystemDrone.h"

void SystemDrone_init(outC_SystemDrone* outC)
{
    outC->init = kcg_true;
    outC->init1 = kcg_true;
    outC->init2 = kcg_true;
    outC->init3 = kcg_true;
    outC->DroneMovement_reset_act = kcg_true;
    outC->DroneMovement_state_nxt = SSM_st_STANDBY_DroneMovement;
    outC->times_4 = 0;
    outC->times_6 = 0;
    outC->times_5 = 0;
    outC->Yaw = 0.0;
    outC->Gaz = 0.0;
    outC->Pitch = 0.0;
    outC->Roll = 0.0;
    CheckAngle_init(&outC->Context_1);
}


void SystemDrone_reset(outC_SystemDrone* outC)
{
    outC->init = kcg_true;
    outC->init1 = kcg_true;
    outC->init2 = kcg_true;
    outC->init3 = kcg_true;
    /* 1 */ CheckAngle_reset(&outC->Context_1);
}

/* SystemDrone */
void SystemDrone(inC_SystemDrone* inC, outC_SystemDrone* outC)
{
    kcg_int times_5;
    kcg_int times_6;
    kcg_int times_4;
    /* SystemDrone::DroneMovement::STANDBY */ kcg_bool br_5_guard_DroneMovement_STANDBY;
    /* SystemDrone::DroneMovement::STANDBY */ kcg_bool br_1_guard_DroneMovement_STANDBY;
    /* SystemDrone::DroneMovement::GO_LEFT */ kcg_bool br_1_guard_DroneMovement_GO_LEFT;
    /* SystemDrone::DroneMovement::GO_RIGHT */ kcg_bool br_1_guard_DroneMovement_GO_RIGHT;
    /* SystemDrone::DroneMovement::FLYING */ kcg_bool br_1_guard_DroneMovement_FLYING;
    /* SystemDrone::DroneMovement::CTRL */ kcg_bool br_2_guard_DroneMovement_CTRL;
    /* SystemDrone::DroneMovement::CTRL */ kcg_bool br_1_guard_DroneMovement_CTRL;
    /* SystemDrone::DroneMovement */ SSM_ST_DroneMovement DroneMovement_state_sel;
    /* SystemDrone::DroneMovement */ SSM_ST_DroneMovement DroneMovement_state_act;

    if (outC->init) {
        DroneMovement_state_sel = SSM_st_STANDBY_DroneMovement;
        br_2_guard_DroneMovement_CTRL = kcg_false;
    } else {
        DroneMovement_state_sel = outC->DroneMovement_state_nxt;
        br_2_guard_DroneMovement_CTRL = outC->DroneMovement_reset_act;
    }
    switch (DroneMovement_state_sel) {
    case SSM_st_GO_LEFT_DroneMovement:
        if (br_2_guard_DroneMovement_CTRL) {
            outC->init3 = kcg_true;
        }
        if (outC->init3) {
            times_5 = 5;
        } else {
            times_5 = outC->times_5;
        }
        if (times_5 < 0) {
            outC->times_5 = times_5;
        } else {
            outC->times_5 = times_5 - 1;
        }
        br_1_guard_DroneMovement_GO_LEFT = outC->times_5 == 0;
        break;
    case SSM_st_GO_RIGHT_DroneMovement:
        if (br_2_guard_DroneMovement_CTRL) {
            outC->init2 = kcg_true;
        }
        if (outC->init2) {
            times_6 = 5;
        } else {
            times_6 = outC->times_6;
        }
        if (times_6 < 0) {
            outC->times_6 = times_6;
        } else {
            outC->times_6 = times_6 - 1;
        }
        br_1_guard_DroneMovement_GO_RIGHT = outC->times_6 == 0;
        break;
    case SSM_st_CTRL_DroneMovement:
        if (br_2_guard_DroneMovement_CTRL) {
            outC->init1 = kcg_true;
        }
        if (outC->init1) {
            times_4 = 4;
        } else {
            times_4 = outC->times_4;
        }
        if (times_4 < 0) {
            outC->times_4 = times_4;
        } else {
            outC->times_4 = times_4 - 1;
        }
        br_1_guard_DroneMovement_CTRL = outC->times_4 == 0;
        break;
    default:
        break;

    }
    /* 1 */
    CheckAngle(
        inC->currentAngle,
        inC->LineAngle,
        inC->ImageUpdate,
        &outC->Context_1);
    switch (DroneMovement_state_sel) {
    case SSM_st_STANDBY_DroneMovement:
        br_1_guard_DroneMovement_STANDBY = !inC->RightWay;
        br_5_guard_DroneMovement_STANDBY = outC->Context_1.AngleOk &
                                           inC->RightWay & !inC->GoLeft & !inC->GoRight;
        if (br_1_guard_DroneMovement_STANDBY) {
            DroneMovement_state_act = SSM_st_CHANGE_WAY_DroneMovement;
        } else if (outC->Context_1.AngleNotOk) {
            DroneMovement_state_act = SSM_st_TURN_LEFTRIGHT_DroneMovement;
        } else if (inC->GoLeft) {
            DroneMovement_state_act = SSM_st_GO_LEFT_DroneMovement;
        } else if (inC->GoRight) {
            DroneMovement_state_act = SSM_st_GO_RIGHT_DroneMovement;
        } else if (br_5_guard_DroneMovement_STANDBY) {
            DroneMovement_state_act = SSM_st_CTRL_DroneMovement;
        } else {
            DroneMovement_state_act = SSM_st_STANDBY_DroneMovement;
        }
        break;
    case SSM_st_CHANGE_WAY_DroneMovement:
        if (inC->RightWay) {
            DroneMovement_state_act = SSM_st_STANDBY_DroneMovement;
        } else {
            DroneMovement_state_act = SSM_st_CHANGE_WAY_DroneMovement;
        }
        break;
    case SSM_st_TURN_LEFTRIGHT_DroneMovement:
        if (outC->Context_1.AngleOk) {
            DroneMovement_state_act = SSM_st_STANDBY_DroneMovement;
        } else {
            DroneMovement_state_act = SSM_st_TURN_LEFTRIGHT_DroneMovement;
        }
        break;
    case SSM_st_GO_LEFT_DroneMovement:
        if (br_1_guard_DroneMovement_GO_LEFT) {
            DroneMovement_state_act = SSM_st_STANDBY_DroneMovement;
        } else {
            DroneMovement_state_act = SSM_st_GO_LEFT_DroneMovement;
        }
        break;
    case SSM_st_GO_RIGHT_DroneMovement:
        if (br_1_guard_DroneMovement_GO_RIGHT) {
            DroneMovement_state_act = SSM_st_STANDBY_DroneMovement;
        } else {
            DroneMovement_state_act = SSM_st_GO_RIGHT_DroneMovement;
        }
        break;
    case SSM_st_FLYING_DroneMovement:
        br_1_guard_DroneMovement_FLYING = outC->Context_1.AngleNotOk |
                                          !inC->RightWay | inC->GoLeft | inC->GoRight;
        if (br_1_guard_DroneMovement_FLYING) {
            DroneMovement_state_act = SSM_st_STANDBY_DroneMovement;
        } else {
            DroneMovement_state_act = SSM_st_FLYING_DroneMovement;
        }
        break;
    case SSM_st_CTRL_DroneMovement:
        br_2_guard_DroneMovement_CTRL = outC->Context_1.AngleNotOk |
                                        !inC->RightWay | inC->GoLeft | inC->GoRight;
        if (br_1_guard_DroneMovement_CTRL) {
            DroneMovement_state_act = SSM_st_FLYING_DroneMovement;
        } else if (br_2_guard_DroneMovement_CTRL) {
            DroneMovement_state_act = SSM_st_STANDBY_DroneMovement;
        } else {
            DroneMovement_state_act = SSM_st_CTRL_DroneMovement;
        }
        break;

    }
    switch (DroneMovement_state_act) {
    case SSM_st_STANDBY_DroneMovement:
        outC->DroneMovement_state_nxt = SSM_st_STANDBY_DroneMovement;
        outC->Yaw = 0.0;
        outC->Gaz = 0.0;
        outC->Pitch = 0.0;
        outC->Roll = 0.0;
        break;
    case SSM_st_CHANGE_WAY_DroneMovement:
        outC->DroneMovement_state_nxt = SSM_st_CHANGE_WAY_DroneMovement;
        outC->Yaw = 0.4;
        outC->Gaz = 0.0;
        outC->Pitch = 0.0;
        outC->Roll = 0.0;
        break;
    case SSM_st_TURN_LEFTRIGHT_DroneMovement:
        outC->DroneMovement_state_nxt = SSM_st_TURN_LEFTRIGHT_DroneMovement;
        outC->Yaw = outC->Context_1.YawSpeed;
        outC->Gaz = 0.0;
        outC->Pitch = 0.0;
        outC->Roll = 0.0;
        break;
    case SSM_st_GO_LEFT_DroneMovement:
        outC->DroneMovement_state_nxt = SSM_st_GO_LEFT_DroneMovement;
        outC->Yaw = 0.0;
        outC->Gaz = 0.0;
        outC->Pitch = 0.0;
        outC->Roll = -0.2;
        break;
    case SSM_st_GO_RIGHT_DroneMovement:
        outC->DroneMovement_state_nxt = SSM_st_GO_RIGHT_DroneMovement;
        outC->Yaw = 0.0;
        outC->Gaz = 0.0;
        outC->Pitch = 0.0;
        outC->Roll = 0.2;
        break;
    case SSM_st_FLYING_DroneMovement:
        outC->DroneMovement_state_nxt = SSM_st_FLYING_DroneMovement;
        outC->Yaw = 0.0;
        outC->Gaz = 0.0;
        outC->Pitch = -0.2;
        outC->Roll = 0.0;
        break;
    case SSM_st_CTRL_DroneMovement:
        outC->DroneMovement_state_nxt = SSM_st_CTRL_DroneMovement;
        outC->Yaw = 0.0;
        outC->Gaz = 0.0;
        outC->Pitch = 0.0;
        outC->Roll = 0.0;
        break;

    }
    switch (DroneMovement_state_sel) {
    case SSM_st_STANDBY_DroneMovement:
        if (br_1_guard_DroneMovement_STANDBY) {
            outC->DroneMovement_reset_act = kcg_true;
        } else if (outC->Context_1.AngleNotOk) {
            outC->DroneMovement_reset_act = kcg_true;
        } else if (inC->GoLeft) {
            outC->DroneMovement_reset_act = kcg_true;
        } else if (inC->GoRight) {
            outC->DroneMovement_reset_act = kcg_true;
        } else {
            outC->DroneMovement_reset_act = br_5_guard_DroneMovement_STANDBY;
        }
        break;
    case SSM_st_CHANGE_WAY_DroneMovement:
        outC->DroneMovement_reset_act = inC->RightWay;
        break;
    case SSM_st_TURN_LEFTRIGHT_DroneMovement:
        outC->DroneMovement_reset_act = outC->Context_1.AngleOk;
        break;
    case SSM_st_GO_LEFT_DroneMovement:
        outC->DroneMovement_reset_act = br_1_guard_DroneMovement_GO_LEFT;
        break;
    case SSM_st_GO_RIGHT_DroneMovement:
        outC->DroneMovement_reset_act = br_1_guard_DroneMovement_GO_RIGHT;
        break;
    case SSM_st_FLYING_DroneMovement:
        outC->DroneMovement_reset_act = br_1_guard_DroneMovement_FLYING;
        break;
    case SSM_st_CTRL_DroneMovement:
        if (br_1_guard_DroneMovement_CTRL) {
            outC->DroneMovement_reset_act = kcg_true;
        } else {
            outC->DroneMovement_reset_act = br_2_guard_DroneMovement_CTRL;
        }
        break;

    }
    outC->init = kcg_false;
    switch (DroneMovement_state_sel) {
    case SSM_st_GO_LEFT_DroneMovement:
        outC->init3 = kcg_false;
        break;
    case SSM_st_GO_RIGHT_DroneMovement:
        outC->init2 = kcg_false;
        break;
    case SSM_st_CTRL_DroneMovement:
        outC->init1 = kcg_false;
        break;
    default:
        break;

    }
}

/* $************* KCG Version 6.4 beta3 (build i9) **************
** SystemDrone.c
** Generation date: 2014-12-10T14:20:16
*************************************************************$ */
