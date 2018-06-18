#include "FingerDetect.h"

void FingerDetect::Capture()
{
	int key;
    CvCapture* capture1 = NULL;
	//CvCapture* capture2 = NULL;

    IplImage* img1 = NULL;
	//IplImage* img2 = NULL;

	IplImage* img2 = NULL;
	
    IplImage* mask1 = cvCreateImage(cvSize(320,240),8,3);
	//IplImage* mask2 = cvCreateImage(cvSize(320,240),8,3);
	
    IplImage* dst  = cvCreateImage(cvSize(320,240),8,3);
    int erosions=1,dilations=1;

    char* captureWindow1 = "Capture1";
	//char* captureWindow2 = "Capture2";

    // (a) 変数群の定義 --------------------------------------
    CvVideoWriter* VideoWriter = NULL;
    char* filename = "test.avi";     // 出力ファイル名
    double fps = 15.0;               // ビデオのフレームレート
    //--------------------------------------------------------    
    
    cvNamedWindow(captureWindow1,  CV_WINDOW_AUTOSIZE);
	//cvNamedWindow(captureWindow2,  CV_WINDOW_AUTOSIZE);

    img1 = cvQueryFrame(capture1); //キャプチャサイズを知るために画像取得
	//img2 = cvQueryFrame(capture2);

    while(1)
	{
        img1 = cvQueryFrame(capture1); //キャプチャサイズを知るために画像取得
		//img2 = cvQueryFrame(capture2);

        cvShowImage(captureWindow1, img1);
		//cvShowImage(captureWindow2, img2);

        key = cvWaitKey(1);
        if (key == 0x1b)
		{
			break;
		}
	}

    
    // (d) 後処理--------------------------------------
    cvReleaseVideoWriter(&VideoWriter); // 後処理
    // ------------------------------------------------

    cvReleaseCapture(&capture1);
	//cvReleaseCapture(&capture2);
	cvDestroyWindow(captureWindow1);
    //cvDestroyWindow(captureWindow2);

}

void FingerDetect::GetSkinMask(IplImage* src_RGB, IplImage *mask_BW,int erosions, int dilations)
{
    int x=0,y=0;
    uchar L, a, b;
    uchar *ptr_src, *ptr_dst;
    CvPixelPosition8u pos_src, pos_dst;
    
    // Temporary image
    IplImage* src = cvCreateImage(cvGetSize(src_RGB), IPL_DEPTH_8U , 3);

    // Backup
    cvCopyImage(src_RGB, src); 

    // L*a*b* Color Space
    IplImage* tmpLab = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U , 3);
    cvCvtColor(src ,tmpLab , CV_BGR2Lab);
    
    CV_INIT_PIXEL_POS( pos_src,
                       (unsigned char *) tmpLab->imageData,
                       tmpLab->widthStep,
                       cvGetSize(tmpLab),
                       x, y,
                       tmpLab->origin);

    CV_INIT_PIXEL_POS( pos_dst,
                       (unsigned char *) mask_BW->imageData,
                       mask_BW->widthStep,
                       cvGetSize(mask_BW),
                       x, y,
                       mask_BW->origin);

	for( y=0; y < src->height; y++) {
        for ( x=0; x < src->width; x++){
            ptr_src = CV_MOVE_TO(pos_src, x, y, 3);
            ptr_dst = CV_MOVE_TO(pos_dst, x, y, 3);

            L = ptr_src[0];
            a = ptr_src[1];
            b = ptr_src[2];

			if( L >= 20 && L <= 80 &&
                a >=  5 && a <= 25 &&
                b >=  0 && b <= 25 &&
                (b - a > -5) && (b - a < 25)){
                ptr_dst[0] = 255;
                ptr_dst[1] = 255;
                ptr_dst[2] = 255;
				}
            else{
                ptr_dst[0] = 0;
                ptr_dst[1] = 0;
                ptr_dst[2] = 0;
			}
            /* -----------------------------------------------------------------------------------*/
		}
	}

    // Erosion
    if ( erosions  > 0 )  cvErode( mask_BW,mask_BW,0,erosions );

    // Dilattion
    if ( dilations > 0 ) cvDilate( mask_BW,mask_BW,    0, dilations );

    cvReleaseImage(&tmpLab);
    cvReleaseImage(&src); 
}

void FingerDetect::SkinDetect()
{
    CvCapture* capture = 0;
    IplImage* image = 0;
    IplImage* mask = cvCreateImage(cvSize(320,240),8,3);
    IplImage* dst  = cvCreateImage(cvSize(320,240),8,3);
    int erosions=1,dilations=1;
    printf( "Hot keys: \n"
        "\tESC - quit the program\n");
    
    cvNamedWindow("Normal", CV_WINDOW_AUTOSIZE );
    cvNamedWindow("Skin", CV_WINDOW_AUTOSIZE );

	for(;;){

        IplImage* frame = 0;
        int c;

        frame = cvQueryFrame( capture );
        if( !frame )
            break;

        if( !image ){
            /* allocate all the buffers */
            image         = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 3 );
            image->origin = frame->origin;
            dst->origin   = frame->origin;
		}

        cvCopy( frame, image, 0 );
        cvShowImage( "Normal", image );

        GetSkinMask( image, mask, erosions, dilations );
        cvAnd( image, mask, dst );

        // Result image
        cvShowImage( "Skin", dst );

        c = cvWaitKey(1);
		if( c == 27 ){
            break;
		}
	}
}