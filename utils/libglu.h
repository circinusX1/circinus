#ifndef LIBR_RESOLVER_H
#define LIBR_RESOLVER_H
// Copyright Marius C. https://github/comarius (do not remove)
#include <stdio.h>
#include <errno.h>
#include <dlfcn.h>


// -------------- constants ---------------
// review before compile...
#define __glu_h__
#define GLAPIENTRY __stdcall
#define GLAPIENTRY
#define GLAPIENTRYP GLAPIENTRY *
#define GLU_EXT_object_space_tess          1
#define GLU_EXT_nurbs_tessellator          1
#define GLU_FALSE                          0
#define GLU_TRUE                           1
#define GLU_VERSION_1_1                    1
#define GLU_VERSION_1_2                    1
#define GLU_VERSION_1_3                    1
#define GLU_VERSION                        100800
#define GLU_EXTENSIONS                     100801
#define GLU_INVALID_ENUM                   100900
#define GLU_INVALID_VALUE                  100901
#define GLU_OUT_OF_MEMORY                  100902
#define GLU_INCOMPATIBLE_GL_VERSION        100903
#define GLU_INVALID_OPERATION              100904
#define GLU_OUTLINE_POLYGON                100240
#define GLU_OUTLINE_PATCH                  100241
#define GLU_NURBS_ERROR                    100103
#define GLU_ERROR                          100103
#define GLU_NURBS_BEGIN                    100164
#define GLU_NURBS_BEGIN_EXT                100164
#define GLU_NURBS_VERTEX                   100165
#define GLU_NURBS_VERTEX_EXT               100165
#define GLU_NURBS_NORMAL                   100166
#define GLU_NURBS_NORMAL_EXT               100166
#define GLU_NURBS_COLOR                    100167
#define GLU_NURBS_COLOR_EXT                100167
#define GLU_NURBS_TEXTURE_COORD            100168
#define GLU_NURBS_TEX_COORD_EXT            100168
#define GLU_NURBS_END                      100169
#define GLU_NURBS_END_EXT                  100169
#define GLU_NURBS_BEGIN_DATA               100170
#define GLU_NURBS_BEGIN_DATA_EXT           100170
#define GLU_NURBS_VERTEX_DATA              100171
#define GLU_NURBS_VERTEX_DATA_EXT          100171
#define GLU_NURBS_NORMAL_DATA              100172
#define GLU_NURBS_NORMAL_DATA_EXT          100172
#define GLU_NURBS_COLOR_DATA               100173
#define GLU_NURBS_COLOR_DATA_EXT           100173
#define GLU_NURBS_TEXTURE_COORD_DATA       100174
#define GLU_NURBS_TEX_COORD_DATA_EXT       100174
#define GLU_NURBS_END_DATA                 100175
#define GLU_NURBS_END_DATA_EXT             100175
#define GLU_NURBS_ERROR1                   100251
#define GLU_NURBS_ERROR2                   100252
#define GLU_NURBS_ERROR3                   100253
#define GLU_NURBS_ERROR4                   100254
#define GLU_NURBS_ERROR5                   100255
#define GLU_NURBS_ERROR6                   100256
#define GLU_NURBS_ERROR7                   100257
#define GLU_NURBS_ERROR8                   100258
#define GLU_NURBS_ERROR9                   100259
#define GLU_NURBS_ERROR10                  100260
#define GLU_NURBS_ERROR11                  100261
#define GLU_NURBS_ERROR12                  100262
#define GLU_NURBS_ERROR13                  100263
#define GLU_NURBS_ERROR14                  100264
#define GLU_NURBS_ERROR15                  100265
#define GLU_NURBS_ERROR16                  100266
#define GLU_NURBS_ERROR17                  100267
#define GLU_NURBS_ERROR18                  100268
#define GLU_NURBS_ERROR19                  100269
#define GLU_NURBS_ERROR20                  100270
#define GLU_NURBS_ERROR21                  100271
#define GLU_NURBS_ERROR22                  100272
#define GLU_NURBS_ERROR23                  100273
#define GLU_NURBS_ERROR24                  100274
#define GLU_NURBS_ERROR25                  100275
#define GLU_NURBS_ERROR26                  100276
#define GLU_NURBS_ERROR27                  100277
#define GLU_NURBS_ERROR28                  100278
#define GLU_NURBS_ERROR29                  100279
#define GLU_NURBS_ERROR30                  100280
#define GLU_NURBS_ERROR31                  100281
#define GLU_NURBS_ERROR32                  100282
#define GLU_NURBS_ERROR33                  100283
#define GLU_NURBS_ERROR34                  100284
#define GLU_NURBS_ERROR35                  100285
#define GLU_NURBS_ERROR36                  100286
#define GLU_NURBS_ERROR37                  100287
#define GLU_AUTO_LOAD_MATRIX               100200
#define GLU_CULLING                        100201
#define GLU_SAMPLING_TOLERANCE             100203
#define GLU_DISPLAY_MODE                   100204
#define GLU_PARAMETRIC_TOLERANCE           100202
#define GLU_SAMPLING_METHOD                100205
#define GLU_U_STEP                         100206
#define GLU_V_STEP                         100207
#define GLU_NURBS_MODE                     100160
#define GLU_NURBS_MODE_EXT                 100160
#define GLU_NURBS_TESSELLATOR              100161
#define GLU_NURBS_TESSELLATOR_EXT          100161
#define GLU_NURBS_RENDERER                 100162
#define GLU_NURBS_RENDERER_EXT             100162
#define GLU_OBJECT_PARAMETRIC_ERROR        100208
#define GLU_OBJECT_PARAMETRIC_ERROR_EXT    100208
#define GLU_OBJECT_PATH_LENGTH             100209
#define GLU_OBJECT_PATH_LENGTH_EXT         100209
#define GLU_PATH_LENGTH                    100215
#define GLU_PARAMETRIC_ERROR               100216
#define GLU_DOMAIN_DISTANCE                100217
#define GLU_MAP1_TRIM_2                    100210
#define GLU_MAP1_TRIM_3                    100211
#define GLU_POINT                          100010
#define GLU_LINE                           100011
#define GLU_FILL                           100012
#define GLU_SILHOUETTE                     100013
#define GLU_SMOOTH                         100000
#define GLU_FLAT                           100001
#define GLU_NONE                           100002
#define GLU_OUTSIDE                        100020
#define GLU_INSIDE                         100021
#define GLU_TESS_BEGIN                     100100
#define GLU_BEGIN                          100100
#define GLU_TESS_VERTEX                    100101
#define GLU_VERTEX                         100101
#define GLU_TESS_END                       100102
#define GLU_END                            100102
#define GLU_TESS_ERROR                     100103
#define GLU_TESS_EDGE_FLAG                 100104
#define GLU_EDGE_FLAG                      100104
#define GLU_TESS_COMBINE                   100105
#define GLU_TESS_BEGIN_DATA                100106
#define GLU_TESS_VERTEX_DATA               100107
#define GLU_TESS_END_DATA                  100108
#define GLU_TESS_ERROR_DATA                100109
#define GLU_TESS_EDGE_FLAG_DATA            100110
#define GLU_TESS_COMBINE_DATA              100111
#define GLU_CW                             100120
#define GLU_CCW                            100121
#define GLU_INTERIOR                       100122
#define GLU_EXTERIOR                       100123
#define GLU_UNKNOWN                        100124
#define GLU_TESS_WINDING_RULE              100140
#define GLU_TESS_BOUNDARY_ONLY             100141
#define GLU_TESS_TOLERANCE                 100142
#define GLU_TESS_ERROR1                    100151
#define GLU_TESS_ERROR2                    100152
#define GLU_TESS_ERROR3                    100153
#define GLU_TESS_ERROR4                    100154
#define GLU_TESS_ERROR5                    100155
#define GLU_TESS_ERROR6                    100156
#define GLU_TESS_ERROR7                    100157
#define GLU_TESS_ERROR8                    100158
#define GLU_TESS_MISSING_BEGIN_POLYGON     100151
#define GLU_TESS_MISSING_BEGIN_CONTOUR     100152
#define GLU_TESS_MISSING_END_POLYGON       100153
#define GLU_TESS_MISSING_END_CONTOUR       100154
#define GLU_TESS_COORD_TOO_LARGE           100155
#define GLU_TESS_NEED_COMBINE_CALLBACK     100156
#define GLU_TESS_WINDING_ODD               100130
#define GLU_TESS_WINDING_NONZERO           100131
#define GLU_TESS_WINDING_POSITIVE          100132
#define GLU_TESS_WINDING_NEGATIVE          100133
#define GLU_TESS_WINDING_ABS_GEQ_TWO       100134
#define GLU_TESS_MAX_COORD 1.0e150


// -------------- functions ---------------
// review before compile...
//         GLAPI void GLAPIENTRY gluNurbsCurve (GLUnurbs* nurb, GLint knotCount, GLfloat *knots, GLint stride, GLfloat *control, GLint order, GLenum type);
#define PF_void (* (GLAPI (*)(GLUnurbs*,GLint,GLfloat,GLint,GLfloat,GLint,GLenum))    _ptr[0].ptr)
//         GLAPI void GLAPIENTRY gluTessProperty (GLUtesselator* tess, GLenum which, GLdouble data);
#define PF_void (* (GLAPI (*)(GLUtesselator*,GLenum,GLdouble))                     _ptr[1].ptr)
//         GLAPI GLint GLAPIENTRY gluBuild3DMipmapLevels (GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLint level, GLint base, GLint max, const void *data);
#define PF_GLint (* (GLAPI (*)(GLenum,GLint,GLsizei,GLsizei,GLsizei,GLenum,GLenum,GLint,GLint,GLint,const))    _ptr[2].ptr)
//         GLAPI void GLAPIENTRY gluGetNurbsProperty (GLUnurbs* nurb, GLenum property, GLfloat* data);
#define PF_void (* (GLAPI (*)(GLUnurbs*,GLenum,GLfloat*))                          _ptr[3].ptr)
//         GLAPI void GLAPIENTRY gluQuadricTexture (GLUquadric* quad, GLboolean texture);
#define PF_void (* (GLAPI (*)(GLUquadric*,GLboolean))                              _ptr[4].ptr)
//         GLAPI GLint GLAPIENTRY gluBuild1DMipmaps (GLenum target, GLint internalFormat, GLsizei width, GLenum format, GLenum type, const void *data);
#define PF_GLint (* (GLAPI (*)(GLenum,GLint,GLsizei,GLenum,GLenum,const))          _ptr[5].ptr)
//         GLAPI void GLAPIENTRY gluCylinder (GLUquadric* quad, GLdouble base, GLdouble top, GLdouble height, GLint slices, GLint stacks);
#define PF_void (* (GLAPI (*)(GLUquadric*,GLdouble,GLdouble,GLdouble,GLint,GLint))    _ptr[6].ptr)
//         GLAPI void GLAPIENTRY gluBeginCurve (GLUnurbs* nurb);
#define PF_void (* (GLAPI (*)(GLUnurbs*))                                          _ptr[7].ptr)
//         GLAPI GLUtesselator* GLAPIENTRY gluNewTess (void);
#define PF_GLUtesselator* (* (GLAPI (*)(void))                                     _ptr[8].ptr)
//         GLAPI const GLubyte * GLAPIENTRY gluGetString (GLenum name);
#define PF_const (* (GLAPI (*)(GLenum))                                            _ptr[9].ptr)
//         GLAPI const GLubyte * GLAPIENTRY gluErrorString (GLenum error);
#define PF_const (* (GLAPI (*)(GLenum))                                            _ptr[10].ptr)
//         GLAPI GLint GLAPIENTRY gluScaleImage (GLenum format, GLsizei wIn, GLsizei hIn, GLenum typeIn, const void *dataIn, GLsizei wOut, GLsizei hOut, GLenum typeOut, GLvoid* dataOut);
#define PF_GLint (* (GLAPI (*)(GLenum,GLsizei,GLsizei,GLenum,const,GLsizei,GLsizei,GLenum,GLvoid*))    _ptr[11].ptr)
//         GLAPI GLint GLAPIENTRY gluBuild2DMipmaps (GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *data);
#define PF_GLint (* (GLAPI (*)(GLenum,GLint,GLsizei,GLsizei,GLenum,GLenum,const))    _ptr[12].ptr)
//         GLAPI void GLAPIENTRY gluTessEndPolygon (GLUtesselator* tess);
#define PF_void (* (GLAPI (*)(GLUtesselator*))                                     _ptr[13].ptr)
//         GLAPI void GLAPIENTRY gluTessEndContour (GLUtesselator* tess);
#define PF_void (* (GLAPI (*)(GLUtesselator*))                                     _ptr[14].ptr)
//         GLAPI void GLAPIENTRY gluNurbsProperty (GLUnurbs* nurb, GLenum property, GLfloat value);
#define PF_void (* (GLAPI (*)(GLUnurbs*,GLenum,GLfloat))                           _ptr[15].ptr)
//         GLAPI void GLAPIENTRY gluNurbsSurface (GLUnurbs* nurb, GLint sKnotCount, GLfloat* sKnots, GLint tKnotCount, GLfloat* tKnots, GLint sStride, GLint tStride, GLfloat* control, GLint sOrder, GLint tOrder, GLenum type);
#define PF_void (* (GLAPI (*)(GLUnurbs*,GLint,GLfloat*,GLint,GLfloat*,GLint,GLint,GLfloat*,GLint,GLint,GLenum))    _ptr[16].ptr)
//         GLAPI void GLAPIENTRY gluDeleteNurbsRenderer (GLUnurbs* nurb);
#define PF_void (* (GLAPI (*)(GLUnurbs*))                                          _ptr[17].ptr)
//         GLAPI void GLAPIENTRY gluDisk (GLUquadric* quad, GLdouble inner, GLdouble outer, GLint slices, GLint loops);
#define PF_void (* (GLAPI (*)(GLUquadric*,GLdouble,GLdouble,GLint,GLint))          _ptr[18].ptr)
//         GLAPI void GLAPIENTRY gluEndTrim (GLUnurbs* nurb);
#define PF_void (* (GLAPI (*)(GLUnurbs*))                                          _ptr[19].ptr)
//         GLAPI void GLAPIENTRY gluBeginTrim (GLUnurbs* nurb);
#define PF_void (* (GLAPI (*)(GLUnurbs*))                                          _ptr[20].ptr)
//         GLAPI void GLAPIENTRY gluNurbsCallbackData (GLUnurbs* nurb, GLvoid* userData);
#define PF_void (* (GLAPI (*)(GLUnurbs*,GLvoid*))                                  _ptr[21].ptr)
//         GLAPI void GLAPIENTRY gluNurbsCallbackDataEXT (GLUnurbs* nurb, GLvoid* userData);
#define PF_void (* (GLAPI (*)(GLUnurbs*,GLvoid*))                                  _ptr[22].ptr)
//         GLAPI GLUnurbs* GLAPIENTRY gluNewNurbsRenderer (void);
#define PF_GLUnurbs* (* (GLAPI (*)(void))                                          _ptr[23].ptr)
//         GLAPI void GLAPIENTRY gluQuadricNormals (GLUquadric* quad, GLenum normal);
#define PF_void (* (GLAPI (*)(GLUquadric*,GLenum))                                 _ptr[24].ptr)
//         GLAPI void GLAPIENTRY gluBeginSurface (GLUnurbs* nurb);
#define PF_void (* (GLAPI (*)(GLUnurbs*))                                          _ptr[25].ptr)
//         GLAPI void GLAPIENTRY gluQuadricCallback (GLUquadric* quad, GLenum which, _GLUfuncptr CallBackFunc);
#define PF_void (* (GLAPI (*)(GLUquadric*,GLenum,_GLUfuncptr))                     _ptr[26].ptr)
//         GLAPI void GLAPIENTRY gluTessNormal (GLUtesselator* tess, GLdouble valueX, GLdouble valueY, GLdouble valueZ);
#define PF_void (* (GLAPI (*)(GLUtesselator*,GLdouble,GLdouble,GLdouble))          _ptr[27].ptr)
//         GLAPI void GLAPIENTRY gluPartialDisk (GLUquadric* quad, GLdouble inner, GLdouble outer, GLint slices, GLint loops, GLdouble start, GLdouble sweep);
#define PF_void (* (GLAPI (*)(GLUquadric*,GLdouble,GLdouble,GLint,GLint,GLdouble,GLdouble))    _ptr[28].ptr)
//         GLAPI void GLAPIENTRY gluLoadSamplingMatrices (GLUnurbs* nurb, const GLfloat *model, const GLfloat *perspective, const GLint *view);
#define PF_void (* (GLAPI (*)(GLUnurbs*,const,const,const))                        _ptr[29].ptr)
//         GLAPI void GLAPIENTRY gluPwlCurve (GLUnurbs* nurb, GLint count, GLfloat* data, GLint stride, GLenum type);
#define PF_void (* (GLAPI (*)(GLUnurbs*,GLint,GLfloat*,GLint,GLenum))              _ptr[30].ptr)
//         GLAPI void GLAPIENTRY gluTessCallback (GLUtesselator* tess, GLenum which, _GLUfuncptr CallBackFunc);
#define PF_void (* (GLAPI (*)(GLUtesselator*,GLenum,_GLUfuncptr))                  _ptr[31].ptr)
//         GLAPI GLboolean GLAPIENTRY gluCheckExtension (const GLubyte *extName, const GLubyte *extString);
#define PF_GLboolean (* (GLAPI (*)(const,const))                                   _ptr[32].ptr)
//         GLAPI void GLAPIENTRY gluSphere (GLUquadric* quad, GLdouble radius, GLint slices, GLint stacks);
#define PF_void (* (GLAPI (*)(GLUquadric*,GLdouble,GLint,GLint))                   _ptr[33].ptr)
//         GLAPI void GLAPIENTRY gluEndSurface (GLUnurbs* nurb);
#define PF_void (* (GLAPI (*)(GLUnurbs*))                                          _ptr[34].ptr)
//         GLAPI void GLAPIENTRY gluLookAt (GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ, GLdouble centerX, GLdouble centerY, GLdouble centerZ, GLdouble upX, GLdouble upY, GLdouble upZ);
#define PF_void (* (GLAPI (*)(GLdouble,GLdouble,GLdouble,GLdouble,GLdouble,GLdouble,GLdouble,GLdouble,GLdouble))    _ptr[35].ptr)
//         GLAPI void GLAPIENTRY gluDeleteTess (GLUtesselator* tess);
#define PF_void (* (GLAPI (*)(GLUtesselator*))                                     _ptr[36].ptr)
//         GLAPI GLint GLAPIENTRY gluBuild3DMipmaps (GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *data);
#define PF_GLint (* (GLAPI (*)(GLenum,GLint,GLsizei,GLsizei,GLsizei,GLenum,GLenum,const))    _ptr[37].ptr)
//         GLAPI void GLAPIENTRY gluNextContour (GLUtesselator* tess, GLenum type);
#define PF_void (* (GLAPI (*)(GLUtesselator*,GLenum))                              _ptr[38].ptr)
//         GLAPI void GLAPIENTRY gluBeginPolygon (GLUtesselator* tess);
#define PF_void (* (GLAPI (*)(GLUtesselator*))                                     _ptr[39].ptr)
//         GLAPI GLint GLAPIENTRY gluBuild1DMipmapLevels (GLenum target, GLint internalFormat, GLsizei width, GLenum format, GLenum type, GLint level, GLint base, GLint max, const void *data);
#define PF_GLint (* (GLAPI (*)(GLenum,GLint,GLsizei,GLenum,GLenum,GLint,GLint,GLint,const))    _ptr[40].ptr)
//         GLAPI void GLAPIENTRY gluEndCurve (GLUnurbs* nurb);
#define PF_void (* (GLAPI (*)(GLUnurbs*))                                          _ptr[41].ptr)
//         GLAPI void GLAPIENTRY gluOrtho2D (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top);
#define PF_void (* (GLAPI (*)(GLdouble,GLdouble,GLdouble,GLdouble))                _ptr[42].ptr)
//         GLAPI void GLAPIENTRY gluDeleteQuadric (GLUquadric* quad);
#define PF_void (* (GLAPI (*)(GLUquadric*))                                        _ptr[43].ptr)
//         GLAPI void GLAPIENTRY gluQuadricDrawStyle (GLUquadric* quad, GLenum draw);
#define PF_void (* (GLAPI (*)(GLUquadric*,GLenum))                                 _ptr[44].ptr)
//         GLAPI GLint GLAPIENTRY gluUnProject4 (GLdouble winX, GLdouble winY, GLdouble winZ, GLdouble clipW, const GLdouble *model, const GLdouble *proj, const GLint *view, GLdouble nearVal, GLdouble farVal, GLdouble* objX, GLdouble* objY, GLdouble* objZ, GLdouble* objW);
#define PF_GLint (* (GLAPI (*)(GLdouble,GLdouble,GLdouble,GLdouble,const,const,const,GLdouble,GLdouble,GLdouble*,GLdouble*,GLdouble*,GLdouble*))    _ptr[45].ptr)
//         GLAPI void GLAPIENTRY gluPickMatrix (GLdouble x, GLdouble y, GLdouble delX, GLdouble delY, GLint *viewport);
#define PF_void (* (GLAPI (*)(GLdouble,GLdouble,GLdouble,GLdouble,GLint))          _ptr[46].ptr)
//         GLAPI void GLAPIENTRY gluTessVertex (GLUtesselator* tess, GLdouble *location, GLvoid* data);
#define PF_void (* (GLAPI (*)(GLUtesselator*,GLdouble,GLvoid*))                    _ptr[47].ptr)
//         GLAPI void GLAPIENTRY gluEndPolygon (GLUtesselator* tess);
#define PF_void (* (GLAPI (*)(GLUtesselator*))                                     _ptr[48].ptr)
//         GLAPI void GLAPIENTRY gluQuadricOrientation (GLUquadric* quad, GLenum orientation);
#define PF_void (* (GLAPI (*)(GLUquadric*,GLenum))                                 _ptr[49].ptr)
//         GLAPI void GLAPIENTRY gluNurbsCallback (GLUnurbs* nurb, GLenum which, _GLUfuncptr CallBackFunc);
#define PF_void (* (GLAPI (*)(GLUnurbs*,GLenum,_GLUfuncptr))                       _ptr[50].ptr)
//         GLAPI GLint GLAPIENTRY gluProject (GLdouble objX, GLdouble objY, GLdouble objZ, const GLdouble *model, const GLdouble *proj, const GLint *view, GLdouble* winX, GLdouble* winY, GLdouble* winZ);
#define PF_GLint (* (GLAPI (*)(GLdouble,GLdouble,GLdouble,const,const,const,GLdouble*,GLdouble*,GLdouble*))    _ptr[51].ptr)
//         GLAPI GLUquadric* GLAPIENTRY gluNewQuadric (void);
#define PF_GLUquadric* (* (GLAPI (*)(void))                                        _ptr[52].ptr)
//         GLAPI void GLAPIENTRY gluPerspective (GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
#define PF_void (* (GLAPI (*)(GLdouble,GLdouble,GLdouble,GLdouble))                _ptr[53].ptr)
//         GLAPI GLint GLAPIENTRY gluUnProject (GLdouble winX, GLdouble winY, GLdouble winZ, const GLdouble *model, const GLdouble *proj, const GLint *view, GLdouble* objX, GLdouble* objY, GLdouble* objZ);
#define PF_GLint (* (GLAPI (*)(GLdouble,GLdouble,GLdouble,const,const,const,GLdouble*,GLdouble*,GLdouble*))    _ptr[54].ptr)
//         GLAPI void GLAPIENTRY gluTessBeginPolygon (GLUtesselator* tess, GLvoid* data);
#define PF_void (* (GLAPI (*)(GLUtesselator*,GLvoid*))                             _ptr[55].ptr)
//         GLAPI void GLAPIENTRY gluGetTessProperty (GLUtesselator* tess, GLenum which, GLdouble* data);
#define PF_void (* (GLAPI (*)(GLUtesselator*,GLenum,GLdouble*))                    _ptr[56].ptr)
//         GLAPI void GLAPIENTRY gluTessBeginContour (GLUtesselator* tess);
#define PF_void (* (GLAPI (*)(GLUtesselator*))                                     _ptr[57].ptr)
//         GLAPI GLint GLAPIENTRY gluBuild2DMipmapLevels (GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, GLint level, GLint base, GLint max, const void *data);
#define PF_GLint (* (GLAPI (*)(GLenum,GLint,GLsizei,GLsizei,GLenum,GLenum,GLint,GLint,GLint,const))    _ptr[58].ptr)


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
        {"void", nullptr},
        {"void", nullptr},
        {"GLint", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"GLint", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"GLUtesselator*", nullptr},
        {"const", nullptr},
        {"const", nullptr},
        {"GLint", nullptr},
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
        {"GLUnurbs*", nullptr},
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
        {"GLint", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"GLint", nullptr},
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
        {"GLint", nullptr},
        {"GLUquadric*", nullptr},
        {"void", nullptr},
        {"GLint", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"void", nullptr},
        {"GLint", nullptr},
        {nullptr, nullptr}
    };
    void  *dll_handle;
    if ((dll_handle = dlopen("/usr/lib/x86_64-linux-gnu/libGLU.so", RTLD_LAZY)) == 0)
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
 
