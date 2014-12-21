/* $************* KCG Version 6.4 beta3 (build i9) **************
** Command: kcg64.exe -config U:/Windows/Bureau/Projet DRONE/v2/KCG/config.txt
** Generation date: 2014-12-17T14:43:37
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "SystemFlying.h"

void SystemFlying_init(outC_SystemFlying* outC)
{
    outC->init = kcg_true;
    outC->init1 = kcg_true;
    outC->init2 = kcg_true;
    outC->FlyingMode_reset_act = kcg_true;
    outC->FlyingMode_state_nxt = SSM_st_STANDBY_FlyingMode;
    outC->times_8 = 0;
    outC->times_7 = 0;
    outC->Yaw = 0.0;
    outC->Gaz = 0.0;
    outC->Pitch = 0.0;
    outC->Roll = 0.0;
    CheckAngle_init(&outC->Context_1);
}


void SystemFlying_reset(outC_SystemFlying* outC)
{
    outC->init = kcg_true;
    outC->init1 = kcg_true;
    outC->init2 = kcg_true;
    /* 1 */ CheckAngle_reset(&outC->Context_1);
}

/* SystemFlying */
void SystemFlying(
    /* SystemFlying::LineAngle */ kcg_real LineAngle,
    /* SystemFlying::RightWay */ kcg_bool RightWay,
    /* SystemFlying::GoLeft */ kcg_bool GoLeft,
    /* SystemFlying::GoRight */ kcg_bool GoRight,
    /* SystemFlying::currentAngle */ kcg_real currentAngle,
    /* SystemFlying::ImageUpdate */ kcg_bool ImageUpdate,
    outC_SystemFlying* outC)
{
    kcg_int times_7;
    kcg_int times_8;
    /* SystemFlying::FlyingMode::STANDBY */ kcg_bool br_5_guard_FlyingMode_STANDBY;
    /* SystemFlying::FlyingMode::STANDBY */ kcg_bool br_3_guard_FlyingMode_STANDBY;
    /* SystemFlying::FlyingMode::STANDBY */ kcg_bool br_2_guard_FlyingMode_STANDBY;
    /* SystemFlying::FlyingMode::STANDBY */ kcg_bool br_1_guard_FlyingMode_STANDBY;
    /* SystemFlying::FlyingMode::GO_LEFT */ kcg_bool br_1_guard_FlyingMode_GO_LEFT;
    /* SystemFlying::FlyingMode::GO_RIGHT */ kcg_bool br_1_guard_FlyingMode_GO_RIGHT;
    /* SystemFlying::FlyingMode::FLYING */ kcg_bool br_1_guard_FlyingMode_FLYING;
    /* SystemFlying::FlyingMode */ SSM_ST_FlyingMode FlyingMode_state_sel;
    /* SystemFlying::FlyingMode */ SSM_ST_FlyingMode FlyingMode_state_act;
    /* SystemFlying::Centered */ kcg_bool Centered;

    if (outC->init) {
        FlyingMode_state_sel = SSM_st_STANDBY_FlyingMode;
        Centered = kcg_false;
    } else {
        FlyingMode_state_sel = outC->FlyingMode_state_nxt;
        Centered = outC->FlyingMode_reset_act;
    }
    switch (FlyingMode_state_sel) {
    case SSM_st_GO_LEFT_FlyingMode:
        if (Centered) {
            outC->init2 = kcg_true;
        }
        if (outC->init2) {
            times_7 = 3;
        } else {
            times_7 = outC->times_7;
        }
        if (times_7 < 0) {
            outC->times_7 = times_7;
        } else {
            outC->times_7 = times_7 - 1;
        }
        br_1_guard_FlyingMode_GO_LEFT = outC->times_7 == 0;
        break;
    case SSM_st_GO_RIGHT_FlyingMode:
        if (Centered) {
            outC->init1 = kcg_true;
        }
        if (outC->init1) {
            times_8 = 3;
        } else {
            times_8 = outC->times_8;
        }
        if (times_8 < 0) {
            outC->times_8 = times_8;
        } else {
            outC->times_8 = times_8 - 1;
        }
        br_1_guard_FlyingMode_GO_RIGHT = outC->times_8 == 0;
        break;
    default:
        break;
    }
    Centered = !GoLeft & !GoRight;
    /* 1 */ CheckAngle(currentAngle, LineAngle, ImageUpdate, &outC->Context_1);
    switch (FlyingMode_state_sel) {
    case SSM_st_STANDBY_FlyingMode:
        br_1_guard_FlyingMode_STANDBY = GoLeft & ImageUpdate;
        br_2_guard_FlyingMode_STANDBY = GoRight & ImageUpdate;
        br_3_guard_FlyingMode_STANDBY = (!RightWay) & Centered;
        br_5_guard_FlyingMode_STANDBY = outC->Context_1.AngleOk & RightWay &
                                        Centered;
        if (br_1_guard_FlyingMode_STANDBY) {
            FlyingMode_state_act = SSM_st_GO_LEFT_FlyingMode;
        } else if (br_2_guard_FlyingMode_STANDBY) {
            FlyingMode_state_act = SSM_st_GO_RIGHT_FlyingMode;
        } else if (br_3_guard_FlyingMode_STANDBY) {
            FlyingMode_state_act = SSM_st_CHANGE_WAY_FlyingMode;
        } else if (outC->Context_1.AngleNotOk) {
            FlyingMode_state_act = SSM_st_TURN_LEFTRIGHT_FlyingMode;
        } else if (br_5_guard_FlyingMode_STANDBY) {
            FlyingMode_state_act = SSM_st_FLYING_FlyingMode;
        } else {
            FlyingMode_state_act = SSM_st_STANDBY_FlyingMode;
        }
        break;
    case SSM_st_CHANGE_WAY_FlyingMode:
        if (RightWay) {
            FlyingMode_state_act = SSM_st_STANDBY_FlyingMode;
        } else {
            FlyingMode_state_act = SSM_st_CHANGE_WAY_FlyingMode;
        }
        break;
    case SSM_st_TURN_LEFTRIGHT_FlyingMode:
        if (outC->Context_1.AngleOk) {
            FlyingMode_state_act = SSM_st_STANDBY_FlyingMode;
        } else {
            FlyingMode_state_act = SSM_st_TURN_LEFTRIGHT_FlyingMode;
        }
        break;
    case SSM_st_GO_LEFT_FlyingMode:
        if (br_1_guard_FlyingMode_GO_LEFT) {
            FlyingMode_state_act = SSM_st_STANDBY_FlyingMode;
        } else {
            FlyingMode_state_act = SSM_st_GO_LEFT_FlyingMode;
        }
        break;
    case SSM_st_GO_RIGHT_FlyingMode:
        if (br_1_guard_FlyingMode_GO_RIGHT) {
            FlyingMode_state_act = SSM_st_STANDBY_FlyingMode;
        } else {
            FlyingMode_state_act = SSM_st_GO_RIGHT_FlyingMode;
        }
        break;
    case SSM_st_FLYING_FlyingMode:
        br_1_guard_FlyingMode_FLYING = outC->Context_1.AngleNotOk | !RightWay |
                                       !Centered;
        if (br_1_guard_FlyingMode_FLYING) {
            FlyingMode_state_act = SSM_st_STANDBY_FlyingMode;
        } else {
            FlyingMode_state_act = SSM_st_FLYING_FlyingMode;
        }
        break;

    }
    switch (FlyingMode_state_act) {
    case SSM_st_STANDBY_FlyingMode:
        outC->FlyingMode_state_nxt = SSM_st_STANDBY_FlyingMode;
        outC->Yaw = 0.0;
        outC->Gaz = 0.0;
        outC->Pitch = 0.0;
        outC->Roll = 0.0;
        break;
    case SSM_st_CHANGE_WAY_FlyingMode:
        outC->FlyingMode_state_nxt = SSM_st_CHANGE_WAY_FlyingMode;
        outC->Yaw = 0.4;
        outC->Gaz = 0.0;
        outC->Pitch = 0.0;
        outC->Roll = 0.0;
        break;
    case SSM_st_TURN_LEFTRIGHT_FlyingMode:
        outC->FlyingMode_state_nxt = SSM_st_TURN_LEFTRIGHT_FlyingMode;
        outC->Yaw = outC->Context_1.YawSpeed;
        outC->Gaz = 0.0;
        outC->Pitch = 0.0;
        outC->Roll = 0.0;
        break;
    case SSM_st_GO_LEFT_FlyingMode:
        outC->FlyingMode_state_nxt = SSM_st_GO_LEFT_FlyingMode;
        outC->Yaw = 0.0;
        outC->Gaz = 0.0;
        outC->Pitch = 0.0;
        outC->Roll = -0.2;
        break;
    case SSM_st_GO_RIGHT_FlyingMode:
        outC->FlyingMode_state_nxt = SSM_st_GO_RIGHT_FlyingMode;
        outC->Yaw = 0.0;
        outC->Gaz = 0.0;
        outC->Pitch = 0.0;
        outC->Roll = 0.2;
        break;
    case SSM_st_FLYING_FlyingMode:
        outC->FlyingMode_state_nxt = SSM_st_FLYING_FlyingMode;
        outC->Yaw = 0.0;
        outC->Gaz = 0.0;
        outC->Pitch = -0.2;
        outC->Roll = 0.0;
        break;

    }
    switch (FlyingMode_state_sel) {
    case SSM_st_STANDBY_FlyingMode:
        if (br_1_guard_FlyingMode_STANDBY) {
            outC->FlyingMode_reset_act = kcg_true;
        } else if (br_2_guard_FlyingMode_STANDBY) {
            outC->FlyingMode_reset_act = kcg_true;
        } else if (br_3_guard_FlyingMode_STANDBY) {
            outC->FlyingMode_reset_act = kcg_true;
        } else if (outC->Context_1.AngleNotOk) {
            outC->FlyingMode_reset_act = kcg_true;
        } else {
            outC->FlyingMode_reset_act = br_5_guard_FlyingMode_STANDBY;
        }
        break;
    case SSM_st_CHANGE_WAY_FlyingMode:
        outC->FlyingMode_reset_act = RightWay;
        break;
    case SSM_st_TURN_LEFTRIGHT_FlyingMode:
        outC->FlyingMode_reset_act = outC->Context_1.AngleOk;
        break;
    case SSM_st_GO_LEFT_FlyingMode:
        outC->FlyingMode_reset_act = br_1_guard_FlyingMode_GO_LEFT;
        break;
    case SSM_st_GO_RIGHT_FlyingMode:
        outC->FlyingMode_reset_act = br_1_guard_FlyingMode_GO_RIGHT;
        break;
    case SSM_st_FLYING_FlyingMode:
        outC->FlyingMode_reset_act = br_1_guard_FlyingMode_FLYING;
        break;

    }
    outC->init = kcg_false;
    switch (FlyingMode_state_sel) {
    case SSM_st_GO_LEFT_FlyingMode:
        outC->init2 = kcg_false;
        break;
    case SSM_st_GO_RIGHT_FlyingMode:
        outC->init1 = kcg_false;
        break;
    default:
        break;

    }
}

/* $************* KCG Version 6.4 beta3 (build i9) **************
** SystemFlying.c
** Generation date: 2014-12-17T14:43:37
*************************************************************$ */
