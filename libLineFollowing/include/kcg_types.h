/* $************* KCG Version 6.4 beta3 (build i9) **************
** Command: kcg64.exe -config U:/Windows/Bureau/Projet DRONE/v2/KCG/config.txt
** Generation date: 2014-12-17T14:43:37
*************************************************************$ */
#ifndef _KCG_TYPES_H_
#define _KCG_TYPES_H_

#define KCG_MAPW_CPY

#include "./user_macros.h"

#ifndef kcg_int
#define kcg_int kcg_int
typedef int kcg_int;
#endif /* kcg_int */

#ifndef kcg_bool
#define kcg_bool kcg_bool
typedef unsigned char kcg_bool;
#endif /* kcg_bool */

#ifndef kcg_real
#define kcg_real kcg_real
typedef double kcg_real;
#endif /* kcg_real */

#ifndef kcg_char
#define kcg_char kcg_char
typedef char kcg_char;
#endif /* kcg_char */

#ifndef kcg_false
#define kcg_false ((kcg_bool)0)
#endif /* kcg_false */

#ifndef kcg_true
#define kcg_true ((kcg_bool)1)
#endif /* kcg_true */

#ifndef kcg_assign
#include "kcg_assign.h"
#endif /* kcg_assign */

#ifndef kcg_assign_struct
#define kcg_assign_struct kcg_assign
#endif /* kcg_assign_struct */

#ifndef kcg_assign_array
#define kcg_assign_array kcg_assign
#endif /* kcg_assign_array */

/* SystemFlying::FlyingMode */
typedef enum kcg_tag_SSM_TR_FlyingMode {
    SSM_TR_no_trans_FlyingMode,
    SSM_TR_STANDBY_1_FlyingMode,
    SSM_TR_STANDBY_2_FlyingMode,
    SSM_TR_STANDBY_3_FlyingMode,
    SSM_TR_STANDBY_4_FlyingMode,
    SSM_TR_STANDBY_5_FlyingMode,
    SSM_TR_CHANGE_WAY_1_FlyingMode,
    SSM_TR_TURN_LEFTRIGHT_1_FlyingMode,
    SSM_TR_GO_LEFT_1_FlyingMode,
    SSM_TR_GO_RIGHT_1_FlyingMode,
    SSM_TR_FLYING_1_FlyingMode
} SSM_TR_FlyingMode;
/* SystemFlying::FlyingMode */
typedef enum kcg_tag_SSM_ST_FlyingMode {
    SSM_st_STANDBY_FlyingMode,
    SSM_st_CHANGE_WAY_FlyingMode,
    SSM_st_TURN_LEFTRIGHT_FlyingMode,
    SSM_st_GO_LEFT_FlyingMode,
    SSM_st_GO_RIGHT_FlyingMode,
    SSM_st_FLYING_FlyingMode
} SSM_ST_FlyingMode;
/* SystemEmergency::EmergencyMode */
typedef enum kcg_tag_SSM_TR_EmergencyMode {
    SSM_TR_no_trans_EmergencyMode,
    SSM_TR_MIDDLE_SEARCH_1_EmergencyMode,
    SSM_TR_MIDDLE_SEARCH_2_EmergencyMode,
    SSM_TR_LEFT_SEARCH_1_EmergencyMode,
    SSM_TR_RIGHT_SEARCH_1_EmergencyMode,
    SSM_TR_LEFT_COMEBACK_1_EmergencyMode,
    SSM_TR_RIGHT_COMEBACK_1_EmergencyMode
} SSM_TR_EmergencyMode;
/* SystemEmergency::EmergencyMode */
typedef enum kcg_tag_SSM_ST_EmergencyMode {
    SSM_st_MIDDLE_SEARCH_EmergencyMode,
    SSM_st_LEFT_SEARCH_EmergencyMode,
    SSM_st_RIGHT_SEARCH_EmergencyMode,
    SSM_st_LEFT_COMEBACK_EmergencyMode,
    SSM_st_RIGHT_COMEBACK_EmergencyMode
} SSM_ST_EmergencyMode;
/* SystemDrone::Drone */
typedef enum kcg_tag_SSM_TR_Drone {
    SSM_TR_no_trans_Drone,
    SSM_TR_FLYING_1_Drone,
    SSM_TR_EMERGENCY_1_Drone,
    SSM_TR_WAITING_1_Drone
} SSM_TR_Drone;
/* SystemDrone::Drone */
typedef enum kcg_tag_SSM_ST_Drone {
    SSM_st_FLYING_Drone,
    SSM_st_EMERGENCY_Drone,
    SSM_st_WAITING_Drone
} SSM_ST_Drone;
#endif /* _KCG_TYPES_H_ */
/* $************* KCG Version 6.4 beta3 (build i9) **************
** kcg_types.h
** Generation date: 2014-12-17T14:43:37
*************************************************************$ */
