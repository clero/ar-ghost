/* $************* KCG Version 6.4 beta3 (build i9) **************
** Command: kcg64.exe -config U:/Windows/Bureau/ProjetDRONE/AR-Ghost/KCG/config.txt
** Generation date: 2014-12-10T14:20:16
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

/* SystemDrone::DroneMovement */
typedef enum kcg_tag_SSM_TR_DroneMovement {
    SSM_TR_no_trans_DroneMovement,
    SSM_TR_STANDBY_1_DroneMovement,
    SSM_TR_STANDBY_2_DroneMovement,
    SSM_TR_STANDBY_3_DroneMovement,
    SSM_TR_STANDBY_4_DroneMovement,
    SSM_TR_STANDBY_5_DroneMovement,
    SSM_TR_CHANGE_WAY_1_DroneMovement,
    SSM_TR_TURN_LEFTRIGHT_1_DroneMovement,
    SSM_TR_GO_LEFT_1_DroneMovement,
    SSM_TR_GO_RIGHT_1_DroneMovement,
    SSM_TR_FLYING_1_DroneMovement,
    SSM_TR_CTRL_1_DroneMovement,
    SSM_TR_CTRL_2_DroneMovement
} SSM_TR_DroneMovement;
/* SystemDrone::DroneMovement */
typedef enum kcg_tag_SSM_ST_DroneMovement {
    SSM_st_STANDBY_DroneMovement,
    SSM_st_CHANGE_WAY_DroneMovement,
    SSM_st_TURN_LEFTRIGHT_DroneMovement,
    SSM_st_GO_LEFT_DroneMovement,
    SSM_st_GO_RIGHT_DroneMovement,
    SSM_st_FLYING_DroneMovement,
    SSM_st_CTRL_DroneMovement
} SSM_ST_DroneMovement;
#endif /* _KCG_TYPES_H_ */
/* $************* KCG Version 6.4 beta3 (build i9) **************
** kcg_types.h
** Generation date: 2014-12-10T14:20:16
*************************************************************$ */
