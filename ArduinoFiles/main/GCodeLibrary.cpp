#ifndef GCodeLibrary
#define GCodeLibrary
#include "Arduino.h"

/* Indices for characters:
	0 - 25: A - Z
	26 - 51: a - z
	52 - 61: 0 - 9
	62: ,
	63: !
	64: .
	65: ?
	*/

inline const double LINE_HEIGHT = 60;
inline const double MAX_DESCENDER = -15;

inline const double CHARACTER_WIDTHS[67] {
	40, //A
	38, //B
	42, //C
	40, //D
	36, //E
	36, //F
	36, //G
	36, //H
	22, //I
	32, //J
	36, //K
	34, //L
	45, //M
	38, //N
	43, //O
	35, //P
	45, //Q
	36, //R
	36, //S
	32, //T
	38, //U
	40, //V
	51, //W
	33, //X
	33, //Y
	40, //Z
	32, //a
	34, //b
	34, //c
	32, //d
	32, //e
	28, //f
	32, //g
	32, //h
	25, //i
	28, //j
	34, //k
	25, //l
	45, //m
	36, //n
	32, //o
	36, //p
	36, //q
	28, //r
	32, //s
	28, //t
	36, //u
	36, //v
	45, //w
	35, //x
	36, //y
	36, //z
	36, //0
	28, //1
	36, //2
	36, //3
	36, //4
	36, //5
	36, //6
	36, //7
	36, //8
	36, //9
	12, //,
	12, //!
	12, //.
	28, //?
	28 //(space)
};

inline const char* GRBL_SETTINGS[33] = {
	"$0=10", //step pulse time, microseconds
	"$1=25", //step idle delay, microseconds
	"$2=7", //step pulse invert, mask
	"$3=0", //step directino invert, mask
	"$4=1", //invert step enable pin, boolean
	"$5=0", //invert limit pins, boolean
	"$6=0", //invert probe pin, boolean
	"$10=1", //status report options, mask
	"$11=0.01", //junction deviation, mm
	"$12=0.002", //arc tolerance, mm
	"$13=0", //report in inches, boolean
	"$20=0", //soft limits enable, boolean
	"$21=0", //hard limits enable, boolean
	"$22=0", //homing cycle enable, booleanm
	"$23=0", //homing direction invert, mask
	"$24=25", //homing locate feed rate, mm/min
	"$25=500", //homing search seek rate, mm/min
	"$26=250", //homing switch debounce delay, ms
	"$27=1", //homing switch pull-off distance, mm
	"$30=1000", //maximum spindle speed, RPM
	"$31=0", //minimum spindle speed, RPM
	"$100=106.667", //x-axis travel resolution, steps/mm
	"$101=106.667", //y-axis travel resolution, steps/mm
	"$102=250", //z-axis travel resolution, steps/mm
	"$110=15000", //x-axis maximum rate, mm/min
	"$111=15000", //y-axis maximum rate, mm/min
	"$112=7000", //z-axis maximum rate, mm/min
	"$120=800", //x-axis acceleration, mm/s^2
	"$121=800", //y-axis acceleration, mm/s^2
	"$122=500", //z-axis acceleration, mm/s^2
	"$130=200", //x-axis maximum travel, mm
	"$131=200", //y-axis maximum travel, mm
	"$132=200" //z-axis maximum travel, mm
};

inline const char* GC_CAP_A[] = {
	"G00 Z0.30",
	"G00 X1.933440 Y0.662080",
	"G01 Z-0.30 F15000",
	"G01 X14.653440 Y34.262080 Z-0.30 F15000",
	"G01 X27.373440 Y0.662080 Z-0.30",
	"G00 Z0.30",
	"G00 X5.629440 Y10.454080",
	"G01 Z-0.30 F15000",
	"G02 X13.873542 Y10.457450 Z-0.30 I4.307003 J-452.408164 F15000",
	"G03 X23.666880 Y10.454080 Z-0.30 I5.098298 J585.844701",
	"G00 Z0.30",
	"END"
};

inline const char* GC_CAP_B[] = {
	"G00 Z0.30",
	"G00 X4.285440 Y34.262080",
	"G01 Z-0.30 F15000",
	"G01 X4.285440 Y0.662080 Z-0.30 F15000",
	"G01 X13.792800 Y0.662080 Z-0.30",
	"G03 X22.283018 Y8.049168 Z-0.30 I0.000000 J8.572585",
	"G03 X13.792800 Y18.806080 Z-0.30 I-9.397063 J1.312120",
	"G01 X4.258560 Y18.806080 Z-0.30",
	"G00 Z0.30",
	"G00 X13.792800 Y18.806080",
	"G01 Z-0.30 F15000",
	"G03 X19.955667 Y25.509402 Z-0.30 I-2.323871 J8.321163 F15000",
	"G03 X16.476960 Y32.886400 Z-0.30 I-6.808756 J1.297964",
	"G03 X13.159119 Y33.937147 Z-0.30 I-4.328986 J-7.905631",
	"G03 X4.285440 Y34.262080 Z-0.30 I-6.575543 J-58.243936",
	"G00 Z0.30",
	"END"
};

inline const char* GC_CAP_C[] = {
	"G00 Z0.30",
	"G00 X29.904480 Y30.382240",
	"G01 Z-0.30 F15000",
	"G03 X13.118062 Y33.681924 Z-0.30 I-10.870257 J-10.951596 F15000",
	"G03 X2.339040 Y14.758720 Z-0.30 I6.813401 J-16.412616",
	"G03 X12.957576 Y1.861609 Z-0.30 I15.896949 J2.268565",
	"G03 X30.397440 Y4.694080 Z-0.30 I6.102353 J17.532981",
	"G00 Z0.30",
	"END"
};

inline const char* GC_CAP_D[] = {
	"G00 Z0.30",
	"G00 X4.285440 Y34.262080",
	"G01 Z-0.30 F15000",
	"G01 X4.285440 Y0.662080 Z-0.30 F15000",
	"G01 X8.487840 Y0.662080 Z-0.30",
	"G03 X17.110086 Y1.998761 Z-0.30 I-0.000000 J28.477190",
	"G03 X19.975200 Y3.388000 Z-0.30 I-3.362057 J10.582850",
	"G03 X26.949401 Y13.717872 Z-0.30 I-8.303465 J13.125323",
	"G03 X19.975200 Y31.231840 Z-0.30 I-17.456727 J3.194168",
	"G03 X15.078923 Y33.644445 Z-0.30 I-8.463565 J-11.001745",
	"G03 X10.353120 Y34.262080 Z-0.30 I-4.725803 J-17.770802",
	"G01 X4.285440 Y34.262080 Z-0.30",
	"G00 Z0.30",
	"END"
};

inline const char* GC_CAP_E[] = {
	"G00 Z0.30",
	"G00 X22.430880 Y0.662080",
	"G01 Z-0.30 F15000",
	"G01 X4.237440 Y0.662080 Z-0.30 F15000",
	"G01 X4.237440 Y34.262080 Z-0.30",
	"G01 X22.429440 Y34.262080 Z-0.30",
	"G00 Z0.30",
	"G00 X4.237440 Y16.886080",
	"G01 Z-0.30 F15000",
	"G02 X13.422598 Y16.881729 Z-0.30 I4.321145 J-572.984649 F15000",
	"G03 X20.557440 Y16.886080 Z-0.30 I3.328323 J392.076228",
	"G00 Z0.30",
	"END"
};

inline const char* GC_CAP_F[] = {
	"G00 Z0.30",
	"G00 X4.237440 Y16.886080",
	"G01 Z-0.30 F15000",
	"G02 X12.019678 Y16.887829 Z-0.30 I3.983312 J-410.281726 F15000",
	"G03 X20.557440 Y16.886080 Z-0.30 I4.365461 J471.469041",
	"G00 Z0.30",
	"G00 X4.237440 Y0.662080",
	"G01 Z-0.30 F15000",
	"G01 X4.237440 Y34.262080 Z-0.30 F15000",
	"G01 X22.429440 Y34.262080 Z-0.30",
	"G00 Z0.30",
	"END"
};

inline const char* GC_CAP_G[] = {
	"G00 Z0.30",
	"G00 X19.261440 Y17.846080",
	"G01 Z-0.30 F15000",
	"G01 X30.253440 Y17.846080 Z-0.30 F15000",
	"G01 X30.253440 Y4.838080 Z-0.30",
	"G02 X21.646544 Y1.411124 Z-0.30 I-13.735771 J21.976068",
	"G02 X8.279520 Y4.838080 Z-0.30 I-3.138068 J15.542672",
	"G02 X2.716578 Y14.867872 Z-0.30 I10.413359 J12.333300",
	"G02 X8.279520 Y30.751360 Z-0.30 I18.033176 J2.600074",
	"G02 X16.592194 Y34.944048 Z-0.30 I11.267101 J-12.001884",
	"G02 X30.560160 Y30.538720 Z-0.30 I2.813954 J-15.424570",
	"G00 Z0.30",
	"END"
};

inline const char* GC_CAP_H[] = {
	"G00 Z0.30",
	"G00 X4.285440 Y34.262080",
	"G01 Z-0.30 F15000",
	"G03 X4.295161 Y13.223596 Z-0.30 I2550.076410 J-9.340915 F15000",
	"G02 X4.285440 Y0.662080 Z-0.30 I-1171.556819 J-5.374119",
	"G00 Z0.30",
	"G00 X4.285440 Y17.462080",
	"G01 Z-0.30 F15000",
	"G02 X13.998914 Y17.508334 Z-0.30 I9.713474 J-1019.914738 F15000",
	"G02 X24.637440 Y17.462080 Z-0.30 I0.000000 J-1223.476778",
	"G00 Z0.30",
	"G00 X24.637440 Y34.262080",
	"G01 Z-0.30 F15000",
	"G03 X24.647161 Y13.223596 Z-0.30 I2550.076410 J-9.340915 F15000",
	"G02 X24.637440 Y0.662080 Z-0.30 I-1171.556819 J-5.374119",
	"G00 Z0.30",
	"END"
};

inline const char* GC_CAP_I[] = {
	"G00 Z0.30",
	"G00 X4.610400 Y34.262080",
	"G01 Z-0.30 F15000",
	"G03 X4.571323 Y17.462152 Z-0.30 I16986.326298 J-47.910150 F15000",
	"G03 X4.610400 Y0.662080 Z-0.30 I2020.484421 J-3.700481",
	"G00 Z0.30",
	"END"
};

inline const char* GC_CAP_J[] = {
	"G00 Z0.30",
	"G00 X22.093440 Y34.262080",
	"G01 Z-0.30 F15000",
	"G01 X22.093440 Y13.839520 Z-0.30 F15000",
	"G02 X9.731278 Y1.192273 Z-0.30 I-12.650534 J0.000000",
	"G02 X1.425120 Y9.311200 Z-0.30 I-0.185121 J8.118927",
	"G00 Z0.30",
	"END"
};

inline const char* GC_CAP_K[] = {
	"G00 Z0.30",
	"G00 X4.285440 Y34.262080",
	"G01 Z-0.30 F15000",
	"G02 X4.287333 Y18.438817 Z-0.30 I-1980.086257 J-8.149055 F15000",
	"G03 X4.285440 Y0.662080 Z-0.30 I2357.977798 J-9.138753",
	"G00 Z0.30",
	"G00 X13.272000 Y19.516000",
	"G01 Z-0.30 F15000",
	"G02 X20.558089 Y10.090690 Z-0.30 I-848.765643 J-663.653783 F15000",
	"G03 X27.853440 Y0.662080 Z-0.30 I792.790253 J605.881569",
	"G00 Z0.30",
	"G00 X27.805440 Y34.262080",
	"G01 Z-0.30 F15000",
	"G01 X4.285440 Y10.406080 Z-0.30 F15000",
	"G00 Z0.30",
	"END"
};

inline const char* GC_CAP_L[] = {
	"G00 Z0.30",
	"G00 X4.285440 Y34.262080",
	"G01 Z-0.30 F15000",
	"G01 X4.285440 Y0.662080 Z-0.30 F15000",
	"G01 X22.189440 Y0.662080 Z-0.30",
	"G00 Z0.30",
	"END"
};

inline const char* GC_CAP_M[] = {
	"G00 Z0.30",
	"G00 X4.285440 Y0.662080",
	"G01 Z-0.30 F15000",
	"G01 X4.285440 Y34.262080 Z-0.30 F15000",
	"G01 X17.773440 Y15.328480 Z-0.30",
	"G01 X31.261440 Y34.262080 Z-0.30",
	"G01 X31.213440 Y0.662080 Z-0.30",
	"G00 Z0.30",
	"END"
};

inline const char* GC_CAP_N[] = {
	"G00 Z0.30",
	"G00 X4.285440 Y0.662080",
	"G01 Z-0.30 F15000",
	"G01 X4.285440 Y34.262080 Z-0.30 F15000",
	"G01 X27.565440 Y0.662080 Z-0.30",
	"G01 X27.565440 Y34.262080 Z-0.30",
	"G00 Z0.30",
	"END"
};

inline const char* GC_CAP_O[] = {
	"G00 Z0.30",
	"G00 X18.253440 Y34.742080",
	"G01 Z-0.30 F15000",
	"G03 X6.834541 Y29.806881 Z-0.30 I0.000000 J-15.677931 F15000",
	"G03 X1.885440 Y17.414080 Z-0.30 I13.269653 J-12.483894",
	"G03 X6.850372 Y4.970458 Z-0.30 I18.304448 J0.091067",
	"G03 X18.253440 Y0.038080 Z-0.30 I11.403068 J10.715075",
	"G03 X29.663494 Y4.969184 Z-0.30 I0.000000 J15.666382",
	"G03 X34.669440 Y17.414080 Z-0.30 I-13.422019 J12.628259",
	"G03 X29.679316 Y29.808152 Z-0.30 I-18.342339 J-0.183423",
	"G03 X18.253440 Y34.742080 Z-0.30 I-11.425876 J-10.762926",
	"G00 Z0.30",
	"END"
};

inline const char* GC_CAP_P[] = {
	"G00 Z0.30",
	"G00 X4.285440 Y0.662080",
	"G01 Z-0.30 F15000",
	"G01 X4.285440 Y16.201600 Z-0.30 F15000",
	"G01 X4.285440 Y34.262080 Z-0.30",
	"G01 X11.240160 Y34.262080 Z-0.30",
	"G02 X20.225233 Y30.523877 Z-0.30 I-0.000000 J-12.667274",
	"G02 X22.372080 Y25.262080 Z-0.30 I-5.239105 J-5.206451",
	"G02 X20.145658 Y19.607410 Z-0.30 I-8.454991 J0.063356",
	"G02 X12.533280 Y16.262080 Z-0.30 I-7.612378 J6.988407",
	"G01 X4.284960 Y16.262080 Z-0.30",
	"G00 Z0.30",
	"END"
};

inline const char* GC_CAP_Q[] = {
	"G00 Z0.30",
	"G00 X37.069440 Y-2.649920",
	"G01 Z-0.30 F15000",
	"G02 X33.271781 Y-4.753209 Z-0.30 I-8.063872 J10.079840 F15000",
	"G02 X30.397440 Y-5.241920 Z-0.30 I-2.874341 J8.208326",
	"G02 X26.580882 Y-4.542468 Z-0.30 I0.000000 J10.762242",
	"G02 X22.621440 Y-2.649920 Z-0.30 I9.344669 J24.638286",
	"G03 X19.907119 Y-1.546895 Z-0.30 I-4.473234 J-7.116508",
	"G03 X17.629440 Y-1.305920 Z-0.30 I-2.277679 J-10.643768",
	"G03 X28.630325 Y4.791124 Z-0.30 I-9.144053 J29.471557",
	"G03 X34.525440 Y17.654080 Z-0.30 I-11.085726 J12.862956",
	"G03 X29.688569 Y29.859974 Z-0.30 I-17.819285 J-0.000000",
	"G03 X18.397440 Y34.742080 Z-0.30 I-11.291129 J-10.615770",
	"G03 X7.250959 Y29.914109 Z-0.30 I0.000000 J-15.281094",
	"G03 X2.365440 Y17.654080 Z-0.30 I13.171478 J-12.352157",
	"G03 X7.424265 Y5.460851 Z-0.30 I17.439576 J0.089434",
	"G03 X18.829440 Y0.710080 Z-0.30 I11.405175 J11.314816",
	"G00 Z0.30",
	"END"
};

inline const char* GC_CAP_R[] = {
	"G00 Z0.30",
	"G00 X3.565440 Y15.974080",
	"G01 Z-0.30 F15000",
	"G01 X14.004000 Y15.974080 Z-0.30 F15000",
	"G03 X22.763894 Y25.356606 Z-0.30 I0.000000 J8.780553",
	"G03 X13.225440 Y34.262080 Z-0.30 I-9.538454 J-0.655475",
	"G01 X3.565440 Y34.262080 Z-0.30",
	"G01 X3.565440 Y0.662080 Z-0.30",
	"G00 Z0.30",
	"G00 X11.433600 Y15.974080",
	"G01 Z-0.30 F15000",
	"G02 X19.979106 Y10.878865 Z-0.30 I-0.000000 J-9.713711 F15000",
	"G02 X22.141440 Y3.468160 Z-0.30 I-16.169054 J-8.738710",
	"G03 X23.138354 Y1.331996 Z-0.30 I3.581166 J0.370569",
	"G03 X24.979680 Y0.662080 Z-0.30 I1.611100 J1.562773",
	"G00 Z0.30",
	"END"
};

inline const char* GC_CAP_S[] = {
	"G00 Z0.30",
	"G00 X24.480480 Y29.960800",
	"G01 Z-0.30 F15000",
	"G03 X11.574702 Y34.474375 Z-0.30 I-10.602645 J-9.608713 F15000",
	"G03 X4.538400 Y26.197600 Z-0.30 I1.349818 J-8.276775",
	"G03 X11.749902 Y18.704334 Z-0.30 I7.498771 J0.000000",
	"G02 X21.686400 Y16.397440 Z-0.30 I-1.035636 J-27.014021",
	"G02 X24.975298 Y6.705162 Z-0.30 I-2.825924 J-6.363078",
	"G02 X14.592000 Y0.535840 Z-0.30 I-10.383298 J5.653160",
	"G02 X7.795551 Y1.985417 Z-0.30 I-0.000000 J16.657621",
	"G02 X2.140800 Y5.747680 Z-0.30 I8.345359 J18.673956",
	"G00 Z0.30",
	"END"
};

inline const char* GC_CAP_T[] = {
	"G00 Z0.30",
	"G00 X1.405440 Y34.262080",
	"G01 Z-0.30 F15000",
	"G03 X13.834000 Y34.219223 Z-0.30 I12.428562 J1802.141270 F15000",
	"G03 X25.405440 Y34.262080 Z-0.30 I-0.000001 J1562.187192",
	"G00 Z0.30",
	"G00 X13.405440 Y34.262080",
	"G01 Z-0.30 F15000",
	"G03 X13.406248 Y17.093662 Z-0.30 I1978.427745 J-8.485512 F15000",
	"G02 X13.405440 Y0.662080 Z-0.30 I-1852.433606 J-8.129833",
	"G00 Z0.30",
	"END"
};

inline const char* GC_CAP_U[] = {
	"G00 Z0.30",
	"G00 X4.285440 Y34.262080",
	"G01 Z-0.30 F15000",
	"G01 X4.285440 Y14.115040 Z-0.30 F15000",
	"G03 X16.994416 Y1.275709 Z-0.30 I12.840000 J0.000000",
	"G03 X29.965440 Y14.115040 Z-0.30 I0.131024 J12.839331",
	"G01 X29.965440 Y34.262080 Z-0.30",
	"G00 Z0.30",
	"END"
};

inline const char* GC_CAP_V[] = {
	"G00 Z0.30",
	"G00 X1.885440 Y34.262080",
	"G01 Z-0.30 F15000",
	"G01 X15.229440 Y0.662080 Z-0.30 F15000",
	"G01 X28.573440 Y34.262080 Z-0.30",
	"G00 Z0.30",
	"END"
};

inline const char* GC_CAP_W[] = {
	"G00 Z0.30",
	"G00 X48.088800 Y34.262080",
	"G01 Z-0.30 F15000",
	"G01 X34.744800 Y0.662080 Z-0.30 F15000",
	"G01 X25.213440 Y24.662080 Z-0.30",
	"G01 X15.709440 Y0.662080 Z-0.30",
	"G01 X2.365440 Y34.262080 Z-0.30",
	"G00 Z0.30",
	"END"
};

inline const char* GC_CAP_X[] = {
	"G00 Z0.30",
	"G00 X2.845440 Y34.262080",
	"G01 Z-0.30 F15000",
	"G03 X15.254190 Y17.463836 Z-0.30 I2130.853021 J1561.064724 F15000",
	"G02 X27.672960 Y0.662080 Z-0.30 I-2284.570704 J-1701.597537",
	"G00 Z0.30",
	"G00 X2.845440 Y0.662080",
	"G01 Z-0.30 F15000",
	"G03 X15.254216 Y17.460307 Z-0.30 I-2652.589194 J1972.436841 F15000",
	"G02 X27.672960 Y34.262080 Z-0.30 I2446.957169 J-1795.635207",
	"G00 Z0.30",
	"END"
};

inline const char* GC_CAP_Y[] = {
	"G00 Z0.30",
	"G00 X14.172480 Y0.662080",
	"G01 Z-0.30 F15000",
	"G03 X14.173095 Y9.043605 Z-0.30 I-565.130210 J4.231870 F15000",
	"G02 X14.172480 Y17.425120 Z-0.30 I576.546367 J4.233485",
	"G00 Z0.30",
	"G00 X1.933440 Y34.262080",
	"G01 Z-0.30 F15000",
	"G01 X14.172480 Y17.425120 Z-0.30 F15000",
	"G01 X26.412480 Y34.262080 Z-0.30",
	"G00 Z0.30",
	"END"
};

inline const char* GC_CAP_Z[] = {
	"G00 Z0.30",
	"G00 X2.605440 Y34.262080",
	"G01 Z-0.30 F15000",
	"G01 X26.797440 Y34.262080 Z-0.30 F15000",
	"G01 X2.605440 Y0.662080 Z-0.30",
	"G01 X26.797440 Y0.662080 Z-0.30",
	"G00 Z0.30",
	"END"
};

inline const char* GC_LOW_A[] = {
	"G00 Z0.30",
	"G00 X22.813440 Y24.662080",
	"G01 Z-0.30 F15000",
	"G02 X22.833192 Y17.995231 Z-0.30 I-321.431083 J-4.285747 F15000",
	"G03 X22.813440 Y0.662080 Z-0.30 I1382.710165 J-10.242231",
	"G00 Z0.30",
	"G00 X22.813440 Y16.780000",
	"G01 Z-0.30 F15000",
	"G03 X18.996783 Y22.413695 Z-0.30 I-10.470817 J-2.983977 F15000",
	"G03 X12.406080 Y24.662080 Z-0.30 I-6.590703 J-8.535491",
	"G03 X4.667825 Y21.345017 Z-0.30 I-0.000000 J-10.684676",
	"G03 X1.165440 Y12.587200 Z-0.30 I9.198396 J-8.757817",
	"G03 X4.683363 Y3.924100 Z-0.30 I12.425667 J0.000000",
	"G03 X12.406080 Y0.662080 Z-0.30 I7.722717 J7.510619",
	"G03 X18.998034 Y2.869489 Z-0.30 I0.000000 J10.946433",
	"G03 X22.813440 Y8.468800 Z-0.30 I-6.362795 J8.435226",
	"G00 Z0.30",
	"END"
};

inline const char* GC_LOW_B[] = {
	"G00 Z0.30",
	"G00 X3.661440 Y0.662080",
	"G01 Z-0.30 F15000",
	"G02 X3.680409 Y23.645994 Z-0.30 I2164.372955 J9.705712 F15000",
	"G03 X3.661440 Y34.262080 Z-0.30 I-670.062990 J4.110803",
	"G00 Z0.30",
	"G00 X3.661440 Y8.468800",
	"G01 Z-0.30 F15000",
	"G03 X7.476846 Y2.869489 Z-0.30 I10.178202 J2.835915 F15000",
	"G03 X14.068800 Y0.662080 Z-0.30 I6.591954 J8.739024",
	"G03 X21.791496 Y3.924080 Z-0.30 I-0.000000 J10.772636",
	"G03 X25.309440 Y12.587200 Z-0.30 I-8.907719 J8.663120",
	"G03 X21.807274 Y21.344803 Z-0.30 I-12.700820 J-0.000000",
	"G03 X14.068800 Y24.662080 Z-0.30 I-7.738474 J-7.367434",
	"G03 X7.477602 Y22.413528 Z-0.30 I-0.000000 J-10.784690",
	"G03 X3.661440 Y16.780000 Z-0.30 I6.652804 J-8.615923",
	"G00 Z0.30",
	"END"
};

inline const char* GC_LOW_C[] = {
	"G00 Z0.30",
	"G00 X21.973920 Y21.088960",
	"G01 Z-0.30 F15000",
	"G03 X18.532416 Y23.652087 Z-0.30 I-7.572501 J-6.575566 F15000",
	"G03 X13.846080 Y24.662080 Z-0.30 I-4.686336 J-10.367227",
	"G03 X5.830990 Y21.394254 Z-0.30 I-0.000000 J-11.463333",
	"G03 X2.125440 Y12.587200 Z-0.30 I8.613174 J-8.807054",
	"G03 X5.845981 Y3.874325 Z-0.30 I12.062309 J0.000000",
	"G03 X13.846080 Y0.662080 Z-0.30 I8.000099 J8.356007",
	"G03 X18.555130 Y1.659111 Z-0.30 I0.000000 J11.619108",
	"G03 X21.973920 Y4.184320 Z-0.30 I-3.966354 J8.946798",
	"G00 Z0.30",
	"END"
};

inline const char* GC_LOW_D[] = {
	"G00 Z0.30",
	"G00 X23.773440 Y34.262080",
	"G01 Z-0.30 F15000",
	"G03 X23.765287 Y20.772383 Z-0.30 I1219.561069 J-7.481965 F15000",
	"G02 X23.773440 Y0.662080 Z-0.30 I-2224.259526 J-10.956948",
	"G00 Z0.30",
	"G00 X23.773440 Y16.780000",
	"G01 Z-0.30 F15000",
	"G03 X19.956783 Y22.413695 Z-0.30 I-10.470817 J-2.983977 F15000",
	"G03 X13.366080 Y24.662080 Z-0.30 I-6.590703 J-8.535491",
	"G03 X5.627825 Y21.345017 Z-0.30 I0.000000 J-10.684676",
	"G03 X2.125440 Y12.587200 Z-0.30 I9.198396 J-8.757817",
	"G03 X5.643363 Y3.924100 Z-0.30 I12.425667 J0.000000",
	"G03 X13.366080 Y0.662080 Z-0.30 I7.722717 J7.510619",
	"G03 X19.958034 Y2.869489 Z-0.30 I-0.000000 J10.946433",
	"G03 X23.773440 Y8.468800 Z-0.30 I-6.362795 J8.435226",
	"G00 Z0.30",
	"END"
};

inline const char* GC_LOW_E[] = {
	"G00 Z0.30",
	"G00 X2.952000 Y12.872800",
	"G01 Z-0.30 F15000",
	"G01 X25.612800 Y12.872800 Z-0.30 F15000",
	"G03 X21.960656 Y21.585057 Z-0.30 I-12.862354 J-0.270238",
	"G03 X14.661600 Y24.662080 Z-0.30 I-7.299056 J-7.118592",
	"G03 X6.646510 Y21.394254 Z-0.30 I-0.000000 J-11.463333",
	"G03 X2.940960 Y12.587200 Z-0.30 I8.613174 J-8.807054",
	"G03 X6.661501 Y3.874325 Z-0.30 I12.062309 J-0.000000",
	"G03 X14.661600 Y0.662080 Z-0.30 I8.000099 J8.356007",
	"G03 X20.279206 Y1.594554 Z-0.30 I0.000000 J17.387627",
	"G03 X25.191360 Y6.288640 Z-0.30 I-2.611715 J7.650263",
	"G00 Z0.30",
	"END"
};

inline const char* GC_LOW_F[] = {
	"G00 Z0.30",
	"G00 X3.469440 Y24.662080",
	"G01 Z-0.30 F15000",
	"G01 X13.165440 Y24.662080 Z-0.30 F15000",
	"G00 Z0.30",
	"G00 X13.608960 Y33.888160",
	"G01 Z-0.30 F15000",
	"G03 X9.938014 Y33.439871 Z-0.30 I-1.495835 J-3.005369 F15000",
	"G03 X7.933440 Y29.103040 Z-0.30 I3.689008 J-4.336831",
	"G01 X7.933440 Y0.662080 Z-0.30",
	"G00 Z0.30",
	"END"
};

inline const char* GC_LOW_G[] = {
	"G00 Z0.30",
	"G00 X23.533440 Y24.662080",
	"G01 Z-0.30 F15000",
	"G01 X23.533440 Y0.662080 Z-0.30 F15000",
	"G02 X18.498099 Y-7.842930 Z-0.30 I-9.700421 J0.000000",
	"G02 X8.077440 Y-7.557920 Z-0.30 I-4.966577 J9.054677",
	"G00 Z0.30",
	"G00 X23.533440 Y16.780000",
	"G01 Z-0.30 F15000",
	"G03 X19.716783 Y22.413695 Z-0.30 I-10.470817 J-2.983977 F15000",
	"G03 X13.126080 Y24.662080 Z-0.30 I-6.590703 J-8.535491",
	"G03 X5.387825 Y21.345017 Z-0.30 I0.000000 J-10.684676",
	"G03 X1.885440 Y12.587200 Z-0.30 I9.198396 J-8.757817",
	"G03 X5.403363 Y3.924100 Z-0.30 I12.425667 J0.000000",
	"G03 X13.126080 Y0.662080 Z-0.30 I7.722717 J7.510619",
	"G03 X19.718034 Y2.869489 Z-0.30 I0.000000 J10.946433",
	"G03 X23.533440 Y8.468800 Z-0.30 I-6.362795 J8.435226",
	"G00 Z0.30",
	"END"
};

inline const char* GC_LOW_H[] = {
	"G00 Z0.30",
	"G00 X3.373440 Y34.262080",
	"G01 Z-0.30 F15000",
	"G02 X3.379699 Y11.377962 Z-0.30 I-7936.254975 J-13.613850 F15000",
	"G03 X3.373440 Y0.662080 Z-0.30 I9172.798341 J-10.712850",
	"G00 Z0.30",
	"G00 X3.373440 Y15.062080",
	"G01 Z-0.30 F15000",
	"G02 X12.539568 Y24.262925 Z-0.30 I9.166128 J0.034651 F15000",
	"G02 X21.696000 Y15.062080 Z-0.30 I0.000000 J-9.156539",
	"G01 X21.696000 Y0.662080 Z-0.30",
	"G00 Z0.30",
	"END"
};

inline const char* GC_LOW_I[] = {
	"G00 Z0.30",
	"G00 X4.861440 Y24.662080",
	"G01 Z-0.30 F15000",
	"G03 X4.871679 Y9.487957 Z-0.30 I1313.368239 J-6.700861 F15000",
	"G02 X4.861440 Y0.662080 Z-0.30 I-579.758162 J-3.740369",
	"G00 Z0.30",
	"G00 X4.861440 Y31.317760",
	"G01 Z-0.30 F15000",
	"G03 X4.475462 Y31.128398 Z-0.30 I0.000000 J-0.488053 F15000",
	"G03 X4.285920 Y30.573760 Z-0.30 I0.716721 J-0.554638",
	"G03 X4.475237 Y30.019347 Z-0.30 I0.906457 J-0.000000",
	"G03 X4.861440 Y29.829760 Z-0.30 I0.386203 J0.298570",
	"G03 X5.247418 Y30.019122 Z-0.30 I0.000000 J0.488053",
	"G03 X5.436960 Y30.573760 Z-0.30 I-0.716721 J0.554638",
	"G03 X5.247418 Y31.128398 Z-0.30 I-0.906263 J-0.000000",
	"G03 X4.861440 Y31.317760 Z-0.30 I-0.385978 J-0.298691",
	"G00 Z0.30",
	"END"
};

inline const char* GC_LOW_J[] = {
	"G00 Z0.30",
	"G00 X3.037440 Y24.662080",
	"G01 Z-0.30 F15000",
	"G01 X3.037440 Y-5.531360 Z-0.30 F15000",
	"G02 X1.718522 Y-8.090242 Z-0.30 I-3.141750 J-0.000000",
	"G02 X-0.514560 Y-8.553920 Z-0.30 I-1.643831 J2.307603",
	"G00 Z0.30",
	"G00 X3.037440 Y31.317760",
	"G01 Z-0.30 F15000",
	"G03 X2.651462 Y31.128398 Z-0.30 I0.000000 J-0.488053 F15000",
	"G03 X2.461920 Y30.573760 Z-0.30 I0.716721 J-0.554638",
	"G03 X2.651237 Y30.019347 Z-0.30 I0.906457 J-0.000000",
	"G03 X3.037440 Y29.829760 Z-0.30 I0.386203 J0.298570",
	"G03 X3.423418 Y30.019122 Z-0.30 I0.000000 J0.488053",
	"G03 X3.612960 Y30.573760 Z-0.30 I-0.716721 J0.554638",
	"G03 X3.423418 Y31.128398 Z-0.30 I-0.906263 J-0.000000",
	"G03 X3.037440 Y31.317760 Z-0.30 I-0.385978 J-0.298691",
	"G00 Z0.30",
	"END"
};

inline const char* GC_LOW_K[] = {
	"G00 Z0.30",
	"G00 X3.805440 Y34.262080",
	"G01 Z-0.30 F15000",
	"G01 X3.805440 Y0.662080 Z-0.30 F15000",
	"G00 Z0.30",
	"G00 X10.045440 Y13.775680",
	"G01 Z-0.30 F15000",
	"G02 X15.468801 Y7.216969 Z-0.30 I-310.560332 J-262.322195 F15000",
	"G03 X20.881440 Y0.662080 Z-0.30 I337.736029 J273.370257",
	"G00 Z0.30",
	"G00 X20.881440 Y24.662080",
	"G01 Z-0.30 F15000",
	"G03 X12.318694 Y16.055788 Z-0.30 I1120.432216 J-1123.326555 F15000",
	"G02 X3.805440 Y7.506400 Z-0.30 I-1011.354126 J998.566791",
	"G00 Z0.30",
	"END"
};

inline const char* GC_LOW_L[] = {
	"G00 Z0.30",
	"G00 X4.573440 Y34.262080",
	"G01 Z-0.30 F15000",
	"G03 X4.511902 Y17.462080 Z-0.30 I2293.200000 J-16.800002 F15000",
	"G03 X4.573440 Y0.662080 Z-0.30 I2293.261538 J0.000002",
	"G00 Z0.30",
	"END"
};

inline const char* GC_LOW_M[] = {
	"G00 Z0.30",
	"G00 X3.325440 Y13.472800",
	"G01 Z-0.30 F15000",
	"G02 X6.576132 Y21.909252 Z-0.30 I12.572818 J-0.000000 F15000",
	"G02 X11.740260 Y24.569680 Z-0.30 I6.121272 J-5.539703",
	"G02 X16.593705 Y22.959954 Z-0.30 I0.701205 J-6.007428",
	"G02 X20.173440 Y14.675680 Z-0.30 I-7.839767 J-8.303218",
	"G01 X20.196960 Y0.662080 Z-0.30",
	"G00 Z0.30",
	"G00 X20.125440 Y13.472800",
	"G01 Z-0.30 F15000",
	"G02 X23.376132 Y21.909252 Z-0.30 I12.572818 J0.000000 F15000",
	"G02 X28.540260 Y24.569680 Z-0.30 I6.121272 J-5.539703",
	"G02 X33.393705 Y22.959954 Z-0.30 I0.701205 J-6.007428",
	"G02 X36.973440 Y14.675680 Z-0.30 I-7.839767 J-8.303218",
	"G01 X36.996960 Y0.662080 Z-0.30",
	"G00 Z0.30",
	"G00 X3.325440 Y24.662080",
	"G01 Z-0.30 F15000",
	"G03 X3.306035 Y16.368662 Z-0.30 I323.667236 J-4.904050 F15000",
	"G02 X3.325440 Y0.662080 Z-0.30 I-850.322358 J-8.903846",
	"G00 Z0.30",
	"END"
};

inline const char* GC_LOW_N[] = {
	"G00 Z0.30",
	"G00 X3.853440 Y13.472800",
	"G01 Z-0.30 F15000",
	"G02 X7.198937 Y21.844364 Z-0.30 I12.146991 J0.000000 F15000",
	"G02 X12.692040 Y24.569680 Z-0.30 I6.480984 J-6.164405",
	"G02 X17.866266 Y22.910537 Z-0.30 I0.734298 J-6.607784",
	"G02 X21.550080 Y14.675680 Z-0.30 I-7.405802 J-8.254336",
	"G01 X21.574560 Y0.662080 Z-0.30",
	"G00 Z0.30",
	"G00 X3.853440 Y24.662080",
	"G01 Z-0.30 F15000",
	"G02 X3.834162 Y13.705804 Z-0.30 I-485.506287 J-4.623870 F15000",
	"G03 X3.853440 Y0.662080 Z-0.30 I449.121937 J-5.858082",
	"G00 Z0.30",
	"END"
};

inline const char* GC_LOW_O[] = {
	"G00 Z0.30",
	"G00 X13.082880 Y24.662080",
	"G01 Z-0.30 F15000",
	"G02 X20.470677 Y21.775483 Z-0.30 I0.000000 J-10.897257 F15000",
	"G02 X24.252960 Y13.142080 Z-0.30 I-7.962119 J-8.633403",
	"G02 X20.567424 Y3.884271 Z-0.30 I-13.470254 J-0.000000",
	"G02 X13.082880 Y0.662080 Z-0.30 I-7.484544 J7.081500",
	"G02 X5.375733 Y4.102077 Z-0.30 I0.000000 J10.353745",
	"G02 X1.915200 Y13.142080 Z-0.30 I10.077405 J9.040003",
	"G02 X5.697861 Y21.776509 Z-0.30 I11.745949 J0.000000",
	"G02 X13.082880 Y24.662080 Z-0.30 I7.385019 J-8.007424",
	"G00 Z0.30",
	"END"
};

inline const char* GC_LOW_P[] = {
	"G00 Z0.30",
	"G00 X3.336000 Y24.662080",
	"G01 Z-0.30 F15000",
	"G03 X3.352420 Y2.914450 Z-0.30 I7017.463091 J-5.575676 F15000",
	"G02 X3.336000 Y-9.417920 Z-0.30 I-1695.861270 J-3.908204",
	"G00 Z0.30",
	"G00 X3.373440 Y8.468800",
	"G01 Z-0.30 F15000",
	"G03 X7.188846 Y2.869489 Z-0.30 I10.178202 J2.835915 F15000",
	"G03 X13.780800 Y0.662080 Z-0.30 I6.591954 J8.739024",
	"G03 X21.503496 Y3.924080 Z-0.30 I0.000000 J10.772636",
	"G03 X25.021440 Y12.587200 Z-0.30 I-8.907719 J8.663120",
	"G03 X21.519274 Y21.344803 Z-0.30 I-12.700820 J0.000000",
	"G03 X13.780800 Y24.662080 Z-0.30 I-7.738474 J-7.367434",
	"G03 X7.189602 Y22.413528 Z-0.30 I-0.000000 J-10.784690",
	"G03 X3.373440 Y16.780000 Z-0.30 I6.652804 J-8.615923",
	"G00 Z0.30",
	"END"
};

inline const char* GC_LOW_Q[] = {
	"G00 Z0.30",
	"G00 X23.570880 Y-9.417920",
	"G01 Z-0.30 F15000",
	"G02 X23.554460 Y2.914450 Z-0.30 I1695.844850 J8.424141 F15000",
	"G03 X23.570880 Y24.662080 Z-0.30 I-7017.446671 J16.172205",
	"G00 Z0.30",
	"G00 X23.533440 Y16.780000",
	"G01 Z-0.30 F15000",
	"G03 X19.716783 Y22.413695 Z-0.30 I-10.470817 J-2.983977 F15000",
	"G03 X13.126080 Y24.662080 Z-0.30 I-6.590703 J-8.535491",
	"G03 X5.387825 Y21.345017 Z-0.30 I0.000000 J-10.684676",
	"G03 X1.885440 Y12.587200 Z-0.30 I9.198396 J-8.757817",
	"G03 X5.403363 Y3.924100 Z-0.30 I12.425667 J0.000000",
	"G03 X13.126080 Y0.662080 Z-0.30 I7.722717 J7.510619",
	"G03 X19.718034 Y2.869489 Z-0.30 I0.000000 J10.946433",
	"G03 X23.533440 Y8.468800 Z-0.30 I-6.362795 J8.435226",
	"G00 Z0.30",
	"END"
};

inline const char* GC_LOW_R[] = {
	"G00 Z0.30",
	"G00 X3.369120 Y24.662080",
	"G01 Z-0.30 F15000",
	"G02 X3.379704 Y13.804835 Z-0.30 I-517.068967 J-5.932688 F15000",
	"G03 X3.369120 Y0.662080 Z-0.30 I753.730164 J-7.178369",
	"G00 Z0.30",
	"G00 X3.369120 Y13.835680",
	"G01 Z-0.30 F15000",
	"G02 X9.405510 Y23.895868 Z-0.30 I11.316487 J0.050890 F15000",
	"G02 X15.649920 Y23.714080 Z-0.30 I2.961420 J-5.613862",
	"G00 Z0.30",
	"END"
};

inline const char* GC_LOW_S[] = {
	"G00 Z0.30",
	"G00 X19.059840 Y21.584800",
	"G01 Z-0.30 F15000",
	"G03 X15.204360 Y24.288604 Z-0.30 I-6.019712 J-4.483031 F15000",
	"G03 X10.122240 Y24.741760 Z-0.30 I-3.638668 J-12.083044",
	"G03 X5.724947 Y23.060620 Z-0.30 I1.086052 J-9.432229",
	"G03 X3.868800 Y19.615840 Z-0.30 I2.540711 J-3.591471",
	"G03 X11.144707 Y13.249795 Z-0.30 I6.240884 J-0.208071",
	"G02 X18.843360 Y6.358720 Z-0.30 I1.096109 J-6.521410",
	"G02 X16.745318 Y2.361062 Z-0.30 I-4.861396 J0.001973",
	"G02 X11.840580 Y0.682480 Z-0.30 I-5.428698 J7.857405",
	"G02 X6.217170 Y1.608587 Z-0.30 I-0.722481 J13.149023",
	"G02 X1.925760 Y5.306560 Z-0.30 I3.253806 J8.114991",
	"G00 Z0.30",
	"END"
};

inline const char* GC_LOW_T[] = {
	"G00 Z0.30",
	"G00 X6.877440 Y31.862080",
	"G01 Z-0.30 F15000",
	"G01 X6.877440 Y4.769920 Z-0.30 F15000",
	"G03 X10.323832 Y0.784633 Z-0.30 I4.027419 J0.000000",
	"G03 X13.137120 Y2.721760 Z-0.30 I0.362328 J2.485223",
	"G00 Z0.30",
	"G00 X1.387680 Y24.662080",
	"G01 Z-0.30 F15000",
	"G02 X6.796400 Y24.666869 Z-0.30 I2.884071 J-202.952046 F15000",
	"G03 X13.745280 Y24.662080 Z-0.30 I3.678237 J295.693922",
	"G00 Z0.30",
	"END"
};

inline const char* GC_LOW_U[] = {
	"G00 Z0.30",
	"G00 X3.565440 Y24.662080",
	"G01 Z-0.30 F15000",
	"G01 X3.593280 Y10.618240 Z-0.30 F15000",
	"G03 X7.005447 Y2.859046 Z-0.30 I10.576207 J0.021110",
	"G03 X12.393600 Y0.676000 Z-0.30 I5.600651 J6.082430",
	"G03 X17.918029 Y2.831318 Z-0.30 I-0.223529 J8.730600",
	"G03 X21.462720 Y10.618240 Z-0.30 I-6.824591 J7.806880",
	"G01 X21.490560 Y24.662080 Z-0.30",
	"G00 Z0.30",
	"END"
};

inline const char* GC_LOW_V[] = {
	"G00 Z0.30",
	"G00 X1.405440 Y24.662080",
	"G01 Z-0.30 F15000",
	"G01 X10.957440 Y0.662080 Z-0.30 F15000",
	"G01 X20.509440 Y24.662080 Z-0.30",
	"G00 Z0.30",
	"END"
};

inline const char* GC_LOW_W[] = {
	"G00 Z0.30",
	"G00 X1.405440 Y24.662080",
	"G01 Z-0.30 F15000",
	"G01 X9.037440 Y0.662080 Z-0.30 F15000",
	"G01 X16.299360 Y20.359840 Z-0.30",
	"G01 X23.561280 Y0.662080 Z-0.30",
	"G01 X31.193280 Y24.662080 Z-0.30",
	"G00 Z0.30",
	"END"
};

inline const char* GC_LOW_X[] = {
	"G00 Z0.30",
	"G00 X1.383360 Y24.662080",
	"G01 Z-0.30 F15000",
	"G03 X10.553803 Y12.673555 Z-0.30 I685.639083 J514.967696 F15000",
	"G02 X19.790400 Y0.662080 Z-0.30 I-955.721912 J-744.489217",
	"G00 Z0.30",
	"G00 X1.383360 Y0.662080",
	"G01 Z-0.30 F15000",
	"G02 X10.619533 Y12.673408 Z-0.30 I962.454748 J-730.527838 F15000",
	"G03 X19.790400 Y24.662080 Z-0.30 I-677.747720 J527.952600",
	"G00 Z0.30",
	"END"
};

inline const char* GC_LOW_Y[] = {
	"G00 Z0.30",
	"G00 X3.325440 Y24.662080",
	"G01 Z-0.30 F15000",
	"G01 X3.353280 Y10.618240 Z-0.30 F15000",
	"G03 X6.765447 Y2.859046 Z-0.30 I10.576207 J0.021110",
	"G03 X12.153600 Y0.676000 Z-0.30 I5.600651 J6.082430",
	"G03 X17.678029 Y2.831318 Z-0.30 I-0.223529 J8.730600",
	"G03 X21.222720 Y10.618240 Z-0.30 I-6.824591 J7.806880",
	"G00 Z0.30",
	"G00 X3.325440 Y-6.481760",
	"G01 Z-0.30 F15000",
	"G03 X15.768789 Y-8.064292 Z-0.30 I7.463628 J8.974137 F15000",
	"G03 X21.300960 Y0.662080 Z-0.30 I-4.116345 J8.726372",
	"G01 X21.250560 Y24.662080 Z-0.30",
	"G00 Z0.30",
	"END"
};

inline const char* GC_LOW_Z[] = {
	"G00 Z0.30",
	"G00 X1.885440 Y24.662080",
	"G01 Z-0.30 F15000",
	"G01 X19.288320 Y24.662080 Z-0.30 F15000",
	"G01 X1.885440 Y0.662080 Z-0.30",
	"G01 X19.309440 Y0.662080 Z-0.30",
	"G00 Z0.30",
	"END"
};

inline const char* GC_NUM_0[] = {
	"G00 Z0.30",
	"G00 X13.221600 Y34.262080",
	"G01 Z-0.30 F15000",
	"G02 X20.238345 Y30.438887 Z-0.30 I0.000000 J-8.350548 F15000",
	"G02 X24.061440 Y17.462080 Z-0.30 I-20.112169 J-12.976807",
	"G02 X20.238345 Y4.485273 Z-0.30 I-23.935264 J0.000000",
	"G02 X13.221600 Y0.662080 Z-0.30 I-7.016745 J4.527356",
	"G02 X6.205323 Y4.485065 Z-0.30 I0.000000 J8.349936",
	"G02 X2.382240 Y17.462080 Z-0.30 I20.112951 J12.977015",
	"G02 X6.205323 Y30.439095 Z-0.30 I23.936034 J-0.000000",
	"G02 X13.221600 Y34.262080 Z-0.30 I7.016277 J-4.526950",
	"G00 Z0.30",
	"END"
};

inline const char* GC_NUM_1[] = {
	"G00 Z0.30",
	"G00 X0.000000 Y27.221920",
	"G01 Z-0.30 F15000",
	"G01 X9.805440 Y34.262080 Z-0.30 F15000",
	"G01 X9.805440 Y0.662080 Z-0.30",
	"G00 Z0.30",
	"END"
};

inline const char* GC_NUM_2[] = {
	"G00 Z0.30",
	"G00 X3.360960 Y26.938240",
	"G01 Z-0.30 F15000",
	"G02 X12.814287 Y33.293891 Z-0.30 I9.138816 J-3.384763 F15000",
	"G02 X21.065280 Y25.478080 Z-0.30 I-0.276226 J-8.554714",
	"G02 X16.563599 Y14.358381 Z-0.30 I-13.166656 J-1.140708",
	"G03 X3.948000 Y0.662080 Z-0.30 I54.904150 J-63.230191",
	"G01 X23.638080 Y0.662080 Z-0.30",
	"G00 Z0.30",
	"END"
};

inline const char* GC_NUM_3[] = {
	"G00 Z0.30",
	"G00 X1.902240 Y34.262080",
	"G01 Z-0.30 F15000",
	"G01 X19.741440 Y34.262080 Z-0.30 F15000",
	"G01 X7.182720 Y20.535520 Z-0.30",
	"G02 X15.770272 Y20.046760 Z-0.30 I3.146247 J-20.406580",
	"G02 X19.617757 Y17.754100 Z-0.30 I-2.206872 J-8.078225",
	"G02 X21.757591 Y13.569131 Z-0.30 I-5.941057 J-5.677294",
	"G02 X21.571260 Y8.830720 Z-0.30 I-10.593318 J-1.956303",
	"G02 X16.483408 Y2.586390 Z-0.30 I-9.489093 J2.536727",
	"G02 X3.194400 Y3.198400 Z-0.30 I-6.098267 J12.166847",
	"G00 Z0.30",
	"END"
};

inline const char* GC_NUM_4[] = {
	"G00 Z0.30",
	"G00 X17.389440 Y0.662080",
	"G01 Z-0.30 F15000",
	"G01 X17.389440 Y34.262080 Z-0.30 F15000",
	"G01 X1.411680 Y11.974240 Z-0.30",
	"G01 X23.082240 Y11.974240 Z-0.30",
	"G00 Z0.30",
	"END"
};

inline const char* GC_NUM_5[] = {
	"G00 Z0.30",
	"G00 X21.563040 Y34.262080",
	"G01 Z-0.30 F15000",
	"G01 X5.388960 Y34.262080 Z-0.30 F15000",
	"G01 X3.327360 Y18.935200 Z-0.30",
	"G02 X15.209777 Y21.185926 Z-0.30 I8.079433 J-10.163116",
	"G02 X20.244960 Y17.640160 Z-0.30 I-2.899931 J-9.466063",
	"G02 X21.171570 Y6.611515 Z-0.30 I-8.388655 J-6.258051",
	"G02 X17.389440 Y2.406880 Z-0.30 I-9.817954 J5.028035",
	"G02 X6.075518 Y1.275773 Z-0.30 I-6.745079 J10.318364",
	"G02 X2.388480 Y4.815520 Z-0.30 I2.519910 J6.314867",
	"G00 Z0.30",
	"END"
};

inline const char* GC_NUM_6[] = {
	"G00 Z0.30",
	"G00 X16.965120 Y34.262080",
	"G01 Z-0.30 F15000",
	"G03 X10.070341 Y28.842616 Z-0.30 I13.462490 J-24.222907 F15000",
	"G03 X6.199680 Y23.548960 Z-0.30 I21.102694 J-19.491970",
	"G03 X2.562880 Y12.974546 Z-0.30 I25.544719 J-14.698055",
	"G03 X5.876160 Y2.827360 Z-0.30 I12.001149 J-1.695883",
	"G03 X9.318176 Y0.852457 Z-0.30 I5.177927 J5.037537",
	"G03 X19.151040 Y2.503360 Z-0.30 I3.012098 J12.167763",
	"G03 X22.853424 Y6.933551 Z-0.30 I-5.329458 J8.216084",
	"G03 X20.365920 Y18.530560 Z-0.30 I-9.571721 J4.012195",
	"G03 X11.914949 Y21.343588 Z-0.30 I-7.293173 J-7.809510",
	"G03 X2.797920 Y14.887840 Z-0.30 I1.245155 J-11.424015",
	"G00 Z0.30",
	"END"
};

inline const char* GC_NUM_7[] = {
	"G00 Z0.30",
	"G00 X2.638080 Y34.262080",
	"G01 Z-0.30 F15000",
	"G01 X21.085440 Y34.262080 Z-0.30 F15000",
	"G01 X7.354080 Y0.564640 Z-0.30",
	"G00 Z0.30",
	"END"
};

inline const char* GC_NUM_8[] = {
	"G00 Z0.30",
	"G00 X11.965440 Y17.397280",
	"G01 Z-0.30 F15000",
	"G02 X6.187162 Y19.834923 Z-0.30 I0.000000 J8.067344 F15000",
	"G02 X3.749760 Y25.829440 Z-0.30 I6.152723 J5.994517",
	"G02 X6.187364 Y31.824160 Z-0.30 I8.590111 J0.000000",
	"G02 X11.965440 Y34.261600 Z-0.30 I5.778076 J-5.629892",
	"G02 X17.743516 Y31.824160 Z-0.30 I0.000000 J-8.067332",
	"G02 X20.181120 Y25.829440 Z-0.30 I-6.152507 J-5.994720",
	"G02 X17.743718 Y19.834923 Z-0.30 I-8.590124 J-0.000000",
	"G02 X11.965440 Y17.397280 Z-0.30 I-5.778278 J5.629701",
	"G00 Z0.30",
	"G00 X11.965440 Y17.413120",
	"G01 Z-0.30 F15000",
	"G02 X18.248083 Y14.913471 Z-0.30 I0.000000 J-9.145251 F15000",
	"G02 X20.820480 Y8.941600 Z-0.30 I-5.645708 J-5.971871",
	"G02 X18.262677 Y2.884124 Z-0.30 I-8.451660 J0.000000",
	"G02 X11.965440 Y0.326080 Z-0.30 I-6.297237 J6.472053",
	"G02 X5.668203 Y2.884124 Z-0.30 I0.000000 J9.030098",
	"G02 X3.110400 Y8.941600 Z-0.30 I5.893857 J6.057476",
	"G02 X5.682797 Y14.913471 Z-0.30 I8.218105 J0.000000",
	"G02 X11.965440 Y17.413120 Z-0.30 I6.282643 J-6.645602",
	"G00 Z0.30",
	"END"
};

inline const char* GC_NUM_9[] = {
	"G00 Z0.30",
	"G00 X7.968000 Y0.669760",
	"G01 Z-0.30 F15000",
	"G03 X14.862900 Y6.089254 Z-0.30 I-13.463163 J24.224116 F15000",
	"G03 X18.733440 Y11.382400 Z-0.30 I-21.104410 J19.494019",
	"G03 X22.381896 Y21.965295 Z-0.30 I-25.669788 J14.770018",
	"G03 X19.057440 Y32.104000 Z-0.30 I-11.927402 J1.703427",
	"G03 X14.643531 Y34.445885 Z-0.30 I-5.895479 J-5.781067",
	"G03 X5.782560 Y32.427520 Z-0.30 I-2.072330 J-11.361893",
	"G03 X2.162154 Y27.903261 Z-0.30 I6.438239 J-8.862705",
	"G03 X4.567680 Y16.400320 Z-0.30 I9.373268 J-4.042832",
	"G03 X13.018651 Y13.587292 Z-0.30 I7.293173 J7.809510",
	"G03 X22.135680 Y20.043040 Z-0.30 I-1.245155 J11.424015",
	"G00 Z0.30",
	"END"
};

inline const char* GC_PUNCT_COMMA[] = {
	"G00 Z0.30",
	"G00 X1.439040 Y1.923520",
	"G01 Z-0.30 F15000",
	"G02 X3.588335 Y0.928691 Z-0.30 I0.803312 J-1.083627 F15000",
	"G02 X1.781760 Y-2.353280 Z-0.30 I-3.468704 J-0.228841",
	"G00 Z0.30",
	"END"
};

inline const char* GC_PUNCT_EXCLAMATION[] = {
	"G00 Z0.30",
	"G00 X3.298080 Y34.262080",
	"G01 Z-0.30 F15000",
	"G02 X3.300169 Y22.163237 Z-0.30 I-1150.332437 J-6.247747 F15000",
	"G03 X3.298080 Y10.112320 Z-0.30 I1226.456976 J-6.238306",
	"G00 Z0.30",
	"G00 X3.301440 Y3.636640",
	"G01 Z-0.30 F15000",
	"G02 X3.900443 Y3.304812 Z-0.30 I0.000000 J-0.706563 F15000",
	"G02 X4.232160 Y2.149120 Z-0.30 I-1.847340 J-1.155692",
	"G02 X3.900197 Y0.993182 Z-0.30 I-2.178545 J0.000000",
	"G02 X3.301440 Y0.661600 Z-0.30 I-0.598757 J0.374815",
	"G02 X2.702437 Y0.993428 Z-0.30 I0.000000 J0.706563",
	"G02 X2.370720 Y2.149120 Z-0.30 I1.847340 J1.155692",
	"G02 X2.702437 Y3.304812 Z-0.30 I2.179057 J0.000000",
	"G02 X3.301440 Y3.636640 Z-0.30 I0.599003 J-0.374735",
	"G00 Z0.30",
	"END"
};

inline const char* GC_PUNCT_PERIOD[] = {
	"G00 Z0.30",
	"G00 X3.061440 Y3.636640",
	"G01 Z-0.30 F15000",
	"G02 X3.660443 Y3.304812 Z-0.30 I0.000000 J-0.706563 F15000",
	"G02 X3.992160 Y2.149120 Z-0.30 I-1.847340 J-1.155692",
	"G02 X3.660197 Y0.993182 Z-0.30 I-2.178545 J0.000000",
	"G02 X3.061440 Y0.661600 Z-0.30 I-0.598757 J0.374815",
	"G02 X2.462683 Y0.993182 Z-0.30 I0.000000 J0.706396",
	"G02 X2.130720 Y2.149120 Z-0.30 I1.846581 J1.155938",
	"G02 X2.462437 Y3.304812 Z-0.30 I2.179057 J0.000000",
	"G02 X3.061440 Y3.636640 Z-0.30 I0.599003 J-0.374735",
	"G00 Z0.30",
	"END"
};

inline const char* GC_PUNCT_QUESTION[] = {
	"G00 Z0.30",
	"G00 X2.173440 Y29.006080",
	"G01 Z-0.30 F15000",
	"G02 X4.447390 Y32.943618 Z-0.30 I4.542773 J0.001902 F15000",
	"G02 X10.165740 Y34.358560 Z-0.30 I5.315959 J-9.221380",
	"G02 X15.228401 Y32.539831 Z-0.30 I-0.336402 J-8.892058",
	"G02 X18.345600 Y26.415520 Z-0.30 I-4.812621 J-6.305028",
	"G02 X15.948837 Y19.600554 Z-0.30 I-10.600815 J-0.100730",
	"G02 X9.680160 Y15.669280 Z-0.30 I-8.971777 J7.342546",
	"G01 X9.680160 Y9.638080 Z-0.30",
	"G00 Z0.30",
	"G00 X9.680160 Y3.636640",
	"G01 Z-0.30 F15000",
	"G02 X10.279163 Y3.304812 Z-0.30 I0.000000 J-0.706563 F15000",
	"G02 X10.610880 Y2.149120 Z-0.30 I-1.847340 J-1.155692",
	"G02 X10.278917 Y0.993182 Z-0.30 I-2.178545 J0.000000",
	"G02 X9.680160 Y0.661600 Z-0.30 I-0.598757 J0.374815",
	"G02 X9.081403 Y0.993182 Z-0.30 I0.000000 J0.706396",
	"G02 X8.749440 Y2.149120 Z-0.30 I1.846581 J1.155938",
	"G02 X9.081403 Y3.305058 Z-0.30 I2.178545 J-0.000000",
	"G02 X9.680160 Y3.636640 Z-0.30 I0.598757 J-0.374815",
	"G00 Z0.30",
	"END"
};

inline const char* GC_SPACE[] = {
	"END"
};

inline const char** CHARACTERS[67] {
	GC_CAP_A,
	GC_CAP_B,
	GC_CAP_C,
	GC_CAP_D,
	GC_CAP_E,
	GC_CAP_F,
	GC_CAP_G,
	GC_CAP_H,
	GC_CAP_I,
	GC_CAP_J,
	GC_CAP_K,
	GC_CAP_L,
	GC_CAP_M,
	GC_CAP_N,
	GC_CAP_O,
	GC_CAP_P,
	GC_CAP_Q,
	GC_CAP_R,
	GC_CAP_S,
	GC_CAP_T,
	GC_CAP_U,
	GC_CAP_V,
	GC_CAP_W,
	GC_CAP_X,
	GC_CAP_Y,
	GC_CAP_Z,
	GC_LOW_A,
	GC_LOW_B,
	GC_LOW_C,
	GC_LOW_D,
	GC_LOW_E,
	GC_LOW_F,
	GC_LOW_G,
	GC_LOW_H,
	GC_LOW_I,
	GC_LOW_J,
	GC_LOW_K,
	GC_LOW_L,
	GC_LOW_M,
	GC_LOW_N,
	GC_LOW_O,
	GC_LOW_P,
	GC_LOW_Q,
	GC_LOW_R,
	GC_LOW_S,
	GC_LOW_T,
	GC_LOW_U,
	GC_LOW_V,
	GC_LOW_W,
	GC_LOW_X,
	GC_LOW_Y,
	GC_LOW_Z,
	GC_NUM_0,
	GC_NUM_1,
	GC_NUM_2,
	GC_NUM_3,
	GC_NUM_4,
	GC_NUM_5,
	GC_NUM_6,
	GC_NUM_7,
	GC_NUM_8,
	GC_NUM_9,
	GC_PUNCT_COMMA,
	GC_PUNCT_EXCLAMATION,
	GC_PUNCT_PERIOD,
	GC_PUNCT_QUESTION,
	GC_SPACE
};

#endif