/* $************* KCG Version 6.4 beta3 (build i9) **************
** Command: kcg64.exe -config U:/Windows/Bureau/Projet DRONE/v2/KCG/config.txt
** Generation date: 2014-12-17T14:43:37
*************************************************************$ */
#ifndef _SystemEmergency_H_
#define _SystemEmergency_H_

#include "kcg_types.h"

/* =====================  no input structure  ====================== */

/* ========================  context type  ========================= */
typedef struct {
    /* ---------------------------  outputs  --------------------------- */
    kcg_real /* SystemEmergency::Roll */ Roll;
    kcg_real /* SystemEmergency::Pitch */ Pitch;
    kcg_real /* SystemEmergency::Gaz */ Gaz;
    kcg_real /* SystemEmergency::Yaw */ Yaw;
    /* -----------------------  no local probes  ----------------------- */
    /* -------------------- initialization variables  ------------------ */
    kcg_bool init;
    kcg_bool init1;
    kcg_bool init2;
    kcg_bool init3;
    kcg_bool init4;
    /* ----------------------- local memories  ------------------------- */
    kcg_bool /* SystemEmergency::EmergencyMode */ EmergencyMode_reset_nxt;
    kcg_bool /* SystemEmergency::EmergencyMode */ EmergencyMode_reset_act;
    SSM_ST_EmergencyMode /* SystemEmergency::EmergencyMode */ EmergencyMode_state_nxt;
    kcg_int times_11;
    kcg_int times_9;
    kcg_int times_10;
    kcg_int times_8;
    /* -------------------- no sub nodes' contexts  -------------------- */
    /* ----------------- no clocks of observable data ------------------ */
} outC_SystemEmergency;

/* ===========  node initialization and cycle functions  =========== */
/* SystemEmergency */
extern void SystemEmergency(outC_SystemEmergency* outC);

extern void SystemEmergency_reset(outC_SystemEmergency* outC);
extern void SystemEmergency_init(outC_SystemEmergency* outC);
#endif /* _SystemEmergency_H_ */
/* $************* KCG Version 6.4 beta3 (build i9) **************
** SystemEmergency.h
** Generation date: 2014-12-17T14:43:37
*************************************************************$ */
