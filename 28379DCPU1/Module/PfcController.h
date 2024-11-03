/*******************************************************************************
 * File Name: PfcController.H
 * Description: The PFC controller functions.
 * Designer: Miller Xue
 * Version: 00
 * Copyright: Liyuan Haina Co.Ltd
 *******************************************************************************/
#ifndef _PFCCONTROLLER_H
#define _PFCCONTROLLER_H


#define  cPFC_I_Ratio             0.09768f
#define  cGrid_I_Ratio            0.222f
#define  cGrid_V_Ratio            0.3932f
#define  cBus_V_Ratio             0.2536f
#define  cNBus_V_Ratio            0.1282f

//#define  cBuck_I_Ratio            1.0f
//#define  cOutput_I_Ratio          1.0f
//#define  cOutput_V_Ratio          1.0f

#define CPU_CLK     200          //MHz
#define Fs          36           //kHz

#define   PI                3.14159265358979f
#define   PI2               6.28318530717958f
#define   TAN60             1.73205080756888f
#define   COS30             0.86602540378444f
//#define   COS30             14189
#define   COS60             0.5f
//#define   COS60             8192

#define   INV_SQRT3         0.57735026918963f   // 1/sqrt(3) = 0.57735026918963
#define   SQRT2             1.414213562373f     // sqrt(2) = 1.4142135623730950488016887242097
#define   INV_SQRT2         0.7071067812f   // 1/sqrt(2) = 0.70710678118654752440084436210485
#define   ONE_THIRD         0.33333333333333f
#define   TWO_THIRD         0.66666666666667f
#define   PFC_L             150.0f              //uH
#define   GRID_L            16.0f               //uH
#define   FLT_C             15.0f               //uF
#define   DUMMY_R           2.0f



/**************************************************************************
 *                      Variable Declaration
 *************************************************************************/
typedef struct {

    float f32_GridVolt_R;
    float f32_GridVolt_S;
    float f32_GridVolt_T;
    float f32_GridVolt_RS;
    float f32_GridVolt_ST;
    float f32_GridVolt_TR;

    float f32_PfcCurr_R_1;
    float f32_PfcCurr_S_1;
    float f32_PfcCurr_T_1;

    float f32_PfcCurr_R_2;
    float f32_PfcCurr_S_2;
    float f32_PfcCurr_T_2;

    float f32_GridCurr_R;
    float f32_GridCurr_S;
    float f32_GridCurr_T;

    float f32_BusVolt;
}SAMPLE_ST;

typedef struct {
    float f32_Ref;
    float f32_Fbk;
    float f32_A0;
    float f32_A1;
    float f32_B0;
    float f32_B1;
    float f32_B2;
    float f32_x0;
    float f32_x1;
    float f32_x2;
    float f32_y;
    float f32_y0;
    float f32_y1;
    float f32_Max;
    float f32_Min;
    float f32_Pid_Err_Max;
    float f32_Pid_Err_Min;
    float f32_Temp;
}CTRL_2P2Z_ST;

typedef struct {
    float f32_Ref;
    float f32_Fbk;

    float f32_Kp;
    float f32_Ki;

    float f32_Err;
    float f32_Err0;

    float f32_Up;
    float f32_Ui;

    float f32_Uo;
    float f32_Out;
    float f32_Max;
    float f32_Min;
}CTRL_PI_ST;

typedef struct {
    float f32_As;
    float f32_Bs;
    float f32_Zs;
}CLARK_ST;

typedef struct {
    float f32_Dr;
    float f32_Qr;
    float f32_Sin;
    float f32_Cos;
    float f32_FreqRef;
    float f32_Ts;
    float f32_Theta;
    float f32_SinGraph;
    float f32_CosGraph;
    float f32_FreqRefGraph;
    float f32_ThetaGraph;

}PLL_ST;

typedef struct {
    uint16_t Sector;
    uint16_t Flag_PwmType;
    float Vx;
    float Vy;
    float Ta;
    float Tb;
    float Tc;
    float Tx;
    float Ty;
    float Tz;

    float M_Limit;
}SVPWM_ST;

typedef struct {
    uint16_t u16_PwmCmpr1;
    uint16_t u16_PwmCmpr2;
    uint16_t u16_PwmCmpr3;
}CMPR_ST;


#define CTRL_2P2Z(v)                     \
    v.f32_Temp =  v.f32_B0 * v.f32_x0;   \
    v.f32_Temp += v.f32_B1  * v.f32_x1;  \
    v.f32_Temp += v.f32_B1 * v.f32_x2;   \
    v.f32_Temp += v.f32_A0 * v.f32_y0;   \
    v.f32_Temp += v.f32_A1 * v.f32_y1;   \
    v.f32_x2 = v.f32_x1;     \
    v.f32_y1 = v.f32_y0;     \
    v.f32_y0 = v.f32_Temp;   \
    if(v.f32_y0 >= v.f32_Max)     \
    {stCtrl2p2zBs1.f32_y0 = stCtrl2p2zBs1.f32_Max;}  \
    else if(v.f32_y0 <= v.f32_Min)                  \
    {v.f32_y0 = v.f32_Min;}

#define CTRL_PI(v)          \
    v.f32_Up =  (v.f32_Err - v.f32_Err0) * v.f32_Kp; \
    v.f32_Ui = v.f32_Err * v.f32_Ki;                 \
    v.f32_Uo += v.f32_Up + v.f32_Ui;                 \
    v.f32_Err0 = v.f32_Err;                          \
    if(v.f32_Uo >= v.f32_Max)                        \
    {v.f32_Uo = v.f32_Max;}                          \
    else if(v.f32_Uo <= v.f32_Min)                   \
      {v.f32_Uo = v.f32_Min;}

extern void sSampelCalculation(void);
extern void sPfcControllerInit(void);
extern void sPfcControllerReset(void);
extern void sPfcController(void);
extern CTRL_PI_ST   stCtrlPiBus;
extern CMPR_ST      stPwmCmpr1;
extern CMPR_ST      stPwmCmpr2;
extern CTRL_2P2Z_ST stCtrl2p2zAs1;    //Grid current loop
extern CTRL_2P2Z_ST stCtrl2p2zBs1;
extern CTRL_2P2Z_ST stCtrl2p2zAs2;
extern CTRL_2P2Z_ST stCtrl2p2zBs2;
extern CTRL_2P2Z_ST stCtrl2p2zZs2;
//CTRL_2P2Z_ST stCtrl2p2zBus;

extern CTRL_PI_ST   stCtrlPiPll;
extern CLARK_ST     stClarkGridVolt;
extern CLARK_ST     stClarkGridCurr1;
extern CLARK_ST     stClarkGridCurr2;
extern CLARK_ST     stClarkGridCurr;
extern CLARK_ST     stClarkCapCurr;
extern SVPWM_ST     stSvPwm1;
extern SVPWM_ST     stSvPwm2;
extern PLL_ST       stDqPll;
#endif







