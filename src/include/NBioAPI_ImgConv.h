#ifndef _NITGEN_NBioAPI_IMGCONV_H
#define _NITGEN_NBioAPI_IMGCONV_H

#include "NBioAPI_Basic.h"
#include "NBioAPI_Type.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _NBioAPI_LOAD_LIBRARY
   
   NBioAPI_RETURN NBioAPI NBioAPI_ImgConvRawToBmpBuf(LPBYTE lpImageBuffer, 
                                                      UINT nWidth, 
                                                      UINT nHeight,
                                                      LPBYTE lpBMPBuffer,
                                                      int * BMPBufLen);

   NBioAPI_RETURN NBioAPI NBioAPI_ImgConvRawToBmpBufEx(LPBYTE lpImageBuffer, 
                                                      UINT nWidth, 
                                                      UINT nHeight,
                                                      UINT nDPI,
                                                      LPBYTE lpBMPBuffer,
                                                      int * BMPBufLen);

   NBioAPI_RETURN NBioAPI NBioAPI_ImgConvBmpToRawBuf(LPBYTE lpImageBuffer, 
                                                      UINT * nWidth, 
                                                      UINT * nHeight, 
                                                      LPBYTE lpRawBuffer);

   NBioAPI_RETURN NBioAPI NBioAPI_ImgConvBmpToRawBufEx(LPBYTE lpImageBuffer, 
                                                      UINT nBmpBufLen,
                                                      UINT * nWidth, 
                                                      UINT * nHeight, 
                                                      LPBYTE lpRawBuffer);
   
   NBioAPI_RETURN NBioAPI NBioAPI_ImgConvRawToJpgBuf(LPBYTE lpRawBuffer,	
							                                 UINT nWidth,	
							                                 UINT nHeight,
							                                 int nQuality,	
                                                      LPBYTE lpJpgBuffer, 
                                                      int * nJpgBufLen);
   
   NBioAPI_RETURN NBioAPI NBioAPI_ImgConvRawToJpgBufEx(LPBYTE lpRawBuffer,	
							                                 UINT nWidth,	
							                                 UINT nHeight,
							                                 int nQuality,
                                                      UINT nDPI,
                                                      LPBYTE lpJpgBuffer, 
                                                      int * nJpgBufLen);
  
   NBioAPI_RETURN NBioAPI NBioAPI_ImgConvJpgToRawBuf(LPBYTE lpJpgBuffer,
                                                      UINT nJpgBufLen,
							                                 UINT * nWidth,
							                                 UINT * nHeight,
                                                      LPBYTE lpRawBuffer);
  
   NBioAPI_RETURN NBioAPI NBioAPI_ImgConvRawToWSQBuf(LPBYTE lpRawBuffer,
                                                      int nWidth,
                                                      int nHeight,
                                                      LPBYTE lpWSQBuffer,
                                                      int * nReturn_size,
                                                      float p);
   
   NBioAPI_RETURN NBioAPI NBioAPI_ImgConvWSQToRawBuf(LPBYTE lpWSQBuffer,
                                                      LPBYTE lpRawBuffer,
                                                      int * nReturn_size,
                                                      int * nWidth,
                                                      int * nHeight);

#endif/*_NBioAPI_LOAD_LIBRARY */

   typedef NBioAPI_RETURN (NBioAPI* FP_NBioAPI_ImgConvRawToBmpBuf)(LPBYTE lpImageBuffer, 
                                                      UINT nWidth, 
                                                      UINT nHeight,
                                                      LPBYTE lpBMPBuffer,
                                                      int * nBMPBufLen);

   typedef NBioAPI_RETURN (NBioAPI* FP_NBioAPI_ImgConvRawToBmpBufEx)(LPBYTE lpImageBuffer, 
                                                      UINT nWidth, 
                                                      UINT nHeight,
                                                      UINT nDPI,
                                                      LPBYTE lpBMPBuffer,
                                                      int * BMPBufLen);

   typedef NBioAPI_RETURN (NBioAPI* FP_NBioAPI_ImgConvBmpToRawBuf)(LPBYTE lpImageBuffer, 
                                                      UINT * nWidth, 
                                                      UINT * nHeight, 
                                                      LPBYTE lpRawBuffer);

   typedef NBioAPI_RETURN (NBioAPI* FP_NBioAPI_ImgConvBmpToRawBufEx)(LPBYTE lpImageBuffer, 
                                                      UINT nBmpBufLen,
                                                      UINT * nWidth, 
                                                      UINT * nHeight, 
                                                      LPBYTE lpRawBuffer);

   typedef NBioAPI_RETURN (NBioAPI* FP_NBioAPI_ImgConvRawToJpgBuf)(LPBYTE lpRawBuffer,
							                                 UINT nWidth,			
							                                 UINT nHeight,		
							                                 int nQuality,					 
                                                      LPBYTE lpJpgBuffer,  
                                                      int * nJpgBufLen); 

   typedef NBioAPI_RETURN (NBioAPI* FP_NBioAPI_ImgConvRawToJpgBufEx)(LPBYTE lpRawBuffer,	
							                                 UINT nWidth,	
							                                 UINT nHeight,
							                                 int nQuality,
                                                      UINT nDPI,
                                                      LPBYTE lpJpgBuffer, 
                                                      int * nJpgBufLen);

   typedef NBioAPI_RETURN (NBioAPI* FP_NBioAPI_ImgConvJpgToRawBuf)(LPBYTE lpJpgBuffer,
                                                      UINT nJpgBufLen,
							                                 UINT * nWidth,
							                                 UINT * nHeight,
                                                      LPBYTE lpBmpBuffer);
   
   typedef NBioAPI_RETURN (NBioAPI* FP_NBioAPI_ImgConvRawToWSQBuf)(LPBYTE lpRawBuffer,
                                                      int nWwidth,
                                                      int nHeight,
                                                      LPBYTE lpWSQBuffer,
                                                      int * nReturn_size,
                                                      float p);
   
   typedef NBioAPI_RETURN (NBioAPI* FP_NBioAPI_ImgConvWSQToRawBuf)(LPBYTE lpWSQBuffer,
                                                      LPBYTE lpRawBuffer,
                                                      int * nReturn_size,
                                                      int * nWidth,
                                                      int * nHeight);


#ifdef __cplusplus
}
#endif

#endif //_NITGEN_NBioAPI_IMGCONV_H
