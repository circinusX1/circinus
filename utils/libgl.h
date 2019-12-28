#ifndef LIBR_RESOLVER_H
#define LIBR_RESOLVER_H
// Copyright Marius C. https://github/comarius (do not remove)
#include <stdio.h>
#include <errno.h>
#include <dlfcn.h>


// -------------- constants ---------------
// review before compile...
#define __gl_h_
#define __WIN32__
#define WIN32_LEAN_AND_MEAN 1
#define GLAPI extern
#define GLAPIENTRY
#define APIENTRY GLAPIENTRY
#define APIENTRYP APIENTRY *
#define GLAPIENTRYP GLAPIENTRY *
#define GL_VERSION_1_1   1
#define GL_VERSION_1_2   1
#define GL_VERSION_1_3   1
#define GL_ARB_imaging   1
#define GL_FALSE				0
#define GL_TRUE					1
#define GL_BYTE					0x1400
#define GL_UNSIGNED_BYTE			0x1401
#define GL_SHORT				0x1402
#define GL_UNSIGNED_SHORT			0x1403
#define GL_INT					0x1404
#define GL_UNSIGNED_INT				0x1405
#define GL_FLOAT				0x1406
#define GL_2_BYTES				0x1407
#define GL_3_BYTES				0x1408
#define GL_4_BYTES				0x1409
#define GL_DOUBLE				0x140A
#define GL_POINTS				0x0000
#define GL_LINES				0x0001
#define GL_LINE_LOOP				0x0002
#define GL_LINE_STRIP				0x0003
#define GL_TRIANGLES				0x0004
#define GL_TRIANGLE_STRIP			0x0005
#define GL_TRIANGLE_FAN				0x0006
#define GL_QUADS				0x0007
#define GL_QUAD_STRIP				0x0008
#define GL_POLYGON				0x0009
#define GL_VERTEX_ARRAY				0x8074
#define GL_NORMAL_ARRAY				0x8075
#define GL_COLOR_ARRAY				0x8076
#define GL_INDEX_ARRAY				0x8077
#define GL_TEXTURE_COORD_ARRAY			0x8078
#define GL_EDGE_FLAG_ARRAY			0x8079
#define GL_VERTEX_ARRAY_SIZE			0x807A
#define GL_VERTEX_ARRAY_TYPE			0x807B
#define GL_VERTEX_ARRAY_STRIDE			0x807C
#define GL_NORMAL_ARRAY_TYPE			0x807E
#define GL_NORMAL_ARRAY_STRIDE			0x807F
#define GL_COLOR_ARRAY_SIZE			0x8081
#define GL_COLOR_ARRAY_TYPE			0x8082
#define GL_COLOR_ARRAY_STRIDE			0x8083
#define GL_INDEX_ARRAY_TYPE			0x8085
#define GL_INDEX_ARRAY_STRIDE			0x8086
#define GL_TEXTURE_COORD_ARRAY_SIZE		0x8088
#define GL_TEXTURE_COORD_ARRAY_TYPE		0x8089
#define GL_TEXTURE_COORD_ARRAY_STRIDE		0x808A
#define GL_EDGE_FLAG_ARRAY_STRIDE		0x808C
#define GL_VERTEX_ARRAY_POINTER			0x808E
#define GL_NORMAL_ARRAY_POINTER			0x808F
#define GL_COLOR_ARRAY_POINTER			0x8090
#define GL_INDEX_ARRAY_POINTER			0x8091
#define GL_TEXTURE_COORD_ARRAY_POINTER		0x8092
#define GL_EDGE_FLAG_ARRAY_POINTER		0x8093
#define GL_V2F					0x2A20
#define GL_V3F					0x2A21
#define GL_C4UB_V2F				0x2A22
#define GL_C4UB_V3F				0x2A23
#define GL_C3F_V3F				0x2A24
#define GL_N3F_V3F				0x2A25
#define GL_C4F_N3F_V3F				0x2A26
#define GL_T2F_V3F				0x2A27
#define GL_T4F_V4F				0x2A28
#define GL_T2F_C4UB_V3F				0x2A29
#define GL_T2F_C3F_V3F				0x2A2A
#define GL_T2F_N3F_V3F				0x2A2B
#define GL_T2F_C4F_N3F_V3F			0x2A2C
#define GL_T4F_C4F_N3F_V4F			0x2A2D
#define GL_MATRIX_MODE				0x0BA0
#define GL_MODELVIEW				0x1700
#define GL_PROJECTION				0x1701
#define GL_TEXTURE				0x1702
#define GL_POINT_SMOOTH				0x0B10
#define GL_POINT_SIZE				0x0B11
#define GL_POINT_SIZE_GRANULARITY 		0x0B13
#define GL_POINT_SIZE_RANGE			0x0B12
#define GL_LINE_SMOOTH				0x0B20
#define GL_LINE_STIPPLE				0x0B24
#define GL_LINE_STIPPLE_PATTERN			0x0B25
#define GL_LINE_STIPPLE_REPEAT			0x0B26
#define GL_LINE_WIDTH				0x0B21
#define GL_LINE_WIDTH_GRANULARITY		0x0B23
#define GL_LINE_WIDTH_RANGE			0x0B22
#define GL_POINT				0x1B00
#define GL_LINE					0x1B01
#define GL_FILL					0x1B02
#define GL_CW					0x0900
#define GL_CCW					0x0901
#define GL_FRONT				0x0404
#define GL_BACK					0x0405
#define GL_POLYGON_MODE				0x0B40
#define GL_POLYGON_SMOOTH			0x0B41
#define GL_POLYGON_STIPPLE			0x0B42
#define GL_EDGE_FLAG				0x0B43
#define GL_CULL_FACE				0x0B44
#define GL_CULL_FACE_MODE			0x0B45
#define GL_FRONT_FACE				0x0B46
#define GL_POLYGON_OFFSET_FACTOR		0x8038
#define GL_POLYGON_OFFSET_UNITS			0x2A00
#define GL_POLYGON_OFFSET_POINT			0x2A01
#define GL_POLYGON_OFFSET_LINE			0x2A02
#define GL_POLYGON_OFFSET_FILL			0x8037
#define GL_COMPILE				0x1300
#define GL_COMPILE_AND_EXECUTE			0x1301
#define GL_LIST_BASE				0x0B32
#define GL_LIST_INDEX				0x0B33
#define GL_LIST_MODE				0x0B30
#define GL_NEVER				0x0200
#define GL_LESS					0x0201
#define GL_EQUAL				0x0202
#define GL_LEQUAL				0x0203
#define GL_GREATER				0x0204
#define GL_NOTEQUAL				0x0205
#define GL_GEQUAL				0x0206
#define GL_ALWAYS				0x0207
#define GL_DEPTH_TEST				0x0B71
#define GL_DEPTH_BITS				0x0D56
#define GL_DEPTH_CLEAR_VALUE			0x0B73
#define GL_DEPTH_FUNC				0x0B74
#define GL_DEPTH_RANGE				0x0B70
#define GL_DEPTH_WRITEMASK			0x0B72
#define GL_DEPTH_COMPONENT			0x1902
#define GL_LIGHTING				0x0B50
#define GL_LIGHT0				0x4000
#define GL_LIGHT1				0x4001
#define GL_LIGHT2				0x4002
#define GL_LIGHT3				0x4003
#define GL_LIGHT4				0x4004
#define GL_LIGHT5				0x4005
#define GL_LIGHT6				0x4006
#define GL_LIGHT7				0x4007
#define GL_SPOT_EXPONENT			0x1205
#define GL_SPOT_CUTOFF				0x1206
#define GL_CONSTANT_ATTENUATION			0x1207
#define GL_LINEAR_ATTENUATION			0x1208
#define GL_QUADRATIC_ATTENUATION		0x1209
#define GL_AMBIENT				0x1200
#define GL_DIFFUSE				0x1201
#define GL_SPECULAR				0x1202
#define GL_SHININESS				0x1601
#define GL_EMISSION				0x1600
#define GL_POSITION				0x1203
#define GL_SPOT_DIRECTION			0x1204
#define GL_AMBIENT_AND_DIFFUSE			0x1602
#define GL_COLOR_INDEXES			0x1603
#define GL_LIGHT_MODEL_TWO_SIDE			0x0B52
#define GL_LIGHT_MODEL_LOCAL_VIEWER		0x0B51
#define GL_LIGHT_MODEL_AMBIENT			0x0B53
#define GL_FRONT_AND_BACK			0x0408
#define GL_SHADE_MODEL				0x0B54
#define GL_FLAT					0x1D00
#define GL_SMOOTH				0x1D01
#define GL_COLOR_MATERIAL			0x0B57
#define GL_COLOR_MATERIAL_FACE			0x0B55
#define GL_COLOR_MATERIAL_PARAMETER		0x0B56
#define GL_NORMALIZE				0x0BA1
#define GL_CLIP_PLANE0				0x3000
#define GL_CLIP_PLANE1				0x3001
#define GL_CLIP_PLANE2				0x3002
#define GL_CLIP_PLANE3				0x3003
#define GL_CLIP_PLANE4				0x3004
#define GL_CLIP_PLANE5				0x3005
#define GL_ACCUM_RED_BITS			0x0D58
#define GL_ACCUM_GREEN_BITS			0x0D59
#define GL_ACCUM_BLUE_BITS			0x0D5A
#define GL_ACCUM_ALPHA_BITS			0x0D5B
#define GL_ACCUM_CLEAR_VALUE			0x0B80
#define GL_ACCUM				0x0100
#define GL_ADD					0x0104
#define GL_LOAD					0x0101
#define GL_MULT					0x0103
#define GL_RETURN				0x0102
#define GL_ALPHA_TEST				0x0BC0
#define GL_ALPHA_TEST_REF			0x0BC2
#define GL_ALPHA_TEST_FUNC			0x0BC1
#define GL_BLEND				0x0BE2
#define GL_BLEND_SRC				0x0BE1
#define GL_BLEND_DST				0x0BE0
#define GL_ZERO					0
#define GL_ONE					1
#define GL_SRC_COLOR				0x0300
#define GL_ONE_MINUS_SRC_COLOR			0x0301
#define GL_SRC_ALPHA				0x0302
#define GL_ONE_MINUS_SRC_ALPHA			0x0303
#define GL_DST_ALPHA				0x0304
#define GL_ONE_MINUS_DST_ALPHA			0x0305
#define GL_DST_COLOR				0x0306
#define GL_ONE_MINUS_DST_COLOR			0x0307
#define GL_SRC_ALPHA_SATURATE			0x0308
#define GL_FEEDBACK				0x1C01
#define GL_RENDER				0x1C00
#define GL_SELECT				0x1C02
#define GL_2D					0x0600
#define GL_3D					0x0601
#define GL_3D_COLOR				0x0602
#define GL_3D_COLOR_TEXTURE			0x0603
#define GL_4D_COLOR_TEXTURE			0x0604
#define GL_POINT_TOKEN				0x0701
#define GL_LINE_TOKEN				0x0702
#define GL_LINE_RESET_TOKEN			0x0707
#define GL_POLYGON_TOKEN			0x0703
#define GL_BITMAP_TOKEN				0x0704
#define GL_DRAW_PIXEL_TOKEN			0x0705
#define GL_COPY_PIXEL_TOKEN			0x0706
#define GL_PASS_THROUGH_TOKEN			0x0700
#define GL_FEEDBACK_BUFFER_POINTER		0x0DF0
#define GL_FEEDBACK_BUFFER_SIZE			0x0DF1
#define GL_FEEDBACK_BUFFER_TYPE			0x0DF2
#define GL_SELECTION_BUFFER_POINTER		0x0DF3
#define GL_SELECTION_BUFFER_SIZE		0x0DF4
#define GL_FOG					0x0B60
#define GL_FOG_MODE				0x0B65
#define GL_FOG_DENSITY				0x0B62
#define GL_FOG_COLOR				0x0B66
#define GL_FOG_INDEX				0x0B61
#define GL_FOG_START				0x0B63
#define GL_FOG_END				0x0B64
#define GL_LINEAR				0x2601
#define GL_EXP					0x0800
#define GL_EXP2					0x0801
#define GL_LOGIC_OP				0x0BF1
#define GL_INDEX_LOGIC_OP			0x0BF1
#define GL_COLOR_LOGIC_OP			0x0BF2
#define GL_LOGIC_OP_MODE			0x0BF0
#define GL_CLEAR				0x1500
#define GL_SET					0x150F
#define GL_COPY					0x1503
#define GL_COPY_INVERTED			0x150C
#define GL_NOOP					0x1505
#define GL_INVERT				0x150A
#define GL_AND					0x1501
#define GL_NAND					0x150E
#define GL_OR					0x1507
#define GL_NOR					0x1508
#define GL_XOR					0x1506
#define GL_EQUIV				0x1509
#define GL_AND_REVERSE				0x1502
#define GL_AND_INVERTED				0x1504
#define GL_OR_REVERSE				0x150B
#define GL_OR_INVERTED				0x150D
#define GL_STENCIL_BITS				0x0D57
#define GL_STENCIL_TEST				0x0B90
#define GL_STENCIL_CLEAR_VALUE			0x0B91
#define GL_STENCIL_FUNC				0x0B92
#define GL_STENCIL_VALUE_MASK			0x0B93
#define GL_STENCIL_FAIL				0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL		0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS		0x0B96
#define GL_STENCIL_REF				0x0B97
#define GL_STENCIL_WRITEMASK			0x0B98
#define GL_STENCIL_INDEX			0x1901
#define GL_KEEP					0x1E00
#define GL_REPLACE				0x1E01
#define GL_INCR					0x1E02
#define GL_DECR					0x1E03
#define GL_NONE					0
#define GL_LEFT					0x0406
#define GL_RIGHT				0x0407
#define GL_FRONT_LEFT				0x0400
#define GL_FRONT_RIGHT				0x0401
#define GL_BACK_LEFT				0x0402
#define GL_BACK_RIGHT				0x0403
#define GL_AUX0					0x0409
#define GL_AUX1					0x040A
#define GL_AUX2					0x040B
#define GL_AUX3					0x040C
#define GL_COLOR_INDEX				0x1900
#define GL_RED					0x1903
#define GL_GREEN				0x1904
#define GL_BLUE					0x1905
#define GL_ALPHA				0x1906
#define GL_LUMINANCE				0x1909
#define GL_LUMINANCE_ALPHA			0x190A
#define GL_ALPHA_BITS				0x0D55
#define GL_RED_BITS				0x0D52
#define GL_GREEN_BITS				0x0D53
#define GL_BLUE_BITS				0x0D54
#define GL_INDEX_BITS				0x0D51
#define GL_SUBPIXEL_BITS			0x0D50
#define GL_AUX_BUFFERS				0x0C00
#define GL_READ_BUFFER				0x0C02
#define GL_DRAW_BUFFER				0x0C01
#define GL_DOUBLEBUFFER				0x0C32
#define GL_STEREO				0x0C33
#define GL_BITMAP				0x1A00
#define GL_COLOR				0x1800
#define GL_DEPTH				0x1801
#define GL_STENCIL				0x1802
#define GL_DITHER				0x0BD0
#define GL_RGB					0x1907
#define GL_RGBA					0x1908
#define GL_MAX_LIST_NESTING			0x0B31
#define GL_MAX_EVAL_ORDER			0x0D30
#define GL_MAX_LIGHTS				0x0D31
#define GL_MAX_CLIP_PLANES			0x0D32
#define GL_MAX_TEXTURE_SIZE			0x0D33
#define GL_MAX_PIXEL_MAP_TABLE			0x0D34
#define GL_MAX_ATTRIB_STACK_DEPTH		0x0D35
#define GL_MAX_MODELVIEW_STACK_DEPTH		0x0D36
#define GL_MAX_NAME_STACK_DEPTH			0x0D37
#define GL_MAX_PROJECTION_STACK_DEPTH		0x0D38
#define GL_MAX_TEXTURE_STACK_DEPTH		0x0D39
#define GL_MAX_VIEWPORT_DIMS			0x0D3A
#define GL_MAX_CLIENT_ATTRIB_STACK_DEPTH	0x0D3B
#define GL_ATTRIB_STACK_DEPTH			0x0BB0
#define GL_CLIENT_ATTRIB_STACK_DEPTH		0x0BB1
#define GL_COLOR_CLEAR_VALUE			0x0C22
#define GL_COLOR_WRITEMASK			0x0C23
#define GL_CURRENT_INDEX			0x0B01
#define GL_CURRENT_COLOR			0x0B00
#define GL_CURRENT_NORMAL			0x0B02
#define GL_CURRENT_RASTER_COLOR			0x0B04
#define GL_CURRENT_RASTER_DISTANCE		0x0B09
#define GL_CURRENT_RASTER_INDEX			0x0B05
#define GL_CURRENT_RASTER_POSITION		0x0B07
#define GL_CURRENT_RASTER_TEXTURE_COORDS	0x0B06
#define GL_CURRENT_RASTER_POSITION_VALID	0x0B08
#define GL_CURRENT_TEXTURE_COORDS		0x0B03
#define GL_INDEX_CLEAR_VALUE			0x0C20
#define GL_INDEX_MODE				0x0C30
#define GL_INDEX_WRITEMASK			0x0C21
#define GL_MODELVIEW_MATRIX			0x0BA6
#define GL_MODELVIEW_STACK_DEPTH		0x0BA3
#define GL_NAME_STACK_DEPTH			0x0D70
#define GL_PROJECTION_MATRIX			0x0BA7
#define GL_PROJECTION_STACK_DEPTH		0x0BA4
#define GL_RENDER_MODE				0x0C40
#define GL_RGBA_MODE				0x0C31
#define GL_TEXTURE_MATRIX			0x0BA8
#define GL_TEXTURE_STACK_DEPTH			0x0BA5
#define GL_VIEWPORT				0x0BA2
#define GL_AUTO_NORMAL				0x0D80
#define GL_MAP1_COLOR_4				0x0D90
#define GL_MAP1_INDEX				0x0D91
#define GL_MAP1_NORMAL				0x0D92
#define GL_MAP1_TEXTURE_COORD_1			0x0D93
#define GL_MAP1_TEXTURE_COORD_2			0x0D94
#define GL_MAP1_TEXTURE_COORD_3			0x0D95
#define GL_MAP1_TEXTURE_COORD_4			0x0D96
#define GL_MAP1_VERTEX_3			0x0D97
#define GL_MAP1_VERTEX_4			0x0D98
#define GL_MAP2_COLOR_4				0x0DB0
#define GL_MAP2_INDEX				0x0DB1
#define GL_MAP2_NORMAL				0x0DB2
#define GL_MAP2_TEXTURE_COORD_1			0x0DB3
#define GL_MAP2_TEXTURE_COORD_2			0x0DB4
#define GL_MAP2_TEXTURE_COORD_3			0x0DB5
#define GL_MAP2_TEXTURE_COORD_4			0x0DB6
#define GL_MAP2_VERTEX_3			0x0DB7
#define GL_MAP2_VERTEX_4			0x0DB8
#define GL_MAP1_GRID_DOMAIN			0x0DD0
#define GL_MAP1_GRID_SEGMENTS			0x0DD1
#define GL_MAP2_GRID_DOMAIN			0x0DD2
#define GL_MAP2_GRID_SEGMENTS			0x0DD3
#define GL_COEFF				0x0A00
#define GL_ORDER				0x0A01
#define GL_DOMAIN				0x0A02
#define GL_PERSPECTIVE_CORRECTION_HINT		0x0C50
#define GL_POINT_SMOOTH_HINT			0x0C51
#define GL_LINE_SMOOTH_HINT			0x0C52
#define GL_POLYGON_SMOOTH_HINT			0x0C53
#define GL_FOG_HINT				0x0C54
#define GL_DONT_CARE				0x1100
#define GL_FASTEST				0x1101
#define GL_NICEST				0x1102
#define GL_SCISSOR_BOX				0x0C10
#define GL_SCISSOR_TEST				0x0C11
#define GL_MAP_COLOR				0x0D10
#define GL_MAP_STENCIL				0x0D11
#define GL_INDEX_SHIFT				0x0D12
#define GL_INDEX_OFFSET				0x0D13
#define GL_RED_SCALE				0x0D14
#define GL_RED_BIAS				0x0D15
#define GL_GREEN_SCALE				0x0D18
#define GL_GREEN_BIAS				0x0D19
#define GL_BLUE_SCALE				0x0D1A
#define GL_BLUE_BIAS				0x0D1B
#define GL_ALPHA_SCALE				0x0D1C
#define GL_ALPHA_BIAS				0x0D1D
#define GL_DEPTH_SCALE				0x0D1E
#define GL_DEPTH_BIAS				0x0D1F
#define GL_PIXEL_MAP_S_TO_S_SIZE		0x0CB1
#define GL_PIXEL_MAP_I_TO_I_SIZE		0x0CB0
#define GL_PIXEL_MAP_I_TO_R_SIZE		0x0CB2
#define GL_PIXEL_MAP_I_TO_G_SIZE		0x0CB3
#define GL_PIXEL_MAP_I_TO_B_SIZE		0x0CB4
#define GL_PIXEL_MAP_I_TO_A_SIZE		0x0CB5
#define GL_PIXEL_MAP_R_TO_R_SIZE		0x0CB6
#define GL_PIXEL_MAP_G_TO_G_SIZE		0x0CB7
#define GL_PIXEL_MAP_B_TO_B_SIZE		0x0CB8
#define GL_PIXEL_MAP_A_TO_A_SIZE		0x0CB9
#define GL_PIXEL_MAP_S_TO_S			0x0C71
#define GL_PIXEL_MAP_I_TO_I			0x0C70
#define GL_PIXEL_MAP_I_TO_R			0x0C72
#define GL_PIXEL_MAP_I_TO_G			0x0C73
#define GL_PIXEL_MAP_I_TO_B			0x0C74
#define GL_PIXEL_MAP_I_TO_A			0x0C75
#define GL_PIXEL_MAP_R_TO_R			0x0C76
#define GL_PIXEL_MAP_G_TO_G			0x0C77
#define GL_PIXEL_MAP_B_TO_B			0x0C78
#define GL_PIXEL_MAP_A_TO_A			0x0C79
#define GL_PACK_ALIGNMENT			0x0D05
#define GL_PACK_LSB_FIRST			0x0D01
#define GL_PACK_ROW_LENGTH			0x0D02
#define GL_PACK_SKIP_PIXELS			0x0D04
#define GL_PACK_SKIP_ROWS			0x0D03
#define GL_PACK_SWAP_BYTES			0x0D00
#define GL_UNPACK_ALIGNMENT			0x0CF5
#define GL_UNPACK_LSB_FIRST			0x0CF1
#define GL_UNPACK_ROW_LENGTH			0x0CF2
#define GL_UNPACK_SKIP_PIXELS			0x0CF4
#define GL_UNPACK_SKIP_ROWS			0x0CF3
#define GL_UNPACK_SWAP_BYTES			0x0CF0
#define GL_ZOOM_X				0x0D16
#define GL_ZOOM_Y				0x0D17
#define GL_TEXTURE_ENV				0x2300
#define GL_TEXTURE_ENV_MODE			0x2200
#define GL_TEXTURE_1D				0x0DE0
#define GL_TEXTURE_2D				0x0DE1
#define GL_TEXTURE_WRAP_S			0x2802
#define GL_TEXTURE_WRAP_T			0x2803
#define GL_TEXTURE_MAG_FILTER			0x2800
#define GL_TEXTURE_MIN_FILTER			0x2801
#define GL_TEXTURE_ENV_COLOR			0x2201
#define GL_TEXTURE_GEN_S			0x0C60
#define GL_TEXTURE_GEN_T			0x0C61
#define GL_TEXTURE_GEN_R			0x0C62
#define GL_TEXTURE_GEN_Q			0x0C63
#define GL_TEXTURE_GEN_MODE			0x2500
#define GL_TEXTURE_BORDER_COLOR			0x1004
#define GL_TEXTURE_WIDTH			0x1000
#define GL_TEXTURE_HEIGHT			0x1001
#define GL_TEXTURE_BORDER			0x1005
#define GL_TEXTURE_COMPONENTS			0x1003
#define GL_TEXTURE_RED_SIZE			0x805C
#define GL_TEXTURE_GREEN_SIZE			0x805D
#define GL_TEXTURE_BLUE_SIZE			0x805E
#define GL_TEXTURE_ALPHA_SIZE			0x805F
#define GL_TEXTURE_LUMINANCE_SIZE		0x8060
#define GL_TEXTURE_INTENSITY_SIZE		0x8061
#define GL_NEAREST_MIPMAP_NEAREST		0x2700
#define GL_NEAREST_MIPMAP_LINEAR		0x2702
#define GL_LINEAR_MIPMAP_NEAREST		0x2701
#define GL_LINEAR_MIPMAP_LINEAR			0x2703
#define GL_OBJECT_LINEAR			0x2401
#define GL_OBJECT_PLANE				0x2501
#define GL_EYE_LINEAR				0x2400
#define GL_EYE_PLANE				0x2502
#define GL_SPHERE_MAP				0x2402
#define GL_DECAL				0x2101
#define GL_MODULATE				0x2100
#define GL_NEAREST				0x2600
#define GL_REPEAT				0x2901
#define GL_CLAMP				0x2900
#define GL_S					0x2000
#define GL_T					0x2001
#define GL_R					0x2002
#define GL_Q					0x2003
#define GL_VENDOR				0x1F00
#define GL_RENDERER				0x1F01
#define GL_VERSION				0x1F02
#define GL_EXTENSIONS				0x1F03
#define GL_NO_ERROR 				0
#define GL_INVALID_ENUM				0x0500
#define GL_INVALID_VALUE			0x0501
#define GL_INVALID_OPERATION			0x0502
#define GL_STACK_OVERFLOW			0x0503
#define GL_STACK_UNDERFLOW			0x0504
#define GL_OUT_OF_MEMORY			0x0505
#define GL_CURRENT_BIT				0x00000001
#define GL_POINT_BIT				0x00000002
#define GL_LINE_BIT				0x00000004
#define GL_POLYGON_BIT				0x00000008
#define GL_POLYGON_STIPPLE_BIT			0x00000010
#define GL_PIXEL_MODE_BIT			0x00000020
#define GL_LIGHTING_BIT				0x00000040
#define GL_FOG_BIT				0x00000080
#define GL_DEPTH_BUFFER_BIT			0x00000100
#define GL_ACCUM_BUFFER_BIT			0x00000200
#define GL_STENCIL_BUFFER_BIT			0x00000400
#define GL_VIEWPORT_BIT				0x00000800
#define GL_TRANSFORM_BIT			0x00001000
#define GL_ENABLE_BIT				0x00002000
#define GL_COLOR_BUFFER_BIT			0x00004000
#define GL_HINT_BIT				0x00008000
#define GL_EVAL_BIT				0x00010000
#define GL_LIST_BIT				0x00020000
#define GL_TEXTURE_BIT				0x00040000
#define GL_SCISSOR_BIT				0x00080000
#define GL_ALL_ATTRIB_BITS			0xFFFFFFFF
#define GL_PROXY_TEXTURE_1D			0x8063
#define GL_PROXY_TEXTURE_2D			0x8064
#define GL_TEXTURE_PRIORITY			0x8066
#define GL_TEXTURE_RESIDENT			0x8067
#define GL_TEXTURE_BINDING_1D			0x8068
#define GL_TEXTURE_BINDING_2D			0x8069
#define GL_TEXTURE_INTERNAL_FORMAT		0x1003
#define GL_ALPHA4				0x803B
#define GL_ALPHA8				0x803C
#define GL_ALPHA12				0x803D
#define GL_ALPHA16				0x803E
#define GL_LUMINANCE4				0x803F
#define GL_LUMINANCE8				0x8040
#define GL_LUMINANCE12				0x8041
#define GL_LUMINANCE16				0x8042
#define GL_LUMINANCE4_ALPHA4			0x8043
#define GL_LUMINANCE6_ALPHA2			0x8044
#define GL_LUMINANCE8_ALPHA8			0x8045
#define GL_LUMINANCE12_ALPHA4			0x8046
#define GL_LUMINANCE12_ALPHA12			0x8047
#define GL_LUMINANCE16_ALPHA16			0x8048
#define GL_INTENSITY				0x8049
#define GL_INTENSITY4				0x804A
#define GL_INTENSITY8				0x804B
#define GL_INTENSITY12				0x804C
#define GL_INTENSITY16				0x804D
#define GL_R3_G3_B2				0x2A10
#define GL_RGB4					0x804F
#define GL_RGB5					0x8050
#define GL_RGB8					0x8051
#define GL_RGB10				0x8052
#define GL_RGB12				0x8053
#define GL_RGB16				0x8054
#define GL_RGBA2				0x8055
#define GL_RGBA4				0x8056
#define GL_RGB5_A1				0x8057
#define GL_RGBA8				0x8058
#define GL_RGB10_A2				0x8059
#define GL_RGBA12				0x805A
#define GL_RGBA16				0x805B
#define GL_CLIENT_PIXEL_STORE_BIT		0x00000001
#define GL_CLIENT_VERTEX_ARRAY_BIT		0x00000002
#define GL_ALL_CLIENT_ATTRIB_BITS 		0xFFFFFFFF
#define GL_CLIENT_ALL_ATTRIB_BITS 		0xFFFFFFFF
#define GL_RESCALE_NORMAL			0x803A
#define GL_CLAMP_TO_EDGE			0x812F
#define GL_MAX_ELEMENTS_VERTICES		0x80E8
#define GL_MAX_ELEMENTS_INDICES			0x80E9
#define GL_BGR					0x80E0
#define GL_BGRA					0x80E1
#define GL_UNSIGNED_BYTE_3_3_2			0x8032
#define GL_UNSIGNED_BYTE_2_3_3_REV		0x8362
#define GL_UNSIGNED_SHORT_5_6_5			0x8363
#define GL_UNSIGNED_SHORT_5_6_5_REV		0x8364
#define GL_UNSIGNED_SHORT_4_4_4_4		0x8033
#define GL_UNSIGNED_SHORT_4_4_4_4_REV		0x8365
#define GL_UNSIGNED_SHORT_5_5_5_1		0x8034
#define GL_UNSIGNED_SHORT_1_5_5_5_REV		0x8366
#define GL_UNSIGNED_INT_8_8_8_8			0x8035
#define GL_UNSIGNED_INT_8_8_8_8_REV		0x8367
#define GL_UNSIGNED_INT_10_10_10_2		0x8036
#define GL_UNSIGNED_INT_2_10_10_10_REV		0x8368
#define GL_LIGHT_MODEL_COLOR_CONTROL		0x81F8
#define GL_SINGLE_COLOR				0x81F9
#define GL_SEPARATE_SPECULAR_COLOR		0x81FA
#define GL_TEXTURE_MIN_LOD			0x813A
#define GL_TEXTURE_MAX_LOD			0x813B
#define GL_TEXTURE_BASE_LEVEL			0x813C
#define GL_TEXTURE_MAX_LEVEL			0x813D
#define GL_SMOOTH_POINT_SIZE_RANGE		0x0B12
#define GL_SMOOTH_POINT_SIZE_GRANULARITY	0x0B13
#define GL_SMOOTH_LINE_WIDTH_RANGE		0x0B22
#define GL_SMOOTH_LINE_WIDTH_GRANULARITY	0x0B23
#define GL_ALIASED_POINT_SIZE_RANGE		0x846D
#define GL_ALIASED_LINE_WIDTH_RANGE		0x846E
#define GL_PACK_SKIP_IMAGES			0x806B
#define GL_PACK_IMAGE_HEIGHT			0x806C
#define GL_UNPACK_SKIP_IMAGES			0x806D
#define GL_UNPACK_IMAGE_HEIGHT			0x806E
#define GL_TEXTURE_3D				0x806F
#define GL_PROXY_TEXTURE_3D			0x8070
#define GL_TEXTURE_DEPTH			0x8071
#define GL_TEXTURE_WRAP_R			0x8072
#define GL_MAX_3D_TEXTURE_SIZE			0x8073
#define GL_TEXTURE_BINDING_3D			0x806A
#define GL_CONSTANT_COLOR			0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR		0x8002
#define GL_CONSTANT_ALPHA			0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA		0x8004
#define GL_COLOR_TABLE				0x80D0
#define GL_POST_CONVOLUTION_COLOR_TABLE		0x80D1
#define GL_POST_COLOR_MATRIX_COLOR_TABLE	0x80D2
#define GL_PROXY_COLOR_TABLE			0x80D3
#define GL_PROXY_POST_CONVOLUTION_COLOR_TABLE	0x80D4
#define GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE	0x80D5
#define GL_COLOR_TABLE_SCALE			0x80D6
#define GL_COLOR_TABLE_BIAS			0x80D7
#define GL_COLOR_TABLE_FORMAT			0x80D8
#define GL_COLOR_TABLE_WIDTH			0x80D9
#define GL_COLOR_TABLE_RED_SIZE			0x80DA
#define GL_COLOR_TABLE_GREEN_SIZE		0x80DB
#define GL_COLOR_TABLE_BLUE_SIZE		0x80DC
#define GL_COLOR_TABLE_ALPHA_SIZE		0x80DD
#define GL_COLOR_TABLE_LUMINANCE_SIZE		0x80DE
#define GL_COLOR_TABLE_INTENSITY_SIZE		0x80DF
#define GL_CONVOLUTION_1D			0x8010
#define GL_CONVOLUTION_2D			0x8011
#define GL_SEPARABLE_2D				0x8012
#define GL_CONVOLUTION_BORDER_MODE		0x8013
#define GL_CONVOLUTION_FILTER_SCALE		0x8014
#define GL_CONVOLUTION_FILTER_BIAS		0x8015
#define GL_REDUCE				0x8016
#define GL_CONVOLUTION_FORMAT			0x8017
#define GL_CONVOLUTION_WIDTH			0x8018
#define GL_CONVOLUTION_HEIGHT			0x8019
#define GL_MAX_CONVOLUTION_WIDTH		0x801A
#define GL_MAX_CONVOLUTION_HEIGHT		0x801B
#define GL_POST_CONVOLUTION_RED_SCALE		0x801C
#define GL_POST_CONVOLUTION_GREEN_SCALE		0x801D
#define GL_POST_CONVOLUTION_BLUE_SCALE		0x801E
#define GL_POST_CONVOLUTION_ALPHA_SCALE		0x801F
#define GL_POST_CONVOLUTION_RED_BIAS		0x8020
#define GL_POST_CONVOLUTION_GREEN_BIAS		0x8021
#define GL_POST_CONVOLUTION_BLUE_BIAS		0x8022
#define GL_POST_CONVOLUTION_ALPHA_BIAS		0x8023
#define GL_CONSTANT_BORDER			0x8151
#define GL_REPLICATE_BORDER			0x8153
#define GL_CONVOLUTION_BORDER_COLOR		0x8154
#define GL_COLOR_MATRIX				0x80B1
#define GL_COLOR_MATRIX_STACK_DEPTH		0x80B2
#define GL_MAX_COLOR_MATRIX_STACK_DEPTH		0x80B3
#define GL_POST_COLOR_MATRIX_RED_SCALE		0x80B4
#define GL_POST_COLOR_MATRIX_GREEN_SCALE	0x80B5
#define GL_POST_COLOR_MATRIX_BLUE_SCALE		0x80B6
#define GL_POST_COLOR_MATRIX_ALPHA_SCALE	0x80B7
#define GL_POST_COLOR_MATRIX_RED_BIAS		0x80B8
#define GL_POST_COLOR_MATRIX_GREEN_BIAS		0x80B9
#define GL_POST_COLOR_MATRIX_BLUE_BIAS		0x80BA
#define GL_POST_COLOR_MATRIX_ALPHA_BIAS		0x80BB
#define GL_HISTOGRAM				0x8024
#define GL_PROXY_HISTOGRAM			0x8025
#define GL_HISTOGRAM_WIDTH			0x8026
#define GL_HISTOGRAM_FORMAT			0x8027
#define GL_HISTOGRAM_RED_SIZE			0x8028
#define GL_HISTOGRAM_GREEN_SIZE			0x8029
#define GL_HISTOGRAM_BLUE_SIZE			0x802A
#define GL_HISTOGRAM_ALPHA_SIZE			0x802B
#define GL_HISTOGRAM_LUMINANCE_SIZE		0x802C
#define GL_HISTOGRAM_SINK			0x802D
#define GL_MINMAX				0x802E
#define GL_MINMAX_FORMAT			0x802F
#define GL_MINMAX_SINK				0x8030
#define GL_TABLE_TOO_LARGE			0x8031
#define GL_BLEND_EQUATION			0x8009
#define GL_MIN					0x8007
#define GL_MAX					0x8008
#define GL_FUNC_ADD				0x8006
#define GL_FUNC_SUBTRACT			0x800A
#define GL_FUNC_REVERSE_SUBTRACT		0x800B
#define GL_BLEND_COLOR				0x8005
#define GL_TEXTURE0				0x84C0
#define GL_TEXTURE1				0x84C1
#define GL_TEXTURE2				0x84C2
#define GL_TEXTURE3				0x84C3
#define GL_TEXTURE4				0x84C4
#define GL_TEXTURE5				0x84C5
#define GL_TEXTURE6				0x84C6
#define GL_TEXTURE7				0x84C7
#define GL_TEXTURE8				0x84C8
#define GL_TEXTURE9				0x84C9
#define GL_TEXTURE10				0x84CA
#define GL_TEXTURE11				0x84CB
#define GL_TEXTURE12				0x84CC
#define GL_TEXTURE13				0x84CD
#define GL_TEXTURE14				0x84CE
#define GL_TEXTURE15				0x84CF
#define GL_TEXTURE16				0x84D0
#define GL_TEXTURE17				0x84D1
#define GL_TEXTURE18				0x84D2
#define GL_TEXTURE19				0x84D3
#define GL_TEXTURE20				0x84D4
#define GL_TEXTURE21				0x84D5
#define GL_TEXTURE22				0x84D6
#define GL_TEXTURE23				0x84D7
#define GL_TEXTURE24				0x84D8
#define GL_TEXTURE25				0x84D9
#define GL_TEXTURE26				0x84DA
#define GL_TEXTURE27				0x84DB
#define GL_TEXTURE28				0x84DC
#define GL_TEXTURE29				0x84DD
#define GL_TEXTURE30				0x84DE
#define GL_TEXTURE31				0x84DF
#define GL_ACTIVE_TEXTURE			0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE		0x84E1
#define GL_MAX_TEXTURE_UNITS			0x84E2
#define GL_NORMAL_MAP				0x8511
#define GL_REFLECTION_MAP			0x8512
#define GL_TEXTURE_CUBE_MAP			0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP		0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X		0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X		0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y		0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y		0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z		0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z		0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP		0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE		0x851C
#define GL_COMPRESSED_ALPHA			0x84E9
#define GL_COMPRESSED_LUMINANCE			0x84EA
#define GL_COMPRESSED_LUMINANCE_ALPHA		0x84EB
#define GL_COMPRESSED_INTENSITY			0x84EC
#define GL_COMPRESSED_RGB			0x84ED
#define GL_COMPRESSED_RGBA			0x84EE
#define GL_TEXTURE_COMPRESSION_HINT		0x84EF
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE	0x86A0
#define GL_TEXTURE_COMPRESSED			0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS	0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS		0x86A3
#define GL_MULTISAMPLE				0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE		0x809E
#define GL_SAMPLE_ALPHA_TO_ONE			0x809F
#define GL_SAMPLE_COVERAGE			0x80A0
#define GL_SAMPLE_BUFFERS			0x80A8
#define GL_SAMPLES				0x80A9
#define GL_SAMPLE_COVERAGE_VALUE		0x80AA
#define GL_SAMPLE_COVERAGE_INVERT		0x80AB
#define GL_MULTISAMPLE_BIT			0x20000000
#define GL_TRANSPOSE_MODELVIEW_MATRIX		0x84E3
#define GL_TRANSPOSE_PROJECTION_MATRIX		0x84E4
#define GL_TRANSPOSE_TEXTURE_MATRIX		0x84E5
#define GL_TRANSPOSE_COLOR_MATRIX		0x84E6
#define GL_COMBINE				0x8570
#define GL_COMBINE_RGB				0x8571
#define GL_COMBINE_ALPHA			0x8572
#define GL_SOURCE0_RGB				0x8580
#define GL_SOURCE1_RGB				0x8581
#define GL_SOURCE2_RGB				0x8582
#define GL_SOURCE0_ALPHA			0x8588
#define GL_SOURCE1_ALPHA			0x8589
#define GL_SOURCE2_ALPHA			0x858A
#define GL_OPERAND0_RGB				0x8590
#define GL_OPERAND1_RGB				0x8591
#define GL_OPERAND2_RGB				0x8592
#define GL_OPERAND0_ALPHA			0x8598
#define GL_OPERAND1_ALPHA			0x8599
#define GL_OPERAND2_ALPHA			0x859A
#define GL_RGB_SCALE				0x8573
#define GL_ADD_SIGNED				0x8574
#define GL_INTERPOLATE				0x8575
#define GL_SUBTRACT				0x84E7
#define GL_CONSTANT				0x8576
#define GL_PRIMARY_COLOR			0x8577
#define GL_PREVIOUS				0x8578
#define GL_DOT3_RGB				0x86AE
#define GL_DOT3_RGBA				0x86AF
#define GL_CLAMP_TO_BORDER			0x812D
#define GL_ARB_multitexture 1
#define GL_TEXTURE0_ARB				0x84C0
#define GL_TEXTURE1_ARB				0x84C1
#define GL_TEXTURE2_ARB				0x84C2
#define GL_TEXTURE3_ARB				0x84C3
#define GL_TEXTURE4_ARB				0x84C4
#define GL_TEXTURE5_ARB				0x84C5
#define GL_TEXTURE6_ARB				0x84C6
#define GL_TEXTURE7_ARB				0x84C7
#define GL_TEXTURE8_ARB				0x84C8
#define GL_TEXTURE9_ARB				0x84C9
#define GL_TEXTURE10_ARB			0x84CA
#define GL_TEXTURE11_ARB			0x84CB
#define GL_TEXTURE12_ARB			0x84CC
#define GL_TEXTURE13_ARB			0x84CD
#define GL_TEXTURE14_ARB			0x84CE
#define GL_TEXTURE15_ARB			0x84CF
#define GL_TEXTURE16_ARB			0x84D0
#define GL_TEXTURE17_ARB			0x84D1
#define GL_TEXTURE18_ARB			0x84D2
#define GL_TEXTURE19_ARB			0x84D3
#define GL_TEXTURE20_ARB			0x84D4
#define GL_TEXTURE21_ARB			0x84D5
#define GL_TEXTURE22_ARB			0x84D6
#define GL_TEXTURE23_ARB			0x84D7
#define GL_TEXTURE24_ARB			0x84D8
#define GL_TEXTURE25_ARB			0x84D9
#define GL_TEXTURE26_ARB			0x84DA
#define GL_TEXTURE27_ARB			0x84DB
#define GL_TEXTURE28_ARB			0x84DC
#define GL_TEXTURE29_ARB			0x84DD
#define GL_TEXTURE30_ARB			0x84DE
#define GL_TEXTURE31_ARB			0x84DF
#define GL_ACTIVE_TEXTURE_ARB			0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE_ARB		0x84E1
#define GL_MAX_TEXTURE_UNITS_ARB		0x84E2
#define GL_MESA_packed_depth_stencil 1
#define GL_DEPTH_STENCIL_MESA			0x8750
#define GL_UNSIGNED_INT_24_8_MESA		0x8751
#define GL_UNSIGNED_INT_8_24_REV_MESA		0x8752
#define GL_UNSIGNED_SHORT_15_1_MESA		0x8753
#define GL_UNSIGNED_SHORT_1_15_REV_MESA		0x8754
#define GL_ATI_blend_equation_separate 1
#define GL_ALPHA_BLEND_EQUATION_ATI	        0x883D
#define GL_OES_EGL_image 1


// -------------- functions ---------------
// review before compile...
//         #elif defined(__CYGWIN__) && defined(USE_OPENGL32) /* use native windows opengl32 */
#define PF_defined (* (#elif (*)(__CYGWIN__))                                      _ptr[0].ptr)
//         GLAPI void GLAPIENTRY glTexCoord4s( GLshort s, GLshort t, GLshort r, GLshort q );
#define PF_void (* (GLAPI (*)(GLshort,GLshort,GLshort,GLshort))                    _ptr[1].ptr)
//         GLAPI void GLAPIENTRY glTexCoord4sv( const GLshort *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[2].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord2dvARB(GLenum target, const GLdouble *v);
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[3].ptr)
//         GLAPI void GLAPIENTRY glNormal3sv( const GLshort *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[4].ptr)
//         GLAPI void GLAPIENTRY glConvolutionParameterfv( GLenum target, GLenum pname,
#define PF_void (* (GLAPI (*)(GLenum,GLenum,))                                     _ptr[5].ptr)
//         GLAPI void GLAPIENTRY glTexCoord2sv( const GLshort *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[6].ptr)
//         GLAPI void GLAPIENTRY glGetTexEnvfv( GLenum target, GLenum pname, GLfloat *params );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,GLfloat))                              _ptr[7].ptr)
//         GLAPI void GLAPIENTRY glEvalCoord1fv( const GLfloat *u );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[8].ptr)
//         GLAPI void GLAPIENTRY glGetFloatv( GLenum pname, GLfloat *params );
#define PF_void (* (GLAPI (*)(GLenum,GLfloat))                                     _ptr[9].ptr)
//         GLAPI void GLAPIENTRY glMaterialfv( GLenum face, GLenum pname, const GLfloat *params );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,const))                                _ptr[10].ptr)
//         GLAPI void GLAPIENTRY glPixelZoom( GLfloat xfactor, GLfloat yfactor );
#define PF_void (* (GLAPI (*)(GLfloat,GLfloat))                                    _ptr[11].ptr)
//         GLAPI void GLAPIENTRY glTexEnvfv( GLenum target, GLenum pname, const GLfloat *params );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,const))                                _ptr[12].ptr)
//         GLAPI void GLAPIENTRY glEnable( GLenum cap );
#define PF_void (* (GLAPI (*)(GLenum))                                             _ptr[13].ptr)
//         GLAPI void GLAPIENTRY glEnableClientState( GLenum cap );  /* 1.1 */
#define PF_void (* (GLAPI (*)(GLenum))                                             _ptr[14].ptr)
//         GLAPI void GLAPIENTRY glPointSize( GLfloat size );
#define PF_void (* (GLAPI (*)(GLfloat))                                            _ptr[15].ptr)
//         GLAPI void GLAPIENTRY glGenTextures( GLsizei n, GLuint *textures );
#define PF_void (* (GLAPI (*)(GLsizei,GLuint))                                     _ptr[16].ptr)
//         GLAPI void GLAPIENTRY glColor4iv( const GLint *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[17].ptr)
//         GLAPI void GLAPIENTRY glGetBooleanv( GLenum pname, GLboolean *params );
#define PF_void (* (GLAPI (*)(GLenum,GLboolean))                                   _ptr[18].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord3sARB(GLenum target, GLshort s, GLshort t, GLshort r);
#define PF_void (* (GLAPI (*)(GLenum,GLshort,GLshort,GLshort))                     _ptr[19].ptr)
//         GLAPI void GLAPIENTRY glPolygonStipple( const GLubyte *mask );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[20].ptr)
//         GLAPI void GLAPIENTRY glPixelMapuiv( GLenum map, GLsizei mapsize,
#define PF_void (* (GLAPI (*)(GLenum,GLsizei,))                                    _ptr[21].ptr)
//         GLAPI void GLAPIENTRY glPrioritizeTextures( GLsizei n,
#define PF_void (* (GLAPI (*)(GLsizei,))                                           _ptr[22].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord2sv( GLenum target, const GLshort *v );
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[23].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord2svARB(GLenum target, const GLshort *v);
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[24].ptr)
//         GLAPI void GLAPIENTRY glConvolutionParameteriv( GLenum target, GLenum pname,
#define PF_void (* (GLAPI (*)(GLenum,GLenum,))                                     _ptr[25].ptr)
//         GLAPI void GLAPIENTRY glListBase( GLuint base );
#define PF_void (* (GLAPI (*)(GLuint))                                             _ptr[26].ptr)
//         GLAPI void GLAPIENTRY glGetTexEnviv( GLenum target, GLenum pname, GLint *params );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,GLint))                                _ptr[27].ptr)
//         GLAPI void GLAPIENTRY glEvalCoord1d( GLdouble u );
#define PF_void (* (GLAPI (*)(GLdouble))                                           _ptr[28].ptr)
//         GLAPI void GLAPIENTRY glEvalCoord1dv( const GLdouble *u );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[29].ptr)
//         GLAPI void GLAPIENTRY glEvalCoord1f( GLfloat u );
#define PF_void (* (GLAPI (*)(GLfloat))                                            _ptr[30].ptr)
//         GLAPI void GLAPIENTRY glMaterialiv( GLenum face, GLenum pname, const GLint *params );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,const))                                _ptr[31].ptr)
//         GLAPI void GLAPIENTRY glGetTexParameterfv( GLenum target,
#define PF_void (* (GLAPI (*)(GLenum,))                                            _ptr[32].ptr)
//         GLAPI void GLAPIENTRY glTexEnviv( GLenum target, GLenum pname, const GLint *params );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,const))                                _ptr[33].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord4dvARB(GLenum target, const GLdouble *v);
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[34].ptr)
//         GLAPI void GLAPIENTRY glEvalCoord2d( GLdouble u, GLdouble v );
#define PF_void (* (GLAPI (*)(GLdouble,GLdouble))                                  _ptr[35].ptr)
//         GLAPI void GLAPIENTRY glEvalCoord2dv( const GLdouble *u );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[36].ptr)
//         GLAPI void GLAPIENTRY glEvalCoord2f( GLfloat u, GLfloat v );
#define PF_void (* (GLAPI (*)(GLfloat,GLfloat))                                    _ptr[37].ptr)
//         GLAPI void GLAPIENTRY glEvalCoord2fv( const GLfloat *u );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[38].ptr)
//         GLAPI void GLAPIENTRY glLogicOp( GLenum opcode );
#define PF_void (* (GLAPI (*)(GLenum))                                             _ptr[39].ptr)
//         GLAPI void GLAPIENTRY glRasterPos2dv( const GLdouble *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[40].ptr)
//         GLAPI void GLAPIENTRY glColor3usv( const GLushort *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[41].ptr)
//         GLAPI void GLAPIENTRY glEndList( void );
#define PF_void (* (GLAPI (*)(void))                                               _ptr[42].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord4sv( GLenum target, const GLshort *v );
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[43].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord4svARB(GLenum target, const GLshort *v);
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[44].ptr)
//         GLAPI void GLAPIENTRY glCopyConvolutionFilter2D( GLenum target,
#define PF_void (* (GLAPI (*)(GLenum,))                                            _ptr[45].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord4dARB(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
#define PF_void (* (GLAPI (*)(GLenum,GLdouble,GLdouble,GLdouble,GLdouble))         _ptr[46].ptr)
//         GLAPI void GLAPIENTRY glGetTexParameteriv( GLenum target,
#define PF_void (* (GLAPI (*)(GLenum,))                                            _ptr[47].ptr)
//         GLAPI void GLAPIENTRY glDrawBuffer( GLenum mode );
#define PF_void (* (GLAPI (*)(GLenum))                                             _ptr[48].ptr)
//         GLAPI void GLAPIENTRY glRasterPos2fv( const GLfloat *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[49].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord4fARB(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
#define PF_void (* (GLAPI (*)(GLenum,GLfloat,GLfloat,GLfloat,GLfloat))             _ptr[50].ptr)
//         GLAPI void GLAPIENTRY glVertex3sv( const GLshort *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[51].ptr)
//         GLAPI void GLAPIENTRY glIndexdv( const GLdouble *c );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[52].ptr)
//         GLAPI void GLAPIENTRY glCopyColorSubTable( GLenum target, GLsizei start,
#define PF_void (* (GLAPI (*)(GLenum,GLsizei,))                                    _ptr[53].ptr)
//         GLAPI void GLAPIENTRY glColor3b( GLbyte red, GLbyte green, GLbyte blue );
#define PF_void (* (GLAPI (*)(GLbyte,GLbyte,GLbyte))                               _ptr[54].ptr)
//         GLAPI void GLAPIENTRY glColor3bv( const GLbyte *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[55].ptr)
//         GLAPI void GLAPIENTRY glColor3d( GLdouble red, GLdouble green, GLdouble blue );
#define PF_void (* (GLAPI (*)(GLdouble,GLdouble,GLdouble))                         _ptr[56].ptr)
//         GLAPI void GLAPIENTRY glColor3dv( const GLdouble *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[57].ptr)
//         GLAPI void GLAPIENTRY glColor3f( GLfloat red, GLfloat green, GLfloat blue );
#define PF_void (* (GLAPI (*)(GLfloat,GLfloat,GLfloat))                            _ptr[58].ptr)
//         GLAPI void GLAPIENTRY glColor3fv( const GLfloat *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[59].ptr)
//         GLAPI void GLAPIENTRY glColor3i( GLint red, GLint green, GLint blue );
#define PF_void (* (GLAPI (*)(GLint,GLint,GLint))                                  _ptr[60].ptr)
//         GLAPI void GLAPIENTRY glColor3iv( const GLint *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[61].ptr)
//         GLAPI void GLAPIENTRY glResetMinmax( GLenum target );
#define PF_void (* (GLAPI (*)(GLenum))                                             _ptr[62].ptr)
//         GLAPI void GLAPIENTRY glEdgeFlagPointer( GLsizei stride, const GLvoid *ptr );
#define PF_void (* (GLAPI (*)(GLsizei,const))                                      _ptr[63].ptr)
//         GLAPI void GLAPIENTRY glColor3s( GLshort red, GLshort green, GLshort blue );
#define PF_void (* (GLAPI (*)(GLshort,GLshort,GLshort))                            _ptr[64].ptr)
//         GLAPI void GLAPIENTRY glColor3sv( const GLshort *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[65].ptr)
//         GLAPI void GLAPIENTRY glGetHistogramParameteriv( GLenum target, GLenum pname,
#define PF_void (* (GLAPI (*)(GLenum,GLenum,))                                     _ptr[66].ptr)
//         GLAPI void GLAPIENTRY glRasterPos4dv( const GLdouble *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[67].ptr)
//         GLAPI void GLAPIENTRY glVertex2d( GLdouble x, GLdouble y );
#define PF_void (* (GLAPI (*)(GLdouble,GLdouble))                                  _ptr[68].ptr)
//         GLAPI void GLAPIENTRY glVertex2dv( const GLdouble *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[69].ptr)
//         GLAPI void GLAPIENTRY glVertex2f( GLfloat x, GLfloat y );
#define PF_void (* (GLAPI (*)(GLfloat,GLfloat))                                    _ptr[70].ptr)
//         GLAPI void GLAPIENTRY glVertex2fv( const GLfloat *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[71].ptr)
//         GLAPI void GLAPIENTRY glVertex2i( GLint x, GLint y );
#define PF_void (* (GLAPI (*)(GLint,GLint))                                        _ptr[72].ptr)
//         GLAPI void GLAPIENTRY glVertex2iv( const GLint *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[73].ptr)
//         GLAPI void GLAPIENTRY glColor4b( GLbyte red, GLbyte green,
#define PF_void (* (GLAPI (*)(GLbyte,GLbyte,))                                     _ptr[74].ptr)
//         GLAPI void GLAPIENTRY glColor4bv( const GLbyte *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[75].ptr)
//         GLAPI void GLAPIENTRY glPixelTransferf( GLenum pname, GLfloat param );
#define PF_void (* (GLAPI (*)(GLenum,GLfloat))                                     _ptr[76].ptr)
//         GLAPI void GLAPIENTRY glColor4d( GLdouble red, GLdouble green,
#define PF_void (* (GLAPI (*)(GLdouble,GLdouble,))                                 _ptr[77].ptr)
//         GLAPI void GLAPIENTRY glColor4dv( const GLdouble *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[78].ptr)
//         GLAPI void GLAPIENTRY glPixelTransferi( GLenum pname, GLint param );
#define PF_void (* (GLAPI (*)(GLenum,GLint))                                       _ptr[79].ptr)
//         GLAPI void GLAPIENTRY glColor4f( GLfloat red, GLfloat green,
#define PF_void (* (GLAPI (*)(GLfloat,GLfloat,))                                   _ptr[80].ptr)
//         GLAPI void GLAPIENTRY glColor4fv( const GLfloat *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[81].ptr)
//         GLAPI void GLAPIENTRY glVertex2s( GLshort x, GLshort y );
#define PF_void (* (GLAPI (*)(GLshort,GLshort))                                    _ptr[82].ptr)
//         GLAPI void GLAPIENTRY glVertex2sv( const GLshort *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[83].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord1dARB(GLenum target, GLdouble s);
#define PF_void (* (GLAPI (*)(GLenum,GLdouble))                                    _ptr[84].ptr)
//         GLAPI void GLAPIENTRY glColor4i( GLint red, GLint green,
#define PF_void (* (GLAPI (*)(GLint,GLint,))                                       _ptr[85].ptr)
//         GLAPI void GLAPIENTRY glColor4s( GLshort red, GLshort green,
#define PF_void (* (GLAPI (*)(GLshort,GLshort,))                                   _ptr[86].ptr)
//         GLAPI void GLAPIENTRY glColor4sv( const GLshort *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[87].ptr)
//         GLAPI void GLAPIENTRY glTexParameterfv( GLenum target, GLenum pname,
#define PF_void (* (GLAPI (*)(GLenum,GLenum,))                                     _ptr[88].ptr)
//         GLAPI void GLAPIENTRY glVertex3d( GLdouble x, GLdouble y, GLdouble z );
#define PF_void (* (GLAPI (*)(GLdouble,GLdouble,GLdouble))                         _ptr[89].ptr)
//         GLAPI void GLAPIENTRY glVertex3dv( const GLdouble *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[90].ptr)
//         GLAPI void GLAPIENTRY glGetCompressedTexImage( GLenum target, GLint lod, GLvoid *img );
#define PF_void (* (GLAPI (*)(GLenum,GLint,GLvoid))                                _ptr[91].ptr)
//         GLAPI void GLAPIENTRY glVertex3f( GLfloat x, GLfloat y, GLfloat z );
#define PF_void (* (GLAPI (*)(GLfloat,GLfloat,GLfloat))                            _ptr[92].ptr)
//         GLAPI void GLAPIENTRY glVertex3fv( const GLfloat *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[93].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord1fARB(GLenum target, GLfloat s);
#define PF_void (* (GLAPI (*)(GLenum,GLfloat))                                     _ptr[94].ptr)
//         GLAPI void GLAPIENTRY glIndexfv( const GLfloat *c );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[95].ptr)
//         GLAPI void GLAPIENTRY glVertex3i( GLint x, GLint y, GLint z );
#define PF_void (* (GLAPI (*)(GLint,GLint,GLint))                                  _ptr[96].ptr)
//         GLAPI void GLAPIENTRY glVertex3iv( const GLint *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[97].ptr)
//         GLAPI void GLAPIENTRY glRasterPos2iv( const GLint *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[98].ptr)
//         GLAPI void GLAPIENTRY glAccum( GLenum op, GLfloat value );
#define PF_void (* (GLAPI (*)(GLenum,GLfloat))                                     _ptr[99].ptr)
//         GLAPI GLboolean GLAPIENTRY glIsTexture( GLuint texture );
#define PF_GLboolean (* (GLAPI (*)(GLuint))                                        _ptr[100].ptr)
//         GLAPI void GLAPIENTRY glVertex3s( GLshort x, GLshort y, GLshort z );
#define PF_void (* (GLAPI (*)(GLshort,GLshort,GLshort))                            _ptr[101].ptr)
//         GLAPI void GLAPIENTRY glStencilFunc( GLenum func, GLint ref, GLuint mask );
#define PF_void (* (GLAPI (*)(GLenum,GLint,GLuint))                                _ptr[102].ptr)
//         GLAPI void GLAPIENTRY glRasterPos4fv( const GLfloat *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[103].ptr)
//         GLAPI void GLAPIENTRY glVertex4d( GLdouble x, GLdouble y, GLdouble z, GLdouble w );
#define PF_void (* (GLAPI (*)(GLdouble,GLdouble,GLdouble,GLdouble))                _ptr[104].ptr)
//         GLAPI void GLAPIENTRY glVertex4dv( const GLdouble *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[105].ptr)
//         GLAPI void GLAPIENTRY glVertex4f( GLfloat x, GLfloat y, GLfloat z, GLfloat w );
#define PF_void (* (GLAPI (*)(GLfloat,GLfloat,GLfloat,GLfloat))                    _ptr[106].ptr)
//         GLAPI void GLAPIENTRY glVertex4fv( const GLfloat *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[107].ptr)
//         GLAPI void GLAPIENTRY glVertex4i( GLint x, GLint y, GLint z, GLint w );
#define PF_void (* (GLAPI (*)(GLint,GLint,GLint,GLint))                            _ptr[108].ptr)
//         GLAPI void GLAPIENTRY glVertex4iv( const GLint *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[109].ptr)
//         GLAPI void GLAPIENTRY glGetHistogramParameterfv( GLenum target, GLenum pname,
#define PF_void (* (GLAPI (*)(GLenum,GLenum,))                                     _ptr[110].ptr)
//         GLAPI void GLAPIENTRY glStencilMask( GLuint mask );
#define PF_void (* (GLAPI (*)(GLuint))                                             _ptr[111].ptr)
//         GLAPI void GLAPIENTRY glVertex4s( GLshort x, GLshort y, GLshort z, GLshort w );
#define PF_void (* (GLAPI (*)(GLshort,GLshort,GLshort,GLshort))                    _ptr[112].ptr)
//         GLAPI void GLAPIENTRY glVertex4sv( const GLshort *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[113].ptr)
//         GLAPI void APIENTRY glEGLImageTargetTexture2DOES (GLenum target, GLeglImageOES image);
#define PF_void (* (GLAPI (*)(GLenum,GLeglImageOES))                               _ptr[114].ptr)
//         GLAPI void GLAPIENTRY glNormalPointer( GLenum type, GLsizei stride,
#define PF_void (* (GLAPI (*)(GLenum,GLsizei,))                                    _ptr[115].ptr)
//         GLAPI void GLAPIENTRY glPixelMapusv( GLenum map, GLsizei mapsize,
#define PF_void (* (GLAPI (*)(GLenum,GLsizei,))                                    _ptr[116].ptr)
//         GLAPI void GLAPIENTRY glClientActiveTextureARB(GLenum texture);
#define PF_void (* (GLAPI (*)(GLenum))                                             _ptr[117].ptr)
//         GLAPI void GLAPIENTRY glColor4ub( GLubyte red, GLubyte green,
#define PF_void (* (GLAPI (*)(GLubyte,GLubyte,))                                   _ptr[118].ptr)
//         GLAPI void GLAPIENTRY glColor4ubv( const GLubyte *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[119].ptr)
//         GLAPI void GLAPIENTRY glTexParameteriv( GLenum target, GLenum pname,
#define PF_void (* (GLAPI (*)(GLenum,GLenum,))                                     _ptr[120].ptr)
//         GLAPI void GLAPIENTRY glColor4ui( GLuint red, GLuint green,
#define PF_void (* (GLAPI (*)(GLuint,GLuint,))                                     _ptr[121].ptr)
//         GLAPI void GLAPIENTRY glColor4uiv( const GLuint *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[122].ptr)
//         GLAPI void GLAPIENTRY glIndexiv( const GLint *c );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[123].ptr)
//         GLAPI void GLAPIENTRY glReadPixels( GLint x, GLint y,
#define PF_void (* (GLAPI (*)(GLint,GLint,))                                       _ptr[124].ptr)
//         GLAPI void GLAPIENTRY glColor4us( GLushort red, GLushort green,
#define PF_void (* (GLAPI (*)(GLushort,GLushort,))                                 _ptr[125].ptr)
//         GLAPI void GLAPIENTRY glColor4usv( const GLushort *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[126].ptr)
//         GLAPI void GLAPIENTRY glLineWidth( GLfloat width );
#define PF_void (* (GLAPI (*)(GLfloat))                                            _ptr[127].ptr)
//         GLAPI void GLAPIENTRY glRasterPos4iv( const GLint *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[128].ptr)
//         GLAPI void GLAPIENTRY glActiveTextureARB(GLenum texture);
#define PF_void (* (GLAPI (*)(GLenum))                                             _ptr[129].ptr)
//         GLAPI void GLAPIENTRY glNormal3b( GLbyte nx, GLbyte ny, GLbyte nz );
#define PF_void (* (GLAPI (*)(GLbyte,GLbyte,GLbyte))                               _ptr[130].ptr)
//         GLAPI void GLAPIENTRY glNormal3bv( const GLbyte *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[131].ptr)
//         GLAPI void GLAPIENTRY glNormal3d( GLdouble nx, GLdouble ny, GLdouble nz );
#define PF_void (* (GLAPI (*)(GLdouble,GLdouble,GLdouble))                         _ptr[132].ptr)
//         GLAPI void GLAPIENTRY glNormal3dv( const GLdouble *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[133].ptr)
//         GLAPI void GLAPIENTRY glNormal3f( GLfloat nx, GLfloat ny, GLfloat nz );
#define PF_void (* (GLAPI (*)(GLfloat,GLfloat,GLfloat))                            _ptr[134].ptr)
//         GLAPI void GLAPIENTRY glNormal3fv( const GLfloat *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[135].ptr)
//         GLAPI void GLAPIENTRY glNormal3i( GLint nx, GLint ny, GLint nz );
#define PF_void (* (GLAPI (*)(GLint,GLint,GLint))                                  _ptr[136].ptr)
//         GLAPI void GLAPIENTRY glNormal3iv( const GLint *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[137].ptr)
//         GLAPI void GLAPIENTRY glTexCoord1dv( const GLdouble *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[138].ptr)
//         GLAPI void GLAPIENTRY glNormal3s( GLshort nx, GLshort ny, GLshort nz );
#define PF_void (* (GLAPI (*)(GLshort,GLshort,GLshort))                            _ptr[139].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord1svARB(GLenum target, const GLshort *v);
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[140].ptr)
//         GLAPI void GLAPIENTRY glRectd( GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2 );
#define PF_void (* (GLAPI (*)(GLdouble,GLdouble,GLdouble,GLdouble))                _ptr[141].ptr)
//         GLAPI void GLAPIENTRY glRectdv( const GLdouble *v1, const GLdouble *v2 );
#define PF_void (* (GLAPI (*)(const,const))                                        _ptr[142].ptr)
//         GLAPI void GLAPIENTRY glRectf( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 );
#define PF_void (* (GLAPI (*)(GLfloat,GLfloat,GLfloat,GLfloat))                    _ptr[143].ptr)
//         GLAPI void GLAPIENTRY glRectfv( const GLfloat *v1, const GLfloat *v2 );
#define PF_void (* (GLAPI (*)(const,const))                                        _ptr[144].ptr)
//         GLAPI void GLAPIENTRY glRecti( GLint x1, GLint y1, GLint x2, GLint y2 );
#define PF_void (* (GLAPI (*)(GLint,GLint,GLint,GLint))                            _ptr[145].ptr)
//         GLAPI void GLAPIENTRY glRectiv( const GLint *v1, const GLint *v2 );
#define PF_void (* (GLAPI (*)(const,const))                                        _ptr[146].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord2ivARB(GLenum target, const GLint *v);
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[147].ptr)
//         GLAPI void GLAPIENTRY glIndexMask( GLuint mask );
#define PF_void (* (GLAPI (*)(GLuint))                                             _ptr[148].ptr)
//         GLAPI void GLAPIENTRY glRects( GLshort x1, GLshort y1, GLshort x2, GLshort y2 );
#define PF_void (* (GLAPI (*)(GLshort,GLshort,GLshort,GLshort))                    _ptr[149].ptr)
//         GLAPI void GLAPIENTRY glRectsv( const GLshort *v1, const GLshort *v2 );
#define PF_void (* (GLAPI (*)(const,const))                                        _ptr[150].ptr)
//         GLAPI void GLAPIENTRY glCompressedTexSubImage3D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data );
#define PF_void (* (GLAPI (*)(GLenum,GLint,GLint,GLint,GLint,GLsizei,GLsizei,GLsizei,GLenum,GLsizei,const))    _ptr[151].ptr)
//         GLAPI void GLAPIENTRY glTexCoord1fv( const GLfloat *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[152].ptr)
//         GLAPI GLuint GLAPIENTRY glGenLists( GLsizei range );
#define PF_GLuint (* (GLAPI (*)(GLsizei))                                          _ptr[153].ptr)
//         GLAPI void GLAPIENTRY glGetMapdv( GLenum target, GLenum query, GLdouble *v );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,GLdouble))                             _ptr[154].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord1dv( GLenum target, const GLdouble *v );
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[155].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord1dvARB(GLenum target, const GLdouble *v);
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[156].ptr)
//         GLAPI void GLAPIENTRY glMapGrid1d( GLint un, GLdouble u1, GLdouble u2 );
#define PF_void (* (GLAPI (*)(GLint,GLdouble,GLdouble))                            _ptr[157].ptr)
//         GLAPI void GLAPIENTRY glMapGrid1f( GLint un, GLfloat u1, GLfloat u2 );
#define PF_void (* (GLAPI (*)(GLint,GLfloat,GLfloat))                              _ptr[158].ptr)
//         GLAPI void GLAPIENTRY glIndexubv( const GLubyte *c );  /* 1.1 */
#define PF_void (* (GLAPI (*)(const))                                              _ptr[159].ptr)
//         GLAPI void GLAPIENTRY glGetPixelMapfv( GLenum map, GLfloat *values );
#define PF_void (* (GLAPI (*)(GLenum,GLfloat))                                     _ptr[160].ptr)
//         GLAPI void GLAPIENTRY glColorPointer( GLint size, GLenum type,
#define PF_void (* (GLAPI (*)(GLint,GLenum,))                                      _ptr[161].ptr)
//         GLAPI void GLAPIENTRY glMapGrid2d( GLint un, GLdouble u1, GLdouble u2,
#define PF_void (* (GLAPI (*)(GLint,GLdouble,GLdouble,))                           _ptr[162].ptr)
//         GLAPI void GLAPIENTRY glMapGrid2f( GLint un, GLfloat u1, GLfloat u2,
#define PF_void (* (GLAPI (*)(GLint,GLfloat,GLfloat,))                             _ptr[163].ptr)
//         GLAPI void GLAPIENTRY glTexCoord3dv( const GLdouble *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[164].ptr)
//         GLAPI void GLAPIENTRY glMinmax( GLenum target, GLenum internalformat,
#define PF_void (* (GLAPI (*)(GLenum,GLenum,))                                     _ptr[165].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord2fvARB(GLenum target, const GLfloat *v);
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[166].ptr)
//         GLAPI void GLAPIENTRY glDrawElements( GLenum mode, GLsizei count,
#define PF_void (* (GLAPI (*)(GLenum,GLsizei,))                                    _ptr[167].ptr)
//         GLAPI void GLAPIENTRY glGetMapfv( GLenum target, GLenum query, GLfloat *v );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,GLfloat))                              _ptr[168].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord1fv( GLenum target, const GLfloat *v );
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[169].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord1fvARB(GLenum target, const GLfloat *v);
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[170].ptr)
//         GLAPI void GLAPIENTRY glConvolutionFilter1D( GLenum target,
#define PF_void (* (GLAPI (*)(GLenum,))                                            _ptr[171].ptr)
//         GLAPI void GLAPIENTRY glTexCoord1iv( const GLint *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[172].ptr)
//         GLAPI void GLAPIENTRY glPushAttrib( GLbitfield mask );
#define PF_void (* (GLAPI (*)(GLbitfield))                                         _ptr[173].ptr)
//         GLAPI void GLAPIENTRY glClearDepth( GLclampd depth );
#define PF_void (* (GLAPI (*)(GLclampd))                                           _ptr[174].ptr)
//         GLAPI void GLAPIENTRY glEdgeFlag( GLboolean flag );
#define PF_void (* (GLAPI (*)(GLboolean))                                          _ptr[175].ptr)
//         GLAPI void GLAPIENTRY glEdgeFlagv( const GLboolean *flag );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[176].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord3svARB(GLenum target, const GLshort *v);
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[177].ptr)
//         GLAPI void GLAPIENTRY glMatrixMode( GLenum mode );
#define PF_void (* (GLAPI (*)(GLenum))                                             _ptr[178].ptr)
//         GLAPI void GLAPIENTRY glRasterPos2sv( const GLshort *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[179].ptr)
//         GLAPI void GLAPIENTRY glSeparableFilter2D( GLenum target,
#define PF_void (* (GLAPI (*)(GLenum,))                                            _ptr[180].ptr)
//         GLAPI void GLAPIENTRY glTexCoord3fv( const GLfloat *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[181].ptr)
//         GLAPI void GLAPIENTRY glConvolutionFilter2D( GLenum target,
#define PF_void (* (GLAPI (*)(GLenum,))                                            _ptr[182].ptr)
//         GLAPI void GLAPIENTRY glGetColorTableParameteriv( GLenum target, GLenum pname,
#define PF_void (* (GLAPI (*)(GLenum,GLenum,))                                     _ptr[183].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord2iARB(GLenum target, GLint s, GLint t);
#define PF_void (* (GLAPI (*)(GLenum,GLint,GLint))                                 _ptr[184].ptr)
//         GLAPI void GLAPIENTRY glCallLists( GLsizei n, GLenum type,
#define PF_void (* (GLAPI (*)(GLsizei,GLenum,))                                    _ptr[185].ptr)
//         GLAPI void GLAPIENTRY glLightModelf( GLenum pname, GLfloat param );
#define PF_void (* (GLAPI (*)(GLenum,GLfloat))                                     _ptr[186].ptr)
//         GLAPI void GLAPIENTRY glLightModelfv( GLenum pname, const GLfloat *params );
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[187].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord3dv( GLenum target, const GLdouble *v );
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[188].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord3dvARB(GLenum target, const GLdouble *v);
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[189].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord4ivARB(GLenum target, const GLint *v);
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[190].ptr)
//         GLAPI void GLAPIENTRY glLightModeli( GLenum pname, GLint param );
#define PF_void (* (GLAPI (*)(GLenum,GLint))                                       _ptr[191].ptr)
//         GLAPI void GLAPIENTRY glLightModeliv( GLenum pname, const GLint *params );
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[192].ptr)
//         GLAPI void GLAPIENTRY glDrawArrays( GLenum mode, GLint first, GLsizei count );
#define PF_void (* (GLAPI (*)(GLenum,GLint,GLsizei))                               _ptr[193].ptr)
//         GLAPI void GLAPIENTRY glGetMapiv( GLenum target, GLenum query, GLint *v );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,GLint))                                _ptr[194].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord1iv( GLenum target, const GLint *v );
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[195].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord1ivARB(GLenum target, const GLint *v);
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[196].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord3fv( GLenum target, const GLfloat *v );
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[197].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord3fvARB(GLenum target, const GLfloat *v);
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[198].ptr)
//         GLAPI void GLAPIENTRY glGetColorTableParameterfv( GLenum target, GLenum pname,
#define PF_void (* (GLAPI (*)(GLenum,GLenum,))                                     _ptr[199].ptr)
//         GLAPI void GLAPIENTRY glFogfv( GLenum pname, const GLfloat *params );
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[200].ptr)
//         GLAPI void GLAPIENTRY glIndexsv( const GLshort *c );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[201].ptr)
//         GLAPI void GLAPIENTRY glGetMaterialfv( GLenum face, GLenum pname, GLfloat *params );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,GLfloat))                              _ptr[202].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord2sARB(GLenum target, GLshort s, GLshort t);
#define PF_void (* (GLAPI (*)(GLenum,GLshort,GLshort))                             _ptr[203].ptr)
//         GLAPI void GLAPIENTRY glTexCoord3iv( const GLint *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[204].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord4fvARB(GLenum target, const GLfloat *v);
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[205].ptr)
//         GLAPI void GLAPIENTRY glGetConvolutionParameterfv( GLenum target, GLenum pname,
#define PF_void (* (GLAPI (*)(GLenum,GLenum,))                                     _ptr[206].ptr)
//         GLAPI void GLAPIENTRY glGetConvolutionParameteriv( GLenum target, GLenum pname,
#define PF_void (* (GLAPI (*)(GLenum,GLenum,))                                     _ptr[207].ptr)
//         GLAPI void GLAPIENTRY glFeedbackBuffer( GLsizei size, GLenum type, GLfloat *buffer );
#define PF_void (* (GLAPI (*)(GLsizei,GLenum,GLfloat))                             _ptr[208].ptr)
//         GLAPI void GLAPIENTRY glRasterPos4sv( const GLshort *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[209].ptr)
//         GLAPI void GLAPIENTRY glGetMinmax( GLenum target, GLboolean reset,
#define PF_void (* (GLAPI (*)(GLenum,GLboolean,))                                  _ptr[210].ptr)
//         GLAPI void GLAPIENTRY glGetMinmaxParameterfv( GLenum target, GLenum pname,
#define PF_void (* (GLAPI (*)(GLenum,GLenum,))                                     _ptr[211].ptr)
//         GLAPI void GLAPIENTRY glGetMinmaxParameteriv( GLenum target, GLenum pname,
#define PF_void (* (GLAPI (*)(GLenum,GLenum,))                                     _ptr[212].ptr)
//         GLAPI void GLAPIENTRY glBitmap( GLsizei width, GLsizei height,
#define PF_void (* (GLAPI (*)(GLsizei,GLsizei,))                                   _ptr[213].ptr)
//         GLAPI void GLAPIENTRY glGetPointerv( GLenum pname, GLvoid **params );
#define PF_void (* (GLAPI (*)(GLenum,GLvoid))                                      _ptr[214].ptr)
//         GLAPI void GLAPIENTRY glPixelMapfv( GLenum map, GLsizei mapsize,
#define PF_void (* (GLAPI (*)(GLenum,GLsizei,))                                    _ptr[215].ptr)
//         GLAPI void GLAPIENTRY glIndexub( GLubyte c );  /* 1.1 */
#define PF_void (* (GLAPI (*)(GLubyte))                                            _ptr[216].ptr)
//         GLAPI void GLAPIENTRY glCallList( GLuint list );
#define PF_void (* (GLAPI (*)(GLuint))                                             _ptr[217].ptr)
//         GLAPI void GLAPIENTRY glPushMatrix( void );
#define PF_void (* (GLAPI (*)(void))                                               _ptr[218].ptr)
//         GLAPI void GLAPIENTRY glArrayElement( GLint i );
#define PF_void (* (GLAPI (*)(GLint))                                              _ptr[219].ptr)
//         GLAPI void GLAPIENTRY glBlendEquation( GLenum mode );
#define PF_void (* (GLAPI (*)(GLenum))                                             _ptr[220].ptr)
//         GLAPI void GLAPIENTRY glBlendEquationSeparateATI( GLenum modeRGB, GLenum modeA );
#define PF_void (* (GLAPI (*)(GLenum,GLenum))                                      _ptr[221].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord3iv( GLenum target, const GLint *v );
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[222].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord3ivARB(GLenum target, const GLint *v);
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[223].ptr)
//         GLAPI void GLAPIENTRY glFogiv( GLenum pname, const GLint *params );
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[224].ptr)
//         GLAPI void GLAPIENTRY glGetMaterialiv( GLenum face, GLenum pname, GLint *params );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,GLint))                                _ptr[225].ptr)
//         GLAPI void GLAPIENTRY glHint( GLenum target, GLenum mode );
#define PF_void (* (GLAPI (*)(GLenum,GLenum))                                      _ptr[226].ptr)
//         GLAPI void GLAPIENTRY glCopyColorTable( GLenum target, GLenum internalformat,
#define PF_void (* (GLAPI (*)(GLenum,GLenum,))                                     _ptr[227].ptr)
//         GLAPI GLint GLAPIENTRY glRenderMode( GLenum mode );
#define PF_GLint (* (GLAPI (*)(GLenum))                                            _ptr[228].ptr)
//         GLAPI void GLAPIENTRY glDepthRange( GLclampd near_val, GLclampd far_val );
#define PF_void (* (GLAPI (*)(GLclampd,GLclampd))                                  _ptr[229].ptr)
//         GLAPI void GLAPIENTRY glPopName( void );
#define PF_void (* (GLAPI (*)(void))                                               _ptr[230].ptr)
//         GLAPI void GLAPIENTRY glCullFace( GLenum mode );
#define PF_void (* (GLAPI (*)(GLenum))                                             _ptr[231].ptr)
//         GLAPI void GLAPIENTRY glCopyConvolutionFilter1D( GLenum target,
#define PF_void (* (GLAPI (*)(GLenum,))                                            _ptr[232].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord3dARB(GLenum target, GLdouble s, GLdouble t, GLdouble r);
#define PF_void (* (GLAPI (*)(GLenum,GLdouble,GLdouble,GLdouble))                  _ptr[233].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord3fARB(GLenum target, GLfloat s, GLfloat t, GLfloat r);
#define PF_void (* (GLAPI (*)(GLenum,GLfloat,GLfloat,GLfloat))                     _ptr[234].ptr)
//         GLAPI void GLAPIENTRY glIndexd( GLdouble c );
#define PF_void (* (GLAPI (*)(GLdouble))                                           _ptr[235].ptr)
//         GLAPI void GLAPIENTRY glIndexf( GLfloat c );
#define PF_void (* (GLAPI (*)(GLfloat))                                            _ptr[236].ptr)
//         GLAPI void GLAPIENTRY glClearAccum( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha );
#define PF_void (* (GLAPI (*)(GLfloat,GLfloat,GLfloat,GLfloat))                    _ptr[237].ptr)
//         GLAPI void GLAPIENTRY glIndexi( GLint c );
#define PF_void (* (GLAPI (*)(GLint))                                              _ptr[238].ptr)
//         GLAPI void GLAPIENTRY glTexCoord1sv( const GLshort *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[239].ptr)
//         GLAPI void GLAPIENTRY glIndexs( GLshort c );
#define PF_void (* (GLAPI (*)(GLshort))                                            _ptr[240].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord1d( GLenum target, GLdouble s );
#define PF_void (* (GLAPI (*)(GLenum,GLdouble))                                    _ptr[241].ptr)
//         GLAPI void GLAPIENTRY glFlush( void );
#define PF_void (* (GLAPI (*)(void))                                               _ptr[242].ptr)
//         GLAPI void GLAPIENTRY glTexEnvf( GLenum target, GLenum pname, GLfloat param );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,GLfloat))                              _ptr[243].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord1f( GLenum target, GLfloat s );
#define PF_void (* (GLAPI (*)(GLenum,GLfloat))                                     _ptr[244].ptr)
//         GLAPI void GLAPIENTRY glFogf( GLenum pname, GLfloat param );
#define PF_void (* (GLAPI (*)(GLenum,GLfloat))                                     _ptr[245].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord1i( GLenum target, GLint s );
#define PF_void (* (GLAPI (*)(GLenum,GLint))                                       _ptr[246].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord1iARB(GLenum target, GLint s);
#define PF_void (* (GLAPI (*)(GLenum,GLint))                                       _ptr[247].ptr)
//         GLAPI void GLAPIENTRY glTexEnvi( GLenum target, GLenum pname, GLint param );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,GLint))                                _ptr[248].ptr)
//         GLAPI void GLAPIENTRY glFogi( GLenum pname, GLint param );
#define PF_void (* (GLAPI (*)(GLenum,GLint))                                       _ptr[249].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord1s( GLenum target, GLshort s );
#define PF_void (* (GLAPI (*)(GLenum,GLshort))                                     _ptr[250].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord1sv( GLenum target, const GLshort *v );
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[251].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord1sARB(GLenum target, GLshort s);
#define PF_void (* (GLAPI (*)(GLenum,GLshort))                                     _ptr[252].ptr)
//         GLAPI void GLAPIENTRY glDrawPixels( GLsizei width, GLsizei height,
#define PF_void (* (GLAPI (*)(GLsizei,GLsizei,))                                   _ptr[253].ptr)
//         GLAPI void GLAPIENTRY glGetTexGendv( GLenum coord, GLenum pname, GLdouble *params );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,GLdouble))                             _ptr[254].ptr)
//         GLAPI void GLAPIENTRY glSampleCoverage( GLclampf value, GLboolean invert );
#define PF_void (* (GLAPI (*)(GLclampf,GLboolean))                                 _ptr[255].ptr)
//         GLAPI void GLAPIENTRY glPixelStoref( GLenum pname, GLfloat param );
#define PF_void (* (GLAPI (*)(GLenum,GLfloat))                                     _ptr[256].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord2d( GLenum target, GLdouble s, GLdouble t );
#define PF_void (* (GLAPI (*)(GLenum,GLdouble,GLdouble))                           _ptr[257].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord2dv( GLenum target, const GLdouble *v );
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[258].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord2dARB(GLenum target, GLdouble s, GLdouble t);
#define PF_void (* (GLAPI (*)(GLenum,GLdouble,GLdouble))                           _ptr[259].ptr)
//         GLAPI void GLAPIENTRY glPixelStorei( GLenum pname, GLint param );
#define PF_void (* (GLAPI (*)(GLenum,GLint))                                       _ptr[260].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord2f( GLenum target, GLfloat s, GLfloat t );
#define PF_void (* (GLAPI (*)(GLenum,GLfloat,GLfloat))                             _ptr[261].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord2fv( GLenum target, const GLfloat *v );
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[262].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord2fARB(GLenum target, GLfloat s, GLfloat t);
#define PF_void (* (GLAPI (*)(GLenum,GLfloat,GLfloat))                             _ptr[263].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord2i( GLenum target, GLint s, GLint t );
#define PF_void (* (GLAPI (*)(GLenum,GLint,GLint))                                 _ptr[264].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord2iv( GLenum target, const GLint *v );
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[265].ptr)
//         GLAPI void GLAPIENTRY glTexGendv( GLenum coord, GLenum pname, const GLdouble *params );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,const))                                _ptr[266].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord2s( GLenum target, GLshort s, GLshort t );
#define PF_void (* (GLAPI (*)(GLenum,GLshort,GLshort))                             _ptr[267].ptr)
//         GLAPI void GLAPIENTRY glTexImage1D( GLenum target, GLint level,
#define PF_void (* (GLAPI (*)(GLenum,GLint,))                                      _ptr[268].ptr)
//         GLAPI void GLAPIENTRY glStencilOp( GLenum fail, GLenum zfail, GLenum zpass );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,GLenum))                               _ptr[269].ptr)
//         GLAPI void GLAPIENTRY glMaterialf( GLenum face, GLenum pname, GLfloat param );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,GLfloat))                              _ptr[270].ptr)
//         GLAPI void GLAPIENTRY glClearColor( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );
#define PF_void (* (GLAPI (*)(GLclampf,GLclampf,GLclampf,GLclampf))                _ptr[271].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord3d( GLenum target, GLdouble s, GLdouble t, GLdouble r );
#define PF_void (* (GLAPI (*)(GLenum,GLdouble,GLdouble,GLdouble))                  _ptr[272].ptr)
//         GLAPI void GLAPIENTRY glMateriali( GLenum face, GLenum pname, GLint param );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,GLint))                                _ptr[273].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord3f( GLenum target, GLfloat s, GLfloat t, GLfloat r );
#define PF_void (* (GLAPI (*)(GLenum,GLfloat,GLfloat,GLfloat))                     _ptr[274].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord3i( GLenum target, GLint s, GLint t, GLint r );
#define PF_void (* (GLAPI (*)(GLenum,GLint,GLint,GLint))                           _ptr[275].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord3iARB(GLenum target, GLint s, GLint t, GLint r);
#define PF_void (* (GLAPI (*)(GLenum,GLint,GLint,GLint))                           _ptr[276].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord3s( GLenum target, GLshort s, GLshort t, GLshort r );
#define PF_void (* (GLAPI (*)(GLenum,GLshort,GLshort,GLshort))                     _ptr[277].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord3sv( GLenum target, const GLshort *v );
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[278].ptr)
//         GLAPI void GLAPIENTRY glTexImage2D( GLenum target, GLint level,
#define PF_void (* (GLAPI (*)(GLenum,GLint,))                                      _ptr[279].ptr)
//         GLAPI void GLAPIENTRY glPopAttrib( void );
#define PF_void (* (GLAPI (*)(void))                                               _ptr[280].ptr)
//         GLAPI void GLAPIENTRY glClearIndex( GLfloat c );
#define PF_void (* (GLAPI (*)(GLfloat))                                            _ptr[281].ptr)
//         GLAPI void GLAPIENTRY glDeleteLists( GLuint list, GLsizei range );
#define PF_void (* (GLAPI (*)(GLuint,GLsizei))                                     _ptr[282].ptr)
//         GLAPI void GLAPIENTRY glGetTexGenfv( GLenum coord, GLenum pname, GLfloat *params );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,GLfloat))                              _ptr[283].ptr)
//         GLAPI void GLAPIENTRY glMultMatrixd( const GLdouble *m );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[284].ptr)
//         GLAPI void GLAPIENTRY glMultMatrixf( const GLfloat *m );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[285].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord4d( GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q );
#define PF_void (* (GLAPI (*)(GLenum,GLdouble,GLdouble,GLdouble,GLdouble))         _ptr[286].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord4dv( GLenum target, const GLdouble *v );
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[287].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord4f( GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q );
#define PF_void (* (GLAPI (*)(GLenum,GLfloat,GLfloat,GLfloat,GLfloat))             _ptr[288].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord4fv( GLenum target, const GLfloat *v );
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[289].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord4i( GLenum target, GLint s, GLint t, GLint r, GLint q );
#define PF_void (* (GLAPI (*)(GLenum,GLint,GLint,GLint,GLint))                     _ptr[290].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord4iv( GLenum target, const GLint *v );
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[291].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord4iARB(GLenum target, GLint s, GLint t, GLint r, GLint q);
#define PF_void (* (GLAPI (*)(GLenum,GLint,GLint,GLint,GLint))                     _ptr[292].ptr)
//         GLAPI void GLAPIENTRY glTexCoord3sv( const GLshort *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[293].ptr)
//         GLAPI void GLAPIENTRY glTexGenfv( GLenum coord, GLenum pname, const GLfloat *params );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,const))                                _ptr[294].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord4s( GLenum target, GLshort s, GLshort t, GLshort r, GLshort q );
#define PF_void (* (GLAPI (*)(GLenum,GLshort,GLshort,GLshort,GLshort))             _ptr[295].ptr)
//         GLAPI void GLAPIENTRY glMultiTexCoord4sARB(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
#define PF_void (* (GLAPI (*)(GLenum,GLshort,GLshort,GLshort,GLshort))             _ptr[296].ptr)
//         GLAPI void GLAPIENTRY glTexImage3D( GLenum target, GLint level,
#define PF_void (* (GLAPI (*)(GLenum,GLint,))                                      _ptr[297].ptr)
//         GLAPI void GLAPIENTRY glTranslated( GLdouble x, GLdouble y, GLdouble z );
#define PF_void (* (GLAPI (*)(GLdouble,GLdouble,GLdouble))                         _ptr[298].ptr)
//         GLAPI void GLAPIENTRY glTranslatef( GLfloat x, GLfloat y, GLfloat z );
#define PF_void (* (GLAPI (*)(GLfloat,GLfloat,GLfloat))                            _ptr[299].ptr)
//         GLAPI void GLAPIENTRY glDepthFunc( GLenum func );
#define PF_void (* (GLAPI (*)(GLenum))                                             _ptr[300].ptr)
//         GLAPI void GLAPIENTRY glDepthMask( GLboolean flag );
#define PF_void (* (GLAPI (*)(GLboolean))                                          _ptr[301].ptr)
//         GLAPI void GLAPIENTRY glMultTransposeMatrixd( const GLdouble m[16] );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[302].ptr)
//         GLAPI void GLAPIENTRY glMultTransposeMatrixf( const GLfloat m[16] );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[303].ptr)
//         GLAPI void GLAPIENTRY glGetIntegerv( GLenum pname, GLint *params );
#define PF_void (* (GLAPI (*)(GLenum,GLint))                                       _ptr[304].ptr)
//         GLAPI void GLAPIENTRY glLoadTransposeMatrixd( const GLdouble m[16] );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[305].ptr)
//         GLAPI void GLAPIENTRY glLoadTransposeMatrixf( const GLfloat m[16] );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[306].ptr)
//         GLAPI void GLAPIENTRY glGetTexGeniv( GLenum coord, GLenum pname, GLint *params );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,GLint))                                _ptr[307].ptr)
//         GLAPI void GLAPIENTRY glScaled( GLdouble x, GLdouble y, GLdouble z );
#define PF_void (* (GLAPI (*)(GLdouble,GLdouble,GLdouble))                         _ptr[308].ptr)
//         GLAPI void GLAPIENTRY glScalef( GLfloat x, GLfloat y, GLfloat z );
#define PF_void (* (GLAPI (*)(GLfloat,GLfloat,GLfloat))                            _ptr[309].ptr)
//         GLAPI void GLAPIENTRY glScissor( GLint x, GLint y, GLsizei width, GLsizei height);
#define PF_void (* (GLAPI (*)(GLint,GLint,GLsizei,GLsizei))                        _ptr[310].ptr)
//         GLAPI void GLAPIENTRY glTexGeniv( GLenum coord, GLenum pname, const GLint *params );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,const))                                _ptr[311].ptr)
//         GLAPI void GLAPIENTRY glConvolutionParameterf( GLenum target, GLenum pname,
#define PF_void (* (GLAPI (*)(GLenum,GLenum,))                                     _ptr[312].ptr)
//         GLAPI void GLAPIENTRY glConvolutionParameteri( GLenum target, GLenum pname,
#define PF_void (* (GLAPI (*)(GLenum,GLenum,))                                     _ptr[313].ptr)
//         GLAPI void GLAPIENTRY glTexGend( GLenum coord, GLenum pname, GLdouble param );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,GLdouble))                             _ptr[314].ptr)
//         GLAPI void GLAPIENTRY glTexGenf( GLenum coord, GLenum pname, GLfloat param );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,GLfloat))                              _ptr[315].ptr)
//         GLAPI void GLAPIENTRY glTexGeni( GLenum coord, GLenum pname, GLint param );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,GLint))                                _ptr[316].ptr)
//         GLAPI void GLAPIENTRY glColorMask( GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha );
#define PF_void (* (GLAPI (*)(GLboolean,GLboolean,GLboolean,GLboolean))            _ptr[317].ptr)
//         GLAPI void GLAPIENTRY glTexCoordPointer( GLint size, GLenum type,
#define PF_void (* (GLAPI (*)(GLint,GLenum,))                                      _ptr[318].ptr)
//         GLAPI void GLAPIENTRY glColorSubTable( GLenum target,
#define PF_void (* (GLAPI (*)(GLenum,))                                            _ptr[319].ptr)
//         GLAPI void GLAPIENTRY glClipPlane( GLenum plane, const GLdouble *equation );
#define PF_void (* (GLAPI (*)(GLenum,const))                                       _ptr[320].ptr)
//         GLAPI void GLAPIENTRY glPopMatrix( void );
#define PF_void (* (GLAPI (*)(void))                                               _ptr[321].ptr)
//         GLAPI void GLAPIENTRY glCompressedTexSubImage2D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data );
#define PF_void (* (GLAPI (*)(GLenum,GLint,GLint,GLint,GLsizei,GLsizei,GLenum,GLsizei,const))    _ptr[322].ptr)
//         GLAPI void GLAPIENTRY glPushClientAttrib( GLbitfield mask );  /* 1.1 */
#define PF_void (* (GLAPI (*)(GLbitfield))                                         _ptr[323].ptr)
//         GLAPI void GLAPIENTRY glPopClientAttrib( void );  /* 1.1 */
#define PF_void (* (GLAPI (*)(void))                                               _ptr[324].ptr)
//         GLAPI void GLAPIENTRY glViewport( GLint x, GLint y,
#define PF_void (* (GLAPI (*)(GLint,GLint,))                                       _ptr[325].ptr)
//         GLAPI void GLAPIENTRY glRasterPos3dv( const GLdouble *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[326].ptr)
//         GLAPI void GLAPIENTRY glGetColorTable( GLenum target, GLenum format,
#define PF_void (* (GLAPI (*)(GLenum,GLenum,))                                     _ptr[327].ptr)
//         GLAPI void GLAPIENTRY glColorTableParameterfv(GLenum target, GLenum pname,
#define PF_void (* (GLAPI (*)(GLenum,GLenum,))                                     _ptr[328].ptr)
//         GLAPI void GLAPIENTRY glEnd( void );
#define PF_void (* (GLAPI (*)(void))                                               _ptr[329].ptr)
//         GLAPI void GLAPIENTRY glGetPixelMapuiv( GLenum map, GLuint *values );
#define PF_void (* (GLAPI (*)(GLenum,GLuint))                                      _ptr[330].ptr)
//         GLAPI void GLAPIENTRY glDisable( GLenum cap );
#define PF_void (* (GLAPI (*)(GLenum))                                             _ptr[331].ptr)
//         GLAPI void GLAPIENTRY glDisableClientState( GLenum cap );  /* 1.1 */
#define PF_void (* (GLAPI (*)(GLenum))                                             _ptr[332].ptr)
//         GLAPI void GLAPIENTRY glCompressedTexImage1D( GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data );
#define PF_void (* (GLAPI (*)(GLenum,GLint,GLenum,GLsizei,GLint,GLsizei,const))    _ptr[333].ptr)
//         GLAPI void GLAPIENTRY glDeleteTextures( GLsizei n, const GLuint *textures);
#define PF_void (* (GLAPI (*)(GLsizei,const))                                      _ptr[334].ptr)
//         GLAPI void GLAPIENTRY glRasterPos3fv( const GLfloat *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[335].ptr)
//         GLAPI void GLAPIENTRY glEvalMesh1( GLenum mode, GLint i1, GLint i2 );
#define PF_void (* (GLAPI (*)(GLenum,GLint,GLint))                                 _ptr[336].ptr)
//         GLAPI void GLAPIENTRY glEvalMesh2( GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2 );
#define PF_void (* (GLAPI (*)(GLenum,GLint,GLint,GLint,GLint))                     _ptr[337].ptr)
//         GLAPI void GLAPIENTRY glCompressedTexImage2D( GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data );
#define PF_void (* (GLAPI (*)(GLenum,GLint,GLenum,GLsizei,GLsizei,GLint,GLsizei,const))    _ptr[338].ptr)
//         GLAPI void GLAPIENTRY glRasterPos2d( GLdouble x, GLdouble y );
#define PF_void (* (GLAPI (*)(GLdouble,GLdouble))                                  _ptr[339].ptr)
//         GLAPI void GLAPIENTRY glRasterPos2f( GLfloat x, GLfloat y );
#define PF_void (* (GLAPI (*)(GLfloat,GLfloat))                                    _ptr[340].ptr)
//         GLAPI void GLAPIENTRY glGetClipPlane( GLenum plane, GLdouble *equation );
#define PF_void (* (GLAPI (*)(GLenum,GLdouble))                                    _ptr[341].ptr)
//         GLAPI void GLAPIENTRY glRasterPos2i( GLint x, GLint y );
#define PF_void (* (GLAPI (*)(GLint,GLint))                                        _ptr[342].ptr)
//         GLAPI void GLAPIENTRY glGetTexLevelParameterfv( GLenum target, GLint level,
#define PF_void (* (GLAPI (*)(GLenum,GLint,))                                      _ptr[343].ptr)
//         GLAPI void GLAPIENTRY glCompressedTexImage3D( GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data );
#define PF_void (* (GLAPI (*)(GLenum,GLint,GLenum,GLsizei,GLsizei,GLsizei,GLint,GLsizei,const))    _ptr[344].ptr)
//         GLAPI void GLAPIENTRY glRasterPos2s( GLshort x, GLshort y );
#define PF_void (* (GLAPI (*)(GLshort,GLshort))                                    _ptr[345].ptr)
//         GLAPI void GLAPIENTRY glColorTableParameteriv(GLenum target, GLenum pname,
#define PF_void (* (GLAPI (*)(GLenum,GLenum,))                                     _ptr[346].ptr)
//         GLAPI void GLAPIENTRY glInitNames( void );
#define PF_void (* (GLAPI (*)(void))                                               _ptr[347].ptr)
//         GLAPI void GLAPIENTRY glRasterPos3d( GLdouble x, GLdouble y, GLdouble z );
#define PF_void (* (GLAPI (*)(GLdouble,GLdouble,GLdouble))                         _ptr[348].ptr)
//         GLAPI void GLAPIENTRY glRasterPos3f( GLfloat x, GLfloat y, GLfloat z );
#define PF_void (* (GLAPI (*)(GLfloat,GLfloat,GLfloat))                            _ptr[349].ptr)
//         GLAPI GLboolean GLAPIENTRY glIsList( GLuint list );
#define PF_GLboolean (* (GLAPI (*)(GLuint))                                        _ptr[350].ptr)
//         GLAPI void GLAPIENTRY glIndexPointer( GLenum type, GLsizei stride,
#define PF_void (* (GLAPI (*)(GLenum,GLsizei,))                                    _ptr[351].ptr)
//         GLAPI void GLAPIENTRY glColor3ub( GLubyte red, GLubyte green, GLubyte blue );
#define PF_void (* (GLAPI (*)(GLubyte,GLubyte,GLubyte))                            _ptr[352].ptr)
//         GLAPI void GLAPIENTRY glColor3ubv( const GLubyte *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[353].ptr)
//         GLAPI void GLAPIENTRY glRasterPos3i( GLint x, GLint y, GLint z );
#define PF_void (* (GLAPI (*)(GLint,GLint,GLint))                                  _ptr[354].ptr)
//         GLAPI void GLAPIENTRY glRasterPos3iv( const GLint *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[355].ptr)
//         GLAPI void GLAPIENTRY glColor3ui( GLuint red, GLuint green, GLuint blue );
#define PF_void (* (GLAPI (*)(GLuint,GLuint,GLuint))                               _ptr[356].ptr)
//         GLAPI void GLAPIENTRY glColor3uiv( const GLuint *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[357].ptr)
//         GLAPI void GLAPIENTRY glRasterPos3s( GLshort x, GLshort y, GLshort z );
#define PF_void (* (GLAPI (*)(GLshort,GLshort,GLshort))                            _ptr[358].ptr)
//         GLAPI void GLAPIENTRY glRasterPos3sv( const GLshort *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[359].ptr)
//         GLAPI void GLAPIENTRY glFinish( void );
#define PF_void (* (GLAPI (*)(void))                                               _ptr[360].ptr)
//         GLAPI void GLAPIENTRY glFrustum( GLdouble left, GLdouble right,
#define PF_void (* (GLAPI (*)(GLdouble,GLdouble,))                                 _ptr[361].ptr)
//         GLAPI void GLAPIENTRY glOrtho( GLdouble left, GLdouble right,
#define PF_void (* (GLAPI (*)(GLdouble,GLdouble,))                                 _ptr[362].ptr)
//         GLAPI void GLAPIENTRY glClearStencil( GLint s );
#define PF_void (* (GLAPI (*)(GLint))                                              _ptr[363].ptr)
//         GLAPI void GLAPIENTRY glColor3us( GLushort red, GLushort green, GLushort blue );
#define PF_void (* (GLAPI (*)(GLushort,GLushort,GLushort))                         _ptr[364].ptr)
//         GLAPI void GLAPIENTRY glLoadName( GLuint name );
#define PF_void (* (GLAPI (*)(GLuint))                                             _ptr[365].ptr)
//         GLAPI void GLAPIENTRY glRasterPos4d( GLdouble x, GLdouble y, GLdouble z, GLdouble w );
#define PF_void (* (GLAPI (*)(GLdouble,GLdouble,GLdouble,GLdouble))                _ptr[366].ptr)
//         GLAPI void GLAPIENTRY glRasterPos4f( GLfloat x, GLfloat y, GLfloat z, GLfloat w );
#define PF_void (* (GLAPI (*)(GLfloat,GLfloat,GLfloat,GLfloat))                    _ptr[367].ptr)
//         GLAPI void GLAPIENTRY glRasterPos4i( GLint x, GLint y, GLint z, GLint w );
#define PF_void (* (GLAPI (*)(GLint,GLint,GLint,GLint))                            _ptr[368].ptr)
//         GLAPI void GLAPIENTRY glResetHistogram( GLenum target );
#define PF_void (* (GLAPI (*)(GLenum))                                             _ptr[369].ptr)
//         GLAPI void GLAPIENTRY glRasterPos4s( GLshort x, GLshort y, GLshort z, GLshort w );
#define PF_void (* (GLAPI (*)(GLshort,GLshort,GLshort,GLshort))                    _ptr[370].ptr)
//         GLAPI void GLAPIENTRY glCopyTexImage1D( GLenum target, GLint level,
#define PF_void (* (GLAPI (*)(GLenum,GLint,))                                      _ptr[371].ptr)
//         GLAPI void GLAPIENTRY glBlendFunc( GLenum sfactor, GLenum dfactor );
#define PF_void (* (GLAPI (*)(GLenum,GLenum))                                      _ptr[372].ptr)
//         GLAPI void GLAPIENTRY glGetTexLevelParameteriv( GLenum target, GLint level,
#define PF_void (* (GLAPI (*)(GLenum,GLint,))                                      _ptr[373].ptr)
//         GLAPI void GLAPIENTRY glCopyTexImage2D( GLenum target, GLint level,
#define PF_void (* (GLAPI (*)(GLenum,GLint,))                                      _ptr[374].ptr)
//         GLAPI void GLAPIENTRY glRotated( GLdouble angle,
#define PF_void (* (GLAPI (*)(GLdouble,))                                          _ptr[375].ptr)
//         GLAPI void GLAPIENTRY glRotatef( GLfloat angle,
#define PF_void (* (GLAPI (*)(GLfloat,))                                           _ptr[376].ptr)
//         GLAPI void APIENTRY glEGLImageTargetRenderbufferStorageOES (GLenum target, GLeglImageOES image);
#define PF_void (* (GLAPI (*)(GLenum,GLeglImageOES))                               _ptr[377].ptr)
//         GLAPI void GLAPIENTRY glColorTable( GLenum target, GLenum internalformat,
#define PF_void (* (GLAPI (*)(GLenum,GLenum,))                                     _ptr[378].ptr)
//         GLAPI void GLAPIENTRY glLightfv( GLenum light, GLenum pname,
#define PF_void (* (GLAPI (*)(GLenum,GLenum,))                                     _ptr[379].ptr)
//         GLAPI void GLAPIENTRY glEvalPoint1( GLint i );
#define PF_void (* (GLAPI (*)(GLint))                                              _ptr[380].ptr)
//         GLAPI void GLAPIENTRY glEvalPoint2( GLint i, GLint j );
#define PF_void (* (GLAPI (*)(GLint,GLint))                                        _ptr[381].ptr)
//         GLAPI void GLAPIENTRY glInterleavedArrays( GLenum format, GLsizei stride,
#define PF_void (* (GLAPI (*)(GLenum,GLsizei,))                                    _ptr[382].ptr)
//         GLAPI void GLAPIENTRY glHistogram( GLenum target, GLsizei width,
#define PF_void (* (GLAPI (*)(GLenum,GLsizei,))                                    _ptr[383].ptr)
//         GLAPI void GLAPIENTRY glAlphaFunc( GLenum func, GLclampf ref );
#define PF_void (* (GLAPI (*)(GLenum,GLclampf))                                    _ptr[384].ptr)
//         GLAPI void GLAPIENTRY glClear( GLbitfield mask );
#define PF_void (* (GLAPI (*)(GLbitfield))                                         _ptr[385].ptr)
//         GLAPI void GLAPIENTRY glBegin( GLenum mode );
#define PF_void (* (GLAPI (*)(GLenum))                                             _ptr[386].ptr)
//         GLAPI void GLAPIENTRY glFrontFace( GLenum mode );
#define PF_void (* (GLAPI (*)(GLenum))                                             _ptr[387].ptr)
//         GLAPI void GLAPIENTRY glTexCoord2dv( const GLdouble *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[388].ptr)
//         GLAPI void GLAPIENTRY glLightiv( GLenum light, GLenum pname,
#define PF_void (* (GLAPI (*)(GLenum,GLenum,))                                     _ptr[389].ptr)
//         GLAPI void GLAPIENTRY glGetPixelMapusv( GLenum map, GLushort *values );
#define PF_void (* (GLAPI (*)(GLenum,GLushort))                                    _ptr[390].ptr)
//         GLAPI void GLAPIENTRY glCopyTexSubImage1D( GLenum target, GLint level,
#define PF_void (* (GLAPI (*)(GLenum,GLint,))                                      _ptr[391].ptr)
//         GLAPI void GLAPIENTRY glMap1d( GLenum target, GLdouble u1, GLdouble u2,
#define PF_void (* (GLAPI (*)(GLenum,GLdouble,GLdouble,))                          _ptr[392].ptr)
//         GLAPI void GLAPIENTRY glMap1f( GLenum target, GLfloat u1, GLfloat u2,
#define PF_void (* (GLAPI (*)(GLenum,GLfloat,GLfloat,))                            _ptr[393].ptr)
//         GLAPI void GLAPIENTRY glTexSubImage1D( GLenum target, GLint level,
#define PF_void (* (GLAPI (*)(GLenum,GLint,))                                      _ptr[394].ptr)
//         GLAPI void GLAPIENTRY glTexCoord2fv( const GLfloat *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[395].ptr)
//         GLAPI void GLAPIENTRY glGetHistogram( GLenum target, GLboolean reset,
#define PF_void (* (GLAPI (*)(GLenum,GLboolean,))                                  _ptr[396].ptr)
//         GLAPI void GLAPIENTRY glCopyTexSubImage2D( GLenum target, GLint level,
#define PF_void (* (GLAPI (*)(GLenum,GLint,))                                      _ptr[397].ptr)
//         GLAPI void GLAPIENTRY glVertexPointer( GLint size, GLenum type,
#define PF_void (* (GLAPI (*)(GLint,GLenum,))                                      _ptr[398].ptr)
//         GLAPI void GLAPIENTRY glMap2d( GLenum target,
#define PF_void (* (GLAPI (*)(GLenum,))                                            _ptr[399].ptr)
//         GLAPI void GLAPIENTRY glMap2f( GLenum target,
#define PF_void (* (GLAPI (*)(GLenum,))                                            _ptr[400].ptr)
//         GLAPI void GLAPIENTRY glTexSubImage2D( GLenum target, GLint level,
#define PF_void (* (GLAPI (*)(GLenum,GLint,))                                      _ptr[401].ptr)
//         GLAPI void GLAPIENTRY glCompressedTexSubImage1D( GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data );
#define PF_void (* (GLAPI (*)(GLenum,GLint,GLint,GLsizei,GLenum,GLsizei,const))    _ptr[402].ptr)
//         GLAPI void GLAPIENTRY glGetConvolutionFilter( GLenum target, GLenum format,
#define PF_void (* (GLAPI (*)(GLenum,GLenum,))                                     _ptr[403].ptr)
//         GLAPI void GLAPIENTRY glCopyTexSubImage3D( GLenum target, GLint level,
#define PF_void (* (GLAPI (*)(GLenum,GLint,))                                      _ptr[404].ptr)
//         GLAPI void GLAPIENTRY glTexSubImage3D( GLenum target, GLint level,
#define PF_void (* (GLAPI (*)(GLenum,GLint,))                                      _ptr[405].ptr)
//         GLAPI void GLAPIENTRY glTexCoord4dv( const GLdouble *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[406].ptr)
//         GLAPI GLboolean GLAPIENTRY glAreTexturesResident( GLsizei n,
#define PF_GLboolean (* (GLAPI (*)(GLsizei,))                                      _ptr[407].ptr)
//         GLAPI void GLAPIENTRY glReadBuffer( GLenum mode );
#define PF_void (* (GLAPI (*)(GLenum))                                             _ptr[408].ptr)
//         GLAPI void GLAPIENTRY glGetLightfv( GLenum light, GLenum pname,
#define PF_void (* (GLAPI (*)(GLenum,GLenum,))                                     _ptr[409].ptr)
//         GLAPI void GLAPIENTRY glPassThrough( GLfloat token );
#define PF_void (* (GLAPI (*)(GLfloat))                                            _ptr[410].ptr)
//         GLAPI void GLAPIENTRY glTexCoord2iv( const GLint *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[411].ptr)
//         GLAPI void GLAPIENTRY glNewList( GLuint list, GLenum mode );
#define PF_void (* (GLAPI (*)(GLuint,GLenum))                                      _ptr[412].ptr)
//         GLAPI void GLAPIENTRY glPushName( GLuint name );
#define PF_void (* (GLAPI (*)(GLuint))                                             _ptr[413].ptr)
//         GLAPI void GLAPIENTRY glCopyPixels( GLint x, GLint y,
#define PF_void (* (GLAPI (*)(GLint,GLint,))                                       _ptr[414].ptr)
//         GLAPI void GLAPIENTRY glLoadIdentity( void );
#define PF_void (* (GLAPI (*)(void))                                               _ptr[415].ptr)
//         GLAPI void GLAPIENTRY glTexCoord4fv( const GLfloat *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[416].ptr)
//         GLAPI void GLAPIENTRY glActiveTexture( GLenum texture );
#define PF_void (* (GLAPI (*)(GLenum))                                             _ptr[417].ptr)
//         GLAPI GLenum GLAPIENTRY glGetError( void );
#define PF_GLenum (* (GLAPI (*)(void))                                             _ptr[418].ptr)
//         GLAPI void GLAPIENTRY glLightf( GLenum light, GLenum pname, GLfloat param );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,GLfloat))                              _ptr[419].ptr)
//         GLAPI void GLAPIENTRY glClientActiveTexture( GLenum texture );
#define PF_void (* (GLAPI (*)(GLenum))                                             _ptr[420].ptr)
//         GLAPI void GLAPIENTRY glLighti( GLenum light, GLenum pname, GLint param );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,GLint))                                _ptr[421].ptr)
//         GLAPI void GLAPIENTRY glDrawRangeElements( GLenum mode, GLuint start,
#define PF_void (* (GLAPI (*)(GLenum,GLuint,))                                     _ptr[422].ptr)
//         GLAPI void GLAPIENTRY glShadeModel( GLenum mode );
#define PF_void (* (GLAPI (*)(GLenum))                                             _ptr[423].ptr)
//         GLAPI void GLAPIENTRY glSelectBuffer( GLsizei size, GLuint *buffer );
#define PF_void (* (GLAPI (*)(GLsizei,GLuint))                                     _ptr[424].ptr)
//         GLAPI void GLAPIENTRY glGetLightiv( GLenum light, GLenum pname,
#define PF_void (* (GLAPI (*)(GLenum,GLenum,))                                     _ptr[425].ptr)
//         GLAPI const GLubyte * GLAPIENTRY glGetString( GLenum name );
#define PF_const (* (GLAPI (*)(GLenum))                                            _ptr[426].ptr)
//         GLAPI void GLAPIENTRY glBlendColor( GLclampf red, GLclampf green,
#define PF_void (* (GLAPI (*)(GLclampf,GLclampf,))                                 _ptr[427].ptr)
//         GLAPI void GLAPIENTRY glGetSeparableFilter( GLenum target, GLenum format,
#define PF_void (* (GLAPI (*)(GLenum,GLenum,))                                     _ptr[428].ptr)
//         GLAPI void GLAPIENTRY glTexCoord4iv( const GLint *v );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[429].ptr)
//         GLAPI GLboolean GLAPIENTRY glIsEnabled( GLenum cap );
#define PF_GLboolean (* (GLAPI (*)(GLenum))                                        _ptr[430].ptr)
//         GLAPI void GLAPIENTRY glPolygonOffset( GLfloat factor, GLfloat units );
#define PF_void (* (GLAPI (*)(GLfloat,GLfloat))                                    _ptr[431].ptr)
//         GLAPI void GLAPIENTRY glGetPolygonStipple( GLubyte *mask );
#define PF_void (* (GLAPI (*)(GLubyte))                                            _ptr[432].ptr)
//         GLAPI void GLAPIENTRY glTexCoord1d( GLdouble s );
#define PF_void (* (GLAPI (*)(GLdouble))                                           _ptr[433].ptr)
//         GLAPI void GLAPIENTRY glTexCoord1f( GLfloat s );
#define PF_void (* (GLAPI (*)(GLfloat))                                            _ptr[434].ptr)
//         GLAPI void GLAPIENTRY glTexCoord1i( GLint s );
#define PF_void (* (GLAPI (*)(GLint))                                              _ptr[435].ptr)
//         GLAPI void GLAPIENTRY glTexCoord1s( GLshort s );
#define PF_void (* (GLAPI (*)(GLshort))                                            _ptr[436].ptr)
//         GLAPI void GLAPIENTRY glPolygonMode( GLenum face, GLenum mode );
#define PF_void (* (GLAPI (*)(GLenum,GLenum))                                      _ptr[437].ptr)
//         GLAPI void GLAPIENTRY glTexParameterf( GLenum target, GLenum pname, GLfloat param );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,GLfloat))                              _ptr[438].ptr)
//         GLAPI void GLAPIENTRY glTexCoord2d( GLdouble s, GLdouble t );
#define PF_void (* (GLAPI (*)(GLdouble,GLdouble))                                  _ptr[439].ptr)
//         GLAPI void GLAPIENTRY glTexParameteri( GLenum target, GLenum pname, GLint param );
#define PF_void (* (GLAPI (*)(GLenum,GLenum,GLint))                                _ptr[440].ptr)
//         GLAPI void GLAPIENTRY glTexCoord2f( GLfloat s, GLfloat t );
#define PF_void (* (GLAPI (*)(GLfloat,GLfloat))                                    _ptr[441].ptr)
//         GLAPI void GLAPIENTRY glTexCoord2i( GLint s, GLint t );
#define PF_void (* (GLAPI (*)(GLint,GLint))                                        _ptr[442].ptr)
//         GLAPI void GLAPIENTRY glLoadMatrixd( const GLdouble *m );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[443].ptr)
//         GLAPI void GLAPIENTRY glTexCoord2s( GLshort s, GLshort t );
#define PF_void (* (GLAPI (*)(GLshort,GLshort))                                    _ptr[444].ptr)
//         GLAPI void GLAPIENTRY glLoadMatrixf( const GLfloat *m );
#define PF_void (* (GLAPI (*)(const))                                              _ptr[445].ptr)
//         GLAPI void GLAPIENTRY glColorMaterial( GLenum face, GLenum mode );
#define PF_void (* (GLAPI (*)(GLenum,GLenum))                                      _ptr[446].ptr)
//         GLAPI void GLAPIENTRY glTexCoord3d( GLdouble s, GLdouble t, GLdouble r );
#define PF_void (* (GLAPI (*)(GLdouble,GLdouble,GLdouble))                         _ptr[447].ptr)
//         GLAPI void GLAPIENTRY glTexCoord3f( GLfloat s, GLfloat t, GLfloat r );
#define PF_void (* (GLAPI (*)(GLfloat,GLfloat,GLfloat))                            _ptr[448].ptr)
//         GLAPI void GLAPIENTRY glTexCoord3i( GLint s, GLint t, GLint r );
#define PF_void (* (GLAPI (*)(GLint,GLint,GLint))                                  _ptr[449].ptr)
//         GLAPI void GLAPIENTRY glTexCoord3s( GLshort s, GLshort t, GLshort r );
#define PF_void (* (GLAPI (*)(GLshort,GLshort,GLshort))                            _ptr[450].ptr)
//         GLAPI void GLAPIENTRY glGetDoublev( GLenum pname, GLdouble *params );
#define PF_void (* (GLAPI (*)(GLenum,GLdouble))                                    _ptr[451].ptr)
//         GLAPI void GLAPIENTRY glLineStipple( GLint factor, GLushort pattern );
#define PF_void (* (GLAPI (*)(GLint,GLushort))                                     _ptr[452].ptr)
//         GLAPI void GLAPIENTRY glGetTexImage( GLenum target, GLint level,
#define PF_void (* (GLAPI (*)(GLenum,GLint,))                                      _ptr[453].ptr)
//         GLAPI void GLAPIENTRY glTexCoord4d( GLdouble s, GLdouble t, GLdouble r, GLdouble q );
#define PF_void (* (GLAPI (*)(GLdouble,GLdouble,GLdouble,GLdouble))                _ptr[454].ptr)
//         GLAPI void GLAPIENTRY glTexCoord4f( GLfloat s, GLfloat t, GLfloat r, GLfloat q );
#define PF_void (* (GLAPI (*)(GLfloat,GLfloat,GLfloat,GLfloat))                    _ptr[455].ptr)
//         GLAPI void GLAPIENTRY glTexCoord4i( GLint s, GLint t, GLint r, GLint q );
#define PF_void (* (GLAPI (*)(GLint,GLint,GLint,GLint))                            _ptr[456].ptr)
//         GLAPI void GLAPIENTRY glBindTexture( GLenum target, GLuint texture );
#define PF_void (* (GLAPI (*)(GLenum,GLuint))                                      _ptr[457].ptr)


// -------------- all funcs array ---------------
struct FUNCS_ {
  const char *name;
  void  (*ptr)(void);
};

inline const FUNCS_* load()
{
    union
    {
    void *p;
    void (*fp)(void);
    } u;

    static struct FUNCS_ _funcs[] ={
        {"defined", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"GLboolean", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"GLuint", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"GLint", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"GLboolean", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"GLboolean", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"GLenum", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"const", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"GLboolean", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {nullptr, nullptr}
    };
    void  *dll_handle;
    if ((dll_handle = dlopen("/usr/lib/x86_64-linux-gnu/libGL.so.1.0.0", RTLD_LAZY)) == 0)
    {
        perror("cannot load:");
        return nullptr;
    }
    struct FUNCS_ *fp = _funcs;
    for (; fp->name != nullptr; fp++)
    {
        u.p = dlsym(dll_handle, fp->name);
        if (u.fp == 0)
        {
            perror("cannot load:");
        }
        else
        {
            fp->ptr = u.fp;
        }
    }
    return _funcs;
}
#endif // LIB_RESOLVER
/*
 add this to cpp 
const FUNCS_* _ptr; // global var
  _ptr = load(); // in main() 
*/
 
