/* $************* KCG Version 6.4 beta3 (build i9) **************
** Command: kcg64.exe -config U:/Windows/Bureau/Projet DRONE/v2/KCG/config.txt
** Generation date: 2014-12-17T14:43:37
*************************************************************$ */

#include "kcg_consts.h"
#include "kcg_sensors.h"
#include "SystemEmergency.h"

void SystemEmergency_init(outC_SystemEmergency* outC)
{
    outC->init4 = kcg_true;
    outC->init3 = kcg_true;
    outC->init2 = kcg_true;
    outC->init1 = kcg_true;
    outC->init = kcg_true;
    outC->times_8 = 0;
    outC->times_10 = 0;
    outC->times_9 = 0;
    outC->times_11 = 0;
    outC->EmergencyMode_state_nxt = SSM_st_MIDDLE_SEARCH_EmergencyMode;
    outC->EmergencyMode_reset_act = kcg_true;
    outC->EmergencyMode_reset_nxt = kcg_true;
    outC->Yaw = 0.0;
    outC->Gaz = 0.0;
    outC->Pitch = 0.0;
    outC->Roll = 0.0;
}


void SystemEmergency_reset(outC_SystemEmergency* outC)
{
    outC->init4 = kcg_true;
    outC->init3 = kcg_true;
    outC->init2 = kcg_true;
    outC->init1 = kcg_true;
    outC->init = kcg_true;
}

/* SystemEmergency */
void SystemEmergency(outC_SystemEmergency* outC)
{
    SSM_TR_EmergencyMode tmp;
    /* SystemEmergency::EmergencyMode */ kcg_bool EmergencyMode_reset_sel;
    /* SystemEmergency::EmergencyMode */ SSM_ST_EmergencyMode EmergencyMode_state_act;
    /* SystemEmergency::EmergencyMode */ SSM_ST_EmergencyMode EmergencyMode_state_sel;
    /* SystemEmergency::EmergencyMode::RIGHT_COMEBACK */ kcg_bool
        br_1_guard_EmergencyMode_RIGHT_COMEBACK;
    /* SystemEmergency::EmergencyMode::LEFT_COMEBACK */ kcg_bool
        br_1_guard_EmergencyMode_LEFT_COMEBACK;
    /* SystemEmergency::EmergencyMode::RIGHT_SEARCH */ kcg_bool
        _1_br_1_guard_EmergencyMode_RIGHT_SEARCH;
    /* SystemEmergency::EmergencyMode::LEFT_SEARCH */ kcg_bool br_1_guard_EmergencyMode_LEFT_SEARCH;
    kcg_int times_10;
    kcg_int times_8;

    if (outC->init) {
        EmergencyMode_state_sel = SSM_st_MIDDLE_SEARCH_EmergencyMode;
        EmergencyMode_reset_sel = kcg_false;
    } else {
        EmergencyMode_state_sel = outC->EmergencyMode_state_nxt;
        EmergencyMode_reset_sel = outC->EmergencyMode_reset_act;
    }
    switch (EmergencyMode_state_sel) {
    case SSM_st_LEFT_COMEBACK_EmergencyMode:
        if (EmergencyMode_reset_sel) {
            outC->init2 = kcg_true;
        }
        if (outC->init2) {
            times_8 = 5;
        } else {
            times_8 = outC->times_9;
        }
        if (times_8 < 0) {
            outC->times_9 = times_8;
        } else {
            outC->times_9 = times_8 - 1;
        }
        br_1_guard_EmergencyMode_LEFT_COMEBACK = outC->times_9 == 0;
        break;
    case SSM_st_RIGHT_COMEBACK_EmergencyMode:
        if (EmergencyMode_reset_sel) {
            outC->init1 = kcg_true;
        }
        if (outC->init1) {
            times_10 = 5;
        } else {
            times_10 = outC->times_11;
        }
        if (times_10 < 0) {
            outC->times_11 = times_10;
        } else {
            outC->times_11 = times_10 - 1;
        }
        br_1_guard_EmergencyMode_RIGHT_COMEBACK = outC->times_11 == 0;
        break;
    default:
        break;

    }
    if (outC->init) {
        EmergencyMode_reset_sel = kcg_false;
    } else {
        EmergencyMode_reset_sel = outC->EmergencyMode_reset_nxt;
    }
    switch (EmergencyMode_state_sel) {
    case SSM_st_MIDDLE_SEARCH_EmergencyMode:
        EmergencyMode_state_act = SSM_st_MIDDLE_SEARCH_EmergencyMode;
        break;
    case SSM_st_LEFT_SEARCH_EmergencyMode:
        if (EmergencyMode_reset_sel) {
            outC->init4 = kcg_true;
        }
        if (outC->init4) {
            times_8 = 5;
        } else {
            times_8 = outC->times_8;
        }
        if (times_8 < 0) {
            outC->times_8 = times_8;
        } else {
            outC->times_8 = times_8 - 1;
        }
        br_1_guard_EmergencyMode_LEFT_SEARCH = outC->times_8 == 0;
        if (br_1_guard_EmergencyMode_LEFT_SEARCH) {
            EmergencyMode_state_act = SSM_st_LEFT_COMEBACK_EmergencyMode;
        } else {
            EmergencyMode_state_act = SSM_st_LEFT_SEARCH_EmergencyMode;
        }
        break;
    case SSM_st_RIGHT_SEARCH_EmergencyMode:
        if (EmergencyMode_reset_sel) {
            outC->init3 = kcg_true;
        }
        if (outC->init3) {
            times_10 = 5;
        } else {
            times_10 = outC->times_10;
        }
        if (times_10 < 0) {
            outC->times_10 = times_10;
        } else {
            outC->times_10 = times_10 - 1;
        }
        _1_br_1_guard_EmergencyMode_RIGHT_SEARCH = outC->times_10 == 0;
        if (_1_br_1_guard_EmergencyMode_RIGHT_SEARCH) {
            EmergencyMode_state_act = SSM_st_RIGHT_COMEBACK_EmergencyMode;
        } else {
            EmergencyMode_state_act = SSM_st_RIGHT_SEARCH_EmergencyMode;
        }
        break;
    case SSM_st_LEFT_COMEBACK_EmergencyMode:
        if (br_1_guard_EmergencyMode_LEFT_COMEBACK) {
            EmergencyMode_state_act = SSM_st_MIDDLE_SEARCH_EmergencyMode;
        } else {
            EmergencyMode_state_act = SSM_st_LEFT_COMEBACK_EmergencyMode;
        }
        break;
    case SSM_st_RIGHT_COMEBACK_EmergencyMode:
        if (br_1_guard_EmergencyMode_RIGHT_COMEBACK) {
            EmergencyMode_state_act = SSM_st_MIDDLE_SEARCH_EmergencyMode;
        } else {
            EmergencyMode_state_act = SSM_st_RIGHT_COMEBACK_EmergencyMode;
        }
        break;

    }
    switch (EmergencyMode_state_act) {
    case SSM_st_MIDDLE_SEARCH_EmergencyMode:
        outC->Pitch = 0.0;
        break;
    case SSM_st_LEFT_SEARCH_EmergencyMode:
        outC->Pitch = 0.0;
        break;
    case SSM_st_RIGHT_SEARCH_EmergencyMode:
        outC->Pitch = 0.0;
        break;
    case SSM_st_LEFT_COMEBACK_EmergencyMode:
        outC->Pitch = 0.0;
        break;
    case SSM_st_RIGHT_COMEBACK_EmergencyMode:
        outC->Pitch = 0.0;
        break;

    }
    outC->Gaz = outC->Pitch;
    outC->Yaw = outC->Gaz;
    switch (EmergencyMode_state_sel) {
    case SSM_st_MIDDLE_SEARCH_EmergencyMode:
        outC->EmergencyMode_reset_act = kcg_false;
        break;
    case SSM_st_LEFT_SEARCH_EmergencyMode:
        outC->EmergencyMode_reset_act = br_1_guard_EmergencyMode_LEFT_SEARCH;
        break;
    case SSM_st_RIGHT_SEARCH_EmergencyMode:
        outC->EmergencyMode_reset_act = _1_br_1_guard_EmergencyMode_RIGHT_SEARCH;
        break;
    case SSM_st_LEFT_COMEBACK_EmergencyMode:
        outC->EmergencyMode_reset_act = br_1_guard_EmergencyMode_LEFT_COMEBACK;
        break;
    case SSM_st_RIGHT_COMEBACK_EmergencyMode:
        outC->EmergencyMode_reset_act = br_1_guard_EmergencyMode_RIGHT_COMEBACK;
        break;

    }
    switch (EmergencyMode_state_act) {
    case SSM_st_MIDDLE_SEARCH_EmergencyMode:
        outC->Roll = 0.0;
        switch (EmergencyMode_state_sel) {
        case SSM_st_MIDDLE_SEARCH_EmergencyMode:
            tmp = SSM_TR_no_trans_EmergencyMode;
            break;
        case SSM_st_LEFT_SEARCH_EmergencyMode:
            if (br_1_guard_EmergencyMode_LEFT_SEARCH) {
                tmp = SSM_TR_LEFT_SEARCH_1_EmergencyMode;
            } else {
                tmp = SSM_TR_no_trans_EmergencyMode;
            }
            break;
        case SSM_st_RIGHT_SEARCH_EmergencyMode:
            if (_1_br_1_guard_EmergencyMode_RIGHT_SEARCH) {
                tmp = SSM_TR_RIGHT_SEARCH_1_EmergencyMode;
            } else {
                tmp = SSM_TR_no_trans_EmergencyMode;
            }
            break;
        case SSM_st_LEFT_COMEBACK_EmergencyMode:
            if (br_1_guard_EmergencyMode_LEFT_COMEBACK) {
                tmp = SSM_TR_LEFT_COMEBACK_1_EmergencyMode;
            } else {
                tmp = SSM_TR_no_trans_EmergencyMode;
            }
            break;
        case SSM_st_RIGHT_COMEBACK_EmergencyMode:
            if (br_1_guard_EmergencyMode_RIGHT_COMEBACK) {
                tmp = SSM_TR_RIGHT_COMEBACK_1_EmergencyMode;
            } else {
                tmp = SSM_TR_no_trans_EmergencyMode;
            }
            break;

        }
        EmergencyMode_reset_sel = tmp != SSM_TR_no_trans_EmergencyMode;
        if (EmergencyMode_reset_sel) {
            outC->EmergencyMode_state_nxt = SSM_st_MIDDLE_SEARCH_EmergencyMode;
            outC->EmergencyMode_reset_nxt = kcg_false;
        } else {
            outC->EmergencyMode_state_nxt = SSM_st_LEFT_SEARCH_EmergencyMode;
            outC->EmergencyMode_reset_nxt = kcg_true;
        }
        break;
    case SSM_st_LEFT_SEARCH_EmergencyMode:
        outC->Roll = -0.2;
        outC->EmergencyMode_state_nxt = SSM_st_LEFT_SEARCH_EmergencyMode;
        outC->EmergencyMode_reset_nxt = kcg_false;
        break;
    case SSM_st_RIGHT_SEARCH_EmergencyMode:
        outC->Roll = 0.2;
        outC->EmergencyMode_state_nxt = SSM_st_RIGHT_SEARCH_EmergencyMode;
        outC->EmergencyMode_reset_nxt = kcg_false;
        break;
    case SSM_st_LEFT_COMEBACK_EmergencyMode:
        outC->Roll = 0.2;
        outC->EmergencyMode_state_nxt = SSM_st_LEFT_COMEBACK_EmergencyMode;
        outC->EmergencyMode_reset_nxt = kcg_false;
        break;
    case SSM_st_RIGHT_COMEBACK_EmergencyMode:
        outC->Roll = -0.2;
        outC->EmergencyMode_state_nxt = SSM_st_RIGHT_COMEBACK_EmergencyMode;
        outC->EmergencyMode_reset_nxt = kcg_false;
        break;

    }
    switch (EmergencyMode_state_sel) {
    case SSM_st_LEFT_SEARCH_EmergencyMode:
        outC->init4 = kcg_false;
        break;
    case SSM_st_RIGHT_SEARCH_EmergencyMode:
        outC->init3 = kcg_false;
        break;
    case SSM_st_LEFT_COMEBACK_EmergencyMode:
        outC->init2 = kcg_false;
        break;
    case SSM_st_RIGHT_COMEBACK_EmergencyMode:
        outC->init1 = kcg_false;
        break;
    default:
        break;

    }
    outC->init = kcg_false;
}

/* $************* KCG Version 6.4 beta3 (build i9) **************
** SystemEmergency.c
** Generation date: 2014-12-17T14:43:37
*************************************************************$ */
